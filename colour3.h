#ifndef COLOUR3_H
#define COLOUR3_H

#include "vec3.h"

#include <iostream>

void write_colour(std::ostream &out, colour3 pixel_colour) {
    // Display [0, 255) value of each colour component
    out << static_cast<int>(255.999 * pixel_colour.x()) << ' '
        << static_cast<int>(255.999 * pixel_colour.y()) << ' '
        << static_cast<int>(255.999 * pixel_colour.z()) << '\n';
}

#endif