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


class Vec3 {
public:
	double x, y, z;

	Vec3(double x, double y, double z);

	Vec3(const Vec3 &other);

	Vec3();

	Vec3(Vec3 &&other) noexcept;

	[[nodiscard]] std::string makeColor() const;

	Vec3 &operator=(const Vec3 &other);

	double operator[](int i) const;

	double operator[](int i);

	Vec3 operator-() const;

	Vec3 operator-(const Vec3 &other) const;

	Vec3 operator+(const Vec3 &other) const;

	Vec3 operator*(double t) const;

	Vec3 operator/(double t) const;

	Vec3 &operator+=(const Vec3 &other);

	Vec3 &operator*=(double t);

	Vec3 &operator*(const Vec3 &other);

	Vec3 &operator/=(double t);

	[[nodiscard]] double lengthSq() const;

	[[nodiscard]] double length() const;

	[[nodiscard]] double dot(const Vec3 &other) const;

	[[nodiscard]] Vec3 cross(const Vec3 &other) const;

	[[nodiscard]] Vec3 unit() const;

	[[nodiscard]] operator std::string() const;

	static Vec3 random();

	static Vec3 random(double min, double max);

	static Vec3 randomVec3InUnitSphere();

	static Vec3 randomUnitVec3InHemiSphere(const Vec3 &normal);

	static Vec3 randomUnitVec3();

	static Vec3 reflect(const Vec3 &v, const Vec3 &n);

	static Vec3 refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat);

	static Vec3 randomVec3InUnitDisk();

	bool verySmall() const;


};

std::ostream &operator<<(std::ostream &out, const Vec3 &other);

inline Vec3 operator*(double t, const Vec3 &v) {
	return {t*v.x, t*v.y, t*v.z};
}

using Point3 = Vec3;

class Ray {
private:
	Point3 position;
	Vec3 direction;
	double tm;
public:
	Ray(Vec3 pos, Vec3 dir, double time);
	Ray(Vec3 pos, Vec3 dir);
	Ray() = default;

	Vec3 pos() const;
	Vec3 dir() const;
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


#endif //ONEWEEKEND_MATHUTIL_H
