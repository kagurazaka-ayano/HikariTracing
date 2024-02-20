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

	virtual Color value(double u, double v, const Point3& p) const = 0;
};

class SolidColor : public ITexture {
public:
	SolidColor(const Color& c);

	SolidColor(double r, double g, double b);

	Color value(double u, double v, const Point3& p) const override;

private:
	Color color_val;
};

class CheckerTexture : public ITexture {
public:
	CheckerTexture(double scale, std::shared_ptr<ITexture> even_tex, std::shared_ptr<ITexture> odd_tex);

	CheckerTexture(double scale, const Color& even_color, const Color& odd_color);

	Color value(double u, double v, const Point3 &p) const override;

private:
	double inv_scale;
	std::shared_ptr<ITexture> even;
	std::shared_ptr<ITexture> odd;
};

class ImageTexture : public ITexture {
public:
	ImageTexture(const std::string& image, const std::string& parent = IMG_INPUT_DIR);

	Color value(double u, double v, const Point3 &p) const override;

private:
	Image img;
};

class NoiseTexture : public ITexture {
public:
	NoiseTexture(double frequency, int octave_count, double persistence);

	Color value(double u, double v, const Point3& p) const override;
private:
	Perlin noise;
	double frequency;
	int octave_count;
	double persistence;
};

class TerrainTexture : public ITexture {
public:
	TerrainTexture(double frequency, int octave_count, double persistence);

	Color value(double u, double v, const Point3& p) const override;
private:
	Perlin noise;
	double frequency;
	int octave_count;
	double persistence;
};

#endif // RAYTRACING_TEXTURE_H
