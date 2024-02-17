/**
 * @file scenes.h
 * @author ayano
 * @date 2/15/24
 * @brief
 */

#ifndef RAYTRACING_SCENES_H
#define RAYTRACING_SCENES_H

#include "ImageUtil.h"
#include "GraphicObjects.h"
#include "Material.h"
#include "Camera.h"
#include "Texture.h"

void randomSpheres();

void twoSpheres();

void huajiSphere();

void render(HittableList object, Camera camera);

#endif // RAYTRACING_SCENES_H
