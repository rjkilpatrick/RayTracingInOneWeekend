#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

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

// Common headers
#include "ray.hpp"
#include "vec3.hpp"

#endif