/**
 * @file GraphicObjects.h
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#ifndef ONEWEEKEND_GRAPHICOBJECTS_H
#define ONEWEEKEND_GRAPHICOBJECTS_H


#include <memory>
#include <vector>
#include "AppleMath/Vector.hpp"
#include "MathUtil.h"

class IMaterial;

struct HitRecord {
	bool hit;
	Point3 p;
	double t;
	double u;
	double v;
	AppleMath::Vector3 normal;
	std::shared_ptr<IMaterial> material;
	bool front_face;
	void setFaceNormal(const Ray& r, const AppleMath::Vector3& normal_out);
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
	Sphere(double radius, AppleMath::Vector3 position, std::shared_ptr<IMaterial> mat);

	Sphere(double radius, const Point3& init_position, const Point3& final_position, std::shared_ptr<IMaterial> mat);

	AppleMath::Vector3 getPosition(double time) const;



	bool hit(const Ray &r, Interval interval, HitRecord& record) const override;

	AABB boundingBox() const override;

private:

	static void getSphereUV(const Point3& p, double& u, double& v);

	AppleMath::Vector3 direction_vec;
	bool is_moving = false;
	double radius;
	AABB bbox;
	AppleMath::Vector3 position;
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

class Quad : public IHittable {
public:
	Quad(const AppleMath::Vector3& Q, const AppleMath::Vector3& u, const AppleMath::Vector3& v, std::shared_ptr<IMaterial> mat);

	virtual ~Quad() = default;

	AABB boundingBox() const override;

	virtual void setBoundingBox();

	bool hit(const Ray& r, Interval interval, HitRecord& record) const override;

	bool inside(double a, double b, HitRecord& rec) const;

private:
	AppleMath::Vector3 Q, u, v;
	AppleMath::Vector3 normal;
	double D;
	AppleMath::Vector3 w;
	std::shared_ptr<IMaterial> mat;
	AABB bbox;
	
};

class Triangle : public IHittable {
public:

	Triangle(const AppleMath::Vector3& Q, const AppleMath::Vector3& u, const AppleMath::Vector3& v, std::shared_ptr<IMaterial> mat);

	virtual ~Triangle() = default;

	virtual void setBoundingBox();

	AABB boundingBox() const override;

	bool hit(const Ray& r, Interval interval, HitRecord& record) const override;

	bool inside(const AppleMath::Vector3& intersection) const;

private:

	AppleMath::Vector3 Q, u, v;
	AppleMath::Vector3 normal;
	AppleMath::Vector3 w;
	double D;

	std::shared_ptr<IMaterial> mat;
	AABB box;
};


#endif //ONEWEEKEND_GRAPHICOBJECTS_H
