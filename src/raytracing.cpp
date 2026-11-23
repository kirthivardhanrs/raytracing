#include <numbers>
#include "raytracing/Tuple.h"
#include "raytracing/Color.h"
#include "raytracing/Canvas.h"
#include "raytracing/Matrix.h"
#include "raytracing/transform.h"
#include "raytracing/Sphere.h"
#include "raytracing/Light.h"
#include "raytracing/Camera.h"
#include "raytracing/World.h"

int main() {
    Sphere ground{};
    ground.set_transform(
        transform::scale(300, 0.01, 300)
    );
    ground.material = Material{
        Color{ 0.8, 0.62, 0.45 }, 0.3, 0.9, 0, 200
    };

    Sphere sky{};
    sky.set_transform(
        transform::translate(0, 0, 25) *
        transform::scale(300, 60, 0.01)
    );
    sky.material = Material{
        Color{ 0.9, 0.7, 0.8 }, 0.5, 0.9, 0, 200
    };

    std::vector<Sphere> objects{ ground, sky };

    for (int i = 0; i < 5; ++i) {
        const double r = 0.5 + 0.2 * i;

        Sphere s{};
        s.set_transform(
            transform::translate(
                -1.5 + 0.8 * i,
                r,
                -5 + 2.8 * i
            ) *
            transform::scale(r, r, r)
        );

        s.material = Material{
            Color{ 0.95, 0.9, 0.85 },
            0.25,
            0.9,
            0.3,
            50
        };

        objects.push_back(s);
    }

    World w{
        objects,
        { Light{ Color{ 1.0, 0.65, 0.4 }, Tuple{ -30, 9, -12, 1 } } }
    };

    Camera c{ 100, 50, std::numbers::pi / 3 };
    c.transform = transform::view_transform(
        Tuple{ 2, 2.0, -12, 1 },
        Tuple{ 1, 0.9, 0, 1 },
        Tuple{ 0, 1, 0, 0 }
    );

    Canvas canvas{ c.render(w) };
    canvas.toPPM();
}