#include "raytracing/test_helpers.h"
#include "raytracing/Ray.h"
#include "raytracing/transform.h"

using namespace testutil;

TEST(Rays, CreatingAndQueryingARay) {
    Tuple origin = point(1, 2, 3), direction = vec(4, 5, 6);
    Ray r(origin, direction);
    EXPECT_TUPLE_EQ(r.origin, origin);
    EXPECT_TUPLE_EQ(r.direction, direction);
}

TEST(Rays, ComputingAPointFromADistance) {
    Ray r(point(2, 3, 4), vec(1, 0, 0));
    EXPECT_TUPLE_EQ(r.position(0), point(2, 3, 4));
    EXPECT_TUPLE_EQ(r.position(1), point(3, 3, 4));
    EXPECT_TUPLE_EQ(r.position(-1), point(1, 3, 4));
    EXPECT_TUPLE_EQ(r.position(2.5), point(4.5, 3, 4));
}

TEST(Rays, TranslatingARay) {
    Ray r(point(1, 2, 3), vec(0, 1, 0));
    Ray r2 = r.transform(transform::translate(3, 4, 5));
    EXPECT_TUPLE_EQ(r2.origin, point(4, 6, 8));
    EXPECT_TUPLE_EQ(r2.direction, vec(0, 1, 0));
}

TEST(Rays, ScalingARay) {
    Ray r(point(1, 2, 3), vec(0, 1, 0));
    Ray r2 = r.transform(transform::scale(2, 3, 4));
    EXPECT_TUPLE_EQ(r2.origin, point(2, 6, 12));
    EXPECT_TUPLE_EQ(r2.direction, vec(0, 3, 0));
}
