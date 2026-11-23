#include "raytracing/mathutils.h"
#include <cstdlib>

namespace mathutils {
	bool epsEqual(double a, double b) {
		return std::abs(a - b) < EPSILON;
	}
}