#pragma once
#include <vector>

class Tuple {
public:
	double x{};
	double y{};
	double z{};
	double w{};
	Tuple(double x, double y, double z, double w)
		: x(x), y(y), z(z), w(w)
	{
	}
	Tuple(const std::vector<double>& v)
		: x(v[0]), y(v[1]), z(v[2]), w(v[3])
	{
	}
	bool isPoint() const;
	Tuple operator+(const Tuple& other) const;
	Tuple operator-(const Tuple& other) const;
	Tuple operator*(double k) const;
	Tuple operator/(double k) const;
	Tuple operator-() const;
	bool operator==(const Tuple& other) const;
	double mag() const;
	Tuple norm() const;
	double dot(const Tuple& other) const;
	Tuple cross(const Tuple& other) const;
	Tuple reflect(const Tuple& other) const;
};