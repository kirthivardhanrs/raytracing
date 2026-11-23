#pragma once
#include <vector>
#include "raytracing/Color.h"

class Canvas {
public:
	std::size_t width{};
	std::size_t height{};
	std::vector<std::vector<Color>> pixels{};
	Canvas(std::size_t width, std::size_t height)
		: width(width), height(height), pixels(height, std::vector<Color>(width, Color{0, 0, 0}))
	{
	}
	Color& operator()(std::size_t i, std::size_t j);
	const Color& operator()(std::size_t i, std::size_t j) const;
	void toPPM() const;
};