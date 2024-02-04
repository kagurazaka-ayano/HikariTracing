#include "ImageUtil.h"
#include "GraphicObjects.h"

int main() {
    auto camera = Camera(1920, 16.0 / 9.0, 2.0, 1, MathUtil::Point3(0, 0, 0));
	auto world = HittableList();
	world.add(std::make_shared<Sphere>(Sphere(80, {0, -85, -20})));
	world.add(std::make_shared<Sphere>(Sphere(5, {0, 0, -20})));
	camera.setSampleCount(100);
	camera.Render(world);

    return 0;
}
