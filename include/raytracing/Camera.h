#pragma once
#include "Matrix.h"
#include "mathutils.h"
#include "Ray.h"
#include "World.h"
#include "Canvas.h"
#include <cmath>

class Camera {
public:
	double hsize{};
	double vsize{};
	double fov{};
	double pixel_size{};
	double half_view{};
	double half_width{};
	double half_height{};
	double aspect{};
	Matrix transform{ mathutils::identity };
	Camera(double hsize, double vsize, double fov);
	Ray	ray_for_pixel(double px, double py) const;
	Ray ray_for_pixel(double px, double py, const Matrix& inv_transform) const;
	Canvas render(const World& w) const;
};