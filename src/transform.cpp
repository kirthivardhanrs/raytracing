#include "raytracing/transform.h"
#include "raytracing/Matrix.h"
#include "raytracing/Tuple.h"
#include <vector>
#include <cmath>

namespace transform {
	Matrix translate(double x, double y, double z) {
		return Matrix{ std::vector<std::vector<double>>{ {1,0,0,x},{0,1,0,y},{0,0,1,z},{0,0,0,1} } }; // tmp can remove the vector stuff
	}
	Matrix scale(double x, double y, double z) {
		return Matrix{ std::vector<std::vector<double>>{ {x,0,0,0},{0,y,0,0},{0,0,z,0},{0,0,0,1} } };
	}
	Matrix rotation_x(double r) {
		return Matrix{ std::vector<std::vector<double>>{ {1,0,0,0},{0,std::cos(r),-std::sin(r),0},{0,std::sin(r),std::cos(r),0},{0,0,0,1} }};
	}
	Matrix rotation_y(double r) {
		return Matrix{ std::vector<std::vector<double>>{ {std::cos(r),0,std::sin(r),0},{0,1,0,0},{-std::sin(r),0,std::cos(r),0},{0,0,0,1} } };
	}
	Matrix rotation_z(double r) {
		return Matrix{ std::vector<std::vector<double>>{ {std::cos(r),-std::sin(r),0,0},{std::sin(r),std::cos(r),0,0},{0,0,1,0},{0,0,0,1} } };
	}
	Matrix shear(double x_y, double x_z, double y_x, double y_z, double z_x, double z_y) {
		return Matrix{ std::vector<std::vector<double>>{ {1,x_y,x_z,0},{y_x,1,y_z,0},{z_x,z_y,1,0},{0,0,0,1} } };
	}
	Matrix view_transform(const Tuple& from, const Tuple& to, const Tuple& up) {
		Tuple forward{ (to - from).norm() };
		Tuple upn{ up.norm() };
		Tuple left{ forward.cross(upn) };
		Tuple true_up{ left.cross(forward) };

		Matrix orientation{ {{left.x, left.y, left.z, 0},
							{true_up.x, true_up.y, true_up.z, 0},
							{-forward.x, -forward.y, -forward.z, 0},
							{0, 0, 0, 1}} };
		return orientation * translate(-from.x, -from.y, -from.z);
	}
}