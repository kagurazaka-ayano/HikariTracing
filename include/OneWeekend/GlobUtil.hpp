/**
 * @file GlobUtil.hpp
 * @author ayano
 * @date 1/25/24
 * @brief
 */

#ifndef GLOBUTIL_HPP
#define GLOBUTIL_HPP

#include <limits>

const double PI = 3.1415926;
const double INF = std::numeric_limits<double>::infinity();
const double EPS = 1e-6;


inline double deg2Rad(double deg) {
    return deg * PI / 180.0;
}

#endif // GLOBUTIL_HPP
