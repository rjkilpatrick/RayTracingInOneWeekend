#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// Rays are defined parametricallly via $\vec{P}(\lambda) = \lambda\vec{b} + \vec{A}$
// Which is the extension of $y = mx + c$ to 3-vectors
// $\vec{A}$ is the point of origin of the ray
// $\vec{b}$ is ray direction vector
// $\lambda$ parameterises the ray across the ray direction vector ($\vec{b}$)

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    // Point along the ray at given lambda
    point3 at(double lambda) const {
        return (lambda * dir) + orig;
    }
private:
    point3 orig;
    vec3 dir;
};

#endif