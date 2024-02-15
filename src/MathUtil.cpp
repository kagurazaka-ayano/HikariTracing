/**
 * @file MathUtil.cpp
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#include "MathUtil.h"


std::ostream& operator<<(std::ostream &out, const AppleMath::Vector3 &other) {
	out << "Vec3: " << other[0] << " " << other[1] << " " << other[2];
	return out;
}

Ray::Ray(AppleMath::Vector3 pos, AppleMath::Vector3 dir): position(std::move(pos)), direction(std::move(dir)), tm(0)
{
	
}

Ray::Ray(AppleMath::Vector3 pos, AppleMath::Vector3 dir, double time) : position(std::move(pos)), direction(std::move(dir)), tm(time) {

}

Point3 Ray::at(double t) const {
	return position + direction * t;
}

AppleMath::Vector3 Ray::dir() const {
	return direction;
}

double Ray::time() const
{
	return tm;
}

AppleMath::Vector3 Ray::pos() const {
	return position;
}
Interval::Interval() : min(-INF), max(INF) {

}
Interval::Interval(double min, double max): min(min), max(max) {

}
bool Interval::within(double x) const {
	return (min <= x) && (x <= max);
}
bool Interval::surround(double x) const {
	return (min < x) && (x < max);
}
Interval::Interval(const Interval &first, const Interval &second) {
	min = fmin(first.min, second.min);
	max = fmax(first.max, second.max);
}

AABB::AABB(const Point3 &a, const Point3 &b) {
	x = Interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
	y = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
	z = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
}

AABB::AABB(const Interval &x, const Interval &y, const Interval &z) : x(x), y(y), z(z) {

}

Interval AABB::axis(int i) const {
	switch (i) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::out_of_range("AABB::axis");
	}
}

bool AABB::hit(const Ray &r, Interval ray_int) const {
	for (int i = 0; i < 3; i++) {
		auto invD = 1.0 / r.dir()[i];
		auto orig = r.pos()[i];

		auto t0 = (axis(i).min - orig) * invD;
		auto t1 = (axis(i).max - orig) * invD;

		if (invD < 0.0) {
			std::swap(t0, t1);
		}

		if (ray_int.max < ray_int.min) {
			return false;
		}
	}
	return true;
}
AABB::AABB(const AABB &up, const AABB &down) {
	x = Interval(up.x, down.x);
	y = Interval(up.y, down.y);
	z = Interval(up.z, down.z);
}
