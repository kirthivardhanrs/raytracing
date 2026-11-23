#pragma once
#include <gtest/gtest.h>
#include <cmath>
#include <numbers>
#include <string>
#include <utility>
#include <vector>
#include "Color.h"
#include "Material.h"
#include "Matrix.h"
#include "Tuple.h"

namespace testutil {
    inline constexpr double kTol = 1e-4;
    inline constexpr double kPi = std::numbers::pi;
    inline const double kSqrt2 = std::sqrt(2.0);
    inline const double kSqrt3 = std::sqrt(3.0);

    inline Tuple point(double x, double y, double z) { return Tuple{ x, y, z, 1.0 }; }
    inline Tuple vec(double x, double y, double z) { return Tuple{ x, y, z, 0.0 }; }
    inline bool isVector(const Tuple& t) { return t.w == 0.0; }

    inline Matrix mat(std::vector<std::vector<double>> rows) { return Matrix{ std::move(rows) }; }

    inline ::testing::AssertionResult TupleNear(const Tuple& a, const Tuple& b, double tol = kTol) {
        if (std::abs(a.x - b.x) <= tol && std::abs(a.y - b.y) <= tol &&
            std::abs(a.z - b.z) <= tol && std::abs(a.w - b.w) <= tol)
            return ::testing::AssertionSuccess();
        return ::testing::AssertionFailure()
            << "tuple(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << ") != tuple("
            << b.x << ", " << b.y << ", " << b.z << ", " << b.w << ")";
    }

    inline ::testing::AssertionResult ColorNear(const Color& a, const Color& b, double tol = kTol) {
        if (std::abs(a.r - b.r) <= tol && std::abs(a.g - b.g) <= tol && std::abs(a.b - b.b) <= tol)
            return ::testing::AssertionSuccess();
        return ::testing::AssertionFailure()
            << "color(" << a.r << ", " << a.g << ", " << a.b << ") != color("
            << b.r << ", " << b.g << ", " << b.b << ")";
    }

    inline ::testing::AssertionResult MatrixNear(const Matrix& a, const Matrix& b, double tol = kTol) {
        if (a.n != b.n)
            return ::testing::AssertionFailure() << "size mismatch: " << a.n << " vs " << b.n;
        for (std::size_t i = 0; i < a.n; ++i)
            for (std::size_t j = 0; j < a.n; ++j)
                if (std::abs(a(i, j) - b(i, j)) > tol)
                    return ::testing::AssertionFailure()
                    << "matrices differ at (" << i << ", " << j << "): "
                    << a(i, j) << " vs " << b(i, j);
        return ::testing::AssertionSuccess();
    }

    inline ::testing::AssertionResult MaterialEq(const Material& a, const Material& b) {
        if (ColorNear(a.color, b.color, 1e-12) && a.ambient == b.ambient &&
            a.diffuse == b.diffuse && a.specular == b.specular && a.shininess == b.shininess)
            return ::testing::AssertionSuccess();
        return ::testing::AssertionFailure() << "materials differ";
    }

    inline std::vector<std::string> splitLines(const std::string& s) {
        std::vector<std::string> lines;
        std::string cur;
        for (char ch : s) {
            if (ch == '\n') { 
                lines.push_back(cur); cur.clear(); 
            }
            else {
                cur += ch;
            }
        }
        if (!cur.empty()) lines.push_back(cur);
        return lines;
    }

}

#define EXPECT_TUPLE_EQ(a, b)    EXPECT_TRUE(::testutil::TupleNear((a), (b)))
#define EXPECT_COLOR_EQ(a, b)    EXPECT_TRUE(::testutil::ColorNear((a), (b)))
#define EXPECT_MATRIX_EQ(a, b)   EXPECT_TRUE(::testutil::MatrixNear((a), (b)))
