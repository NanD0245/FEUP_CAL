#ifndef FLIGHTNET_AIRPORT_SET_H
#define FLIGHTNET_AIRPORT_SET_H

#include <filesystem>
#include <map>
#include <optional>
#include <unordered_map>

#include "airport.h"
#include "graph.h"
#include "import.h"
#include "plane.h"
#include "route.h"

/**
 * Represents the set of all airports, and is the main class of this program
 */
class AirportSet {
public:
    /**
     * constructor of AirportSet
     *
     * @param names_path
     * @param locations_path
     * @param edges_path
     * @param airports_to_disable - Amount of airports to forbid landing in (due to atmospheric conditions, for
     * example), where 0 means no airports disabled and 1 means all airports disabled.
     */
    AirportSet(const std::filesystem::path& names_path, const std::filesystem::path& locations_path,
        const std::filesystem::path& edges_path, double airports_to_disable = 0.0);

    /**
     * add random sets of passengers to the airports
     *
     * @param min - Minimum number of passengers per group
     * @param max - Maximum number of passengers per group
     * @param groups - Number of passenger groups (each group has a different destination)
     * @param to_nearby - Amount of passenger groups that want to go to a nearby airport (0 means none, 1 means all)
     */
    void randomizePassengers(unsigned int min, unsigned int max, unsigned int groups, double to_nearby);

    /**
     * create the graph
     */
    void createGraph();

    /**
     * Adds a plane to a random airport
     *
     * The airport is chosen between the ones that aren't forbidden to operate and that have passengers for other
     * airports.
     */
    void addRandomPlane();

    /**
     * Removes a plane
     *
     * @param index
     */
    void deletePlane(size_t index);

    /**
     * @return reference to the graph
     */
    const Graph& getGraph() const;

    /**
     * @return reference to the Plane vector
     */
    std::vector<Plane>& getPlanes();

    /**
     * @return reference to the set of airports
     */
    const std::map<AirportID, Airport>& getAirports() const;

    /**
     * Selects a good destination airport for the given plane
     *
     * @return id of the destination airport
     */
    std::optional<AirportID> pickDestinationForPlane(const Plane& plane) const;

    /**
     * Calculates the route that the specified plane should take between the two specified airports
     *
     * @return the route of the plane between two airports
     */
    Route calculateRoute(Plane& plane, AirportID start_id, AirportID end_id);

    /**
     * Writes the passenger list to a file
     *
     * @param path
     */
    void dumpPassengerList(const std::filesystem::path& path) const;

    /**
     * Writes the strongly connected component list to a file
     *
     * @param path
     */
    void dumpStronglyConnectedComponents(const std::filesystem::path& path) const;

    /**
     * overload of the << operator
     */
    friend std::ostream& operator<<(std::ostream& os, const AirportSet& airport_set);

private:
    std::map<AirportID, Airport> airports;
    std::unordered_multimap<AirportID, std::pair<AirportID, double>> edges;
    Graph graph;
    std::vector<Plane> planes;

    void randomizeUnallowedAirports(double fraction);
    void doFindRoute(Route& route, Plane& plane, AirportID current_airport, AirportID end_airport);
};


#endif  // FLIGHTNET_AIRPORT_SET_H
