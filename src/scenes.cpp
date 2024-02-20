/**
 * @file scenes.cpp
 * @author ayano
 * @date 2/15/24
 * @brief
 */

#include "scenes.h"
#include <memory>
#include "AppleMath/Vector.hpp"
#include "Camera.h"
#include "GraphicObjects.h"
#include "MathUtil.h"
#include "Texture.h"
#include "Material.h"

void render(HittableList world, Camera camera) {
#ifndef ASCII_ART
	auto file = camera.Render(world, "test.ppm");
	system(std::string("open " + file).c_str());
#else
	auto file = camera.Render(world, "test.txt");
	system(std::string("open " + file).c_str());
#endif
}

void randomSpheres() {

	auto camera = Camera(1920, 16.0 / 9.0, 30, AppleMath::Vector3{0, 0, 0}, AppleMath::Vector3{-13, 2, 3}, 0.6);

	camera.setSampleCount(100);
	camera.setShutterSpeed(1.0/24.0);
	camera.setRenderDepth(50);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);
	auto world = HittableList();
	auto checker = std::make_shared<CheckerTexture>(0.5, Color{0.1, 0.1, 0.1}, Color{0.9, 0.9, 0.9});
	auto ground_material = std::make_shared<Lambertian>(Lambertian(checker));
	auto left_ball_material = std::make_shared<Lambertian>(Color{0.357, 0.816, 0.98});
	auto center_ball_material = std::make_shared<Metal>(Metal(Color{0.965, 0.671, 0.729}, 0.4));
	auto right_ball_material = std::make_shared<Dielectric>(Dielectric(1.5, Color{0.8, 0.8, 0.8}));
	world.add(std::make_shared<Sphere>(Sphere(1000, AppleMath::Vector3{0, -1000, -1.0}, ground_material)));
	world.add(std::make_shared<Sphere>(Sphere(1, AppleMath::Vector3{0, 1, 0}, center_ball_material)));
	world.add(std::make_shared<Sphere>(Sphere(1, AppleMath::Vector3{4, 1, 0}, right_ball_material)));
	world.add(std::make_shared<Sphere>(Sphere(1, AppleMath::Vector3{-4, 1, 0}, left_ball_material)));
	int obj = 0;
	for(int i = -11; i < 11; i += 2) {
		for (int j = -11; j < 11; j += 2) {
			obj++;
			auto coord = AppleMath::Vector3{(i + randomDouble(-1, 1)), 0.2,(j + randomDouble(-1, 1))};
			auto displacement = AppleMath::Vector3{0, randomDouble(0, 0), 0};
			auto material = static_cast<int>(3.0 * randomDouble());
			if ((coord - AppleMath::Vector3{0, 1, 0}).length() > 0.9) {
				AppleMath::Vector3 color = randomVec3().componentProd(randomVec3());
				std::shared_ptr<IMaterial> sphere_mat;
				switch (material) {
					case 0:
						sphere_mat = std::make_shared<Lambertian>(color);
						world.add(std::make_shared<Sphere>(0.2, coord, coord + displacement, sphere_mat));
						break;
					case 1:
						sphere_mat = std::make_shared<Metal>(color, randomDouble(0.2, 0.5));
						world.add(std::make_shared<Sphere>(0.2, coord, coord + displacement, sphere_mat));
						break;
					case 2:
						color = randomVec3(0.7, 1);
						sphere_mat = std::make_shared<Dielectric>(randomDouble(1, 2), color);
						world.add(std::make_shared<Sphere>(0.2, coord, coord + displacement, sphere_mat));
						break;
					default:
						break;
				}

			}
		}
	}
	world = HittableList(std::make_shared<BVHNode>(world));
	render(world, camera);
}

void twoSpheres() {
	auto camera = Camera(400, 16.0 / 9.0, 30, AppleMath::Vector3{0, 0, -30}, AppleMath::Vector3{0, 0, 0}, 0.6);
	camera.setSampleCount(100);
	camera.setShutterSpeed(1.0/24.0);
	camera.setRenderDepth(50);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);
	auto world = HittableList();
	auto checker = std::make_shared<CheckerTexture>(2, Color{0.1, 0.1, 0.1}, Color{0.9, 0.9, 0.9});
	auto sphere_material = std::make_shared<Lambertian>(Lambertian(checker));
	world.add(std::make_shared<Sphere>(20.0, Point3{0, -20.0, -30}, sphere_material));
	world.add(std::make_shared<Sphere>(20.0, Point3{0, 20.0, -30}, sphere_material));
	world = HittableList(std::make_shared<BVHNode>(world));
	render(world, camera);
}

void huajiSphere() {
	auto camera = Camera(1920, 16.0 / 9.0, 45, AppleMath::Vector3{0, 0, -30}, AppleMath::Vector3{30, 0, -30}, 0.1);
	camera.setSampleCount(100);
	camera.setShutterSpeed(1.0/24.0);
	camera.setRenderDepth(50);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);
	auto world = HittableList();
	auto huaji_texture = std::make_shared<ImageTexture>("huaji.jpeg");
	auto huaji_material = std::make_shared<Lambertian>(huaji_texture);
	world.add(std::make_shared<Sphere>(10.0, Point3{0, 0, -30}, huaji_material));
	world = HittableList(std::make_shared<BVHNode>(world));
	render(world, camera);
}

void perlinSpheres()
{
	HittableList world; 
	Camera camera(1920, 16.0 / 9.0, 20, Point3{0, 0, 0}, Point3{13, 2, 3}, 0);
	camera.setSampleCount(100);
	camera.setShutterSpeed(1.0/24.0);
	camera.setRenderDepth(50);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);
	auto tex = std::make_shared<NoiseTexture>(1, 10, 0.5);
	world.add(std::make_shared<Sphere>(1000, Point3{0, -1000, 0}, std::make_shared<Lambertian>(tex)));
	world.add(std::make_shared<Sphere>(2, Point3{0, 2, 0}, std::make_shared<Lambertian>(tex)));

	render(world, camera);
}

void terrain()
{
	HittableList world; 
	Camera camera(100, 16.0 / 9.0, 20, Point3{0, 0, 0}, Point3{0, 0, -50}, 0);
	camera.setSampleCount(10);
	camera.setShutterSpeed(1.0/24.0);
	camera.setRenderDepth(4);
	camera.setRenderThreadCount(12);
	camera.setChunkDimension(64);
	auto tex = std::make_shared<TerrainTexture>(0.5, 10, 0.5);
	world.add(std::make_shared<Sphere>(10, Point3{0, 0, 0}, std::make_shared<Lambertian>(tex)));

	render(world, camera);
}


