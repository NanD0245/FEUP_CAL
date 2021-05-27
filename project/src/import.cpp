#include "import.h"

#include <cassert>
#include <fstream>
#include <string_view>

std::pair<std::map<AirportID, Airport>, std::unordered_multimap<AirportID, std::pair<AirportID, double>>>
import_airports(const std::filesystem::path& names_path, const std::filesystem::path& locations_path,
    const std::filesystem::path& edges_path) {
    if (!is_regular_file(names_path)) {
        std::string err = "The supplied path for the names file (";
        err.append(names_path.string());
        err.append(") doesn't refer to a valid file.");
        throw std::invalid_argument(err);
    }

    if (!is_regular_file(locations_path)) {
        std::string err = "The supplied path for the locations file (";
        err.append(locations_path.string());
        err.append(") doesn't refer to a valid file.");
    }

    if (!is_regular_file(edges_path)) {
        std::string err = "The supplied path for the edges file (";
        err.append(edges_path.string());
        err.append(") doesn't refer to a valid file.");
    }

    std::map<AirportID, Airport> airports;
    std::unordered_map<AirportID, AirportID> id_map;
    std::ifstream locations_file(locations_path);

    std::string line;
    std::getline(locations_file, line);

    std::unordered_map<AirportID, std::pair<double, double>> locations;
    while (std::getline(locations_file, line)) {
        std::string_view line_view{line};
        line_view.remove_prefix(1);
        line_view.remove_suffix(1);

        auto comma_pos = line_view.find_first_of(',');
        assert(comma_pos != line_view.npos);

        std::string_view latitude_view = line_view;
        latitude_view.remove_prefix(comma_pos + 1);
        line_view.remove_suffix(line_view.size() - comma_pos);

        comma_pos = latitude_view.find_first_of(',');
        assert(comma_pos != line_view.npos);

        std::string_view longitude_view = latitude_view;
        longitude_view.remove_prefix(comma_pos + 1);
        latitude_view.remove_suffix(latitude_view.size() - comma_pos);

        AirportID id = static_cast<AirportID>(std::stoul(std::string(line_view)));
        double latitude = std::stod(std::string(latitude_view));
        double longitude = std::stod(std::string(longitude_view));

        locations.emplace(id, std::make_pair(latitude, longitude));
    }

    AirportID id = 0;
    std::ifstream names_file(names_path);
    std::getline(names_file, line);
    while (std::getline(names_file, line)) {
        std::string_view line_view{line};
        line_view.remove_prefix(1);
        line_view.remove_suffix(1);

        auto comma_pos = line_view.find_first_of(',');
        assert(comma_pos != line_view.npos);

        std::string_view name_view = line_view;
        name_view.remove_prefix(comma_pos + 1);
        line_view.remove_suffix(line_view.size() - comma_pos);

        AirportID original_id = static_cast<AirportID>(std::stoul(std::string(line_view)));
        std::pair<double, double> location = locations.at(original_id);
        id_map.emplace(original_id, id);

        airports.emplace(id, Airport(name_view, location.first, location.second));
        id++;
    }

    std::unordered_multimap<AirportID, std::pair<AirportID, double>> edges;
    std::ifstream edges_file(edges_path);

    std::getline(edges_file, line);
    while (std::getline(edges_file, line)) {
        std::string_view line_view{line};
        line_view.remove_prefix(1);
        line_view.remove_suffix(1);

        auto comma_pos = line_view.find_first_of(',');
        assert(comma_pos != line_view.npos);

        std::string_view id2_view = line_view;
        id2_view.remove_prefix(comma_pos + 1);
        line_view.remove_suffix(line_view.size() - comma_pos);

        comma_pos = id2_view.find_first_of(',');
        assert(comma_pos != line_view.npos);

        std::string_view distance_view = id2_view;
        distance_view.remove_prefix(comma_pos + 1);
        id2_view.remove_suffix(id2_view.size() - comma_pos);

        AirportID from = id_map.at(static_cast<AirportID>(std::stoul(std::string(line_view))));
        double distance = std::stod(std::string(distance_view));
        AirportID to = id_map.at(static_cast<AirportID>(std::stoul(std::string(id2_view))));

        edges.emplace(from, std::make_pair(to, distance));
    }

    return std::make_pair(std::move(airports), std::move(edges));
}
