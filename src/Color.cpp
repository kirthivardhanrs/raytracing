#include "raytracing/Color.h"

Color Color::operator+(const Color& other) const {
	return Color{ r + other.r, g + other.g, b + other.b };
}
Color Color::operator-(const Color& other) const {
	return Color{ r - other.r, g - other.g, b - other.b };
}
Color Color::operator*(double k) const {
	return Color{ r * k, g * k, b * k };
}
Color Color::operator*(const Color& other) const {
	return Color{r*other.r, g*other.g, b*other.b};
}