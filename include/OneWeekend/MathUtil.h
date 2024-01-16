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
#include "spdlog/fmt/fmt.h"

namespace MathUtil {

    class Vec3 {
    public:
        double x, y, z;

        Vec3(double x, double y, double z);

        Vec3(const Vec3 &other);

        Vec3(Vec3 &&other) noexcept;

        [[nodiscard]] std::string makeColor() const;

        Vec3 &operator=(const Vec3 &other);

        double operator[](int i) const;

        double operator[](int i);

        Vec3 operator-() const;

        Vec3 operator-(const Vec3 &other);

        Vec3 operator+(const Vec3 &other) const;

        Vec3 operator*(double t) const;

        Vec3 operator/(double t) const;

        Vec3 &operator+=(const Vec3 &other);

        Vec3 &operator*=(double t);

        Vec3 &operator/=(double t);

        [[nodiscard]] double lengthSq() const;

        [[nodiscard]] double length() const;

        double dot(const Vec3 &other) const;

        Vec3 cross(const Vec3 &other) const;

        Vec3 unit() const;
    };

    std::ostream &operator<<(std::ostream &out, const Vec3 &other);

    using Point3 = Vec3;

    class Ray {
    private:
        Point3 position;
        Vec3 direction;
    public:
        Ray(const Vec3 &pos, const Vec3 &dir);

        Vec3 pos() const;
        Vec3 dir() const;

        Point3 at(double t) const;
    };
}

#endif //ONEWEEKEND_MATHUTIL_H
