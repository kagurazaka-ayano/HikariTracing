/**
 * @file ITexture.h
 * @author ayano
 * @date 2/15/24
 * @brief
 */

#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H

#include "ImageUtil.h"
#include "MathUtil.h"
class ITexture {
public:
	virtual ~ITexture() = default;

	virtual Color value(float u, float v, const Point3& p) const = 0;
};

class SolidColor : public ITexture {
public:
	SolidColor(const Color& c);

	SolidColor(float r, float g, float b);

	Color value(float u, float v, const Point3& p) const override;

private:
	Color color_val;
};

class CheckerTexture : public ITexture {
public:
	CheckerTexture(float scale, std::shared_ptr<ITexture> even_tex, std::shared_ptr<ITexture> odd_tex);

	CheckerTexture(float scale, const Color& even_color, const Color& odd_color);

	Color value(float u, float v, const Point3 &p) const override;

private:
	float inv_scale;
	std::shared_ptr<ITexture> even;
	std::shared_ptr<ITexture> odd;
};

class ImageTexture : public ITexture {
public:
	ImageTexture(const std::string& image, const std::string& parent = IMG_INPUT_DIR);

	Color value(float u, float v, const Point3 &p) const override;

private:
	Image img;
};

class NoiseTexture : public ITexture {
public:
	NoiseTexture(float frequency, int octave_count, float persistence);

	Color value(float u, float v, const Point3& p) const override;
private:
	Perlin noise;
	float frequency;
	int octave_count;
	float persistence;
};

class TerrainTexture : public ITexture {
public:
	TerrainTexture(float frequency, int octave_count, float persistence);

	Color value(float u, float v, const Point3& p) const override;
private:
	Perlin noise;
	float frequency;
	int octave_count;
	float persistence;
};

#endif // RAYTRACING_TEXTURE_H
