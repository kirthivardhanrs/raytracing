#include "raytracing/World.h"
#include "raytracing/Sphere.h"
#include "raytracing/Light.h"
#include "raytracing/Ray.h"
#include "raytracing/Comps.h"
#include "raytracing/Intersection.h"
#include "raytracing/transform.h"
#include <algorithm>
#include <iostream>
#include "raytracing/Color.h"

World::World()
{
	Sphere s1{}, s2{};
	s1.material.color = { 0.8, 1.0, 0.6 };
	s1.material.diffuse = 0.7;
	s1.material.specular = 0.2;
	s2.set_transform(transform::scale(0.5, 0.5, 0.5));

	Light light{ {1,1,1}, {-10,10,-10,1} };
	objects = { s1, s2 };
	lights = { light };
}

Intersections World::intersect(const Ray& r) const {
	Intersections res{};
	res.reserve(objects.size() * 2);// assuming max 2 xs per object
	for (const auto& object : objects) {
		Intersections xs{ r.intersect(object) };
		res.insert(res.end(), xs.begin(), xs.end());
	}
	std::sort(res.begin(), res.end(), [](const auto& a, const auto& b) {
		return a.t < b.t;
	});
	return res;
}

Color World::shade_hit(const Comps& c) const {
	return this->lights[0].lighting( //tmp
		c.s->material,
		c.pt,
		c.eyev,
		c.normalv,
		is_shadowed(c.over_pt)
	);
}

Color World::color_at(const Ray& r) const {
	Intersections xs{ this->intersect(r) };
	Intersection h{ hit(xs) }; // tmp
	if (h.s == nullptr) return Color{ 0,0,0 };
	return shade_hit(h.prepare_comps(r));
}

bool World::is_shadowed(const Tuple & pt) const {
	Tuple v{ this->lights[0].position - pt };
	double dist{ v.mag() };
	Intersections xs{ this->intersect(Ray{ pt, v.norm() }) };
	if (hit(xs).t < dist) {
		return true;
	}
	else {
		return false;
	}
}