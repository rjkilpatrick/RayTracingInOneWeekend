#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "utils.hpp"

#include "hittable.hpp"

class moving_sphere : public hittable {
public:
    moving_sphere(){};
    moving_sphere(point3 centre_initial, point3 centre_end, double t0,
                  double t1, double radius, std::shared_ptr<material> m)
        : centre_initial(centre_initial), centre_end(centre_end),
          time_initial(t0), time_end(t1), radius(radius), mat_ptr(m){};

    virtual bool hit(const ray& ray, double t_min, double t_max,
                     hit_record& rec) const override;

    virtual bool bounding_box(double t0, double t1,
                              aabb& output_box) const override;

    point3 centre(double time) const;

public:
    point3 centre_initial, centre_end;
    double time_initial, time_end;
    double radius;
    std::shared_ptr<material> mat_ptr;
};

point3 moving_sphere::centre(double time) const {
    return centre_initial +
           ((time - time_initial) / (time_end - time_initial)) *
               (centre_end - centre_initial);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max,
                        hit_record& rec) const {
    vec3 oc = r.origin() - centre(r.time());
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
            auto outward_normal = (rec.p - centre(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }

        // Ease of understanding chosen over putting it into a function
        t = (-half_b + root) / a;
        if (t < t_max && t > t_min) {
            rec.t = t;
            rec.p = r.at(rec.t);
            auto outward_normal = (rec.p - centre(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool moving_sphere::bounding_box(double t0, double t1, aabb& output_box) const {
    aabb box0{centre(t0) - radius * vec3(1, 1, 1),
              centre(t0) + radius * vec3(1, 1, 1)};
    aabb box1{centre(t1) - radius * vec3(1, 1, 1),
              centre(t1) + radius * vec3(1, 1, 1)};
    output_box = surrounding_box(box0, box1);
    return true;
}

#endif