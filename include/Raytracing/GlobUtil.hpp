/**
 * @file GlobUtil.hpp
 * @author ayano
 * @date 1/25/24
 * @brief
 */

#ifndef GLOBUTIL_HPP
#define GLOBUTIL_HPP

#include "Eigen/Dense"
#include <limits>
#include <sstream>
#include <string>

const float PI = 3.141592;
const float INF = std::numeric_limits<float>::infinity();
const float EPS = 1e-3;

inline float deg2Rad(float deg) { return deg * PI / 180.0; }

inline std::string vecToStr(const Eigen::Vector3d &v) {
    std::stringstream ss;
    ss << v.x() << " " << v.y() << " " << v.z();
    return ss.str();
}

inline std::string vecToStr(const Eigen::Vector4d &v) {
    std::stringstream ss;
    ss << v.x() << " " << v.y() << " " << v.z() << " " << v.w();
    return ss.str();
}

#endif // GLOBUTIL_HPP
