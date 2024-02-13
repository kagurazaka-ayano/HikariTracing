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

	virtual AABB boundingBox() const = 0;
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

	AABB boundingBox() const override;

private:
    bool hit(const Ray &r, Interval interval, HitRecord &record) const override;

	AABB bbox;
};

class Sphere : public IHittable {
public:
	Sphere(double radius, Vec3 position, std::shared_ptr<IMaterial> mat);

	Sphere(double radius, const Point3& init_position, const Point3& final_position, std::shared_ptr<IMaterial> mat);

	Vec3 getPosition(double time) const;
	
	bool hit(const Ray &r, Interval interval, HitRecord& record) const override;

	AABB boundingBox() const override;

private:
	Vec3 direction_vec;
	bool is_moving = false;
	double radius;
	AABB bbox;
	Vec3 position;
	std::shared_ptr<IMaterial> material;
};

class BVHNode : public IHittable {
public:
	BVHNode() = default;

	BVHNode(const HittableList& list);

	BVHNode(const std::vector<std::shared_ptr<IHittable>>& objects, size_t start, size_t end);

	bool hit(const Ray &r, Interval interval, HitRecord &record) const override;

	AABB boundingBox() const override;
private:
	std::shared_ptr<IHittable> left;
	std::shared_ptr<IHittable> right;

	static bool compare(const std::shared_ptr<IHittable>& a, const std::shared_ptr<IHittable>& b, int axis);

	static bool compareX(const std::shared_ptr<IHittable>& a, const std::shared_ptr<IHittable>& b);

	static bool compareY(const std::shared_ptr<IHittable>& a, const std::shared_ptr<IHittable>& b);

	static bool compareZ(const std::shared_ptr<IHittable>& a, const std::shared_ptr<IHittable>& b);

	AABB bbox;
};


#endif //ONEWEEKEND_GRAPHICOBJECTS_H
