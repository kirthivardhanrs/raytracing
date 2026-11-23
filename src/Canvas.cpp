#include "raytracing/Canvas.h"
#include "raytracing/Color.h"
#include <cmath>
#include <fstream>
#include <format>
#include <iostream>
#include <algorithm>
#include <string>

Color& Canvas::operator()(std::size_t i, std::size_t j) {
    return pixels[j][i];
}

const Color& Canvas::operator()(std::size_t i, std::size_t j) const {
    return pixels[j][i];
}

void Canvas::toPPM() const {
	std::ofstream f{ "image.ppm" };
    if (!f) {
        std::cerr << "cannot load file\n";
    }
    f << std::format("P3\n{} {}\n255\n", width, height);
    for (const auto& row : pixels) {
        int rowChars{ 0 };
        for (const auto& pixel : row) {
            // use std::round for precise rounding
            int clamped_r{ std::clamp(static_cast<int>(std::round(pixel.r * 255)), 0, 255) };
            int clamped_g{ std::clamp(static_cast<int>(std::round(pixel.g * 255)), 0, 255) };
            int clamped_b{ std::clamp(static_cast<int>(std::round(pixel.b * 255)), 0, 255) };
            for (int val : { clamped_r, clamped_g, clamped_b }) {

                int digits{ static_cast<int>(std::to_string(val).size()) };

                // accounting for the space
                int needed{ digits + (rowChars > 0 ? 1 : 0) };

                if (rowChars + needed > 70) {
                    f << '\n';
                    rowChars = 0;
                }
                else if (rowChars > 0) {
                    f << ' ';
                    ++rowChars;
                }

                f << val;
                rowChars += digits;
            }
        }
        f << '\n';
    }
}