/**
 * @file IMaterial.h
 * @author ayano
 * @date 2/4/24
 * @brief
 */

#ifndef ONEWEEKEND_MATERIAL_H
#define ONEWEEKEND_MATERIAL_H
#include "MathUtil.h"
#include "GraphicObjects.h"
#include "KawaiiMQ/kawaiiMQ.h"
#include "Texture.h"

class IMaterial {
public:
	virtual ~IMaterial() = default;

	virtual bool scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
						 Ray &scattered) const = 0;
};

class Lambertian : public IMaterial {
public:
	explicit Lambertian(Color albedo);

	Lambertian(std::shared_ptr<ITexture> tex);

	bool scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
				 Ray &scattered) const override;

private:
	std::shared_ptr<ITexture> albedo;

};

class Metal : public IMaterial {
public:
	Metal(const std::shared_ptr<ITexture> &albedo, double fuzz);

	Metal(const Color& abledo, double fuzz);

	bool scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
				 Ray &scattered) const override;

private:
	std::shared_ptr<ITexture> albedo;
	double fuzz;
};

class Dielectric : public IMaterial {
public:
	Dielectric(double idx, const std::shared_ptr<ITexture> &albedo);

	Dielectric(double idx, const Color& albedo);

	bool scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
				 Ray &scattered) const override;
private:
	double ir;
	std::shared_ptr<ITexture> albedo;
	static double reflectance(double cosine, double refr_idx);
};


#endif // ONEWEEKEND_MATERIAL_H
