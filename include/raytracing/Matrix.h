#pragma once
#include <vector>
#include <utility>
#include "raytracing/Tuple.h"

class Matrix {
public:
	std::size_t n{};
	std::vector<std::vector<double>> matrix{};
	Matrix(std::size_t n)
		: n(n), matrix(n, std::vector<double>(n, 0.0))
	{
	}
	Matrix(std::vector<std::vector<double>> matrix)
		: n(matrix.size()), matrix(std::move(matrix))
	{
	}
	double& operator()(std::size_t i, std::size_t j);
	const double& operator()(std::size_t i, std::size_t j) const;
	bool operator==(const Matrix& other) const;
	Matrix operator*(const Matrix& other) const;
	Tuple operator*(const Tuple& other) const;
	Matrix transpose() const;
	double det() const;
	Matrix submatrix(std::size_t i, std::size_t j) const;
	double minor(std::size_t i, std::size_t j) const;
	double cofactor(std::size_t i, std::size_t j) const;
	Matrix inverse() const;
};