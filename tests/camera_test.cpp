#include "raytracing/test_helpers.h"
#include "raytracing/mathutils.h"
#include "raytracing/Camera.h"
#include "raytracing/Canvas.h"
#include "raytracing/Light.h"
#include "raytracing/Sphere.h"
#include "raytracing/World.h"
#include "raytracing/transform.h"

using namespace testutil;

TEST(Camera, Constructing) {
    Camera c(160, 120, kPi / 2);
    EXPECT_DOUBLE_EQ(c.hsize, 160);
    EXPECT_DOUBLE_EQ(c.vsize, 120);
    EXPECT_DOUBLE_EQ(c.fov, kPi / 2);
    EXPECT_MATRIX_EQ(c.transform, mathutils::identity);
}

TEST(Camera, PixelSizeForAHorizontalCanvas) {
    Camera c(200, 125, kPi / 2);
    EXPECT_NEAR(c.pixel_size, 0.01, 1e-9);
}

TEST(Camera, PixelSizeForAVerticalCanvas) {
    Camera c(125, 200, kPi / 2);
    EXPECT_NEAR(c.pixel_size, 0.01, 1e-9);
}

TEST(Camera, RayThroughTheCenterOfTheCanvas) {
    Camera c(201, 101, kPi / 2);
    Ray r = c.ray_for_pixel(100, 50);
    EXPECT_TUPLE_EQ(r.origin, point(0, 0, 0));
    EXPECT_TUPLE_EQ(r.direction, vec(0, 0, -1));
}

TEST(Camera, RayThroughACornerOfTheCanvas) {
    Camera c(201, 101, kPi / 2);
    Ray r = c.ray_for_pixel(0, 0);
    EXPECT_TUPLE_EQ(r.origin, point(0, 0, 0));
    EXPECT_TUPLE_EQ(r.direction, vec(0.66519, 0.33259, -0.66851));
}

TEST(Camera, RayWhenTheCameraIsTransformed) {
    Camera c(201, 101, kPi / 2);
    c.transform = transform::rotation_y(kPi / 4) * transform::translate(0, -2, 5);
    Ray r = c.ray_for_pixel(100, 50);
    EXPECT_TUPLE_EQ(r.origin, point(0, 2, -5));
    EXPECT_TUPLE_EQ(r.direction, vec(kSqrt2 / 2, 0, -kSqrt2 / 2));
}

TEST(Camera, RenderingAWorldWithACamera) {
    Sphere s1;
    s1.material.color = Color{ 0.8, 1.0, 0.6 };
    s1.material.diffuse = 0.7;
    s1.material.specular = 0.2;
    Sphere s2;
    s2.set_transform(transform::scale(0.5, 0.5, 0.5));
    World w{ std::vector<Sphere>{ s1, s2 },
             std::vector<Light>{ Light{ Color{ 1, 1, 1 }, point(-10, 10, -10) } } };

    Camera c(11, 11, kPi / 2);
    c.transform = transform::view_transform(point(0, 0, -5), point(0, 0, 0), vec(0, 1, 0));

    Canvas image = c.render(w);
    EXPECT_COLOR_EQ(image(5, 5), (Color{ 0.38066, 0.47583, 0.2855 }));
}
