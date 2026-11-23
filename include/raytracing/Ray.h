#pragma once
#include "Tuple.h"
#include "Intersection.h"

class Sphere;
class Matrix;

class Ray {
public:
	Tuple origin{ 0,0,0,1 };
	Tuple direction{ 0,0,0,0 };
	Ray(const Tuple& origin, const Tuple& direction)
		: origin(origin), direction(direction)
	{
	}
	Tuple position(double t) const;
	Intersections intersect(const Sphere& s) const;
	Ray transform(const Matrix& m) const;
};