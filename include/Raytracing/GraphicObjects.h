/**
 * @file GraphicObjects.h
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#ifndef ONEWEEKEND_GRAPHICOBJECTS_H
#define ONEWEEKEND_GRAPHICOBJECTS_H

#include <chrono>
#include <future>
#include <memory>
#include <utility>
#include <vector>
#include "ImageUtil.h"
#include "MathUtil.h"

class IMaterial;

struct HitRecord {
	bool hit;
	Point3 p;
	double t;
	Vec3 normal;
	std::shared_ptr<IMaterial> material;
	bool front_face;
	void setFaceNormal(const Ray& r, const Vec3& normal_out);
};

class IHittable {
public:
	virtual ~IHittable() = default;

	virtual bool hit(const Ray& r, Interval interval, HitRecord& record) const = 0;
};

class Sphere : public IHittable {
public:
	Sphere(double radius, Vec3 position, std::shared_ptr<IMaterial> mat);

	bool hit(const Ray &r, Interval interval, HitRecord& record) const override;

private:
	double radius;
	Vec3 position;
	std::shared_ptr<IMaterial> material;
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
    bool hit(const Ray &r, Interval interval, HitRecord &record) const override;
};

#endif //ONEWEEKEND_GRAPHICOBJECTS_H
