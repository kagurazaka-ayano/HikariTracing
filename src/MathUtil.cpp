/**
 * @file MathUtil.cpp
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#include "MathUtil.h"


//Vec3::Vec3(double x, double y, double z): x(x), y(y), z(z) {
//	
//}
//
//Vec3::Vec3(const Vec3 &other) {
//	x = other.x;
//	y = other.y;
//	z = other.z;
//}
//
//Vec3::Vec3(Vec3 &&other) noexcept {
//	x = other.x;
//	y = other.y;
//	z = other.z;
//}
//
//Vec3 &Vec3::operator=(const Vec3 &other) {
//	if(this != &other) {
//		x = other.x;
//		y = other.y;
//		z = other.z;
//	}
//	return *this;
//}
//
//double Vec3::operator[](int i) const {
//	double arr[] = {x, y, z};
//	return arr[i];
//}
//
//double Vec3::operator[](int i) {
//	double arr[] = {x, y, z};
//	return arr[i];
//}
//
//Vec3 Vec3::operator-(const Vec3 &other) const {
//	return {x - other.x, y - other.y, z - other.z};
//}
//
//Vec3 &Vec3::operator+=(const Vec3 &other) {
//	x += other.x;
//	y += other.y;
//	z += other.z;
//	return *this;
//}
//
//Vec3 &Vec3::operator*=(double t) {
//	x *= t;
//	y *= t;
//	z *= t;
//	return *this;
//}
//
//Vec3 &Vec3::operator*(const Vec3 &other) {
//	x *= other.x;
//	y *= other.y;
//	z *= other.z;
//	return *this;
//}
//
//Vec3 &Vec3::operator/=(double t) {
//	x /= t;
//	y /= t;
//	z /= t;
//	return *this;
//}
//
//double Vec3::lengthSq() const {
//	return x * x + y * y + z * z;
//}
//
//double Vec3::length() const {
//	return std::sqrt(lengthSq());
//}
//
//Vec3 Vec3::operator-() const {
//	return {-x, -y, -z};
//}
//
//Vec3 Vec3::operator+(const Vec3 &other) const {
//	return {x + other.x, y + other.y, z + other.z};
//}
//
//double Vec3::dot(const Vec3 &other) const {
//	return x * other.x + y * other.y + z * other.z;
//}
//
//Vec3 Vec3::cross(const Vec3 &other) const {
//	return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
//}
//
//Vec3 Vec3::unit() const {
//	auto v = *this / this->length();
//	return v;
//}
//
//Vec3 Vec3::operator/(double t) const {
//	return {x / t, y / t, z / t};
//}
//
//Vec3 Vec3::operator*(double t) const {
//	return {x * t, y * t, z * t};
//}
//
//std::string Vec3::makeColor() const {
//	return fmt::format("{} {} {}\n", static_cast<int>(x * 255.999), static_cast<int>(y * 255.999), static_cast<int>(z * 255.999));
//}
//
//Vec3::operator std::string() const {
//	return fmt::format("Vec3: {:.6f} {:.6f} {:.6f}", x, y, z);
//}
//
//Vec3::Vec3(): Vec3(0, 0, 0) {
//
//}
//Vec3 Vec3::random() {
//	return {randomDouble(), randomDouble(), randomDouble()};
//}
//
//Vec3 Vec3::random(double min, double max) {
//	return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
//}
//
//Vec3 Vec3::randomVec3InUnitSphere() {
//	while (true) {
//		auto p = Vec3::random(-1, 1);
//		if (p.lengthSq() >= 1) continue;
//		return p;
//	}
//}
//
//Vec3 Vec3::randomUnitVec3() {
//	return randomVec3InUnitSphere().unit();
//}
//
//Vec3 Vec3::randomUnitVec3InHemiSphere(const Vec3 &normal) {
//	Vec3 in_unit_sphere = randomVec3InUnitSphere();
//	if (in_unit_sphere.dot(normal) > 0.0) {
//		return in_unit_sphere;
//	}
//	else {
//		return -in_unit_sphere;
//	}
//}
//bool Vec3::verySmall() const {
//	return (std::abs(x) < EPS) && (std::abs(y) < EPS) && (std::abs(z) < EPS);
//}
//
//Vec3 Vec3::reflect(const Vec3 &v, const Vec3 &n) {
//	return v - 2 * v.dot(n) * n;
//}
//Vec3 Vec3::refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat) {
//	auto cos_theta = fmin((-uv).dot(n), 1.0);
//	Vec3 refr_x = etai_over_etat * (uv + cos_theta * n);
//	Vec3 refr_y = -sqrt(fabs(1.0 - refr_x.lengthSq())) * n;
//	return refr_x + refr_y;
//}
//Vec3 Vec3::randomVec3InUnitDisk() {
//	while (true) {
//		auto v = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
//		if (v.lengthSq() >= 1)
//			continue;
//		return v;
//	}
//}

std::ostream& operator<<(std::ostream &out, const Math::Vector3 &other) {
	out << "Vec3: " << other[0] << " " << other[1] << " " << other[2];
	return out;
}

Ray::Ray(Math::Vector3 pos, Math::Vector3 dir): position(std::move(pos)), direction(std::move(dir)), tm(0)
{
	
}

Ray::Ray(Math::Vector3 pos, Math::Vector3 dir, double time) : position(std::move(pos)), direction(std::move(dir)), tm(time) {

}

Point3 Ray::at(double t) const {
	return position + direction * t;
}

Math::Vector3 Ray::dir() const {
	return direction;
}

double Ray::time() const
{
	return tm;
}

Math::Vector3 Ray::pos() const {
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
