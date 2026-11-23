#include "raytracing/test_helpers.h"
#include "raytracing/mathutils.h"
#include "raytracing/Ray.h"
#include "raytracing/Sphere.h"
#include "raytracing/transform.h"

using namespace testutil;

TEST(Spheres, RayIntersectsAtTwoPoints) {
    Ray r(point(0, 0, -5), vec(0, 0, 1));
    Sphere s;
    Intersections xs = r.intersect(s);
    ASSERT_EQ(xs.size(), 2u);
    EXPECT_DOUBLE_EQ(xs[0].t, 4.0);
    EXPECT_DOUBLE_EQ(xs[1].t, 6.0);
}

TEST(Spheres, RayIntersectsAtATangent) {
    Ray r(point(0, 1, -5), vec(0, 0, 1));
    Sphere s;
    Intersections xs = r.intersect(s);
    ASSERT_EQ(xs.size(), 2u);
    EXPECT_DOUBLE_EQ(xs[0].t, 5.0);
    EXPECT_DOUBLE_EQ(xs[1].t, 5.0);
}

TEST(Spheres, RayMissesASphere) {
    Ray r(point(0, 2, -5), vec(0, 0, 1));
    Sphere s;
    EXPECT_EQ(r.intersect(s).size(), 0u);
}

TEST(Spheres, RayOriginatesInsideASphere) {
    Ray r(point(0, 0, 0), vec(0, 0, 1));
    Sphere s;
    Intersections xs = r.intersect(s);
    ASSERT_EQ(xs.size(), 2u);
    EXPECT_DOUBLE_EQ(xs[0].t, -1.0);
    EXPECT_DOUBLE_EQ(xs[1].t, 1.0);
}

TEST(Spheres, SphereIsBehindARay) {
    Ray r(point(0, 0, 5), vec(0, 0, 1));
    Sphere s;
    Intersections xs = r.intersect(s);
    ASSERT_EQ(xs.size(), 2u);
    EXPECT_DOUBLE_EQ(xs[0].t, -6.0);
    EXPECT_DOUBLE_EQ(xs[1].t, -4.0);
}

TEST(Spheres, IntersectSetsTheObjectOnTheIntersection) {
    Ray r(point(0, 0, -5), vec(0, 0, 1));
    Sphere s;
    Intersections xs = r.intersect(s);
    ASSERT_EQ(xs.size(), 2u);
    EXPECT_EQ(xs[0].s, &s);
    EXPECT_EQ(xs[1].s, &s);
}

TEST(Spheres, DefaultTransformationIsIdentity) {
    Sphere s;
    EXPECT_MATRIX_EQ(s.transform(), mathutils::identity);
}

TEST(Spheres, ChangingASpheresTransformation) {
    Sphere s;
    Matrix t = transform::translate(2, 3, 4);
    s.set_transform(t);
    EXPECT_MATRIX_EQ(s.transform(), t);
}

TEST(Spheres, IntersectingAScaledSphereWithARay) {
    Ray r(point(0, 0, -5), vec(0, 0, 1));
    Sphere s;
    s.set_transform(transform::scale(2, 2, 2));
    Intersections xs = r.intersect(s);
    ASSERT_EQ(xs.size(), 2u);
    EXPECT_DOUBLE_EQ(xs[0].t, 3);
    EXPECT_DOUBLE_EQ(xs[1].t, 7);
}

TEST(Spheres, IntersectingATranslatedSphereWithARay) {
    Ray r(point(0, 0, -5), vec(0, 0, 1));
    Sphere s;
    s.set_transform(transform::translate(5, 0, 0));
    EXPECT_EQ(r.intersect(s).size(), 0u);
}

TEST(Spheres, NormalAtAPointOnTheXAxis) {
    Sphere s;
    EXPECT_TUPLE_EQ(s.norm_at(point(1, 0, 0)), vec(1, 0, 0));
}

TEST(Spheres, NormalAtAPointOnTheYAxis) {
    Sphere s;
    EXPECT_TUPLE_EQ(s.norm_at(point(0, 1, 0)), vec(0, 1, 0));
}

TEST(Spheres, NormalAtAPointOnTheZAxis) {
    Sphere s;
    EXPECT_TUPLE_EQ(s.norm_at(point(0, 0, 1)), vec(0, 0, 1));
}

TEST(Spheres, NormalAtANonaxialPoint) {
    Sphere s;
    double k = kSqrt3 / 3;
    EXPECT_TUPLE_EQ(s.norm_at(point(k, k, k)), vec(k, k, k));
}

TEST(Spheres, NormalIsANormalizedVector) {
    Sphere s;
    double k = kSqrt3 / 3;
    Tuple n = s.norm_at(point(k, k, k));
    EXPECT_TUPLE_EQ(n, n.norm());
    EXPECT_NEAR(n.mag(), 1.0, 1e-9);
    EXPECT_TRUE(isVector(n));
}

TEST(Spheres, NormalOnATranslatedSphere) {
    Sphere s;
    s.set_transform(transform::translate(0, 1, 0));
    EXPECT_TUPLE_EQ(s.norm_at(point(0, 1.70711, -0.70711)), vec(0, 0.70711, -0.70711));
}

TEST(Spheres, NormalOnATransformedSphere) {
    Sphere s;
    s.set_transform(transform::scale(1, 0.5, 1) * transform::rotation_z(kPi / 5));
    EXPECT_TUPLE_EQ(s.norm_at(point(0, kSqrt2 / 2, -kSqrt2 / 2)), vec(0, 0.97014, -0.24254));
}



TEST(Spheres, DefaultMaterial) {
    Sphere s;
    EXPECT_TRUE(MaterialEq(s.material, Material{}));
}

TEST(Spheres, MayBeAssignedAMaterial) {
    Sphere s;
    Material m;
    m.ambient = 1;
    s.material = m;
    EXPECT_TRUE(MaterialEq(s.material, m));
    EXPECT_DOUBLE_EQ(s.material.ambient, 1.0);
}

TEST(Spheres, EachSphereGetsAUniqueId) {
    Sphere a, b;
    EXPECT_NE(a.id, b.id);
}