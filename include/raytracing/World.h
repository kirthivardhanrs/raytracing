#pragma once
#include <vector>
#include <utility>
#include "Sphere.h"
#include "Light.h"
#include "Intersection.h"
#include "Color.h"

class Ray;
class Comps;

class World {
public:
	std::vector<Sphere> objects{};
	std::vector<Light> lights{};
	World();
	World(std::vector<Sphere> objects, std::vector<Light> lights)
		: objects(std::move(objects)), lights(std::move(lights))
	{
	}
	Intersections intersect(const Ray& r) const;
	Color shade_hit(const Comps& c) const;
	Color color_at(const Ray& r) const;
	bool is_shadowed(const Tuple& pt) const;
};