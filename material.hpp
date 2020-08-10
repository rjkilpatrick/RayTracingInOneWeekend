#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.hpp"

#include "hittable.hpp" // Hittable contains the full class
// struct hit_record;

class material {
public:
    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,
        colour3& attenuation,
        ray& scattered
    ) const = 0;
};

class lambertian : public material {
public:
    lambertian(const colour3& a) : albedo(a) {}
    
    // Does scatter
    virtual bool scatter(const ray& r_in, const hit_record& rec, 
        colour3& attenuation, ray& scattered
    ) const override {
        // Returns a random unit vector with a lambertian distribution
        
        vec3 scatter_direction = rec.normal + lambertian_unit_vector();
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    colour3 albedo;
};

class metal : public material {
public:
    metal(const colour3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    // Whether the reflected ray is outside of the surface
    virtual bool scatter(const ray& r_in, const hit_record& rec,
        colour3& attenuation, ray& scattered
    )const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    
public:
    colour3 albedo;
    double fuzz;
};

#endif