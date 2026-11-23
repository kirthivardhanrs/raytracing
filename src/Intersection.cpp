#include "raytracing/Intersection.h"
#include "raytracing/Ray.h"
#include "raytracing/Comps.h"
#include "raytracing/Sphere.h"
#include "raytracing/mathutils.h"

Intersection hit(const Intersections& xs) {
	Intersection res{};
	for (const auto& x : xs) {
		if (x.t >= 0 && x.t < res.t) {
			res = x;
		}
	}
	return res;
}

Comps Intersection::prepare_comps(const Ray& r) const {
	Comps c{};
	c.t = this->t;
	c.s = this->s;
	c.pt = r.position(this->t);
	c.eyev = -r.direction;
	c.normalv = this->s->norm_at(c.pt);
	if (c.normalv.dot(c.eyev) < 0) {
		c.inside = true;
		c.normalv = -c.normalv;
	}
	else {
		c.inside = false;
	}

	c.over_pt = c.pt + c.normalv * mathutils::EPSILON;
	return c;
}