#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, double r) : centre(cen), radius(r) {};

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

private:
    point3 centre;
    double radius;
};


bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    vec3 oc = r.origin() - centre;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c; // $$b^2 - 4ac$$

    if (discriminant > 0) { // Grazing incidence, i.e. discriminant == 0, don't count
        auto root = std::sqrt(discriminant);

        auto t = (-half_b - root) / a; // Quadratic formula
        if (t < tmax && t > tmin) {
            rec.t = t;
            rec.p = r.at(rec.t);
            vec3 outward_normal = unit_vector(rec.p - centre);
            rec.set_face_normal(r, outward_normal);
            return true;
        }
        
        // Ease of understanding chosen over putting it into a function
        t = (-half_b + root) / a;
        if (t < tmax && t > tmin) {
            rec.t = t;
            rec.p = r.at(rec.t);
            rec.normal = unit_vector(rec.p - centre);
            return true;
        }
    }
    return false;
}

#endif