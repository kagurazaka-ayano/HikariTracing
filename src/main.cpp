#include "scenes.h"

int main() {

	int option = 2;
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
		default:
			break;
	}
	return 0;
}