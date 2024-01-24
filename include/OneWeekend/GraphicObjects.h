/**
 * @file GraphicObjects.h
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#ifndef ONEWEEKEND_GRAPHICOBJECTS_H
#define ONEWEEKEND_GRAPHICOBJECTS_H

#include "MathUtil.h"
#include <utility>

class Camera {
public:
    Camera(int width, double aspect_ratio, double viewport_width, double focal_len, MathUtil::Point3 position);

    int getWidth() const;

    int getHeight() const;

    double getAspectRatio() const;

    double getViewportWidth() const;

    double getViewportHeight() const;

    double getFocalLen() const;

    const MathUtil::Point3 &getPosition() const;

    const MathUtil::Vec3 &getHoriVec() const;

    const MathUtil::Vec3 &getVertVec() const;

    const MathUtil::Vec3 &getPixDeltaX() const;

    const MathUtil::Vec3 &getPixDeltaY() const;

    const MathUtil::Point3 &getViewportUl() const;

    const MathUtil::Point3 &getPixel00() const;

    void setWidth(int width);

    void setAspectRatio(double aspect_ratio);

    void setFocalLen(double focal_len);

    void setPosition(const MathUtil::Point3 &position);

    [[nodiscard]] MathUtil::Vec3 getPixelVec(int x, int y) const;

    [[nodiscard]] MathUtil::Vec3 getPixRayDir(int x, int y) const;

private:

    void updateVectors();

    int width;
    int height;
    double aspect_ratio;
    double viewport_width;
    double viewport_height;
    double focal_len;
    MathUtil::Point3 position;
    MathUtil::Vec3 hori_vec;
    MathUtil::Vec3 vert_vec;
    MathUtil::Vec3 pix_delta_x;
    MathUtil::Vec3 pix_delta_y;
    MathUtil::Point3 viewport_ul;
    MathUtil::Point3 pixel_00;
};

#endif //ONEWEEKEND_GRAPHICOBJECTS_H
