/**
 * @file aabb.hpp
 * @author @rjkilpatrick
 * @brief All about an axis aligned bounding box
 * @version 0.1
 * @date 2020-08-17
 *
 */
#ifndef AABB_H
#define AABB_H

#include "utils.hpp"

class aabb {
public:
    aabb(){};
    aabb(const point3& a, const point3& b) {
        _min = a;
        _max = b;
    };

    point3 min() const { return _min; }
    point3 max() const { return _max; }

    inline bool hit(const ray& ray, double t_min, double t_max) const {
        for (int i = 0; i < 3; ++i) {
            auto inverseDirection = 1.0 / ray.direction()[i];
            auto t0 = (_min[i] - ray.origin()[i]) * inverseDirection;
            auto t1 = (_max[i] - ray.origin()[i]) * inverseDirection;

            if (inverseDirection < 0.0) {
                std::swap(t0, t1);
            }

            t_min = fmax(t0, t_min);
            t_max = fmin(t1, t_max);
            if (t_max <= t_min) {
                return false;
            }
        }
        return true;
    }

public:
    point3 _min;
    point3 _max;
};

/**
 * @brief Creates a box that bounds both of the AABBs given
 *
 * @param box0
 * @param box1
 * @return aabb An Axis-aligned bounding box that bounds both box0 and box1
 */
aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(fmin(box0.min(), box1.min()));
    point3 big(fmax(box0.max(), box1.max()));

    return aabb(small, big);
}

#endif
