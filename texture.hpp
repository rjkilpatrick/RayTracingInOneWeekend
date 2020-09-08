#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.hpp"
#include "utils.hpp"

class texture {
public:
    virtual colour3 value(double u, double v, const point3& p) const = 0;
};

class solid_colour : public texture {
public:
    solid_colour(){};
    solid_colour(colour3 a) : albedo_colour(a) {}

    solid_colour(double red, double green, double blue)
        : solid_colour(colour3(red, green, blue)){};

    virtual colour3 value(double u, double v, const point3& p) const override {
        return albedo_colour;
    }

private:
    colour3 albedo_colour;
};

class checker_texture : public texture {
public:
    checker_texture(){};

    checker_texture(std::shared_ptr<texture> t0, std::shared_ptr<texture> t1)
        : even(t0), odd(t1){};

    checker_texture(colour3 c1, colour3 c2)
        : even(std::make_shared<solid_colour>(c1)),
          odd(std::make_shared<solid_colour>(c2)) {}

    virtual colour3 value(double u, double v, const point3& p) const override {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0) {
            return odd->value(u, v, p);
        } else {
            return even->value(u, v, p);
        }
    }

public:
    std::shared_ptr<texture> odd;
    std::shared_ptr<texture> even;
};

class noise_texture : public texture {
public:
    noise_texture() {}
    noise_texture(double sf) : scale(sf){};

    virtual colour3 value(double u, double v, const point3& p) const override {
        // return colour3(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
        // return colour3(1, 1, 1) * noise.turb(scale * p);
        return colour3(1, 1, 1) * 0.5 *
               (1 + sin(scale * p.z() + 10 * noise.turb(p)));
    }

private:
    perlin noise;
    double scale;
};

#endif