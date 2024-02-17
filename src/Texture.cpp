/**
 * @file ITexture.cpp
 * @author ayano
 * @date 2/15/24
 * @brief
 */

#include "Texture.h"

SolidColor::SolidColor(const Color& c) : color_val(c) {

}

SolidColor::SolidColor(double r, double g, double b) : color_val(Color{r, g, b}) {

}
Color SolidColor::value(double u, double v, const Point3 &p) const {
	return color_val;
}
Color CheckerTexture::value(double u, double v, const Point3 &p) const {
	auto u_int = static_cast<int>(u * 50 * inv_scale);
	auto v_int = static_cast<int>(v * 50 * inv_scale);

	bool is_even = (u_int + v_int) % 2 == 0;
	return is_even ? even->value(u, v, p) : odd->value(u, v, p);

}
CheckerTexture::CheckerTexture(double scale, std::shared_ptr<ITexture> even_tex, std::shared_ptr<ITexture> odd_tex) :
	inv_scale(1 / scale), even(std::move(even_tex)), odd(std::move(odd_tex))
{

}

CheckerTexture::CheckerTexture(double scale, const Color& even_color, const Color& odd_color) :
	inv_scale(1 / scale), even(std::make_shared<SolidColor>(even_color)), odd(std::make_shared<SolidColor>(odd_color))
{

}


Color ImageTexture::value(double u, double v, const Point3 &p) const {
	if (img.height() <= 0) return Color{0, 1, 1};

	u = Interval(0, 1).clamp(u);
	v = 1.0 - Interval(0, 1).clamp(v);

	auto i = static_cast<int>(u * img.width());
	auto j = static_cast<int>(v * img.height());
	auto pixel = img.pixelData(i, j);

	auto color_scale = 1.0 / 255.0;
	return Color{color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
}

ImageTexture::ImageTexture(const std::string &image, const std::string &parent) : img(Image(image, parent)) {

}

