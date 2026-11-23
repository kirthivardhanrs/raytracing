#include "raytracing/test_helpers.h"
#include "raytracing/mathutils.h"
#include "raytracing/transform.h"

using namespace testutil;

TEST(Transformations, MultiplyingByATranslationMatrix) {
    Matrix t = transform::translate(5, -3, 2);
    EXPECT_TUPLE_EQ(t * point(-3, 4, 5), point(2, 1, 7));
}

TEST(Transformations, MultiplyingByTheInverseOfATranslationMatrix) {
    Matrix inv = transform::translate(5, -3, 2).inverse();
    EXPECT_TUPLE_EQ(inv * point(-3, 4, 5), point(-8, 7, 3));
}

TEST(Transformations, TranslationDoesNotAffectVectors) {
    Matrix t = transform::translate(5, -3, 2);
    Tuple v = vec(-3, 4, 5);
    EXPECT_TUPLE_EQ(t * v, v);
}

TEST(Transformations, ScalingAPoint) {
    EXPECT_TUPLE_EQ(transform::scale(2, 3, 4) * point(-4, 6, 8), point(-8, 18, 32));
}

TEST(Transformations, ScalingAVector) {
    EXPECT_TUPLE_EQ(transform::scale(2, 3, 4) * vec(-4, 6, 8), vec(-8, 18, 32));
}

TEST(Transformations, MultiplyingByTheInverseOfAScalingMatrix) {
    Matrix inv = transform::scale(2, 3, 4).inverse();
    EXPECT_TUPLE_EQ(inv * vec(-4, 6, 8), vec(-2, 2, 2));
}

TEST(Transformations, ReflectionIsScalingByANegativeValue) {
    EXPECT_TUPLE_EQ(transform::scale(-1, 1, 1) * point(2, 3, 4), point(-2, 3, 4));
}

TEST(Transformations, RotatingAroundTheXAxis) {
    Tuple p = point(0, 1, 0);
    EXPECT_TUPLE_EQ(transform::rotation_x(kPi / 4) * p, point(0, kSqrt2 / 2, kSqrt2 / 2));
    EXPECT_TUPLE_EQ(transform::rotation_x(kPi / 2) * p, point(0, 0, 1));
}

TEST(Transformations, InverseOfXRotationRotatesTheOtherWay) {
    Matrix inv = transform::rotation_x(kPi / 4).inverse();
    EXPECT_TUPLE_EQ(inv * point(0, 1, 0), point(0, kSqrt2 / 2, -kSqrt2 / 2));
}

TEST(Transformations, RotatingAroundTheYAxis) {
    Tuple p = point(0, 0, 1);
    EXPECT_TUPLE_EQ(transform::rotation_y(kPi / 4) * p, point(kSqrt2 / 2, 0, kSqrt2 / 2));
    EXPECT_TUPLE_EQ(transform::rotation_y(kPi / 2) * p, point(1, 0, 0));
}

TEST(Transformations, RotatingAroundTheZAxis) {
    Tuple p = point(0, 1, 0);
    EXPECT_TUPLE_EQ(transform::rotation_z(kPi / 4) * p, point(-kSqrt2 / 2, kSqrt2 / 2, 0));
    EXPECT_TUPLE_EQ(transform::rotation_z(kPi / 2) * p, point(-1, 0, 0));
}


TEST(Transformations, ShearingXInProportionToY) {
    EXPECT_TUPLE_EQ(transform::shear(1, 0, 0, 0, 0, 0) * point(2, 3, 4), point(5, 3, 4));
}
TEST(Transformations, ShearingXInProportionToZ) {
    EXPECT_TUPLE_EQ(transform::shear(0, 1, 0, 0, 0, 0) * point(2, 3, 4), point(6, 3, 4));
}
TEST(Transformations, ShearingYInProportionToX) {
    EXPECT_TUPLE_EQ(transform::shear(0, 0, 1, 0, 0, 0) * point(2, 3, 4), point(2, 5, 4));
}
TEST(Transformations, ShearingYInProportionToZ) {
    EXPECT_TUPLE_EQ(transform::shear(0, 0, 0, 1, 0, 0) * point(2, 3, 4), point(2, 7, 4));
}
TEST(Transformations, ShearingZInProportionToX) {
    EXPECT_TUPLE_EQ(transform::shear(0, 0, 0, 0, 1, 0) * point(2, 3, 4), point(2, 3, 6));
}
TEST(Transformations, ShearingZInProportionToY) {
    EXPECT_TUPLE_EQ(transform::shear(0, 0, 0, 0, 0, 1) * point(2, 3, 4), point(2, 3, 7));
}

TEST(Transformations, IndividualTransformationsAreAppliedInSequence) {
    Tuple p = point(1, 0, 1);
    Matrix A = transform::rotation_x(kPi / 2);
    Matrix B = transform::scale(5, 5, 5);
    Matrix C = transform::translate(10, 5, 7);

    Tuple p2 = A * p;
    EXPECT_TUPLE_EQ(p2, point(1, -1, 0));
    Tuple p3 = B * p2;
    EXPECT_TUPLE_EQ(p3, point(5, -5, 0));
    Tuple p4 = C * p3;
    EXPECT_TUPLE_EQ(p4, point(15, 0, 7));
}

TEST(Transformations, ChainedTransformationsAreAppliedInReverseOrder) {
    Tuple p = point(1, 0, 1);
    Matrix A = transform::rotation_x(kPi / 2);
    Matrix B = transform::scale(5, 5, 5);
    Matrix C = transform::translate(10, 5, 7);
    Matrix T = C * B * A;
    EXPECT_TUPLE_EQ(T * p, point(15, 0, 7));
}



TEST(ViewTransform, DefaultOrientationIsTheIdentity) {
    Matrix t = transform::view_transform(point(0, 0, 0), point(0, 0, -1), vec(0, 1, 0));
    EXPECT_MATRIX_EQ(t, mathutils::identity);
}

TEST(ViewTransform, LookingInPositiveZDirection) {
    Matrix t = transform::view_transform(point(0, 0, 0), point(0, 0, 1), vec(0, 1, 0));
    EXPECT_MATRIX_EQ(t, transform::scale(-1, 1, -1));
}

TEST(ViewTransform, MovesTheWorld) {
    Matrix t = transform::view_transform(point(0, 0, 8), point(0, 0, 0), vec(0, 1, 0));
    EXPECT_MATRIX_EQ(t, transform::translate(0, 0, -8));
}

TEST(ViewTransform, ArbitraryViewTransformation) {
    Matrix t = transform::view_transform(point(1, 3, 2), point(4, -2, 8), vec(1, 1, 0));
    Matrix expected = mat({
        {-0.50709, 0.50709,  0.67612, -2.36643},
        { 0.76772, 0.60609,  0.12122, -2.82843},
        {-0.35857, 0.59761, -0.71714,  0.00000},
        { 0.00000, 0.00000,  0.00000,  1.00000} });
    EXPECT_MATRIX_EQ(t, expected);
}
