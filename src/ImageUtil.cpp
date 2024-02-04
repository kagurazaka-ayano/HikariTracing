/**
 * @file ImageUtil.cpp
 * @author ayano
 * @date 1/15/24
 * @brief
*/

#include "ImageUtil.h"
#include "GlobUtil.hpp"

namespace ImageUtil{
    void makePPM(int width, int height, std::vector<std::vector<Color>> img, const std::string &path,
                            const std::string &name) {
        auto fout = std::ofstream();
        auto dir = path.ends_with("/") ? path : path + "/";
        struct stat st;
        if (stat(dir.c_str(), &st) != 0 && mkdirat(AT_FDCWD, dir.c_str(), 0755) == -1) {
            spdlog::critical("directory create failed at {}", path);
            exit(2);
        }
        fout.open((path.ends_with("/") ? path : path + "/") + name);
        fout << "P3\n" << width << ' ' << height << "\n255\n";
        int progress = 0;
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
//                spdlog::info("progression: {}%", (progress) * 100 / ((width) * (height)));
                fout << img[i][j].makeColor();
                progress++;
            }
        }
        fout.close();
    }
}



