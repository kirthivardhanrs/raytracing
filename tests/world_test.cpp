#include "raytracing/test_helpers.h"
#include "raytracing/Comps.h"
#include "raytracing/Intersection.h"
#include "raytracing/Light.h"
#include "raytracing/Ray.h"
#include "raytracing/Sphere.h"
#include "raytracing/World.h"
#include "raytracing/transform.h"
#include "raytracing/Color.h"
#include <vector>
#include <gtest/gtest.h>

using namespace testutil;

namespace {
    World makeDefaultWorld() {
        Sphere s1;
        s1.material.color = Color{ 0.8, 1.0, 0.6 };
        s1.material.diffuse = 0.7;
        s1.material.specular = 0.2;

        Sphere s2;
        s2.set_transform(transform::scale(0.5, 0.5, 0.5));

        return World{ std::vector<Sphere>{ s1, s2 },
                      std::vector<Light>{ Light{ Color{ 1, 1, 1 }, point(-10, 10, -10) } } };
    }
} 

TEST(World, CreatingAnEmptyWorld) {
    World w{ std::vector<Sphere>{}, std::vector<Light>{} };
    EXPECT_TRUE(w.objects.empty());
    EXPECT_TRUE(w.lights.empty());
}

TEST(World, TheDefaultWorld) {
    World w = makeDefaultWorld();
    ASSERT_EQ(w.lights.size(), 1u);
    EXPECT_TUPLE_EQ(w.lights[0].position, point(-10, 10, -10));
    EXPECT_COLOR_EQ(w.lights[0].intensity, (Color{ 1, 1, 1 }));

    ASSERT_EQ(w.objects.size(), 2u);
    EXPECT_COLOR_EQ(w.objects[0].material.color, (Color{ 0.8, 1.0, 0.6 }));
    EXPECT_DOUBLE_EQ(w.objects[0].material.diffuse, 0.7);
    EXPECT_DOUBLE_EQ(w.objects[0].material.specular, 0.2);
    EXPECT_MATRIX_EQ(w.objects[1].transform(), transform::scale(0.5, 0.5, 0.5));
}

TEST(World, IntersectAWorldWithARay) {
    World w = makeDefaultWorld();
    Ray r(point(0, 0, -5), vec(0, 0, 1));
    Intersections xs = w.intersect(r);
    ASSERT_EQ(xs.size(), 4u);
    
    EXPECT_DOUBLE_EQ(xs[0].t, 4);
    EXPECT_DOUBLE_EQ(xs[1].t, 4.5);
    EXPECT_DOUBLE_EQ(xs[2].t, 5.5);
    EXPECT_DOUBLE_EQ(xs[3].t, 6);
}



TEST(World, ShadingAnIntersection) {
    World w = makeDefaultWorld();
    Ray r(point(0, 0, -5), vec(0, 0, 1));
    Intersection i(4, w.objects[0]);
    Comps comps = i.prepare_comps(r);
    EXPECT_COLOR_EQ(w.shade_hit(comps), (Color{ 0.38066, 0.47583, 0.2855 }));
}

TEST(World, ShadingAnIntersectionFromTheInside) {
    World w = makeDefaultWorld();
    w.lights[0] = Light{ Color{ 1, 1, 1 }, point(0, 0.25, 0) };
    Ray r(point(0, 0, 0), vec(0, 0, 1));
    Intersection i(0.5, w.objects[1]);
    Comps comps = i.prepare_comps(r);
    EXPECT_COLOR_EQ(w.shade_hit(comps), (Color{ 0.90498, 0.90498, 0.90498 }));
}

TEST(World, ShadeHitIsGivenAnIntersectionInShadow) {
    Sphere s1;
    Sphere s2;
    s2.set_transform(transform::translate(0, 0, 10));
    World w{ std::vector<Sphere>{ s1, s2 },
             std::vector<Light>{ Light{ Color{ 1, 1, 1 }, point(0, 0, -10) } } };

    Ray r(point(0, 0, 5), vec(0, 0, 1));
    Intersection i(4, w.objects[1]);
    Comps comps = i.prepare_comps(r);
    EXPECT_COLOR_EQ(w.shade_hit(comps), (Color{ 0.1, 0.1, 0.1 }));
}



TEST(World, ColorWhenARayMisses) {
    World w = makeDefaultWorld();
    Ray r(point(0, 0, -5), vec(0, 1, 0));
    EXPECT_COLOR_EQ(w.color_at(r), (Color{ 0, 0, 0 }));
}

TEST(World, ColorWhenARayHits) {
    World w = makeDefaultWorld();
    Ray r(point(0, 0, -5), vec(0, 0, 1));
    EXPECT_COLOR_EQ(w.color_at(r), (Color{ 0.38066, 0.47583, 0.2855 }));
}

TEST(World, ColorWithAnIntersectionBehindTheRay) {
    World w = makeDefaultWorld();
    w.objects[0].material.ambient = 1;
    w.objects[1].material.ambient = 1;
    Ray r(point(0, 0, 0.75), vec(0, 0, -1));
    EXPECT_COLOR_EQ(w.color_at(r), w.objects[1].material.color);
}



TEST(Shadows, NoShadowWhenNothingIsCollinearWithPointAndLight) {
    World w = makeDefaultWorld();
    EXPECT_FALSE(w.is_shadowed(point(0, 10, 0)));
}

TEST(Shadows, ShadowWhenAnObjectIsBetweenThePointAndTheLight) {
    World w = makeDefaultWorld();
    EXPECT_TRUE(w.is_shadowed(point(10, -10, 10)));
}

TEST(Shadows, NoShadowWhenAnObjectIsBehindTheLight) {
    World w = makeDefaultWorld();
    EXPECT_FALSE(w.is_shadowed(point(-20, 20, -20)));
}

TEST(Shadows, NoShadowWhenAnObjectIsBehindThePoint) {
    World w = makeDefaultWorld();
    EXPECT_FALSE(w.is_shadowed(point(-2, 2, -2)));
}
