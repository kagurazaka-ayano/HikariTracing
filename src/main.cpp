#include "scenes.h"

int main() {

	switch(5) {
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
		default:
			break;
	}
	return 0;
}