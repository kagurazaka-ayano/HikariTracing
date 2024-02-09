/**
 * @file Camera.h
 * @author ayano
 * @date 2/7/24
 * @brief
 */

#ifndef ONEWEEKEND_CAMERA_H
#define ONEWEEKEND_CAMERA_H

#include "GraphicObjects.h"
#include "ImageUtil.h"
#include "KawaiiMQ/Topic.h"
#include "MathUtil.h"
#include <future>
#include <thread>

class Camera {
public:
	Camera(int width, double aspect_ratio, double fov, Vec3 target, Point3 position, double dof_angle);

	int getWidth() const;

	int getHeight() const;

	double getAspectRatio() const;

	double getViewportWidth() const;

	double getViewportHeight() const;

	double getFocalLen() const;

	int getSampleCount() const;

	double getFov() const;

	const Point3 &getPosition() const;

	const Vec3 &getHoriVec() const;

	const Vec3 &getVertVec() const;

	const Vec3 &getPixDeltaX() const;

	const Vec3 &getPixDeltaY() const;

	const Point3 &getViewportUl() const;

	const Point3 &getPixel00() const;

	void setWidth(int width);

	void setAspectRatio(double aspect_ratio);

	void setPosition(const Point3 &position);

	void setSampleCount(int sample_count);

	void setFov(double fov);

	Ray getRay(int x, int y);

	void Render(const IHittable& world, const std::string& path, const std::string& name);

	int getRenderDepth() const;

	void setRenderDepth(int renderDepth);

	int getRenderThreadCount() const;

	void setRenderThreadCount(int renderThreadCount);

	const Point3 &getTarget() const;

	void setTarget(const Point3 &target);

	double getDofAngle() const;

	void setDofAngle(double dofAngle);

	void setFocalLen(double focalLen);

	int getChunkDimension() const;

	void setChunkDimension(int dimension);

	int partition();
private:

	void RenderWorker(const IHittable &world);

	Color rayColor(const Ray &ray, const IHittable &object, int depth);

	void updateVectors();

	Vec3 randomDisplacement() const;

	Point3 dofDiskSample() const;

	int width;
	int height;
	double aspect_ratio;
	double viewport_width;
	double viewport_height;
	double focal_len;
	double fov = 45;
	int sample_count = 20;
	int render_depth = 50;
	int render_thread_count = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	double dof_angle = 0;
	int chunk_dimension;
	Vec3 u, v, w;
	Point3 position;
	Point3 target;
	Vec3 UP = {0, 1, 0};
	Vec3 hori_vec;
	Vec3 vert_vec;
	Vec3 pix_delta_x;
	Vec3 pix_delta_y;
	Point3 viewport_ul;
	Point3 pixel_00;
	Vec3 dof_disk_h;
	Vec3 dof_disk_v;
};

#endif // ONEWEEKEND_CAMERA_H
