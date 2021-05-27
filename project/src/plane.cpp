#include "plane.h"

Plane::Plane(unsigned int capacity, AirportID initial_airport):
    initial_airport{initial_airport}, capacity{capacity}, passengers{} {}

unsigned int Plane::getCapacity() const {
    return capacity;
}

AirportID Plane::getInitialAirport() const {
    return initial_airport;
}

std::unordered_map<AirportID, unsigned int>& Plane::getPassengers() {
    return passengers;
}

unsigned int Plane::availableSeats() const {
    auto available = capacity;
    for (const auto& pair : passengers) {
        available -= pair.second;
    }
    return available;
}
