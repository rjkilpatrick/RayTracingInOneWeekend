#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

// Rays are defined parametricallly via $\vec{P}(\vec{d} ; \lambda) =
// \lambda\vec{d} + \vec{r}$ Which is the extension of the 1-D $y(x ; m) = mx +
// c$ to 3-vectors
// $\vec{r}$ is the point of origin of the ray
// $\vec{d}$ is ray direction vector
// $\lambda$ parameterises the ray across the ray direction vector ($\vec{d}$)

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction, const double& time = 0.0)
        : orig(origin), dir(direction), t(time) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }
    double time() const { return t; }

    // Point along the ray at given t
    point3 at(double t) const { return (t * dir) + orig; }

private:
    point3 orig;
    vec3 dir;
    double t; // Time
};

#endif