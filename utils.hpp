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

inline double random_double() {
    // Returns a random double in the interval [0, 1)
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937_64 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common headers
#include "ray.hpp"
#include "vec3.hpp"

#endif