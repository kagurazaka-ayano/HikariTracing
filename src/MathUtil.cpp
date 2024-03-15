/**
 * @file MathUtil.cpp
 * @author ayano
 * @date 1/16/24
 * @brief
 */

#include "MathUtil.h"
#include <cmath>
#include <simd/quaternion.h>
#include "AppleMath/Vector.hpp"
#include <iostream>


std::ostream &operator<<(std::ostream &out, const AppleMath::Vector3 &other) {
	out << "Vec3: " << other[0] << " " << other[1] << " " << other[2];
	return out;
}

Ray::Ray(AppleMath::Vector3 pos, AppleMath::Vector3 dir) : position(std::move(pos)), direction(std::move(dir)), tm(0) {}

Ray::Ray(AppleMath::Vector3 pos, AppleMath::Vector3 dir, float time) :
	position(std::move(pos)), direction(std::move(dir)), tm(time) {}

Point3 Ray::at(float t) const { return position + direction * t; }

AppleMath::Vector3 Ray::dir() const { return direction; }

float Ray::time() const { return tm; }

AppleMath::Vector3 Ray::pos() const { return position; }
Interval::Interval() : min(-INF), max(INF) {}
Interval::Interval(float min, float max) : min(min), max(max) {}
bool Interval::within(float x) const { return (min <= x) && (x <= max); }
bool Interval::surround(float x) const { return (min < x) && (x < max); }
Interval::Interval(const Interval &first, const Interval &second) {
	min = fmin(first.min, second.min);
	max = fmax(first.max, second.max);
}

float Interval::clamp(float x) const {
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

Interval Interval::expand(float delta) {
	auto padding = delta / 2;
	return Interval(min - padding, max + padding);
}

AABB::AABB(const Point3 &a, const Point3 &b) {
	x = Interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
	y = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
	z = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
}

AABB::AABB(const Interval &x, const Interval &y, const Interval &z) : x(x), y(y), z(z) {}

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

		if (t0 > ray_int.min) ray_int.min = t0;
		if (t1 < ray_int.max) ray_int.max = t1;

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

AABB AABB::pad() {
	float delta = 0.0001;
	float size_x = x.max - x.min;
	float size_y = y.max - y.min;
	float size_z = z.max - z.min;
	Interval new_x = (fabs(size_x) >= delta ? x : x.expand(delta));
	Interval new_y = (fabs(size_y) >= delta ? y : y.expand(delta));
	Interval new_z = (fabs(size_z) >= delta ? z : z.expand(delta));
	return AABB(new_x, new_y, new_z);
}

float Perlin::gradientDotProd(int hash, const AppleMath::Vector3 &pt) const {
	auto x = pt[0];
	auto y = pt[1];
	auto z = pt[2];
	switch (hash & 0xF) {
		case 0x0:
			return x + y;
		case 0x1:
			return -x + y;
		case 0x2:
			return x - y;
		case 0x3:
			return -x - y;
		case 0x4:
			return x + z;
		case 0x5:
			return -x + z;
		case 0x6:
			return x - z;
		case 0x7:
			return -x - z;
		case 0x8:
			return y + z;
		case 0x9:
			return -y + z;
		case 0xA:
			return y - z;
		case 0xB:
			return -y - z;
		case 0xC:
			return y + x;
		case 0xD:
			return -y + z;
		case 0xE:
			return y - x;
		case 0xF:
			return -y - z;
		default:
			return 0; // never happens
	}
}

float Perlin::fade(float t) const { return t * t * t * (t * (t * 6 - 15) + 10); }

float Perlin::lerp(float begin, float end, float weight) const { return begin + weight * (end - begin); }

float Perlin::rawNoise(const Point3 &p) const {
	float x = p[0], y = p[1], z = p[2];
	int xi = static_cast<int>(floor(x)) & 255;
	int yi = static_cast<int>(floor(y)) & 255;
	int zi = static_cast<int>(floor(z)) & 255;
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	int llb, lrb, ulb, urb, llf, lrf, ulf, urf;
	llb = perm[perm[perm[xi		] + yi		] + zi		];
	lrb = perm[perm[perm[xi + 1	] + yi		] + zi		];
	ulb = perm[perm[perm[xi		] + yi + 1	] + zi		]; 
	urb = perm[perm[perm[xi + 1	] + yi + 1	] + zi		];
	llf = perm[perm[perm[xi		] + yi		] + zi + 1	];
	lrf = perm[perm[perm[xi + 1	] + yi		] + zi + 1	];
	ulf = perm[perm[perm[xi		] + yi + 1	] + zi + 1	]; 
	urf = perm[perm[perm[xi + 1	] + yi + 1	] + zi + 1	];
	float dotllb = gradientDotProd(llb, {x		, y		, z		});
	float dotlrb = gradientDotProd(lrb, {x - 1	, y		, z		});
	float dotulb = gradientDotProd(ulb, {x		, y - 1	, z		});
	float doturb = gradientDotProd(urb, {x - 1	, y - 1	, z		});
	float dotllf = gradientDotProd(llf, {x		, y		, z - 1	});
	float dotlrf = gradientDotProd(lrf, {x - 1	, y		, z - 1	});
	float dotulf = gradientDotProd(ulf, {x		, y - 1	, z - 1	});
	float doturf = gradientDotProd(urf, {x - 1	, y - 1	, z - 1	});

	x = fade(x);
	y = fade(y);
	z = fade(z);


	float x0 = lerp(dotllb, dotlrb, x);
	float x1 = lerp(dotulb, doturb, x);
	float x2 = lerp(dotllf, dotlrf, x);
	float x3 = lerp(dotulf, doturf, x);

	float y0 = lerp(x0, x1, y);
	float y1 = lerp(x2, x3, y);

	float out = lerp(y0, y1, z);


	return out;
}

float Perlin::octaveNoise(const Point3& p, float frequency, int octave_count, float persistence) const {
	float sum = 0;
	float max_value = 0;
	float amplitude = 1;
	for (int i = 0; i < octave_count; ++i) {
		sum += rawNoise(p * frequency) * amplitude;
		max_value += amplitude;
		amplitude *= persistence;
		frequency *= 2;
	}
	return sum / max_value;
}
