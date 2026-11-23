#include "raytracing/Camera.h"
#include "raytracing/Ray.h"
#include "raytracing/Matrix.h"
#include "raytracing/Tuple.h"
#include "raytracing/Color.h"
#include "raytracing/Canvas.h"
#include <cmath>
#include <algorithm>
#include <atomic>
#include <thread>
#include <vector>

class World;

Camera::Camera(double hsize, double vsize, double fov)
	: hsize(hsize), vsize(vsize), fov(fov)
{
	half_view = std::tan(fov / 2);
	aspect = hsize / vsize;
	if (aspect >= 1) {
		half_width = half_view;
		half_height = half_view / aspect;
	}
	else {
		half_height = half_view;
		half_width = half_view * aspect;
	}
	pixel_size = (half_width * 2) / hsize;
}

Ray Camera::ray_for_pixel(double px, double py) const {
	return ray_for_pixel(px, py, transform.inverse());
}

Ray	Camera::ray_for_pixel(double px, double py, const Matrix& inv_transform) const {
	double xoffset{ (px + 0.5) * (this->pixel_size) };
	double yoffset{ (py + 0.5) * (this->pixel_size) };
	double worldx{ this->half_width - xoffset };
	double worldy{ this->half_height - yoffset };

	Matrix invT{ inv_transform };

	Tuple pixel{ invT * Tuple { worldx, worldy, -1, 1 } };
	Tuple origin{ invT * Tuple { 0, 0, 0, 1 } };
	Tuple direction{ (pixel - origin).norm() };

	return Ray{ origin, direction };
}

Canvas Camera::render(const World& w) const {
	const std::size_t W = static_cast<std::size_t>(hsize);
	const std::size_t H = static_cast<std::size_t>(vsize);
	Canvas image(W, H);
	const Matrix inv = transform.inverse();

	std::atomic<std::size_t> next_row{ 0 };
	auto worker = [&] {
		while (true) {
			const std::size_t y = next_row.fetch_add(1);
			if (y >= H) break;
			for (std::size_t x = 0; x < W; ++x)
				image(x, y) = w.color_at(
					ray_for_pixel(static_cast<double>(x), static_cast<double>(y), inv));
		}
	};

	const unsigned n = std::max(1u, std::thread::hardware_concurrency());
	{
		std::vector<std::jthread> pool;
		for (unsigned i = 0; i < n; ++i) pool.emplace_back(worker);
	}
	return image;
}