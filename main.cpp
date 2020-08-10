#include "utils.hpp"

#include "colour3.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

#include <iostream>

colour3 ray_colour(const ray& r, const hittable& world, int bounces_remaining) {
    if (bounces_remaining <= 0) {
        return colour3(0, 0, 0);
    }

    hit_record rec;

    const double EPSILON = 0.001; // The amount a ray must be in front of the object

    if (world.hit(r, EPSILON, infinity, rec)) {
        point3 target = rec.p + rec.normal + lambertian_unit_vector();
        return 0.5 * ray_colour(ray(rec.p, target - rec.p), world, bounces_remaining -1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * colour3(1.0, 1.0, 1.0) + (t * colour3(0.5, 0.7, 1.0));
}

// Generates an image in PPM Image Format
// Prints output to stdout
int main() {
    
    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 40;

    // World

    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera

    camera cam;

    // Render

    const int max_bounces = 50;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            colour3 pixel_colour {0, 0, 0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = double(i + random_double()) / (image_width - 1);
                auto v = double(j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_colour += ray_colour(r, world, max_bounces);
            }

            write_colour(std::cout, pixel_colour, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}