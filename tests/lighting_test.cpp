#include "raytracing/test_helpers.h"
#include "raytracing/Light.h"
#include "raytracing/Material.h"

using namespace testutil;

TEST(Lights, PointLightHasPositionAndIntensity) {
    Color intensity{ 1, 1, 1 };
    Tuple position = point(0, 0, 0);
    Light light(intensity, position);
    EXPECT_TUPLE_EQ(light.position, position);
    EXPECT_COLOR_EQ(light.intensity, intensity);
}

TEST(Materials, DefaultMaterial) {
    Material m;
    EXPECT_COLOR_EQ(m.color, (Color{ 1, 1, 1 }));
    EXPECT_DOUBLE_EQ(m.ambient, 0.1);
    EXPECT_DOUBLE_EQ(m.diffuse, 0.9);
    EXPECT_DOUBLE_EQ(m.specular, 0.9);
    EXPECT_DOUBLE_EQ(m.shininess, 200.0);
}

class Lighting : public ::testing::Test {
protected:
    Material m;
    Tuple position = point(0, 0, 0);
};

TEST_F(Lighting, EyeBetweenLightAndSurface) {
    Tuple eyev = vec(0, 0, -1), normalv = vec(0, 0, -1);
    Light light(Color{ 1, 1, 1 }, point(0, 0, -10));
    EXPECT_COLOR_EQ(light.lighting(m, position, eyev, normalv, false), (Color{ 1.9, 1.9, 1.9 }));
}

TEST_F(Lighting, EyeBetweenLightAndSurfaceEyeOffset45) {
    Tuple eyev = vec(0, kSqrt2 / 2, -kSqrt2 / 2), normalv = vec(0, 0, -1);
    Light light(Color{ 1, 1, 1 }, point(0, 0, -10));
    EXPECT_COLOR_EQ(light.lighting(m, position, eyev, normalv, false), (Color{ 1.0, 1.0, 1.0 }));
}

TEST_F(Lighting, EyeOppositeSurfaceLightOffset45) {
    Tuple eyev = vec(0, 0, -1), normalv = vec(0, 0, -1);
    Light light(Color{ 1, 1, 1 }, point(0, 10, -10));
    EXPECT_COLOR_EQ(light.lighting(m, position, eyev, normalv, false), (Color{ 0.7364, 0.7364, 0.7364 }));
}

TEST_F(Lighting, EyeInThePathOfTheReflectionVector) {
    Tuple eyev = vec(0, -kSqrt2 / 2, -kSqrt2 / 2), normalv = vec(0, 0, -1);
    Light light(Color{ 1, 1, 1 }, point(0, 10, -10));
    EXPECT_COLOR_EQ(light.lighting(m, position, eyev, normalv, false), (Color{ 1.6364, 1.6364, 1.6364 }));
}

TEST_F(Lighting, LightBehindTheSurface) {
    Tuple eyev = vec(0, 0, -1), normalv = vec(0, 0, -1);
    Light light(Color{ 1, 1, 1 }, point(0, 0, 10));
    EXPECT_COLOR_EQ(light.lighting(m, position, eyev, normalv, false), (Color{ 0.1, 0.1, 0.1 }));
}

TEST_F(Lighting, SurfaceInShadow) {
    Tuple eyev = vec(0, 0, -1), normalv = vec(0, 0, -1);
    Light light(Color{ 1, 1, 1 }, point(0, 0, -10));
    EXPECT_COLOR_EQ(light.lighting(m, position, eyev, normalv, true), (Color{ 0.1, 0.1, 0.1 }));
}
