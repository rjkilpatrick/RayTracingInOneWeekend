#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// Rays are defined parametricallly via $$\vec{Y}(\lambda) = \lambda\vec{X} + \vec{c}$$
// Which is the extension of $$y = mx + c$$ to 3-vectors
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
public:
    point3 orig;
    vec3 dir;
};

#endif