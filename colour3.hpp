#ifndef COLOUR3_H
#define COLOUR3_H

#include "vec3.hpp"

#include <iostream>

void write_colour(std::ostream &out, colour3 pixel_colour, int samples_per_pixel) {
    auto r = pixel_colour.x();
    auto b = pixel_colour.y();
    auto g = pixel_colour.z();

    // Divide the colour by the number of samples
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    b *= scale;
    g *= scale;

    // Display [0, 255] value of each colour component
    out << static_cast<int>(256 * clamp(r, 0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0, 0.999)) << '\n';
}

#endif