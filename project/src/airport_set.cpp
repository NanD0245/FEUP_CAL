#include "airport_set.h"

#include <fstream>

#include "util.h"

constexpr unsigned int PLANE_CAPACITY = 400;

AirportSet::AirportSet(const std::filesystem::path& names_path, const std::filesystem::path& locations_path,
    const std::filesystem::path& edges_path, double airports_to_disable):
    graph{},
    planes{} {
    auto imported = import_airports(names_path, locations_path, edges_path);
    this->airports = std::move(imported.first);
    this->edges = std::move(imported.second);
    randomizeUnallowedAirports(airports_to_disable);
    createGraph();
}

void AirportSet::randomizeUnallowedAirports(double fraction) {
    for (auto& kv : airports) {
        kv.second.setLandingAllowed(RandDouble(0.0, 1.0) >= fraction);
    }
}

void AirportSet::randomizePassengers(unsigned int min, unsigned int max, unsigned int groups, double to_nearby) {
    std::vector<AirportID> nearby;
    for (auto& kv : airports) {
        kv.second.passengers().clear();

        nearby.clear();
        auto* vertex = graph.findVertex(kv.first);
        if (vertex != nullptr) {
            for (auto* neighbour : vertex->getOutgoing()) {
                nearby.push_back(neighbour->getDest()->getInfo());
            }
        }

        for (unsigned int i = 0; i < groups; i++) {
            AirportID id;
            if (nearby.empty() || RandDouble(0.0, 1.0) > to_nearby) {
                id = RandUInt(0, static_cast<AirportID>(airports.size() - 1));
            } else {
                id = nearby[RandSize(0, nearby.size() - 1)];
            }

            if (kv.second.passengers().contains(id)) {
                i--;
                continue;
            }

            unsigned int inc = RandUInt(min, max);
            kv.second.passengers().emplace(id, inc);
        }
    }
}

void AirportSet::createGraph() {
    graph.clear();

    for (auto& kv : airports) {
        if (airports.at(kv.first).isLandingAllowed()) {
            graph.addVertex(kv.first);
        }
    }
    for (auto& kv : edges) {
        if (airports.at(kv.first).isLandingAllowed() && kv.second.second <= Plane::getMaxDistanceBetweenAirports()) {
            graph.addEdge(kv.first, kv.second.first, kv.second.second);
        }
    }

    graph.tarjanStronglyConnectedComponents();
    graph.floydWarshallShortestPath();
}

std::ostream& operator<<(std::ostream& os, const AirportSet& airport_set) {
    for (const auto& kv : airport_set.airports) {
        auto id = kv.first;
        auto airport = kv.second;
        os << id << '\t' << airport.name().data() << '\t' << airport.longitude() << '\t' << airport.latitude() << '\t'
           << (airport.isLandingAllowed() ? "true" : "false") << '\n';
    }

    return os;
}


void AirportSet::addRandomPlane() {
    AirportID id;
    do {
        id = RandUInt(0, static_cast<AirportID>(airports.size() - 1));
    } while (!airports.at(id).isLandingAllowed() || airports.at(id).passengers().empty());
    planes.emplace_back(PLANE_CAPACITY, id);
}

void AirportSet::deletePlane(size_t index) {
    if (index >= planes.size()) {
        return;
    }
    planes.erase(planes.begin() + static_cast<long>(index));
}

std::vector<Plane>& AirportSet::getPlanes() {
    return planes;
}

const Graph& AirportSet::getGraph() const {
    return graph;
}

const std::map<AirportID, Airport>& AirportSet::getAirports() const {
    return airports;
}

Route AirportSet::calculateRoute(Plane& plane, AirportID start, AirportID end) {
    Route route(plane);

    const std::unordered_set<AirportID>& current_component = graph.findSCC(start);
    if (!current_component.contains(end)) {
        throw std::invalid_argument("End not in the same strongly connected component");
    }

    plane.getPassengers().clear();

    doFindRoute(route, plane, start, end);
    doFindRoute(route, plane, end, start);

    return route;
}

void AirportSet::doFindRoute(Route& route, Plane& plane, AirportID current_airport, AirportID end_airport) {
    std::vector<RouteStep> segment;

    std::vector<AirportID> shortest_path = graph.getFloydWarshallPath(current_airport, end_airport);
    const auto& distances = graph.getFloydWarshallDistances();


    while (current_airport != end_airport) {
        auto outgoing = graph.findVertex(current_airport)->getOutgoing();

        auto passengers_to_pick_up_for_the_final_airport = std::min(
            plane.availableSeats(), map_get_or_default(airports.at(current_airport).passengers(), end_airport, 0U));
        map_add_or_insert(plane.getPassengers(), end_airport, passengers_to_pick_up_for_the_final_airport);
        map_subtract_or_remove(
            airports.at(current_airport).passengers(), end_airport, passengers_to_pick_up_for_the_final_airport);

        auto available_seats = plane.availableSeats();


#ifdef ENABLE_EXTRA_ROUTE_DATA_COLLECTION
        std::vector<std::pair<unsigned int, double>> considered_options;
#endif
        bool followed_shortest_path = false;
        unsigned int passengers_to_pick_for_the_next_airport = 0;
        bool can_pick_up_passengers_to_the_next_airport = false;
        AirportID destination_id;
        if (available_seats != 0) {
            size_t best = std::numeric_limits<size_t>::max();
            double best_score = std::numeric_limits<double>::min();
            for (size_t i = 0; i < outgoing.size(); i++) {
                auto outgoing_id = outgoing[i]->getDest()->getInfo();

                auto passengers = std::min(
                    available_seats, map_get_or_default(airports.at(current_airport).passengers(), outgoing_id, 0U)
                                         + map_get_or_default(airports.at(outgoing_id).passengers(), end_airport, 0U));
                if (passengers > 0) {
                    can_pick_up_passengers_to_the_next_airport = true;
                }

                double added_distance = distances[current_airport][outgoing_id] + distances[outgoing_id][end_airport]
                                      - distances[current_airport][end_airport];

#ifdef ENABLE_EXTRA_ROUTE_DATA_COLLECTION
                considered_options.emplace_back(passengers, added_distance);
#endif

                double score = CalculateStepScore(passengers, added_distance);
                if (score > best_score) {
                    best_score = score;
                    best = i;
                }
            }

            if (best != std::numeric_limits<size_t>::max()) {
                can_pick_up_passengers_to_the_next_airport = false;
            }

            destination_id = outgoing[best]->getDest()->getInfo();

            if (can_pick_up_passengers_to_the_next_airport) {
                passengers_to_pick_for_the_next_airport = std::min(plane.availableSeats(),
                    map_get_or_default(airports.at(current_airport).passengers(), destination_id, 0U));
                map_subtract_or_remove(
                    airports.at(current_airport).passengers(), destination_id, passengers_to_pick_for_the_next_airport);
            }
        }

        if (available_seats == 0 || !can_pick_up_passengers_to_the_next_airport) {
            destination_id = graph.getFloydWarshallPath(current_airport, end_airport).at(0);
            followed_shortest_path = true;
        }

#ifdef ENABLE_EXTRA_ROUTE_DATA_COLLECTION
        segment.emplace_back(current_airport, destination_id, distances[current_airport][destination_id],
            passengers_to_pick_for_the_next_airport, passengers_to_pick_up_for_the_final_airport,
            followed_shortest_path, std::move(considered_options));
#else
        segment.emplace_back(current_airport, destination_id, distances[current_airport][destination_id],
            passengers_to_pick_for_the_next_airport, passengers_to_pick_up_for_the_final_airport,
            followed_shortest_path);
#endif

        current_airport = destination_id;
    }

    plane.getPassengers().clear();
    route.addSegment(std::move(segment), std::move(shortest_path));
}

void AirportSet::dumpPassengerList(const std::filesystem::path& path) const {
    std::ofstream file(path);

    for (const auto& airport : airports) {
        file << airport.second.name().data() << " has:\n";
        for (const auto& passengers : airport.second.constPassengers()) {
            file << '\t' << passengers.second << " passengers to " << airports.at(passengers.first).name().data()
                 << '\n';
        }
    }
}

void AirportSet::dumpStronglyConnectedComponents(const std::filesystem::path& path) const {
    std::ofstream file(path);

    for (const auto& scc : graph.getStronglyConnectedComponents()) {
        for (AirportID id : scc) {
            file << airports.at(id).name().data() << ' ';
        }
        file << '\n';
    }
}

std::optional<AirportID> AirportSet::pickDestinationForPlane(const Plane& plane) const {
    std::optional<AirportID> dest;
    auto start = plane.getInitialAirport();
    const auto& scc = graph.findSCC(start);

    const auto& airport = airports.at(start);
    unsigned int best = 0;
    for (const auto& passengers : airport.constPassengers()) {
        const auto& dest_airport = airports.at(passengers.first);
        if (!dest_airport.isLandingAllowed() || !scc.contains(passengers.first)) {
            continue;
        }

        if (passengers.second > best) {
            dest = passengers.first;
            best = passengers.second;
        }
    }

    if (!dest.has_value() && scc.size() > 1) {
        std::unordered_set<AirportID> scc_copy(scc);
        scc_copy.erase(start);
        auto it = scc_copy.cbegin();
        std::advance(it, RandSize(0, scc_copy.size() - 1));
        dest = *it;
    }

    return dest;
}
