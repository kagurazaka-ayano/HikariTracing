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

class IMaterial {
public:
	virtual ~IMaterial() = default;

	virtual bool scatter(const Ray &r_in, const HitRecord &record, Math::Vector3 &attenuation,
						 Ray &scattered) const = 0;
};

class Lambertian : public IMaterial {
public:
	Lambertian(Color albedo);

	bool scatter(const Ray &r_in, const HitRecord &record, Math::Vector3 &attenuation,
				 Ray &scattered) const override;

private:
	Color albedo;

};

class Metal : public IMaterial {
public:
	Metal(const Color &albedo, double fuzz);

	bool scatter(const Ray &r_in, const HitRecord &record, Math::Vector3 &attenuation,
				 Ray &scattered) const override;

private:
	Color albedo;
	double fuzz;
};

class Dielectric : public IMaterial {
public:
	Dielectric(double idx, const Color &albedo);

	bool scatter(const Ray &r_in, const HitRecord &record, Math::Vector3 &attenuation,
				 Ray &scattered) const override;
private:
	double ir;
	Color albedo;
	static double reflectance(double cosine, double refr_idx);
};


#endif // ONEWEEKEND_MATERIAL_H
