/**
 * @file ImageUtil.h
 * @author ayano
 * @date 1/15/24
 * @brief This is image utils
*/

#ifndef ONEWEEKEND_IMAGEUTIL_H
#define ONEWEEKEND_IMAGEUTIL_H

#include <fstream>
#include <vector>
#include <tuple>
#include <sstream>
#include "spdlog/spdlog.h"

namespace ImageUtil {

    struct Color {
        int r, g, b;
        Color(int r, int g, int b);
        [[nodiscard]] std::string getColor() const noexcept;
    };

    void makePPM(int width, int height, std::vector<std::vector<Color>> img, const std::string &path,
                 const std::string &name);

}

#endif //ONEWEEKEND_IMAGEUTIL_H
