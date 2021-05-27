#ifndef FLIGHTNET_ROUTE_H
#define FLIGHTNET_ROUTE_H

#include <filesystem>
#include <vector>

#include "airport.h"
#include "plane.h"

constexpr unsigned int PASSENGER_WORTH = 1000;

constexpr double CalculateStepScore(unsigned int additional_passengers, double added_distance) {
    return (additional_passengers * PASSENGER_WORTH + 1) * (1.0 / (1.0 + added_distance));
}

/**
 * Represents a single step in a route
 */
struct RouteStep {
    AirportID start;
    AirportID destination;
    double distance;
    unsigned int passengers_transported_this_step;
    unsigned int passengers_collected_for_the_final_airport;
    bool followed_shortest_path;
#ifdef ENABLE_EXTRA_ROUTE_DATA_COLLECTION
    std::vector<std::pair<unsigned int, double>> considered_options;

    RouteStep(AirportID start, AirportID destination, double distance, unsigned int passengers_transported_this_step,
        unsigned int passengers_collected_for_the_final_airport, bool followed_shortest_path,
        std::vector<std::pair<unsigned int, double>> considered_options);
#else
    RouteStep(AirportID start, AirportID destination, double distance, unsigned int passengers_transported_this_step,
        unsigned int passengers_collected_for_the_final_airport, bool followed_shortest_path);
#endif
};

class AirportSet;
class RouteDisplay;

/**
 * Represents a route between two airports
 */
class Route {
public:
    /**
     * Route constructor
     *
     * @param plane
     */
    explicit Route(const Plane& plane);

    /**
     * add a segment to the route
     *
     * @param segment Set of route steps
     * @param shortest_path The shortest path to complete the route
     */
    void addSegment(std::vector<RouteStep> segment, std::vector<AirportID> shortest_path);

    /**
     * @return a reference to the Route segments
     */
    [[nodiscard]] const std::vector<std::vector<RouteStep>>& getSegments() const;

    friend std::ostream& operator<<(std::ostream& os, const RouteDisplay& display);
private:
    unsigned int plane_capacity;
    std::vector<std::vector<RouteStep>> segments;
    std::vector<std::vector<AirportID>> shortest_paths;
};

class RouteDisplay {
public:
    RouteDisplay(const Route& route, const AirportSet& airport_set);

    /** Write the route to a file */
    void dumpToFile(const std::filesystem::path& path) const;

    /** iostreams << overload */
    friend std::ostream& operator<<(std::ostream& os, const RouteDisplay& display);
private:
    const Route& route;
    const AirportSet& airport_set;
};

#endif  // FLIGHTNET_ROUTE_H
