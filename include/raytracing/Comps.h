#pragma once
#include "Tuple.h"

class Sphere;

class Comps {
public:
	double t{};
	const Sphere* s{ nullptr };
	Tuple pt{ 0,0,0,1 };
	Tuple eyev{ 0,0,0,0 };
	Tuple normalv{ 0,0,0,0 };
	bool inside{};
	Tuple over_pt{ 0,0,0,1 };
	Comps() = default;
	Comps(double t, const Sphere* s, const Tuple& pt, const Tuple& eyev, const Tuple& normalv, bool inside, const Tuple& over_pt)
		: t(t), s(s), pt(pt), eyev(eyev), normalv(normalv), inside(inside), over_pt(over_pt)
	{
	}
};