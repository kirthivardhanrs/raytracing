#pragma once
#include "Matrix.h"

class Tuple;

namespace transform {
	Matrix translate(double x, double y, double z);
	Matrix scale(double x, double y, double z);
	Matrix rotation_x(double r);
	Matrix rotation_y(double r);
	Matrix rotation_z(double r);
	Matrix shear(double x_y, double x_z, double y_x, double y_z, double z_x, double z_y);
	Matrix view_transform(const Tuple& from, const Tuple& to, const Tuple& up);
}