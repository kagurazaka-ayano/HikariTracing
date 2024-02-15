/**
 * @file MathUtil.h
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#ifndef ONEWEEKEND_MATHUTIL_H
#define ONEWEEKEND_MATHUTIL_H

#include <iostream>
#include <cmath>
#include <random>
#include "spdlog/fmt/fmt.h"
#include "GlobUtil.hpp"
#include "Math/Vector.hpp"


//class Vec3 {
//public:
//	double x, y, z;
//
//	Vec3(double x, double y, double z);
//
//	Vec3(const Vec3 &other);
//
//	Vec3();
//
//	Vec3(Vec3 &&other) noexcept;
//
//	[[nodiscard]] std::string makeColor() const;
//
//	Vec3 &operator=(const Vec3 &other);
//
//	double operator[](int i) const;
//
//	double operator[](int i);
//
//	Vec3 operator-() const;
//
//	Vec3 operator-(const Vec3 &other) const;
//
//	Vec3 operator+(const Vec3 &other) const;
//
//	Vec3 operator*(double t) const;
//
//	Vec3 operator/(double t) const;
//
//	Vec3 &operator+=(const Vec3 &other);
//
//	Vec3 &operator*=(double t);
//
//	Vec3 &operator*(const Vec3 &other);
//
//	Vec3 &operator/=(double t);
//
//	[[nodiscard]] double lengthSq() const;
//
//	[[nodiscard]] double length() const;
//
//	[[nodiscard]] double dot(const Vec3 &other) const;
//
//	[[nodiscard]] Vec3 cross(const Vec3 &other) const;
//
//	[[nodiscard]] Vec3 unit() const;
//
//	[[nodiscard]] operator std::string() const;
//
//	static Vec3 random();
//
//	static Vec3 random(double min, double max);
//
//	static Vec3 randomVec3InUnitSphere();
//
//	static Vec3 randomUnitVec3InHemiSphere(const Vec3 &normal);
//
//	static Vec3 randomUnitVec3();
//
//	static Vec3 reflect(const Vec3 &v, const Vec3 &n);
//
//	static Vec3 refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat);
//
//	static Vec3 randomVec3InUnitDisk();
//
//	bool verySmall() const;
//
//
//};
//
//std::ostream &operator<<(std::ostream &out, const Vec3 &other);
//
//inline Vec3 operator*(double t, const Vec3 &v) {
//	return {t*v.x, t*v.y, t*v.z};
//}

using Point3 = AppleMath::Vector3;

class Ray {
private:
	Point3 position;
	AppleMath::Vector3 direction;
	double tm;
public:
	Ray(AppleMath::Vector3 pos, AppleMath::Vector3 dir, double time);
	Ray(AppleMath::Vector3 pos, AppleMath::Vector3 dir);
	Ray() = default;

	AppleMath::Vector3 pos() const;
	AppleMath::Vector3 dir() const;
	double time() const;

	Point3 at(double t) const;
};

class Interval {
public:
	double min, max;
	Interval();

	Interval(double min, double max);

	Interval(const Interval& first, const Interval& second);

	bool within(double x) const;

	bool surround(double x) const;

	static const Interval empty, universe;

};
const static Interval empty(+INF, -INF);
const static Interval universe(-INF, +INF);

class AABB {
public:
	Interval x, y, z;

	AABB() = default;

	AABB(const Point3 &a, const Point3 &b);

	AABB(const Interval& x, const Interval& y, const Interval& z);

	AABB(const AABB& up, const AABB& down);

	Interval axis(int i) const;

	[[nodiscard]] bool hit(const Ray &r, Interval ray_int) const;

};

inline double randomDouble() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double randomDouble(double min, double max) {
	return min + (max - min) * randomDouble();
}

inline int randomInt(int min, int max) {
	return static_cast<int>(randomDouble(min, max + 1));
}

inline AppleMath::Vector3 randomVec3() {
	return AppleMath::Vector3{randomDouble(), randomDouble(), randomDouble()};
}

inline AppleMath::Vector3 randomVec3(double min, double max) {
	return AppleMath::Vector3{randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)};
}

inline AppleMath::Vector3 randomVec3InUnitSphere() {
	while(true) {
		auto p = randomVec3(-1, 1);
		if(p.lengthSq() >= 1) continue;
		return p;
	}
}

inline AppleMath::Vector3 randomUnitVec3() {
	return randomVec3InUnitSphere().normalized();
}

inline AppleMath::Vector3 randomVec3InUnitDisk() {
	while(true) {
		auto p = AppleMath::Vector3{randomDouble(-1, 1), randomDouble(-1, 1), 0};
		if(p.lengthSq() >= 1) continue;
		return p;
	}
}

inline AppleMath::Vector3 randomUnitVec3InHemiSphere(const AppleMath::Vector3 &normal) {
	auto in_unit_sphere = randomVec3InUnitSphere();
	if(in_unit_sphere.dot(normal) > 0.0) return in_unit_sphere;
	else return -in_unit_sphere;
}

inline AppleMath::Vector3 reflect(const AppleMath::Vector3 &v, const AppleMath::Vector3 &n) {
	return v - 2 * v.dot(n) * n;
}

inline AppleMath::Vector3 refract(const AppleMath::Vector3 &uv, const AppleMath::Vector3 &n, double etai_over_etat) {
	auto cos_theta = std::fmin(-uv.dot(n), 1.0);
	AppleMath::Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	AppleMath::Vector3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.lengthSq())) * n;
	return r_out_perp + r_out_parallel;
}

inline bool verySmall(const AppleMath::Vector3 &v) {
	return (std::abs(v[0]) < EPS) && (std::abs(v[1]) < EPS) && (std::abs(v[2]) < EPS);
}

inline std::string makeColor(const AppleMath::Vector3 &v) {
	auto r = static_cast<int>(255.999 * v[0]);
	auto g = static_cast<int>(255.999 * v[1]);
	auto b = static_cast<int>(255.999 * v[2]);
	return fmt::format("{} {} {}\n", r, g, b);
}

#endif //ONEWEEKEND_MATHUTIL_H
