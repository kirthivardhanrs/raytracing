#pragma once
#include <limits>
#include <vector>
#include "Matrix.h"

namespace mathutils {
    constexpr double pos_inf = std::numeric_limits<double>::infinity();
    constexpr double neg_inf = -std::numeric_limits<double>::infinity();
    constexpr double EPSILON{ 0.00001 };
    const Matrix identity{ std::vector<std::vector<double>>{ {1,0,0,0},{ 0,1,0,0 },{ 0,0,1,0 },{ 0,0,0,1 }} };
    bool epsEqual(double a, double b);
}