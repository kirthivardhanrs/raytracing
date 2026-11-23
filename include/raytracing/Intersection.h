#pragma once
#include "Comps.h"
#include "mathutils.h"
#include <vector>

class Ray;
class Sphere;

class Intersection {
public:
    double t{ mathutils::pos_inf };
    const Sphere* s{ nullptr };
    Intersection() = default;

    Intersection(double t, const Sphere& s)
        : t(t), s(&s)
    {
    }
    Comps prepare_comps(const Ray& r) const;
};

using Intersections = std::vector<Intersection>;
Intersection hit(const Intersections& xs);