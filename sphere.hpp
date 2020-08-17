#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
public:
    sphere() {}

    sphere(point3 cen, double r, std::shared_ptr<material> m)
        : centre(cen), radius(r), mat_ptr(m){};

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;
    virtual bool bounding_box(double t0, double t1,
                              aabb& output_box) const override;

public:
    point3 centre;
    double radius;
    std::shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max,
                 hit_record& rec) const {
    vec3 oc = r.origin() - centre;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c; // $$b^2 - 4ac$$

    if (discriminant >
        0) { // Grazing incidence, i.e. discriminant == 0, don't count
        auto root = sqrt(discriminant);

        auto t = (-half_b - root) / a; // Quadratic formula
        if (t < t_max && t > t_min) {
            rec.t = t;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - centre) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }

        // Ease of understanding chosen over putting it into a function
        t = (-half_b + root) / a;
        if (t < t_max && t > t_min) {
            rec.t = t;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - centre) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(double t0, double t1, aabb& output_box) const {
    output_box = aabb(centre + (-radius * vec3(1, 1, 1)),
                      centre + (radius * vec3(1, 1, 1)));
    return true;
}

#endif