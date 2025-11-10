/**
 * @file GraphicObjects.h
 * @author ayano
 * @date 1/16/24
 * @brief
 */

#ifndef ONEWEEKEND_GRAPHICOBJECTS_H
#define ONEWEEKEND_GRAPHICOBJECTS_H

#include "MathUtil.h"

#include <memory>
#include <vector>

class IMaterial;

struct HitRecord {
	bool hit;
	Point3 p;
	float t;
	float u;
	float v;
	Eigen::Vector3d normal;
	std::shared_ptr<IMaterial> material;
	bool front_face;
	void setFaceNormal(const Ray &r, const Eigen::Vector3d &normal_out);
};

class IHittable {
public:
	virtual ~IHittable() = default;

	virtual bool hit(const Ray &r, Interval interval, HitRecord &record) const = 0;

	virtual AABB boundingBox() const = 0;
};

class HittableList : public IHittable {
public:
	std::vector<std::shared_ptr<IHittable>> objects;

	HittableList() = default;

	explicit HittableList(const std::shared_ptr<IHittable> &obj);

	void add(const std::shared_ptr<IHittable> &obj);

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
	Sphere(float radius, Eigen::Vector3d position, std::shared_ptr<IMaterial> mat);

	Sphere(float radius, const Point3 &init_position, const Point3 &final_position, std::shared_ptr<IMaterial> mat);

	Eigen::Vector3d getPosition(float time) const;

	bool hit(const Ray &r, Interval interval, HitRecord &record) const override;

	AABB boundingBox() const override;

private:
	static void getSphereUV(const Point3 &p, float &u, float &v);

	Eigen::Vector3d direction_vec;
	bool is_moving = false;
	float radius;
	AABB bbox;
	Eigen::Vector3d position;
	std::shared_ptr<IMaterial> material;
};

class BVHNode : public IHittable {
public:
	BVHNode() = default;

	BVHNode(const HittableList &list);

	BVHNode(const std::vector<std::shared_ptr<IHittable>> &objects, size_t start, size_t end);

	bool hit(const Ray &r, Interval interval, HitRecord &record) const override;

	AABB boundingBox() const override;

private:
	std::shared_ptr<IHittable> left;
	std::shared_ptr<IHittable> right;

	static bool compare(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b, int axis);

	static bool compareX(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b);

	static bool compareY(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b);

	static bool compareZ(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b);

	AABB bbox;
};

class Quad : public IHittable {
public:
	Quad(const Eigen::Vector3d &Q, const Eigen::Vector3d &u, const Eigen::Vector3d &v, std::shared_ptr<IMaterial> mat);

	virtual ~Quad() = default;

	AABB boundingBox() const override;

	virtual void setBoundingBox();

	bool hit(const Ray &r, Interval interval, HitRecord &record) const override;

	bool inside(float a, float b, HitRecord &rec) const;

private:
	Eigen::Vector3d Q, u, v;
	Eigen::Vector3d normal;
	float D;
	Eigen::Vector3d w;
	std::shared_ptr<IMaterial> mat;
	AABB bbox;
};

class Triangle : public IHittable {
public:
	Triangle(const Eigen::Vector3d &Q, const Eigen::Vector3d &u, const Eigen::Vector3d &v,
			 std::shared_ptr<IMaterial> mat);

	virtual ~Triangle() = default;

	virtual void setBoundingBox();

	AABB boundingBox() const override;

	bool hit(const Ray &r, Interval interval, HitRecord &record) const override;

	bool inside(const Eigen::Vector3d &intersection) const;

private:
	Eigen::Vector3d Q, u, v;
	Eigen::Vector3d normal;
	Eigen::Vector3d w;
	float D;

	std::shared_ptr<IMaterial> mat;
	AABB box;
};

inline std::shared_ptr<HittableList> box(const Point3 &a, const Point3 &b, std::shared_ptr<IMaterial> mat) {
	auto sides = std::make_shared<HittableList>();

	auto min = Point3{std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z())};
	auto max = Point3{std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z())};

	auto dx = Eigen::Vector3d{max.x() - min.x(), 0, 0};
	auto dy = Eigen::Vector3d{0, max.y() - min.y(), 0};
	auto dz = Eigen::Vector3d{0, 0, max.z() - min.z()};

	sides->add(make_shared<Quad>(Point3{min.x(), min.y(), max.z()}, dx, dy,
								 mat)); // front
	sides->add(make_shared<Quad>(Point3{max.x(), min.y(), max.z()}, -dz, dy,
								 mat)); // right
	sides->add(make_shared<Quad>(Point3{max.x(), min.y(), min.z()}, -dx, dy,
								 mat)); // back
	sides->add(make_shared<Quad>(Point3{min.x(), min.y(), min.z()}, dz, dy,
								 mat)); // left
	sides->add(make_shared<Quad>(Point3{min.x(), max.y(), max.z()}, dx, -dz,
								 mat)); // top
	sides->add(make_shared<Quad>(Point3{min.x(), min.y(), min.z()}, dx, dz,
								 mat)); // bottom

	return sides;
}

class Translate : public IHittable {
public:
	Translate(std::shared_ptr<IHittable> obj, const Eigen::Vector3d &displacement);

	AABB boundingBox() const override;

	bool hit(const Ray &r, Interval interval, HitRecord &record) const override;

private:
	std::shared_ptr<IHittable> object;
	Eigen::Vector3d offset;
	AABB bbox;
};

class Rotation : public IHittable {
public:
	Rotation(std::shared_ptr<IHittable> obj, float psi, float theta, float phi, Point3 about_pt);

	AABB boundingBox() const override;

	bool hit(const Ray &r, Interval interval, HitRecord &record) const override;

private:
	std::shared_ptr<IHittable> object;
	Eigen::Matrix4d rotation_matrix, inverse_rotation_matrix;
	AABB bbox;
};

#endif // ONEWEEKEND_GRAPHICOBJECTS_H
