#include "raytracing/test_helpers.h"
#include "raytracing/mathutils.h"
#include "raytracing/Intersection.h"
#include "raytracing/Ray.h"
#include "raytracing/Sphere.h"
#include "raytracing/transform.h"

using namespace testutil;

TEST(Intersections, EncapsulatesTAndObject) {
    Sphere s;
    Intersection i(3.5, s);
    EXPECT_DOUBLE_EQ(i.t, 3.5);
    EXPECT_EQ(i.s, &s);
}

TEST(Intersections, AggregatingIntersections) {
    Sphere s;
    Intersections xs{ Intersection(1, s), Intersection(2, s) };
    ASSERT_EQ(xs.size(), 2u);
    EXPECT_DOUBLE_EQ(xs[0].t, 1);
    EXPECT_DOUBLE_EQ(xs[1].t, 2);
}



TEST(Hit, WhenAllIntersectionsHavePositiveT) {
    Sphere s;
    Intersections xs{ Intersection(2, s), Intersection(1, s) };
    EXPECT_DOUBLE_EQ(hit(xs).t, 1);
}

TEST(Hit, WhenSomeIntersectionsHaveNegativeT) {
    Sphere s;
    Intersections xs{ Intersection(1, s), Intersection(-1, s) };
    EXPECT_DOUBLE_EQ(hit(xs).t, 1);
}

TEST(Hit, WhenAllIntersectionsHaveNegativeT) {
    Sphere s;
    Intersections xs{ Intersection(-1, s), Intersection(-2, s) };
    Intersection h = hit(xs);
    
    EXPECT_EQ(h.s, nullptr);
    EXPECT_EQ(h.t, mathutils::pos_inf);
}

TEST(Hit, OfAnEmptyListIsNothing) {
    Intersections xs;
    Intersection h = hit(xs);
    EXPECT_EQ(h.s, nullptr);
    EXPECT_EQ(h.t, mathutils::pos_inf);
}

TEST(Hit, IsAlwaysTheLowestNonnegativeIntersection) {
    Sphere s;
    Intersections xs{ Intersection(5, s), Intersection(7, s), Intersection(-3, s), Intersection(2, s) };
    EXPECT_DOUBLE_EQ(hit(xs).t, 2);
}



TEST(PrepareComps, PrecomputesTheStateOfAnIntersection) {
    Ray r(point(0, 0, -5), vec(0, 0, 1));
    Sphere shape;
    Intersection i(4, shape);
    Comps comps = i.prepare_comps(r);
    EXPECT_DOUBLE_EQ(comps.t, i.t);
    EXPECT_EQ(comps.s, i.s);
    EXPECT_TUPLE_EQ(comps.pt, point(0, 0, -1));
    EXPECT_TUPLE_EQ(comps.eyev, vec(0, 0, -1));
    EXPECT_TUPLE_EQ(comps.normalv, vec(0, 0, -1));
}

TEST(PrepareComps, HitOnTheOutside) {
    Ray r(point(0, 0, -5), vec(0, 0, 1));
    Sphere shape;
    Intersection i(4, shape);
    EXPECT_FALSE(i.prepare_comps(r).inside);
}

TEST(PrepareComps, HitOnTheInside) {
    Ray r(point(0, 0, 0), vec(0, 0, 1));
    Sphere shape;
    Intersection i(1, shape);
    Comps comps = i.prepare_comps(r);
    EXPECT_TUPLE_EQ(comps.pt, point(0, 0, 1));
    EXPECT_TUPLE_EQ(comps.eyev, vec(0, 0, -1));
    EXPECT_TRUE(comps.inside);
    
    EXPECT_TUPLE_EQ(comps.normalv, vec(0, 0, -1));
}

TEST(PrepareComps, HitShouldOffsetThePoint) {
    Ray r(point(0, 0, -5), vec(0, 0, 1));
    Sphere shape;
    shape.set_transform(transform::translate(0, 0, 1));
    Intersection i(5, shape);
    Comps comps = i.prepare_comps(r);
    EXPECT_LT(comps.over_pt.z, -mathutils::EPSILON / 2);
    EXPECT_GT(comps.pt.z, comps.over_pt.z);
}
