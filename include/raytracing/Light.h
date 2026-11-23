#pragma once
#include "Color.h"
#include "Tuple.h"
#include "Material.h"

class Light {
public:
	Color intensity{};
	Tuple position{ 0,0,0,1 };
	Light(const Color& intensity, const Tuple& position)
		: intensity(intensity), position(position)
	{
	}
	Color lighting(const Material& mat, const Tuple& pt, const Tuple& eyev, const Tuple& normalv, bool in_shadow) const;
};