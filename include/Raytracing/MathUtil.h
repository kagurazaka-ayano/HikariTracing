/**
 * @file MathUtil.h
 * @author ayano
 * @date 1/16/24
 * @brief
 */

#ifndef ONEWEEKEND_MATHUTIL_H
#define ONEWEEKEND_MATHUTIL_H

#include "Eigen/Core"
#include "Eigen/Dense"
#include "Eigen/Geometry"
#include "GlobUtil.hpp"
#include "spdlog/fmt/bundled/core.h"
#include <cmath>
#include <cstdlib>
#include <random>

using Point3 = Eigen::Vector3d;

class Ray {
  private:
    Point3 position;
    Eigen::Vector3d direction;
    double tm;

  public:
    Ray(Eigen::Vector3d pos, Eigen::Vector3d dir, double time);
    Ray(Eigen::Vector3d pos, Eigen::Vector3d dir);
    Ray() = default;

    Eigen::Vector3d pos() const;
    Eigen::Vector3d dir() const;
    double time() const;

    Point3 at(double t) const;
};

class Interval {
  public:
    double min, max;
    Interval();

    Interval(float min, float max);

    Interval(const Interval &first, const Interval &second);

    bool within(float x) const;

    bool surround(float x) const;

    float clamp(float x) const;

    Interval expand(float delta);

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

    AABB pad();

    [[nodiscard]] bool hit(const Ray &r, Interval ray_int) const;
};

class Perlin {
  public:
    Perlin() = default;

    ~Perlin() = default;

    float rawNoise(const Point3 &p) const;

    float octaveNoise(const Point3 &p, float frequency, int octave_count,
                      float presistence) const;

  private:
    static constexpr int perm[] = {
        151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233,
        7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,
        23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252,
        219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,
        174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,
        27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230,
        220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,
        63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169,
        200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186,
        3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126,
        255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189,
        28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,
        221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,
        108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228,
        251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
        145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157,
        184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236,
        205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,
        215, 61,  156, 180, 151, 160, 137, 91,  90,  15,  131, 13,  201, 95,
        96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,   99,
        37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,
        197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,
        237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165,
        71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122,
        60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102,
        143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187,
        208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100,
        109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,   202,
        38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
        58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,
        44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,
        39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104,
        218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179,
        162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,
        181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,
        150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141,
        128, 195, 78,  66,  215, 61,  156, 180};

    float fade(float x) const;

    float lerp(float begin, float end, float weight) const;

    float gradientDotProd(int hash, const Eigen::Vector3d &pt) const;
};

inline float randomFloat() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomFloat(double min, double max) {
    return min + (max - min) * randomFloat();
}

inline int randomInt(int min, int max) {
    return static_cast<int>(randomFloat(min, max + 1));
}

inline Eigen::Vector3d randomVec3() {
    return Eigen::Vector3d(randomFloat(), randomFloat(), randomFloat());
}

inline Eigen::Vector3d randomVec3(float min, float max) {
    return Eigen::Vector3d(randomFloat(min, max), randomFloat(min, max),
                           randomFloat(min, max));
}

inline Eigen::Vector3d randomVec3InUnitSphere() {
    while (true) {
        auto p = randomVec3(-1, 1);
        if (p.norm() >= 1)
            continue;
        return p;
    }
}

inline Eigen::Vector3d randomUnitVec3() {
    return randomVec3InUnitSphere().normalized();
}

inline Eigen::Vector3d randomVec3InUnitDisk() {
    while (true) {
        auto p = Eigen::Vector3d{randomFloat(-1, 1), randomFloat(-1, 1), 0};
        if (p.norm() >= 1)
            continue;
        return p;
    }
}

inline Eigen::Vector3d
randomUnitVec3InHemiSphere(const Eigen::Vector3d &normal) {
    auto in_unit_sphere = randomVec3InUnitSphere();
    if (in_unit_sphere.dot(normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline Eigen::Vector3d reflect(const Eigen::Vector3d &v,
                               const Eigen::Vector3d &n) {
    return v - 2 * v.dot(n) * n;
}

inline Eigen::Vector3d refract(const Eigen::Vector3d &uv,
                               const Eigen::Vector3d &n, float etai_over_etat) {
    auto cos_theta = std::fmin(-uv.dot(n), 1.0);
    Eigen::Vector3d r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Eigen::Vector3d r_out_parallel =
        -std::sqrt(std::fabs(1.0 - r_out_perp.norm())) * n;
    return r_out_perp + r_out_parallel;
}

inline bool verySmall(const Eigen::Vector3d &v) {
    return (std::abs(v[0]) < EPS) && (std::abs(v[1]) < EPS) &&
           (std::abs(v[2]) < EPS);
}

inline std::string makeColor(const Eigen::Vector3d &v) {
    auto r = static_cast<int>(255.999 * v[0]);
    auto g = static_cast<int>(255.999 * v[1]);
    auto b = static_cast<int>(255.999 * v[2]);
    return fmt::format("{} {} {}\n", r, g, b);
}

inline Eigen::Matrix4d makeEulerRotationMatrixAboutPt(const Point3 &pt,
                                                      double psi, double theta,
                                                      double phi) {
    Eigen::AngleAxisd yaw(psi, Eigen::Vector3d::UnitZ());
    Eigen::AngleAxisd pitch(theta, Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd roll(phi, Eigen::Vector3d::UnitX());

    Eigen::Quaterniond q = yaw * pitch * roll;
    auto translation = Eigen::Translation<double, 3>(pt.x(), pt.y(), pt.z());
    Eigen::Affine3d affine = Eigen::Affine3d::Identity() * translation *
                             q.toRotationMatrix() * translation.inverse();
    return affine.matrix();
}

inline Eigen::Vector3d deHomo(const Eigen::Vector4d &p) {
    return Eigen::Vector3d(p[0] / p[3], p[1] / p[3], p[2] / p[3]);
}

inline Eigen::Vector4d makeHomo(const Eigen::Vector3d &p) {
    return Eigen::Vector4d(p[0], p[1], p[2], 1);
}

#endif // ONEWEEKEND_MATHUTIL_H
