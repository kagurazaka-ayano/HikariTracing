#include "ImageUtil.h"
#include "GraphicObjects.h"
#include "Material.h"
#include "Camera.h"


int main() {
	auto camera = Camera(200, 16.0 / 5.0, 45, {0, 1, 0}, {0, 1, -4}, 0.6);
	auto world = HittableList();
	auto ground_material = std::make_shared<Metal>(Metal(Color(0.69, 0.72, 0.85), 0.4));
	auto left_ball_material = std::make_shared<Lambertian>(Lambertian(Color(0.357, 0.816, 0.98)));
	auto center_ball_material = std::make_shared<Metal>(Metal(Color(0.965, 0.671, 0.729), 0.4));
	auto right_ball_material = std::make_shared<Dielectric>(Dielectric(1.5, Color(0.8, 0.8, 0.8)));
	world.add(std::make_shared<Sphere>(Sphere(1000, {0, -1000, -1.0}, ground_material)));
	world.add(std::make_shared<Sphere>(Sphere(1, {0, 1, 0}, center_ball_material)));
	world.add(std::make_shared<Sphere>(Sphere(1, {2, 1, 0}, right_ball_material)));
	world.add(std::make_shared<Sphere>(Sphere(1, {-2, 1, 0}, left_ball_material)));
	camera.setSampleCount(5);
	camera.setShutterSpeed(1.0/24.0);
//	int obj = 0;
//	for(int i = -15; i < 15; i += 2) {
//		for (int j = -15; j < 15; j += 2) {
//			obj++;
//			auto coord = Vec3((i + randomDouble(-1, 1)), 0.2,(j + randomDouble(-1, 1)));
//			auto displacement = Vec3{0, randomDouble(0, 0), 0};
//			auto material = static_cast<int>(3.0 * randomDouble());
//			if ((coord - Vec3{0, 1, 0}).length() > 0.9) {
//				Vec3 color;
//				std::shared_ptr<IMaterial> sphere_mat;
//				switch (material) {
//					case 0:
//						color = Vec3::random() * Vec3::random();
//						sphere_mat = std::make_shared<Lambertian>(color);
//						world.add(std::make_shared<Sphere>(0.2, coord, coord + displacement, sphere_mat));
//						break;
//					case 1:
//						color = Vec3::random() * Vec3::random();
//						sphere_mat = std::make_shared<Metal>(color, randomDouble(0.2, 0.5));
//						world.add(std::make_shared<Sphere>(0.2, coord, coord + displacement, sphere_mat));
//						break;
//					case 2:
//						color = Vec3::random(0.7, 1);
//						sphere_mat = std::make_shared<Dielectric>(randomDouble(1, 2), color);
//						world.add(std::make_shared<Sphere>(0.2, coord, coord + displacement, sphere_mat));
//						break;
//					default:
//						break;
//				}
//
//			}
//		}
//	}
	world = HittableList(std::make_shared<BVHNode>(world));
	camera.setRenderDepth(4);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(20);
#ifndef ASCII_ART
	camera.Render(world, "out", "test.ppm");
	system(std::string("open out/test.ppm").c_str());
#else
	camera.Render(world, "out", "test.txt");
	system(std::string("open out/test.txt").c_str());
#endif
	return 0;
}