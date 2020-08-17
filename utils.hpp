#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <cstdlib> // Might not be using this? TODO: Check
#include <limits>
#include <memory>
#include <random>

// I am electing to not use the `using` statements as I do not know the
// standard library well enough to assume things

// Constants
const double infinity = std::numeric_limits<double>::infinity();

// Utility functions
inline double degrees_to_radians(double degrees) {
    return degrees * M_PI / 180.0;
}

inline double radians_to_degrees(double radians) {
    return radians * 180.0 / M_PI;
}

// Returns a random double in the interval [0, 1]
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937_64 generator;
    return distribution(generator);
}

// Returns a random double in the interval [min, max]
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

// Return an int in range [min, max]
inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max + 1));
}

inline double clamp(double x, double min, double max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

const double EPSILON = 1e-3; // The smallest amount of distance

/**
 * @brief Returns sign of a number
 * 
 * +1 for positive values, -1 for negative values and 0 for (or less than \c EPSILON ).
 * @param x To determine sign of
 * @return int +1, 0, or -1 depending on sign of \c val 
 */
inline int sign(double x) {
    if (std::abs(x) < EPSILON)
        return 0;
    if (x < 0)
        return -1;
    return 1;
}

// Common headers
#include "ray.hpp"
#include "vec3.hpp"

#endif