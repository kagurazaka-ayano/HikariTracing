#include "scenes.h"

int main() {

	int option = 4;
	switch(option) {
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
		default:
			break;
	}
	return 0;
}