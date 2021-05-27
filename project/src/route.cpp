#include "route.h"

#include <fstream>
#include <iostream>
#include <numeric>

#include "airport_set.h"

#ifdef ENABLE_EXTRA_ROUTE_DATA_COLLECTION
RouteStep::RouteStep(AirportID start, AirportID destination, double distance,
    unsigned int passengers_transported_this_step, unsigned int passengers_collected_for_the_final_airport,
    bool followed_shortest_path, std::vector<std::pair<unsigned int, double>> considered_options):
    start{start},
    destination{destination}, distance{distance}, passengers_transported_this_step{passengers_transported_this_step},
    passengers_collected_for_the_final_airport{passengers_collected_for_the_final_airport},
    followed_shortest_path{followed_shortest_path}, considered_options{std::move(considered_options)} {}
#else
RouteStep::RouteStep(AirportID start, AirportID destination, double distance,
    unsigned int passengers_transported_this_step, unsigned int passengers_collected_for_the_final_airport,
    bool followed_shortest_path):
    start{start},
    destination{destination}, distance{distance}, passengers_transported_this_step{passengers_transported_this_step},
    passengers_collected_for_the_final_airport{passengers_collected_for_the_final_airport},
    followed_shortest_path{followed_shortest_path} {}
#endif

Route::Route(const Plane& plane): plane_capacity{plane.getCapacity()} {}

void Route::addSegment(std::vector<RouteStep> segment, std::vector<AirportID> shortest_path) {
    segments.push_back(std::move(segment));
    shortest_paths.push_back(std::move(shortest_path));
}

const std::vector<std::vector<RouteStep>>& Route::getSegments() const {
    return segments;
}

RouteDisplay::RouteDisplay(const Route& route, const AirportSet& airport_set): route{route}, airport_set{airport_set} {}

std::ostream& operator<<(std::ostream& os, const RouteDisplay& display) {
    const Route& route = display.route;
    const AirportSet& airport_set = display.airport_set;
    const auto& airports = airport_set.getAirports();

    unsigned int total_passengers_all_segments = 0;
    double total_distance_travelled = 0.0;


    const size_t segments = route.segments.size();
    for (size_t segment = 0; segment < segments; segment++) {
        const size_t steps = route.segments[segment].size();

        const char* const last_airport = airports.at(route.segments[segment][steps - 1].destination).name().data();
        os << "Segment " << segment + 1 << '/' << segments << " ("
           << airports.at(route.segments[segment][0].start).name().data() << " -> " << last_airport << "):\n";

        unsigned int passengers_to_end = 0;
        unsigned int total_passengers = 0;
        for (size_t n_step = 0; n_step < steps; n_step++) {
            const RouteStep& step = route.segments[segment][n_step];
            passengers_to_end += step.passengers_collected_for_the_final_airport;
            total_passengers += step.passengers_transported_this_step;

            os << "\tStep " << n_step + 1 << '/' << steps << ":\n\t\t" << airports.at(step.start).name().data()
               << " -> " << airports.at(step.destination).name().data() << "\n\t\t" << step.distance
               << "m travelled.\n";

            if (n_step < steps - 1) {
                os << "\t\t" << passengers_to_end + step.passengers_transported_this_step
                   << " passengers transported.\n\t\t" << step.passengers_transported_this_step
                   << " left at this destination.\n\t\t" << step.passengers_collected_for_the_final_airport
                   << " picked up to go to the final airport (" << last_airport << ").\n";
            } else {
                total_passengers += passengers_to_end;
                passengers_to_end += step.passengers_transported_this_step;
                os << "\t\t" << passengers_to_end << " passengers transported.\n\t\t" << passengers_to_end
                   << " left at this destination.\n";
            }

            if (step.followed_shortest_path) {
                os << "\t\tFollowed the shortest path to the end, as there was no option to pick up passengers.\n";
            }

#ifdef ENABLE_EXTRA_ROUTE_DATA_COLLECTION
            os << "\n\t\tEvaluated these options while selecting the destination:\n";
            const auto& outgoing = airport_set.getGraph().findVertex(step.start)->getOutgoing();
            for (size_t i = 0; i < step.considered_options.size(); i++) {
                AirportID destination_id = outgoing.at(i)->getDest()->getInfo();
                os << "\t\t\t" << airports.at(destination_id).name().data() << " (" << step.considered_options[i].first
                   << " passengers, +" << step.considered_options[i].second << "m) => "
                   << CalculateStepScore(step.considered_options[i].first, step.considered_options[i].second) << '\n';
            }
#endif
            os << '\n';
        }

        double distance_travelled = std::accumulate(route.segments[segment].cbegin(), route.segments[segment].cend(),
            0.0, [](double acc, const RouteStep& step) { return acc + step.distance; });

        os << "\nTransported " << total_passengers << " passengers in total.\nTravelled " << distance_travelled
           << "m this segment.\nSpent " << (distance_travelled * Plane::fuel_capacity) / Plane::range
           << "m³ of fuel.\n\nThe shortest possible path from beginning to end is:\n\t"
           << airports.at(route.segments[segment][0].start).name().data();

        for (AirportID id : route.shortest_paths[segment]) {
            os << " -> " << airports.at(id).name().data();
        }

        os << "\nThe chosen path was:\n\t" << airports.at(route.segments[segment][0].start).name().data();
        for (const RouteStep& step : route.segments[segment]) {
            os << " -> " << airports.at(step.destination).name().data();
        }

        os << "\n\n";

        total_passengers_all_segments += total_passengers;
        total_distance_travelled += distance_travelled;
    }

    os << "\nIn all segments, " << total_passengers_all_segments << " passengers were transported, "
       << total_distance_travelled << "m were travelled, and "
       << (total_distance_travelled * Plane::fuel_capacity) / Plane::range << "m³ of fuel were used.\n";

    return os;
}

void RouteDisplay::dumpToFile(const std::filesystem::path& path) const {
    std::ofstream(path) << *this;
}
