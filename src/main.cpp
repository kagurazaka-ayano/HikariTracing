#include <cstdlib>
#include <iostream>
#include <spdlog/spdlog.h>
#include "scenes.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <scene_number>" << std::endl;
		return 1;
	}
	if (std::string(argv[1]) == "all") {
		spdlog::info("running all scenes");
		randomSpheres();
		spdlog::info("randomSpheres done");
		twoSpheres();
		spdlog::info("twoSpheres done");
		huajiSphere();
		spdlog::info("huajiSpheres done");
		perlinSpheres();
		spdlog::info("perlinSpheres done");
		terrain();
		spdlog::info("terrain done");
		quads();
		spdlog::info("quads done");
		triangles();
		spdlog::info("triangles done");
		cornellBox();
		spdlog::info("cornellBox done");
		cornellBoxWithObjects();
		spdlog::info("cornellBoxWithObjects done");
	}
	switch (std::atoi(argv[1])) {
		case 0:
			randomSpheres();
			break;
		case 1:
			twoSpheres();
			break;
		case 2:
			huajiSphere();
			break;
		case 3:
			perlinSpheres();
			break;
		case 4:
			terrain();
			break;
		case 5:
			rotationTest();
			break;
		case 6:
			quads();
			break;
		case 7:
			triangles();
			break;
		case 8:
			cornellBox();
			break;
		case 9:
			cornellBoxWithObjects();
			break;
		default:
			break;
	}
	return 0;
}
