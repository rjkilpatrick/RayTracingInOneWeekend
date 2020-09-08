#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.hpp"
#include "stb_image.h"
#include "utils.hpp"

#include <iostream>

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

class image_texture : public texture {
public:
    const static int bytes_per_pixel = 3;

    image_texture()
        : data(nullptr), width(0), height(0), bytes_per_scanline(0){};

    image_texture(const char* filename) {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(filename, &width, &height, &components_per_pixel,
                         components_per_pixel);

        if (!data) {
            std::cerr << "ERROR: Could not load texture image file `"
                      << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture() { delete data; }

    virtual colour3 value(double u, double v, const vec3& p) const override {
        if (data == nullptr) {
            return colour3(1, 0, 1); // Magenta texture not found
        }

        // Clamp input texture co-ordinates to [0,1]^2
        u = clamp(u, 0.0, 1.0);
        v = clamp(v, 0.0, 1.0);

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // Clamp integer mapping
        if (i >= width)
            i = width - 1;
        if (j >= height)
            j = height - 1;

        const auto colour_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return colour3(colour_scale * pixel[0], colour_scale * pixel[1],
                       colour_scale * pixel[2]);
    }

private:
    unsigned char* data;
    int width, height;
    int bytes_per_scanline;
};

#endif