#include <iostream>
#include "ImageUtil.h"
#include <vector>

int main() {
    int width = 256, height = 144;
    int r = 0, g = 0, b = 0;
    auto img = std::vector<std::vector<ImageUtil::Color>>();
    for (int i = 0; i < height; ++i) {
        auto v = std::vector<ImageUtil::Color>();
        for (int j = 0; j < width; ++j) {
            v.emplace_back(i, j, b);
            b++;
        }
        img.emplace_back(v);
    }
    ImageUtil::makePPM(width, height, img, "./out", "test.ppm");
    return 0;
}
