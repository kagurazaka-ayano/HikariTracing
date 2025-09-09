#include <cstdlib>
#include "scenes.h"

int main(int argc, char **argv) {

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
