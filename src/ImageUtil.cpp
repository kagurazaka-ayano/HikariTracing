/**
 * @file ImageUtil.cpp
 * @author ayano
 * @date 1/15/24
 * @brief
*/

#include "ImageUtil.h"

void ImageUtil::makePPM(int width, int height, std::vector<std::vector<Color>> img, const std::string &path,
                        const std::string &name) {
    auto fout = std::ofstream();
    fout.open((path.ends_with("/") ? path : path + "/") + name);
    fout << "P3\n" << width << ' ' << height << "\n255\n";
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            spdlog::info("progression: {}%", (j + j * i) * 100 / ((width - 1) * (height - 1)));
            fout << img[i][j].makeColor();
        }
    }
    fout.close();
}

