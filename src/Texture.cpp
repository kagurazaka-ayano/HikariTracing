/**
 * @file ITexture.cpp
 * @author ayano
 * @date 2/15/24
 * @brief
 */

#include "Texture.h"
#include "MathUtil.h"

SolidColor::SolidColor(const Color &c) : color_val(c) {}

SolidColor::SolidColor(float r, float g, float b) : color_val(Color{r, g, b}) {}
Color SolidColor::value(float u, float v, const Point3 &p) const { return color_val; }
Color CheckerTexture::value(float u, float v, const Point3 &p) const {
	auto u_int = static_cast<int>(u * 50 * inv_scale);
	auto v_int = static_cast<int>(v * 50 * inv_scale);

	bool is_even = (u_int + v_int) % 2 == 0;
	return is_even ? even->value(u, v, p) : odd->value(u, v, p);
}
CheckerTexture::CheckerTexture(float scale, std::shared_ptr<ITexture> even_tex, std::shared_ptr<ITexture> odd_tex) :
	inv_scale(1 / scale), even(std::move(even_tex)), odd(std::move(odd_tex)) {}

CheckerTexture::CheckerTexture(float scale, const Color &even_color, const Color &odd_color) :
	inv_scale(1 / scale), even(std::make_shared<SolidColor>(even_color)), odd(std::make_shared<SolidColor>(odd_color)) {

}


Color ImageTexture::value(float u, float v, const Point3 &p) const {
	if (img.height() <= 0)
		return Color{0, 1, 1};

	u = Interval(0, 1).clamp(u);
	v = 1.0 - Interval(0, 1).clamp(v);

	auto i = static_cast<int>(u * img.width());
	auto j = static_cast<int>(v * img.height());
	auto pixel = img.pixelData(i, j);

	float color_scale = 1.0 / 255.0;
	return Color{color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
}


ImageTexture::ImageTexture(const std::string &image, const std::string &parent) : img(Image(image, parent)) {}

NoiseTexture::NoiseTexture(float frequency, int octave_count, float presistence) :
	frequency(frequency), octave_count(octave_count), persistence(presistence) {}

Color NoiseTexture::value(float u, float v, const Point3 &p) const {
	return Color{1, 1, 1} * fabs(sin(10 * noise.octaveNoise(p, frequency, octave_count, persistence) + 5));
}

TerrainTexture::TerrainTexture(float frequency, int octave_count, float presistence) :
	frequency(frequency), octave_count(octave_count), persistence(presistence) {}

Color TerrainTexture::value(float u, float v, const Point3 &p) const {
	float height = noise.octaveNoise(p, frequency, octave_count, persistence);
	if (Interval(-1, 0).within(height)) {
		return Color{0, 0, 1} + Color{0, 0, 0xee / 255.0} * height;
	}
	else if (Interval(0, 0.05).within(height)) {
		return Color{0xe9 / 255.0, 0xd5 / 255.0, 0x5a / 255.0};
	}
	else if (Interval(0.05, 1).within(height)) {
		return Color{0, 0xDD / 255.0, 0} - Color{0, (0xDD - 0x33) / 255.0, 0} * height / 0.95;
	}
	else {
		spdlog::warn("unexpected value: {}", height);
		return Color{0, 0, 0};
	}
}
