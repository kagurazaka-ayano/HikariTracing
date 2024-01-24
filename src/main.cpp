#include "ImageUtil.h"
#include "MathUtil.h"
#include "GraphicObjects.h"
#include <vector>
#include <cmath>

ImageUtil::Color rayColor(const MathUtil::Ray& ray) {
    auto unit = ray.dir().unit();
//    spdlog::info(unit.y);
    auto progx = std::min(0.5 * unit.x + 1, 1.0);
    auto progy = std::min(0.5 * unit.y + 1, 1.0);
    return {progx, progy, 1};
}


int main() {
    auto camera = Camera(1920, 16.0 / 9.0, 2.0, 1, MathUtil::Point3(0, 0, 0));
    auto img = std::vector<std::vector<ImageUtil::Color>>();
    for (int i = 0; i < camera.getHeight(); ++i) {
        auto v = std::vector<ImageUtil::Color>();
//        if (i % 10 == 0 || i > camera.getHeight() - 10) spdlog::info("line remaining: {}", (camera.getHeight() - i + 1));
        for (int j = 0; j < camera.getWidth(); ++j) {
            auto ray_dir =  camera.getPixRayDir(j, i);
            auto ray = MathUtil::Ray(camera.getPosition(), ray_dir);
            auto color = rayColor(ray);
            v.emplace_back(color);
        }
        img.emplace_back(v);
    }
    ImageUtil::makePPM(camera.getWidth(), camera.getHeight(), img, "./out", "test.ppm");
    camera.setAspectRatio(16.0/8.0);
    img.clear();
    img.shrink_to_fit();
    for (int i = 0; i < camera.getHeight(); ++i) {
        auto v = std::vector<ImageUtil::Color>();
//        if (i % 10 == 0 || i > camera.getHeight() - 10) spdlog::info("line remaining: {}", (camera.getHeight() - i + 1));
        for (int j = 0; j < camera.getWidth(); ++j) {
            auto ray_dir =  camera.getPixRayDir(j, i);
            auto ray = MathUtil::Ray(camera.getPosition(), ray_dir);
            auto color = rayColor(ray);
            v.emplace_back(color);
        }
        img.emplace_back(v);
    }
    ImageUtil::makePPM(camera.getWidth(), camera.getHeight(), img, "./out", "test2.ppm");
    return 0;
}
