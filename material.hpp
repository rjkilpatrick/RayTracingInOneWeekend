#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.hpp"
#include "texture.hpp"
#include "utils.hpp"
#include "vec3.hpp"

double schlick(double cosine, double ior);

class material {
public:
    // All materials emit "black" unless otherwise spoken for
    virtual colour3 emitted(double u, double v, const point3& p) const {
        return colour3{0, 0, 0};
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         colour3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const colour3& a) : albedo(std::make_shared<solid_colour>(a)) {}
    lambertian(std::shared_ptr<texture> a) : albedo(a){};

    // Does scatter
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         colour3& attenuation, ray& scattered) const override {
        // Returns a random unit vector with a lambertian distribution

        vec3 scatter_direction = rec.normal + lambertian_unit_vector();
        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    std::shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(const colour3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    // Whether the reflected ray is outside of the surface
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         colour3& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    colour3 albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double refractive_index) : ior(refractive_index) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         colour3& attenuation, ray& scattered) const override {
        attenuation = colour3(1.0, 1.0, 1.0);
        double eta_i_over_eta_r = rec.front_face ? (1.0 / ior) : ior;

        vec3 ray_hat =
            unit_vector(r_in.direction()); // Ray propogation direction

        double cos_theta = fmin(dot(-ray_hat, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        double reflect_prob = schlick(cos_theta, eta_i_over_eta_r);

        if (eta_i_over_eta_r * sin_theta > 1.0 // Total Internal Reflection
            || random_double() < reflect_prob  // Reflectivity of material
        ) {
            vec3 reflected = reflect(ray_hat, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }
        vec3 refracted = refract(ray_hat, rec.normal, eta_i_over_eta_r);
        scattered = ray(rec.p, refracted);
        return true;
    }

public:
    double ior;
};

/**
 * @brief Simluates phong illumination model
 *
 */
class phong : public material {
public:
    phong(const colour3 a) : albedo(a){};

    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         colour3& attenuation, ray& scattered) const override {
        return true;
    }

private:
    colour3 albedo;
};

class diffuse_light : public material {
public:
    diffuse_light(std::shared_ptr<texture> a) : emit(a){};
    diffuse_light(colour3 c) : emit(std::make_shared<solid_colour>(c)){};

    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         colour3& attenuation, ray& scattered) const override {
        return false;
    }

    virtual colour3 emitted(double u, double v,
                            const point3& p) const override {
        return emit->value(u, v, p);
    }

public:
    std::shared_ptr<texture> emit;
};

// Uses the Schick approximation for a dielectric
double schlick(double cosine, double ior) {
    auto r0 = (1 - ior) / (1 + ior);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

#endif