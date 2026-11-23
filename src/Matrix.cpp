#include "raytracing/Matrix.h"
#include "raytracing/mathutils.h"
#include <iostream>

double& Matrix::operator()(std::size_t i, std::size_t j) {
	return matrix[i][j];
}
const double& Matrix::operator()(std::size_t i, std::size_t j) const {
	return matrix[i][j];
}
bool Matrix::operator==(const Matrix& other) const {
	if (n != other.n) {
		return false;
	}
	for (std::size_t i{ 0 }; i <= n - 1; ++i) {
		for (std::size_t j{ 0 }; j <= n - 1; ++j) {
			if (!mathutils::epsEqual((*this)(i, j), other(i, j))) {
				return false;
			}
		}
	}
	return true;
}
Matrix Matrix::operator*(const Matrix& other) const {
	if (n != 4) {
		std::cerr << "breh";
	}
	Matrix res{ n };
	for (std::size_t i{ 0 }; i <= n - 1; ++i) {
		for (std::size_t j{ 0 }; j <= n - 1; ++j) {
			res(i, j) = 0;
			for (std::size_t k{ 0 }; k < n; ++k) {
				res(i, j) += (*this)(i, k) * other(k, j);
			}
		}
	}
	return res;
}
Tuple Matrix::operator*(const Tuple& other) const {
	Tuple res{0,0,0,0};
	res.x = Tuple{ this->matrix[0] }.dot(other);
	res.y = Tuple{ this->matrix[1] }.dot(other);
	res.z = Tuple{ this->matrix[2] }.dot(other);
	res.w = Tuple{ this->matrix[3] }.dot(other);
	return res;
}
Matrix Matrix::transpose() const {
	Matrix res{ n };
	for (std::size_t i{ 0 }; i <= n - 1; ++i) {
		for (std::size_t j{ 0 }; j <= n - 1; ++j) {
			res(i, j) = (*this)(j, i);
		}
	}
	return res;
}
double Matrix::det() const {
	if (n == 2) {
		return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
	}
	else {
		double res{ 0.0 };
		for (std::size_t i{ 0 }; i < n; ++i) {
			res += ((*this)(0, i)) * ((*this).cofactor(0, i));
		}
		return res;
	}
}
Matrix Matrix::submatrix(std::size_t i, std::size_t j) const {
	Matrix res{ n - 1 };
	std::size_t ri{ 0 };
	for (std::size_t k = 0; k < n; ++k) {
		if (k == i) continue;
		std::size_t ci{ 0 };
		for (std::size_t m = 0; m < n; ++m) {
			if (m == j) continue;
			res(ri, ci) = (*this)(k, m);
			++ci;
		}
		++ri;
	}
	return res;
}
double Matrix::minor(std::size_t i, std::size_t j) const {
	return submatrix(i, j).det();
}
double Matrix::cofactor(std::size_t i, std::size_t j) const {
	if ((i + j) % 2 == 0) {
		return minor(i, j);
	}
	else {
		return -minor(i, j);
	}
}
Matrix Matrix::inverse() const {
	double d{ det() };
	if (d == 0) {
		std::cerr << "non-invertible";
	}
	Matrix res{ n };
	for (std::size_t i{ 0 }; i < n; ++i) {
		for (std::size_t j{ 0 }; j < n; ++j) {
			res(j, i) = cofactor(i, j) / d;
		}
	}
	return res;
}