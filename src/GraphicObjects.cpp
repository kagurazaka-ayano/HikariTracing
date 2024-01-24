/**
 * @file GraphicObjects.cpp
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#include "GraphicObjects.h"


Camera::Camera(int width, double aspect_ratio, double viewport_width, double focal_len,
               MathUtil::Point3 position) : width(width), aspect_ratio(aspect_ratio),
                                                   viewport_width(viewport_width), focal_len(focal_len),
                                                   position(std::move(position)),
                                                   height(static_cast<int>(width / aspect_ratio)),viewport_height(viewport_width / (static_cast<double>(width) / static_cast<double>(height))) {
    updateVectors();

}

void Camera::updateVectors() {
    hori_vec = MathUtil::Vec3(viewport_width, 0, 0);
    vert_vec = MathUtil::Vec3(0, -viewport_height, 0);
    pix_delta_x = hori_vec / width;
    pix_delta_y = vert_vec / height;
    viewport_ul = position - MathUtil::Vec3(0, 0, focal_len) - (vert_vec + hori_vec) / 2;
    pixel_00 = viewport_ul + (pix_delta_y + pix_delta_x) * 0.5;
}

int Camera::getWidth() const {
    return width;
}

int Camera::getHeight() const {
    return height;
}

double Camera::getAspectRatio() const {
    return aspect_ratio;
}

double Camera::getViewportWidth() const {
    return viewport_width;
}

double Camera::getViewportHeight() const {
    return viewport_height;
}

double Camera::getFocalLen() const {
    return focal_len;
}

const MathUtil::Point3 &Camera::getPosition() const {
    return position;
}

const MathUtil::Vec3 &Camera::getHoriVec() const {
    return hori_vec;
}

const MathUtil::Vec3 &Camera::getVertVec() const {
    return vert_vec;
}

const MathUtil::Vec3 &Camera::getPixDeltaX() const {
    return pix_delta_x;
}

const MathUtil::Vec3 &Camera::getPixDeltaY() const {
    return pix_delta_y;
}

const MathUtil::Point3 &Camera::getViewportUl() const {
    return viewport_ul;
}

const MathUtil::Point3 &Camera::getPixel00() const {
    return pixel_00;
}

void Camera::setWidth(int width) {
    Camera::width = width;
    height = static_cast<int>(width / aspect_ratio);
    updateVectors();
}

void Camera::setAspectRatio(double aspect_ratio) {
    Camera::aspect_ratio = aspect_ratio;
    height = static_cast<int>(width / aspect_ratio);
    viewport_height = viewport_width / aspect_ratio;
    updateVectors();
}

void Camera::setFocalLen(double focal_len) {
    Camera::focal_len = focal_len;
    updateVectors();
}

void Camera::setPosition(const MathUtil::Point3 &position) {
    Camera::position = position;
    updateVectors();
}

MathUtil::Vec3 Camera::getPixelVec(int x, int y) const {
    return pixel_00 + pix_delta_x * x + pix_delta_y * y;
}

MathUtil::Vec3 Camera::getPixRayDir(int x, int y) const {
    return getPixelVec(x, y) - position;
}


