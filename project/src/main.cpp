#include <cstdlib>
#include <iostream>

#include "airport_set.h"

const char* const PASSENGERS_AFTER = "passengers_after.txt";
const char* const PASSENGERS_BEFORE = "passengers_before.txt";
const char* const STRONGLY_CONNECTED_COMPONENTS = "strongly_connected_components.txt";
const char* const ROUTE_PREFIX = "route";
const char* const ROUTE_SUFFIX = ".txt";
constexpr unsigned int DEFAULT_PLANE_NUM = 10;

/**
 * Prints the right usage of the program
 *
 * @param argv0 first argument of argv
 */
void print_usage(char* argv0) {
    std::cerr << "USAGE: " << argv0 << " [<names file> <locations file> <edges file>] [-p <plane number>]\n";
}

/**
 * Checks arguments given by user
 *
 * @param argc
 * @param argv
 * @param names_path
 * @param coordinates_path
 * @param edges_path
 * @plane_num
 */
void parse_args(int argc, char* argv[], std::filesystem::path& names_path, std::filesystem::path& coordinates_path,
    std::filesystem::path& edges_path, unsigned int& plane_num) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.emplace_back(argv[i]);
    }

    plane_num = DEFAULT_PLANE_NUM;
    for (auto it = args.begin(); it != args.end(); it++) {
        if ((*it) == "-h" || (*it) == "--help") {
            print_usage(argv[0]);
            exit(0);
        }
        if ((*it) == "-p" || (*it) == "--planes") {
            auto it2(it);
            it2++;

            plane_num = static_cast<unsigned int>(std::stoul(*it2));

            args.erase(it2);
            args.erase(it);
            it = args.begin();
        }
    }

    if (args.empty()) {
        names_path = "airports_full_nodes_names.txt";
        coordinates_path = "airports_full_nodes_latlng.txt";
        edges_path = "airports_full_edges_weights.txt";
    } else if (args.size() == 3) {
        names_path = args[0];
        coordinates_path = args[1];
        edges_path = args[2];
    } else {
        print_usage(argv[0]);
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    std::filesystem::path names_path;
    std::filesystem::path coordinates_path;
    std::filesystem::path edges_path;
    unsigned int plane_num;
    parse_args(argc, argv, names_path, coordinates_path, edges_path, plane_num);

    AirportSet airportSet(names_path, coordinates_path, edges_path);
    airportSet.dumpStronglyConnectedComponents(STRONGLY_CONNECTED_COMPONENTS);

    airportSet.randomizePassengers(50, 250, 10, 0.4);
    airportSet.dumpPassengerList(PASSENGERS_BEFORE);

    for (unsigned int i = 0; i < plane_num; i++) {
        airportSet.addRandomPlane();
        auto& plane = airportSet.getPlanes().at(i);
        auto destination = airportSet.pickDestinationForPlane(plane);
        if (!destination.has_value()) {
            std::cout << "No valid destination found for this plane (located in "
                      << airportSet.getAirports().at(plane.getInitialAirport()).name().data() << ").\n";
            continue;
        }

        auto route = airportSet.calculateRoute(plane, plane.getInitialAirport(), *destination);

        std::stringstream filename;
        filename << ROUTE_PREFIX << i + 1 << ROUTE_SUFFIX;
        RouteDisplay(route, airportSet).dumpToFile(filename.str());

        std::cout << RouteDisplay(route, airportSet) << "\n\n";
    }

    airportSet.dumpPassengerList(PASSENGERS_AFTER);

    return 0;
}
