
#include "raytracing/test_helpers.h"
#include "raytracing/mathutils.h"

using namespace testutil;

TEST(Matrices, Constructing4x4Matrix) {
    Matrix M = mat({ {1, 2, 3, 4}, {5.5, 6.5, 7.5, 8.5}, {9, 10, 11, 12}, {13.5, 14.5, 15.5, 16.5} });
    EXPECT_DOUBLE_EQ(M(0, 0), 1);
    EXPECT_DOUBLE_EQ(M(0, 3), 4);
    EXPECT_DOUBLE_EQ(M(1, 0), 5.5);
    EXPECT_DOUBLE_EQ(M(1, 2), 7.5);
    EXPECT_DOUBLE_EQ(M(2, 2), 11);
    EXPECT_DOUBLE_EQ(M(3, 0), 13.5);
    EXPECT_DOUBLE_EQ(M(3, 2), 15.5);
}

TEST(Matrices, A2x2MatrixIsRepresentable) {
    Matrix M = mat({ {-3, 5}, {1, -2} });
    EXPECT_EQ(M.n, 2u);
    EXPECT_DOUBLE_EQ(M(0, 0), -3);
    EXPECT_DOUBLE_EQ(M(0, 1), 5);
    EXPECT_DOUBLE_EQ(M(1, 0), 1);
    EXPECT_DOUBLE_EQ(M(1, 1), -2);
}

TEST(Matrices, A3x3MatrixIsRepresentable) {
    Matrix M = mat({ {-3, 5, 0}, {1, -2, -7}, {0, 1, 1} });
    EXPECT_EQ(M.n, 3u);
    EXPECT_DOUBLE_EQ(M(0, 0), -3);
    EXPECT_DOUBLE_EQ(M(1, 1), -2);
    EXPECT_DOUBLE_EQ(M(2, 2), 1);
}

TEST(Matrices, EqualityWithIdenticalMatrices) {
    Matrix A = mat({ {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2} });
    Matrix B = mat({ {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2} });
    EXPECT_TRUE(A == B);
}

TEST(Matrices, EqualityWithDifferentMatrices) {
    Matrix A = mat({ {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2} });
    Matrix B = mat({ {2, 3, 4, 5}, {6, 7, 8, 9}, {8, 7, 6, 5}, {4, 3, 2, 1} });
    EXPECT_FALSE(A == B);
}

TEST(Matrices, MultiplyingTwoMatrices) {
    Matrix A = mat({ {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2} });
    Matrix B = mat({ {-2, 1, 2, 3}, {3, 2, 1, -1}, {4, 3, 6, 5}, {1, 2, 7, 8} });
    Matrix expected = mat({ {20, 22, 50, 48}, {44, 54, 114, 108}, {40, 58, 110, 102}, {16, 26, 46, 42} });
    EXPECT_MATRIX_EQ(A * B, expected);
}

TEST(Matrices, MatrixMultipliedByATuple) {
    Matrix A = mat({ {1, 2, 3, 4}, {2, 4, 4, 2}, {8, 6, 4, 1}, {0, 0, 0, 1} });
    Tuple b{ 1, 2, 3, 1 };
    EXPECT_TUPLE_EQ(A * b, (Tuple{ 18, 24, 33, 1 }));
}

TEST(Matrices, MultiplyingByTheIdentityMatrix) {
    Matrix A = mat({ {0, 1, 2, 4}, {1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32} });
    EXPECT_MATRIX_EQ(A * mathutils::identity, A);
}

TEST(Matrices, MultiplyingTheIdentityMatrixByATuple) {
    Tuple a{ 1, 2, 3, 4 };
    EXPECT_TUPLE_EQ(mathutils::identity * a, a);
}

TEST(Matrices, TransposingAMatrix) {
    Matrix A = mat({ {0, 9, 3, 0}, {9, 8, 0, 8}, {1, 8, 5, 3}, {0, 0, 5, 8} });
    Matrix expected = mat({ {0, 9, 1, 0}, {9, 8, 8, 0}, {3, 0, 5, 5}, {0, 8, 3, 8} });
    EXPECT_MATRIX_EQ(A.transpose(), expected);
}

TEST(Matrices, TransposingTheIdentityMatrix) {
    EXPECT_MATRIX_EQ(mathutils::identity.transpose(), mathutils::identity);
}

TEST(Matrices, DeterminantOf2x2) {
    Matrix A = mat({ {1, 5}, {-3, 2} });
    EXPECT_DOUBLE_EQ(A.det(), 17);
}

TEST(Matrices, SubmatrixOf3x3Is2x2) {
    Matrix A = mat({ {1, 5, 0}, {-3, 2, 7}, {0, 6, -3} });
    Matrix S = A.submatrix(0, 2);
    EXPECT_EQ(S.n, 2u);
    EXPECT_MATRIX_EQ(S, mat({ {-3, 2}, {0, 6} }));
}

TEST(Matrices, SubmatrixOf4x4Is3x3) {
    Matrix A = mat({ {-6, 1, 1, 6}, {-8, 5, 8, 6}, {-1, 0, 8, 2}, {-7, 1, -1, 1} });
    Matrix S = A.submatrix(2, 1);
    EXPECT_EQ(S.n, 3u);
    EXPECT_MATRIX_EQ(S, mat({ {-6, 1, 6}, {-8, 8, 6}, {-7, -1, 1} }));
}

TEST(Matrices, MinorOf3x3) {
    Matrix A = mat({ {3, 5, 0}, {2, -1, -7}, {6, -1, 5} });
    Matrix B = A.submatrix(1, 0);
    EXPECT_DOUBLE_EQ(B.det(), 25);
    EXPECT_DOUBLE_EQ(A.minor(1, 0), 25);
}

TEST(Matrices, CofactorOf3x3) {
    Matrix A = mat({ {3, 5, 0}, {2, -1, -7}, {6, -1, 5} });
    EXPECT_DOUBLE_EQ(A.minor(0, 0), -12);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 0), -12);
    EXPECT_DOUBLE_EQ(A.minor(1, 0), 25);
    EXPECT_DOUBLE_EQ(A.cofactor(1, 0), -25);
}

TEST(Matrices, DeterminantOf3x3) {
    Matrix A = mat({ {1, 2, 6}, {-5, 8, -4}, {2, 6, 4} });
    EXPECT_DOUBLE_EQ(A.cofactor(0, 0), 56);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 1), 12);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 2), -46);
    EXPECT_DOUBLE_EQ(A.det(), -196);
}

TEST(Matrices, DeterminantOf4x4) {
    Matrix A = mat({ {-2, -8, 3, 5}, {-3, 1, 7, 3}, {1, 2, -9, 6}, {-6, 7, 7, -9} });
    EXPECT_DOUBLE_EQ(A.cofactor(0, 0), 690);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 1), 447);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 2), 210);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 3), 51);
    EXPECT_DOUBLE_EQ(A.det(), -4071);
}

TEST(Matrices, InvertibleMatrixHasNonzeroDeterminant) {
    Matrix A = mat({ {6, 4, 4, 4}, {5, 5, 7, 6}, {4, -9, 3, -7}, {9, 1, 7, -6} });
    EXPECT_DOUBLE_EQ(A.det(), -2120);
    EXPECT_NE(A.det(), 0.0);
}

TEST(Matrices, NoninvertibleMatrixHasZeroDeterminant) {
    Matrix A = mat({ {-4, 2, -2, -3}, {9, 6, 2, 6}, {0, -5, 1, -5}, {0, 0, 0, 0} });
    EXPECT_DOUBLE_EQ(A.det(), 0);
}

TEST(Matrices, InverseOfAMatrix) {
    Matrix A = mat({ {-5, 2, 6, -8}, {1, -5, 1, 8}, {7, 7, -6, -7}, {1, -3, 7, 4} });
    Matrix B = A.inverse();
    EXPECT_DOUBLE_EQ(A.det(), 532);
    EXPECT_DOUBLE_EQ(A.cofactor(2, 3), -160);
    EXPECT_NEAR(B(3, 2), -160.0 / 532, 1e-9);
    EXPECT_DOUBLE_EQ(A.cofactor(3, 2), 105);
    EXPECT_NEAR(B(2, 3), 105.0 / 532, 1e-9);
    Matrix expected = mat({
        { 0.21805,  0.45113,  0.24060, -0.04511},
        {-0.80827, -1.45677, -0.44361,  0.52068},
        {-0.07895, -0.22368, -0.05263,  0.19737},
        {-0.52256, -0.81391, -0.30075,  0.30639} });
    EXPECT_MATRIX_EQ(B, expected);
}

TEST(Matrices, InverseOfAnotherMatrix) {
    Matrix A = mat({ {8, -5, 9, 2}, {7, 5, 6, 1}, {-6, 0, 9, 6}, {-3, 0, -9, -4} });
    Matrix expected = mat({
        {-0.15385, -0.15385, -0.28205, -0.53846},
        {-0.07692,  0.12308,  0.02564,  0.03077},
        { 0.35897,  0.35897,  0.43590,  0.92308},
        {-0.69231, -0.69231, -0.76923, -1.92308} });
    EXPECT_MATRIX_EQ(A.inverse(), expected);
}

TEST(Matrices, InverseOfAThirdMatrix) {
    Matrix A = mat({ {9, 3, 0, 9}, {-5, -2, -6, -3}, {-4, 9, 6, 4}, {-7, 6, 6, 2} });
    Matrix expected = mat({
        {-0.04074, -0.07778,  0.14444, -0.22222},
        {-0.07778,  0.03333,  0.36667, -0.33333},
        {-0.02901, -0.14630, -0.10926,  0.12963},
        { 0.17778,  0.06667, -0.26667,  0.33333} });
    EXPECT_MATRIX_EQ(A.inverse(), expected);
}

TEST(Matrices, MultiplyingAProductByItsInverse) {
    Matrix A = mat({ {3, -9, 7, 3}, {3, -8, 2, -9}, {-4, 4, 4, 1}, {-6, 5, -1, 1} });
    Matrix B = mat({ {8, 2, 2, 2}, {3, -1, 7, 0}, {7, 0, 5, 4}, {6, -2, 0, 5} });
    Matrix C = A * B;
    EXPECT_MATRIX_EQ(C * B.inverse(), A);
}
