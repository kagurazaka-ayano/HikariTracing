#include "scenes.h"

#define TEST_ALL

int main() {

	switch (
#ifdef TEST_ALL
			0
#else
			3
#endif
	) {
		case 0:
			randomSpheres();
#ifndef TEST_ALL
			break;
#endif
		case 1:
			twoSpheres();
#ifndef TEST_ALL
			break;
#endif
		case 2:
			huajiSphere();
#ifndef TEST_ALL
			break;
#endif
		case 3:
			perlinSpheres();
#ifndef TEST_ALL
			break;
#endif
		case 4:
			terrain();
#ifndef TEST_ALL
			break;
#endif
		case 5:
			rotationTest();
#ifndef TEST_ALL
			break;
#endif
		default:
			break;
	}
	return 0;
}
