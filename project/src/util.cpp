#include "util.h"

#include <ctime>
#include <random>

// Adapted from http://www.stroustrup.com/C++11FAQ.html#std-random
double RandDouble(double low, double high) {
    static std::mt19937 re(static_cast<unsigned long>(time(nullptr)));
    using Dist = std::uniform_real_distribution<double>;
    static Dist uid{};
    return uid(re, Dist::param_type{low, high});
}

unsigned int RandUInt(unsigned int low, unsigned int high) {
    static std::mt19937 re(static_cast<unsigned long>(time(nullptr)));
    using Dist = std::uniform_int_distribution<unsigned int>;
    static Dist uid{};
    return uid(re, Dist::param_type{low, high});
}

size_t RandSize(size_t low, size_t high) {
    static std::mt19937 re(static_cast<unsigned long>(time(nullptr)));
    using Dist = std::uniform_int_distribution<size_t>;
    static Dist uid{};
    return uid(re, Dist::param_type{low, high});
}
