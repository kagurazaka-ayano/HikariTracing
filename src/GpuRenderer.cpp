/**
 * @file GpuRenderer.cpp
 * @author ayano
 * @date 16/08/25
 * @brief
 */

#include "GpuRenderer.h"

GpuRenderer::GpuImage GpuRenderer::render(const IHittable &world, const Camera &cam, const GpuRendererConf &conf) {

	struct SphereGpu {
		float cx, cy, cz, radius;
		uint32_t material_idx;
		// padding is used to better utilizing cache line
		uint32_t pad[3];
	};

	struct MaterialGpu {
		uint32_t type;
		float r, g, b;
		float fuzz;
		float idx_of_refraction;
		float r_emm, g_emm, b_emm;
	};

	struct Params {
		// camera metadata
		float px, py, pz, pad0;
		float ux, uy, uz, pad1;
		float vx, vy, vz, pad2;
		float wx, wy, wz, pad3;
		float pixel00x, pixel00y, pixel00z, pad4;
		float dxx, dxy, dxz, pad5;
		float d
	};
}
