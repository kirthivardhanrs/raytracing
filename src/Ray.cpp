#include "raytracing/Ray.h"
#include "raytracing/Sphere.h"
#include "raytracing/Intersection.h"
#include "raytracing/Tuple.h"
#include "raytracing/Matrix.h"
#include <cmath>

Tuple Ray::position(double t) const {
	return origin + direction * t;
}

Intersections Ray::intersect(const Sphere& s) const {
	Ray tray{ transform(s.inverse()) };
	Tuple sphere_to_ray{ tray.origin - Tuple{0,0,0,1} };
	double a{ tray.direction.dot(tray.direction) };
	double b{ 2 * tray.direction.dot(sphere_to_ray) };
	double c{ sphere_to_ray.dot(sphere_to_ray) - 1 };
	double discriminant{ b * b - 4 * a * c };
	if (discriminant < 0) {
		return Intersections{};
	}
	else {
		double t1{ (-b - std::sqrt(discriminant)) / (2 * a) };
		double t2{ (-b + std::sqrt(discriminant)) / (2 * a) };
		return Intersections{ Intersection{t1, s}, Intersection{t2, s} };
	}
}

Ray Ray::transform(const Matrix& m) const {
	return Ray{ m*origin, m*direction };
}