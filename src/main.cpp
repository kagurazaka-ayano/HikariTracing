#include "ImageUtil.h"
#include "MathUtil.h"
#include <vector>

ImageUtil::Color rayColor(const MathUtil::Ray& ray) {
    auto unit = ray.dir().unit();
    auto prog = (0.5 * unit.y + 1);
    return (1 - prog) * ImageUtil::Color(1, 1, 1) + prog * ImageUtil::Color(0.5, 0.7, 1);
}


int main() {

    // image dimension
    int width = 1920;
    auto aspect = 16.0 / 9.0;
    int height = static_cast<int>(width / aspect);
    height = height < 1 ? 1 : height;

    // camera
    auto focal_len = 1.0;

    auto viewport_width = 2.0;
    auto viewport_height = viewport_width / (static_cast<double>(width) / height);
    auto camera_center = MathUtil::Vec3(0, 0, 0);

    auto viewport_vec_h = MathUtil::Vec3(viewport_width, 0, 0);
    auto viewport_vec_v = MathUtil::Vec3(0, -viewport_height, 0);

    auto pix_delta_h = viewport_vec_h / width;
    auto pix_delta_v = viewport_vec_v / height;

    auto viewport_ul = camera_center - MathUtil::Vec3(0, 0, focal_len) - viewport_vec_h / 2 - viewport_vec_v / 2;


    auto pixel_00 = viewport_ul + (pix_delta_h + pix_delta_v) * 0.5;

    auto img = std::vector<std::vector<ImageUtil::Color>>();
    for (int i = 0; i < height; ++i) {
        auto v = std::vector<ImageUtil::Color>();
        spdlog::info("line remaining: {}", (height - i + 1));
        for (int j = 0; j < width; ++j) {
            auto pix = pixel_00 + pix_delta_h * j + pix_delta_v * i;
            auto ray_dir = pix - camera_center;
            auto ray = MathUtil::Ray(camera_center, ray_dir);
            auto color = rayColor(ray);
            v.emplace_back(color);
        }
        img.emplace_back(v);
    }
    ImageUtil::makePPM(width, height, img, "./out", "test.ppm");
    return 0;
}
