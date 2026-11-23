#include "raytracing/Light.h"
#include "raytracing/Material.h"
#include "raytracing/Tuple.h"
#include "raytracing/Color.h"
#include <cmath>


Color Light::lighting(const Material& mat, const Tuple& pt, const Tuple& eyev, const Tuple& normalv, bool in_shadow) const {
	Color effective_color{ mat.color * this->intensity };
	Tuple l_m{ (this->position - pt).norm() };
	Tuple r_m{ (-l_m.reflect(normalv)).norm() };

	Color amb{ effective_color * mat.ambient };
	Color diffuse{ effective_color * l_m.dot(normalv) * mat.diffuse };
	Color specular{ effective_color * std::pow(r_m.dot(eyev.norm()), mat.shininess) * mat.specular };
	if (l_m.dot(normalv) < 0 || in_shadow) {
		return amb;
	}
	if (r_m.dot(eyev.norm()) < 0) {
		return amb + diffuse;
	}
	return amb + diffuse + specular;
}