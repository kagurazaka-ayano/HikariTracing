/**
 * @file GpuRenderer.h
 * @author Kagurazaka Ayano
 * @date 16/08/25
 * @brief A gpu renderer
 */

#ifndef RAYTRACING_GPURENDERER_H
#define RAYTRACING_GPURENDERER_H

#include <vector>
#include "Camera.h"
#include "GraphicObjects.h"
class GpuRenderer {
public:
	struct GpuImage {
		std::size_t width, height;
		std::vector<float> data;
	};
	struct GpuRendererConf {
		std::size_t workgroup_size_x, workgroup_size_y, sample_per_pixel, max_depth;
	};
	GpuImage render(const IHittable &world, const Camera &cam, const GpuRendererConf &conf);
};

#endif // RAYTRACING_GPURENDERER_H
