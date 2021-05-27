#include "airport.h"

Airport::Airport(const std::string_view& name, double latitude, double longitude):
    latitude_{latitude}, longitude_{longitude}, landing_allowed_{true}, name_{}, passengers_{} {
    size_t i = 0;

    for (; i < name.size() && i < AIRPORT_NAME_LENGTH; i++) {
        name_[i] = name[i];
    }

    for (; i < AIRPORT_NAME_LENGTH + 1; i++) {
        name_[i] = '\0';
    }
}

const std::array<char, AIRPORT_NAME_LENGTH + 1>& Airport::name() const {
    return name_;
}

double Airport::latitude() const {
    return latitude_;
}

double Airport::longitude() const {
    return longitude_;
}

std::unordered_map<AirportID, unsigned int>& Airport::passengers() {
    return passengers_;
}

const std::unordered_map<AirportID, unsigned int>& Airport::constPassengers() const {
    return passengers_;
}

bool Airport::isLandingAllowed() const {
    return landing_allowed_;
}

void Airport::setLandingAllowed(bool landing) {
    this->landing_allowed_ = landing;
}
