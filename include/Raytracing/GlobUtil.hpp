/**
 * @file GlobUtil.hpp
 * @author ayano
 * @date 1/25/24
 * @brief
 */

#ifndef GLOBUTIL_HPP
#define GLOBUTIL_HPP

#include <limits>

const float PI = 3.141592;
const float INF = std::numeric_limits<float>::infinity();
const float EPS = 1e-3;


inline float deg2Rad(float deg) {
    return deg * PI / 180.0;
}

#endif // GLOBUTIL_HPP
