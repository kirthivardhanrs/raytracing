#include "raytracing/Sphere.h"
#include "raytracing/Tuple.h"

Tuple Sphere::norm_at(const Tuple& pt) const {
	Tuple object_pt = inverse_ * pt;
	Tuple object_normal = object_pt - Tuple{ 0, 0, 0, 1 };
	Tuple world_normal = inverse_transpose_ * object_normal;
	world_normal.w = 0;
	return world_normal.norm();
}