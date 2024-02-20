/**
 * @file scenes.h
 * @author ayano
 * @date 2/15/24
 * @brief
 */

#ifndef RAYTRACING_SCENES_H
#define RAYTRACING_SCENES_H

#include "GraphicObjects.h"
#include "Camera.h"

void randomSpheres();

void twoSpheres();

void huajiSphere();

void perlinSpheres();

void terrain();

void render(HittableList object, Camera camera);

#endif // RAYTRACING_SCENES_H
