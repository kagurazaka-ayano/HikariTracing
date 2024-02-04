/**
 * @file GraphicObjects.h
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#ifndef ONEWEEKEND_GRAPHICOBJECTS_H
#define ONEWEEKEND_GRAPHICOBJECTS_H

#include "MathUtil.h"
#include "ImageUtil.h"
#include <chrono>
#include <utility>
#include <vector>
#include <memory>

struct HitRecord {
	bool hit;
	MathUtil::Point3 p;
	double t;
	MathUtil::Vec3 normal;
	bool front_face;
	void setFaceNormal(const MathUtil::Ray& r, const MathUtil::Vec3& normal_out);
};

class IHittable {
public:
	virtual ~IHittable() = default;

	virtual bool hit(const MathUtil::Ray& r, MathUtil::Interval interval, HitRecord& record) const = 0;
};

class Sphere : public IHittable {
public:
	Sphere(double radius, MathUtil::Vec3 position);

	bool hit(const MathUtil::Ray &r, MathUtil::Interval interval, HitRecord& record) const override;

private:
	double radius;
	MathUtil::Vec3 position;
};

class Camera {
public:
    Camera(int width, double aspect_ratio, double viewport_width, double focal_len, MathUtil::Point3 position);

    int getWidth() const;

    int getHeight() const;

    double getAspectRatio() const;

    double getViewportWidth() const;

    double getViewportHeight() const;

    double getFocalLen() const;

	int getSampleCount() const;

    const MathUtil::Point3 &getPosition() const;

    const MathUtil::Vec3 &getHoriVec() const;

    const MathUtil::Vec3 &getVertVec() const;

    const MathUtil::Vec3 &getPixDeltaX() const;

    const MathUtil::Vec3 &getPixDeltaY() const;

    const MathUtil::Point3 &getViewportUl() const;

    const MathUtil::Point3 &getPixel00() const;

    void setWidth(int width);

    void setAspectRatio(double aspect_ratio);

    void setFocalLen(double focal_len);

    void setPosition(const MathUtil::Point3 &position);

	void setSampleCount(int sample_count);

    [[nodiscard]] MathUtil::Vec3 getPixelVec(int x, int y) const;

    [[nodiscard]] MathUtil::Vec3 getPixRayDir(int x, int y) const;

	void Render(const IHittable& world);

	ImageUtil::Color rayColor(const MathUtil::Ray& ray, const IHittable& object);

private:

    void updateVectors();

	MathUtil::Vec3 randomDisplacement() const;

    int width;
    int height;
    double aspect_ratio;
    double viewport_width;
    double viewport_height;
    double focal_len;
	int sample_count = 20;
    MathUtil::Point3 position;
    MathUtil::Vec3 hori_vec;
    MathUtil::Vec3 vert_vec;
    MathUtil::Vec3 pix_delta_x;
    MathUtil::Vec3 pix_delta_y;
    MathUtil::Point3 viewport_ul;
    MathUtil::Point3 pixel_00;
};




class HittableList: public IHittable {
public:
    std::vector<std::shared_ptr<IHittable>> objects;

    HittableList() = default;

    explicit HittableList(const std::shared_ptr<IHittable>& obj);

    void add(const std::shared_ptr<IHittable>& obj);

    void clear();

    auto begin();

    auto end();

private:
    bool hit(const MathUtil::Ray &r, MathUtil::Interval interval, HitRecord &record) const override;
};

#endif //ONEWEEKEND_GRAPHICOBJECTS_H
