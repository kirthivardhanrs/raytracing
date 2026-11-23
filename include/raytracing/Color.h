#pragma once

class Color {
public:
	double r{};
	double g{};
	double b{};
	Color()
		: r(0.0), g(0.0), b(0.0)
	{
	}
	Color(double r, double g, double b)
		: r(r), g(g), b(b)
	{
	}
	Color operator+(const Color& other) const;
	Color operator-(const Color& other) const;
	Color operator*(double k) const;
	Color operator*(const Color& other) const;
};