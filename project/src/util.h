#ifndef FLIGHTNET_UTIL_H
#define FLIGHTNET_UTIL_H

#include <cstddef>
#include <unordered_map>

/**
 *
 * @param low
 * @param high
 * @return Random number of type double between low and high
 */
double RandDouble(double low, double high);

/**
 *
 * @param low
 * @param high
 * @return Random number of type unsigned int between low and high
 */
unsigned int RandUInt(unsigned int low, unsigned int high);

/**
 *
 * @param low
 * @param high
 * @return Random number of type size_t between low and high
 */
size_t RandSize(size_t low, size_t high);

/**
 * Returns the value associated to the key, or a default value if not found
 */
template<typename K, typename V> V map_get_or_default(std::unordered_map<K, V>& map, K key, V def) {
    if (map.contains(key)) {
        return map[key];
    }
    return def;
}

/**
 * Adds to an element of a map, or inserts it if not present
 */
template<typename K, typename V> void map_add_or_insert(std::unordered_map<K, V>& map, K key, V value) {
    if (map.contains(key)) {
        map[key] = map[key] + value;
    } else {
        map.emplace(key, value);
    }
}

/**
 * Subtracts from an element of a map, and removes it if it's 0
 */
template<typename K, typename V> void map_subtract_or_remove(std::unordered_map<K, V>& map, K key, V sub) {
    if (!map.contains(key)) {
        return;
    }

    V new_val = map[key] - sub;

    map.erase(key);
    if (new_val != 0) {
        map.emplace(key, new_val);
    }
}

#endif  // FLIGHTNET_UTIL_H
