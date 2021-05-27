#ifndef FLIGHTNET_PLANE_H
#define FLIGHTNET_PLANE_H

#include "airport.h"

/**
 * Represents a single plane
 */
class Plane {
public:
    /**
     * @brief Constructor of Plane
     * @param capacity Number of passengers the plane can carry
     * @param initial_airport
     */
    Plane(unsigned int capacity, AirportID initial_airport);

    static constexpr double max_hours_for_tripulation = 10.0;  // h

    // Specs of a Boeing 747-8
    static constexpr double average_velocity = 914 / 3.6;  // m/s
    static constexpr double fuel_capacity = 238.6;  // m³
    static constexpr double range = 14320000.0;  // m
    [[nodiscard]] static constexpr double getMaxDistanceBetweenAirports() {
        return std::min(range, average_velocity * max_hours_for_tripulation * 3600);
    }

    /**
     *
     * @return Initial airport of the plane
     */
    AirportID getInitialAirport() const;

    /**
     *
     * @return Unordered map of the passengers in the plane and their destinations
     */
    std::unordered_map<AirportID, unsigned int>& getPassengers();

    /**
     * @return Available seats in the plane
     */
    [[nodiscard]] unsigned int availableSeats() const;

    /**
     * @return The plane's capacity
     */
    unsigned int getCapacity() const;

private:
    AirportID initial_airport;
    unsigned int capacity;
    std::unordered_map<AirportID, unsigned int> passengers;
};


#endif  // FLIGHTNET_PLANE_H
