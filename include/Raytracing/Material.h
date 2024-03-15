/**
 * @file IMaterial.h
 * @author ayano
 * @date 2/4/24
 * @brief
 */

#ifndef ONEWEEKEND_MATERIAL_H
#define ONEWEEKEND_MATERIAL_H
#include <memory>
#include "MathUtil.h"
#include "GraphicObjects.h"
#include "KawaiiMQ/kawaiiMQ.h"
#include "Texture.h"

class IMaterial {
public:
	virtual ~IMaterial() = default;

	virtual bool scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
						 Ray &scattered) const = 0;
	
	virtual Color emitted(float u, float v, const Point3& p) const;
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
	Metal(const std::shared_ptr<ITexture> &albedo, float fuzz);

	Metal(const Color& abledo, float fuzz);

	bool scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
				 Ray &scattered) const override;

private:
	std::shared_ptr<ITexture> albedo;
	float fuzz;
};

class Dielectric : public IMaterial {
public:
	Dielectric(float idx, const std::shared_ptr<ITexture> &albedo);

	Dielectric(float idx, const Color& albedo);

	bool scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
				 Ray &scattered) const override;
private:
	float ir;
	std::shared_ptr<ITexture> albedo;
	static float reflectance(float cosine, float refr_idx);
};

class DiffuseLight : public IMaterial {
public:
	DiffuseLight(std::shared_ptr<ITexture> t);

	DiffuseLight(Color c);

	bool scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
				 Ray &scattered) const override;
	
	Color emitted(float u, float v, const Point3& p) const override;

private:
	std::shared_ptr<ITexture> emit;
};

#endif // ONEWEEKEND_MATERIAL_H
