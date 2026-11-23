#include "raytracing/test_helpers.h"

using namespace testutil;

TEST(Tuples, TupleWithWOneIsAPoint) {
    Tuple a{ 4.3, -4.2, 3.1, 1.0 };
    EXPECT_DOUBLE_EQ(a.x, 4.3);
    EXPECT_DOUBLE_EQ(a.y, -4.2);
    EXPECT_DOUBLE_EQ(a.z, 3.1);
    EXPECT_DOUBLE_EQ(a.w, 1.0);
    EXPECT_TRUE(a.isPoint());
    EXPECT_FALSE(isVector(a));
}

TEST(Tuples, TupleWithWZeroIsAVector) {
    Tuple a{ 4.3, -4.2, 3.1, 0.0 };
    EXPECT_DOUBLE_EQ(a.w, 0.0);
    EXPECT_FALSE(a.isPoint());
    EXPECT_TRUE(isVector(a));
}

TEST(Tuples, PointHelperCreatesWOne) {
    EXPECT_TUPLE_EQ(point(4, -4, 3), (Tuple{ 4, -4, 3, 1 }));
}

TEST(Tuples, VectorHelperCreatesWZero) {
    EXPECT_TUPLE_EQ(vec(4, -4, 3), (Tuple{ 4, -4, 3, 0 }));
}

TEST(Tuples, AddingTwoTuples) {
    Tuple a1{ 3, -2, 5, 1 }, a2{ -2, 3, 1, 0 };
    EXPECT_TUPLE_EQ(a1 + a2, (Tuple{ 1, 1, 6, 1 }));
}

TEST(Tuples, SubtractingTwoPoints) {
    EXPECT_TUPLE_EQ(point(3, 2, 1) - point(5, 6, 7), vec(-2, -4, -6));
}

TEST(Tuples, SubtractingAVectorFromAPoint) {
    EXPECT_TUPLE_EQ(point(3, 2, 1) - vec(5, 6, 7), point(-2, -4, -6));
}

TEST(Tuples, SubtractingTwoVectors) {
    EXPECT_TUPLE_EQ(vec(3, 2, 1) - vec(5, 6, 7), vec(-2, -4, -6));
}

TEST(Tuples, SubtractingAVectorFromTheZeroVector) {
    EXPECT_TUPLE_EQ(vec(0, 0, 0) - vec(1, -2, 3), vec(-1, 2, -3));
}

TEST(Tuples, NegatingATuple) {
    Tuple a{ 1, -2, 3, -4 };
    EXPECT_TUPLE_EQ(-a, (Tuple{ -1, 2, -3, 4 }));
}

TEST(Tuples, MultiplyingByAScalar) {
    Tuple a{ 1, -2, 3, -4 };
    EXPECT_TUPLE_EQ(a * 3.5, (Tuple{ 3.5, -7, 10.5, -14 }));
}

TEST(Tuples, MultiplyingByAFraction) {
    Tuple a{ 1, -2, 3, -4 };
    EXPECT_TUPLE_EQ(a * 0.5, (Tuple{ 0.5, -1, 1.5, -2 }));
}

TEST(Tuples, DividingByAScalar) {
    Tuple a{ 1, -2, 3, -4 };
    EXPECT_TUPLE_EQ(a / 2, (Tuple{ 0.5, -1, 1.5, -2 }));
}

TEST(Tuples, MagnitudeOfUnitVectors) {
    EXPECT_DOUBLE_EQ(vec(1, 0, 0).mag(), 1.0);
    EXPECT_DOUBLE_EQ(vec(0, 1, 0).mag(), 1.0);
    EXPECT_DOUBLE_EQ(vec(0, 0, 1).mag(), 1.0);
}

TEST(Tuples, MagnitudeOfArbitraryVectors) {
    EXPECT_NEAR(vec(1, 2, 3).mag(), std::sqrt(14.0), 1e-9);
    EXPECT_NEAR(vec(-1, -2, -3).mag(), std::sqrt(14.0), 1e-9);
}

TEST(Tuples, NormalizingAxisAlignedVector) {
    EXPECT_TUPLE_EQ(vec(4, 0, 0).norm(), vec(1, 0, 0));
}

TEST(Tuples, NormalizingArbitraryVector) {
    EXPECT_TUPLE_EQ(vec(1, 2, 3).norm(), vec(0.26726, 0.53452, 0.80178));
}

TEST(Tuples, MagnitudeOfNormalizedVectorIsOne) {
    EXPECT_NEAR(vec(1, 2, 3).norm().mag(), 1.0, 1e-9);
}



TEST(Tuples, DotProduct) {
    EXPECT_DOUBLE_EQ(vec(1, 2, 3).dot(vec(2, 3, 4)), 20.0);
}

TEST(Tuples, CrossProduct) {
    Tuple a = vec(1, 2, 3), b = vec(2, 3, 4);
    EXPECT_TUPLE_EQ(a.cross(b), vec(-1, 2, -1));
    EXPECT_TUPLE_EQ(b.cross(a), vec(1, -2, 1));
}

TEST(Colors, ColorsAreRgbTuples) {
    Color c{ -0.5, 0.4, 1.7 };
    EXPECT_DOUBLE_EQ(c.r, -0.5);
    EXPECT_DOUBLE_EQ(c.g, 0.4);
    EXPECT_DOUBLE_EQ(c.b, 1.7);
}

TEST(Colors, DefaultColorIsBlack) {
    Color c;
    EXPECT_COLOR_EQ(c, (Color{ 0, 0, 0 }));
}

TEST(Colors, AddingColors) {
    EXPECT_COLOR_EQ(Color(0.9, 0.6, 0.75) + Color(0.7, 0.1, 0.25), Color(1.6, 0.7, 1.0));
}

TEST(Colors, SubtractingColors) {
    EXPECT_COLOR_EQ(Color(0.9, 0.6, 0.75) - Color(0.7, 0.1, 0.25), Color(0.2, 0.5, 0.5));
}

TEST(Colors, MultiplyingByAScalar) {
    EXPECT_COLOR_EQ(Color(0.2, 0.3, 0.4) * 2, Color(0.4, 0.6, 0.8));
}

TEST(Colors, MultiplyingColors) {
    EXPECT_COLOR_EQ(Color(1, 0.2, 0.4) * Color(0.9, 1, 0.1), Color(0.9, 0.2, 0.04));
}

TEST(Tuples, ReflectingAVectorApproachingAt45Degrees) {
    EXPECT_TUPLE_EQ(vec(1, -1, 0).reflect(vec(0, 1, 0)), vec(1, 1, 0));
}

TEST(Tuples, ReflectingAVectorOffASlantedSurface) {
    EXPECT_TUPLE_EQ(vec(0, -1, 0).reflect(vec(kSqrt2 / 2, kSqrt2 / 2, 0)), vec(1, 0, 0));
}
