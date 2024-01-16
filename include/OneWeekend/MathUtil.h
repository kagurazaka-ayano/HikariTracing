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

class Vec3 {
public:
    double x, y, z;
    Vec3(double x, double y, double z);
    Vec3(const Vec3& other);
    Vec3(Vec3&& other) noexcept;
    double x() const noexcept;
    double y() const noexcept;
    double z() const noexcept;

    Vec3& operator=(const Vec3& other);
    double operator[]() const;
    double operator[]();

    Vec3 operator-() const;
    Vec3& operator+=(const Vec3& other);
    Vec3& operator*=(double t);
    Vec3& operator/=(double t);
    double lengthSq() const;

    double length() const;



};

#endif //ONEWEEKEND_MATHUTIL_H
