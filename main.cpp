#include "utils.hpp"

#include "aarect.hpp"
#include "camera.hpp"
#include "colour3.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "moving_sphere.hpp"
#include "sphere.hpp"
#include "texture.hpp"
#include "vec3.hpp"

#include <iostream>
#include <memory>

hittable_list cornell_box() {
    hittable_list objects;

    auto red = std::make_shared<lambertian>(colour3{0.65, 0.05, 0.05});
    auto white = std::make_shared<lambertian>(colour3{0.73, 0.73, 0.73});
    auto green = std::make_shared<lambertian>(colour3{0.12, 0.45, 0.15});
    auto light = std::make_shared<diffuse_light>(colour3{15, 15, 15});

    // Make Cornell box
    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    return objects;
}

hittable_list simple_light() {
    hittable_list objects;

    auto perlin_tex = std::make_shared<noise_texture>(4);
    // Make "floor"
    objects.add(std::make_shared<sphere>(
        point3{0, -1000, 0}, 1000, std::make_shared<lambertian>(perlin_tex)));
    // Make main sphere
    objects.add(std::make_shared<sphere>(
        point3{0, 2, 0}, 2, std::make_shared<lambertian>(perlin_tex)));

    auto diff_light = std::make_shared<diffuse_light>(colour3{4, 4, 4});
    // objects.add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, diff_light));
    // objects.add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, diff_light));
    objects.add(std::make_shared<yz_rect>(-3, -3, -3, 3, -2, diff_light));

    objects.add(std::make_shared<sphere>(point3{0, 8, 0}, 2, diff_light));

    return objects;
}

hittable_list earth() {
    auto earth_texture = std::make_shared<image_texture>("./img/earthmap.jpg");
    auto earth_surface = std::make_shared<lambertian>(earth_texture);
    auto globe = std::make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = std::make_shared<noise_texture>(4);
    objects.add(std::make_shared<sphere>(
        point3(0, -1000, 0), 1000, std::make_shared<lambertian>(pertext)));
    objects.add(std::make_shared<sphere>(
        point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

    return objects;
}

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

colour3 ray_colour(const ray& r, const colour3& background,
                   const hittable& world, int bounces_remaining) {
    // Can't bounce anymore!
    if (bounces_remaining <= 0) {
        return colour3(0, 0, 0);
    }

    hit_record rec;

    if (!world.hit(r, EPSILON, infinity, rec)) {
        return background;
    }

    ray scattered; // New ray generated
    colour3 attenuation;
    colour3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        return emitted;
    }

    return emitted + attenuation * ray_colour(scattered, background, world,
                                              bounces_remaining - 1);
}

// Generates an image in PPM Image Format
// Prints output to stdout
int main() {

    stbi_set_flip_vertically_on_load(true);

    // Image default values
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    int max_bounces = 50;

    // World

    hittable_list world;
    point3 look_from;
    point3 look_to;
    auto fov = 40.0;
    auto aperture = 0.0;
    colour3 background{0, 0, 0};

    switch (0) {
    case 1:
        world = random_scene();
        background = colour3{0.7, 0.8, 1.0};
        look_from = point3(13, 2, 3);
        fov = 20.0;
        aperture = 0.1;
        break;
    case 2:
        world = two_spheres();
        background = colour3{0.7, 0.8, 1.0};
        look_from = point3(13, 2, 3);
        look_to = point3(0, 0, 0);
        fov = 20.0;
        break;
    case 3:
        world = two_perlin_spheres();
        background = colour3{0.7, 0.8, 1.0};
        look_from = point3(13, 2, 3);
        look_to = point3(0, 0, 0);
        fov = 20.0;
        break;
    case 4:
        world = earth();
        background = colour3{0.7, 0.8, 1.0};
        look_from = point3(13, 2, 3);
        look_to = point3(0, 0, 0);
        fov = 20.0;
        break;
    case 5:
        world = simple_light();
        background = colour3{0., 0., 0.};
        samples_per_pixel = 400;
        look_from = point3{26, 3, 6};
        look_to = point3{0, 2, 0};
        fov = 20.;
        break;
    case 6:
    default:
        world = cornell_box();
        aspect_ratio = 1.0;
        image_width = 300;
        samples_per_pixel = 200;
        background = colour3{0, 0, 0};
        look_from = point3{278, 278, -800};
        look_to = point3{278, 278, 0};
        fov = 40.0;
        break;
    }

    // Camera
    vec3 UP{0, 1, 0};
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);

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
                pixel_colour += ray_colour(r, background, world, max_bounces);
            }

            write_colour(std::cout, pixel_colour, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}