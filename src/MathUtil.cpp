/**
 * @file MathUtil.cpp
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#include "MathUtil.h"

Vec3::Vec3(double x, double y, double z): x(x), y(y), z(z) {

}

Vec3::Vec3(const Vec3 &other) {
    x = other.x;
    y = other.y;
    z = other.z;
}

Vec3::Vec3(Vec3 &&other) noexcept {
    x = other.x;
    y = other.y;
    z = other.z;
}

Vec3 &Vec3::operator=(const Vec3 &other) {
    if(this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

double Vec3::operator[](int i) const {
    double arr[] = {x, y, z};
    return arr[i];
}

double Vec3::operator[](int i) {
    double arr[] = {x, y, z};
    return arr[i];
}

Vec3 Vec3::operator-(const Vec3 &other) {
    return {x - other.x, y - other.y, z - other.z};
}

Vec3 &Vec3::operator+=(const Vec3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3 &Vec3::operator*=(double t) {
    x *= t;
    y *= t;
    z *= t;
    return *this;
}

Vec3 &Vec3::operator/=(double t) {
    x /= t;
    y /= t;
    z /= t;
    return *this;
}

double Vec3::lengthSq() const {
    return x * x + y * y + z * z;
}

double Vec3::length() const {
    return sqrt(lengthSq());
}

Vec3 Vec3::operator-() const {
    return {-x, -y, -z};
}

Vec3 Vec3::operator+(const Vec3 &other) const {
    return {x + other.x, y + other.y, z + other.z};
}

double Vec3::dot(const Vec3 &other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const {
    return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
}

Vec3 Vec3::unit() const {
    auto v = *this / this->length();
    return v;
}

Vec3 Vec3::operator/(double t) const {
    return {x / t, y / t, z / t};
}

Vec3 Vec3::operator*(double t) const {
    return {x * t, y * t, z * t};
}

std::string Vec3::makeColor() const {
    return fmt::format("{} {} {}\n", static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
}

std::ostream& operator<<(std::ostream &out, const Vec3 &other) {
    out << other.x << " " << other.y << " " << other.z;
    return out;
}




