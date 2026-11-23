#pragma once
#include "Matrix.h"
#include "mathutils.h"
#include "Material.h"
#include "Light.h"
#include "Color.h"
#include "Tuple.h"
#include <atomic>

class Sphere {
public:
	int id{};
	static inline std::atomic<int> counter{ 0 };
	Material material{};

	Sphere() : id(++counter) {}

	void set_transform(const Matrix& m) {
		transform_ = m;
		inverse_ = m.inverse();
		inverse_transpose_ = inverse_.transpose();
	}
	const Matrix& transform() const { return transform_; }
	const Matrix& inverse() const { return inverse_; }
	const Matrix& inverse_transpose() const { return inverse_transpose_; }

	Tuple norm_at(const Tuple& pt) const;

private:
	Matrix transform_{ mathutils::identity };
	Matrix inverse_{ mathutils::identity };
	Matrix inverse_transpose_{ mathutils::identity };
};
