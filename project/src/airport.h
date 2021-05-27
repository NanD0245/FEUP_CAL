#ifndef FLIGHTNET_AIRPORT_H
#define FLIGHTNET_AIRPORT_H

#include <array>
#include <string_view>
#include <unordered_map>

constexpr size_t AIRPORT_NAME_LENGTH = 3;

using AirportID = unsigned int;

/**
 * Represents an airport
 */
class Airport {
public:
    /**
     * constructor of Airport
     *
     * @param name
     * @param latitude
     * @param longitude
     */
    Airport(const std::string_view& name, double latitude, double longitude);

    /**
     * @return latitude of the Airport
     */
    double latitude() const;

    /**
     * @return longitude of the Airport
     */
    double longitude() const;

    /**
     * True if the planes are allowed to land on this airport (for example,
     * if atmospheric conditions are favorable), false otherwise
     */
    bool isLandingAllowed() const;

    /**
     * sets permission to land at the airport
     *
     * @param landing
     */
    void setLandingAllowed(bool landing);

    /**
     * @return reference to the airport name
     */
    const std::array<char, AIRPORT_NAME_LENGTH + 1>& name() const;

    /**
     * @return reference to the map of passengers (destination -> number of passengers) that are at the airport
     */
    std::unordered_map<AirportID, unsigned int>& passengers();

    /**
     * @return const reference to the map of passengers (see `passengers()`)
     */
    const std::unordered_map<AirportID, unsigned int>& constPassengers() const;

private:
    double latitude_;
    double longitude_;
    bool landing_allowed_;
    std::array<char, AIRPORT_NAME_LENGTH + 1> name_;
    std::unordered_map<AirportID, unsigned int> passengers_;
};

#endif  // FLIGHTNET_AIRPORT_H
