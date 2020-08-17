#include "utils.hpp"

#include "camera.hpp"
#include "colour3.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "moving_sphere.hpp"
#include "sphere.hpp"
#include "texture.hpp"

#include <iostream>

hittable_list random_scene() {
    hittable_list world;

    auto checker = std::make_shared<checker_texture>(colour3(0.2, 0.3, 0.1),
                                                     colour3(0.9, 0.9, 0.9));
    auto ground_material = std::make_shared<lambertian>(checker);
    world.add(
        std::make_shared<sphere>(point3(0, -1000.5, 0), 1000, ground_material));

    // Draw 484 small spheres approximating a grid
    // for (int j = -11; j < 11; ++j) {
    //     for (int i = -11; i < 11; ++i) {
    //         auto material_distribution = random_double();
    //         point3 sphere_centre{i + 0.9 * random_double(), 0.2,
    //                              j + 0.9 * random_double()};

    //         if ((sphere_centre - point3{4, 0.2, 0}).length() > 0.9) {
    //             std::shared_ptr<material> sphere_material;

    //             if (material_distribution < 0.8) {
    //                 // Lambertian
    //                 auto albedo =
    //                     colour3::random() *
    //                     colour3::random(); // What does this do to the
    //                                        // probability distributions
    //                 sphere_material = std::make_shared<lambertian>(albedo);

    //                 auto end_point = sphere_centre + vec3(0, random_double(0,
    //                 0.5), 0);

    //                 // world.add(std::make_shared<sphere>(sphere_centre, 0.2,
    //                 //                                    sphere_material));
    //                 world.add(std::make_shared<moving_sphere>(sphere_centre,
    //                 end_point, 0.0, 1.0, 0.2, sphere_material));
    //             } else if (material_distribution < 0.95) {
    //                 // Metal
    //                 auto albedo = colour3::random(0.5, 1);
    //                 auto fuzz = random_double(0, 0.5);
    //                 sphere_material = std::make_shared<metal>(albedo, fuzz);
    //                 world.add(std::make_shared<sphere>(sphere_centre, 0.2,
    //                                                    sphere_material));
    //             } else {
    //                 // Glass
    //                 sphere_material = std::make_shared<dielectric>(1.5);
    //                 world.add(std::make_shared<sphere>(sphere_centre, 0.2,
    //                                                    sphere_material));
    //             }
    //         }
    //     }
    // }

    // Draw big spheres
    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3{0, 1, 0}, 1.0, material1));

    auto material2 = std::make_shared<lambertian>(colour3{0.4, 0.2, 0.1});
    world.add(std::make_shared<sphere>(point3{-4, 1, 0}, 1.0, material2));

    auto material3 = std::make_shared<metal>(colour3{0.7, 0.6, 0.5}, 0.0);
    world.add(std::make_shared<sphere>(point3{4, 1, 0}, 1.0, material3));

    return world;
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = std::make_shared<checker_texture>(colour3{0.2, 0.3, 0.1},
                                                     colour3{0.9, 0.9, 0.9});

    objects.add(std::make_shared<sphere>(
        point3(0, -10, 0), 10, std::make_shared<lambertian>(checker)));
    objects.add(std::make_shared<sphere>(
        point3(0, 10, 0), 10, std::make_shared<lambertian>(checker)));

    return objects;
}

colour3 ray_colour(const ray& r, const hittable& world, int bounces_remaining) {
    if (bounces_remaining <= 0) {
        return colour3(0, 0, 0);
    }

    hit_record rec;

    if (world.hit(r, EPSILON, infinity, rec)) {
        ray scattered; // New ray generated
        colour3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation *
                   ray_colour(scattered, world, bounces_remaining - 1);
        }
        return colour3(0, 0, 0);
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
    const int samples_per_pixel = 100;
    const int max_bounces = 50;

    // World

    hittable_list world;
    point3 look_from;
    point3 look_to;
    auto fov = 40.0;
    auto aperture = 0.0;

    switch (0) {
    case 1:
        world = random_scene();
        look_from = point3(13, 2, 3);
        fov = 20.0;
        aperture = 0.1;
        break;
    case 2:
    default:
        world = two_spheres();
        look_from = point3(13, 2, 3);
        look_to = point3(0, 0, 0);
        fov = 20.0;
        break;
    }

    // Camera
    vec3 UP{0, 1, 0};
    auto dist_to_focus = 10.0;

    camera cam{look_from, look_to,       UP,  fov, aspect_ratio,
               aperture,  dist_to_focus, 0.0, 0.0};

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            colour3 pixel_colour{0, 0, 0};
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