#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "utils.hpp"
#include "hittable.hpp"

#include <memory>
#include <vector>

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(std::shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, double tmin, double tmax,
                     hit_record& rec) const override;

    virtual bool bounding_box(double t0, double t1,
                              aabb& output_box) const override;

public:
    std::vector<std::shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double tmin, double tmax,
                        hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = tmax; // The lowest value of t so far

    for (const auto& object : objects) {
        if (object->hit(r, tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(double t0, double t1, aabb& output_box) const {
    if (objects.empty())
        return false;
    
    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(t0, t1, temp_box))
            return false;
        
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

#endif