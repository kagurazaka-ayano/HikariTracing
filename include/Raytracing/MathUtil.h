/**
 * @file MathUtil.h
 * @author ayano
 * @date 1/16/24
 * @brief
 */

#ifndef ONEWEEKEND_MATHUTIL_H
#define ONEWEEKEND_MATHUTIL_H

#include <cmath>
#include <random>
#include "AppleMath/Vector.hpp"
#include "GlobUtil.hpp"
#include "spdlog/fmt/fmt.h"


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

	Interval(const Interval &first, const Interval &second);

	bool within(double x) const;

	bool surround(double x) const;

	double clamp(double x) const;

	static const Interval empty, universe;
};
const static Interval empty(+INF, -INF);
const static Interval universe(-INF, +INF);

class AABB {
public:
	Interval x, y, z;

	AABB() = default;

	AABB(const Point3 &a, const Point3 &b);

	AABB(const Interval &x, const Interval &y, const Interval &z);

	AABB(const AABB &up, const AABB &down);

	Interval axis(int i) const;

	[[nodiscard]] bool hit(const Ray &r, Interval ray_int) const;
};


class Perlin {
public:
	Perlin() = default;

	~Perlin() = default;

	double rawNoise(const Point3 &p) const;

	double octaveNoise(const Point3& p, double frequency, int octave_count, double presistence) const;

private:
	static constexpr int perm[] = {
			151, 160, 137, 91,	90,	 15,  131, 13,	201, 95,  96,  53,	194, 233, 7,   225, 140, 36,  103, 30,	69,
			142, 8,	  99,  37,	240, 21,  10,  23,	190, 6,	  148, 247, 120, 234, 75,  0,	26,	 197, 62,  94,	252,
			219, 203, 117, 35,	11,	 32,  57,  177, 33,	 88,  237, 149, 56,	 87,  174, 20,	125, 136, 171, 168, 68,
			175, 74,  165, 71,	134, 139, 48,  27,	166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230,
			220, 105, 92,  41,	55,	 46,  245, 40,	244, 102, 143, 54,	65,	 25,  63,  161, 1,	 216, 80,  73,	209,
			76,	 132, 187, 208, 89,	 18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186,
			3,	 64,  52,  217, 226, 250, 124, 123, 5,	 202, 38,  147, 118, 126, 255, 82,	85,	 212, 207, 206, 59,
			227, 47,  16,  58,	17,	 182, 189, 28,	42,	 223, 183, 170, 213, 119, 248, 152, 2,	 44,  154, 163, 70,
			221, 153, 101, 155, 167, 43,  172, 9,	129, 22,  39,  253, 19,	 98,  108, 110, 79,	 113, 224, 232, 178,
			185, 112, 104, 218, 246, 97,  228, 251, 34,	 242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,	51,
			145, 235, 249, 14,	239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,	204, 176, 115, 121, 50,
			45,	 127, 4,   150, 254, 138, 236, 205, 93,	 222, 114, 67,	29,	 24,  72,  243, 141, 128, 195, 78,	66,
			215, 61,  156, 180, 151, 160, 137, 91,	90,	 15,  131, 13,	201, 95,  96,  53,	194, 233, 7,   225, 140,
			36,	 103, 30,  69,	142, 8,	  99,  37,	240, 21,  10,  23,	190, 6,	  148, 247, 120, 234, 75,  0,	26,
			197, 62,  94,  252, 219, 203, 117, 35,	11,	 32,  57,  177, 33,	 88,  237, 149, 56,	 87,  174, 20,	125,
			136, 171, 168, 68,	175, 74,  165, 71,	134, 139, 48,  27,	166, 77,  146, 158, 231, 83,  111, 229, 122,
			60,	 211, 133, 230, 220, 105, 92,  41,	55,	 46,  245, 40,	244, 102, 143, 54,	65,	 25,  63,  161, 1,
			216, 80,  73,  209, 76,	 132, 187, 208, 89,	 18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100,
			109, 198, 173, 186, 3,	 64,  52,  217, 226, 250, 124, 123, 5,	 202, 38,  147, 118, 126, 255, 82,	85,
			212, 207, 206, 59,	227, 47,  16,  58,	17,	 182, 189, 28,	42,	 223, 183, 170, 213, 119, 248, 152, 2,
			44,	 154, 163, 70,	221, 153, 101, 155, 167, 43,  172, 9,	129, 22,  39,  253, 19,	 98,  108, 110, 79,
			113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,	 242, 193, 238, 210, 144, 12,  191, 179,
			162, 241, 81,  51,	145, 235, 249, 14,	239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,	204,
			176, 115, 121, 50,	45,	 127, 4,   150, 254, 138, 236, 205, 93,	 222, 114, 67,	29,	 24,  72,  243, 141,
			128, 195, 78,  66,	215, 61,  156, 180
	};

	double fade(double x) const;

	double lerp(double begin, double end, double weight) const;

	double gradientDotProd(int hash, const AppleMath::Vector3 &pt) const;
};


inline double randomDouble() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double randomDouble(double min, double max) { return min + (max - min) * randomDouble(); }

inline int randomInt(int min, int max) { return static_cast<int>(randomDouble(min, max + 1)); }

inline AppleMath::Vector3 randomVec3() { return AppleMath::Vector3{randomDouble(), randomDouble(), randomDouble()}; }

inline AppleMath::Vector3 randomVec3(double min, double max) {
	return AppleMath::Vector3{randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)};
}

inline AppleMath::Vector3 randomVec3InUnitSphere() {
	while (true) {
		auto p = randomVec3(-1, 1);
		if (p.lengthSq() >= 1)
			continue;
		return p;
	}
}

inline AppleMath::Vector3 randomUnitVec3() { return randomVec3InUnitSphere().normalized(); }

inline AppleMath::Vector3 randomVec3InUnitDisk() {
	while (true) {
		auto p = AppleMath::Vector3{randomDouble(-1, 1), randomDouble(-1, 1), 0};
		if (p.lengthSq() >= 1)
			continue;
		return p;
	}
}

inline AppleMath::Vector3 randomUnitVec3InHemiSphere(const AppleMath::Vector3 &normal) {
	auto in_unit_sphere = randomVec3InUnitSphere();
	if (in_unit_sphere.dot(normal) > 0.0)
		return in_unit_sphere;
	else
		return -in_unit_sphere;
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


#endif // ONEWEEKEND_MATHUTIL_H
