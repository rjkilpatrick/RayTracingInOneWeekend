#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

// Rays are defined parametricallly via $\vec{P}(\vec{b} ; t) = t\vec{b} +
// \vec{A}$ Which is the extension of the 1-D $y(x ; m) = mx + c$ to 3-vectors
// $\vec{A}$ is the point of origin of the ray
// $\vec{b}$ is ray direction vector
// $\t$ parameterises the ray across the ray direction vector ($\vec{b}$)

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction)
        : orig(origin), dir(direction) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    // Point along the ray at given t
    point3 at(double t) const { return (t * dir) + orig; }

private:
    point3 orig;
    vec3 dir;
};

#endif