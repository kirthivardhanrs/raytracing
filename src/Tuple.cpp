#include "raytracing/Tuple.h"
#include "raytracing/mathutils.h"
#include <cmath>

bool Tuple::isPoint() const {
	return mathutils::epsEqual(w, 1.0);
}
Tuple Tuple::operator+(const Tuple& other) const {
	return Tuple{ x + other.x, y + other.y, z + other.z, w + other.w };
}
Tuple Tuple::operator-(const Tuple& other) const {
	return Tuple{ x - other.x, y - other.y, z - other.z, w - other.w };
}
Tuple Tuple::operator*(double k) const {
	return Tuple{ x * k, y * k, z * k, w * k };
}
Tuple Tuple::operator/(double k) const {
	return Tuple{ x / k, y / k, z / k, w / k };
}
Tuple Tuple::operator-() const {
	return Tuple{ -x, -y, -z, -w };
}
bool Tuple::operator==(const Tuple& other) const {
	return mathutils::epsEqual(x, other.x) &&
		mathutils::epsEqual(y, other.y) &&
		mathutils::epsEqual(z, other.z) &&
		mathutils::epsEqual(w, other.w);
}
double Tuple::mag() const {
	return std::sqrt(x*x + y*y + z*z + w*w);
}
Tuple Tuple::norm() const {
	return *this/this->mag();
}
double Tuple::dot(const Tuple& other) const {
	return x * other.x +
		y * other.y +
		z * other.z +
		w * other.w;
}
Tuple Tuple::cross(const Tuple& other) const {
	return Tuple{
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x,
		0.0
	};
}
Tuple Tuple::reflect(const Tuple& other) const {
	return *this - other * 2 * this->dot(other);
}