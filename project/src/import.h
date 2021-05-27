#ifndef FLIGHTNET_IMPORT_H
#define FLIGHTNET_IMPORT_H

#include <filesystem>
#include <map>
#include <unordered_map>

#include "airport.h"

/**
 * Reads the information about the airports (names, distance, location) and their connections needed to create
 * the graph's nodes and edges
 * @param names_path
 * @param locations_path
 * @param edges_path
 * @return The map of airport ID -> Airport, and the map of airport ID -> connections to other airports and their length
 */
std::pair<std::map<AirportID, Airport>, std::unordered_multimap<AirportID, std::pair<AirportID, double>>>
import_airports(const std::filesystem::path& names_path, const std::filesystem::path& locations_path,
    const std::filesystem::path& edges_path);

#endif  // FLIGHTNET_IMPORT_H
