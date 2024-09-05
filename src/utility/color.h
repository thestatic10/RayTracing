#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

using color = vec3;

inline double linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel)
{

    auto scale = 1.0 / samples_per_pixel;
    auto r = linear_to_gamma(pixel_color.x() * scale);
    auto g = linear_to_gamma(pixel_color.y() * scale);
    auto b = linear_to_gamma(pixel_color.z() * scale);

    static const interval valid(0.0, 0.999);

    out << static_cast<int>(256 * valid.clamp(r)) << ' '
        << static_cast<int>(256 * valid.clamp(g)) << ' '
        << static_cast<int>(256 * valid.clamp(b)) << '\n';
}  

#endif