/**
 * @file Material.cpp
 * @author ayano
 * @date 2/4/24
 * @brief
 */

#include "Material.h"

Lambertian::Lambertian(Color albedo) : albedo(std::make_shared<SolidColor>(std::move(albedo))) {

}

Lambertian::Lambertian(std::shared_ptr<ITexture> tex) : albedo(tex) {

}

bool Lambertian::scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
						 Ray &scattered) const {
	auto ray_dir = record.normal + randomUnitVec3();
	if (verySmall(ray_dir)) {
		ray_dir = record.normal;
	}
	scattered = Ray(record.p, ray_dir, r_in.time());
	attenuation = albedo->value(record.u, record.v, record.p);
	return true;
}

Metal::Metal(const Color &albedo, double fuzz) : albedo(std::make_shared<SolidColor>(albedo)), fuzz(fuzz) {

}

bool Metal::scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
					Ray &scattered) const {
	auto ray_dir = reflect(r_in.dir().normalized() + randomUnitVec3() * fuzz, record.normal);
	scattered = Ray(record.p, ray_dir, r_in.time());
	attenuation = albedo->value(record.u, record.v, record.p);
	return true;
}
Metal::Metal(const std::shared_ptr<ITexture> &albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {

}

Dielectric::Dielectric(double idx, const std::shared_ptr<ITexture> &albedo): ir(idx), albedo(albedo) {}

Dielectric::Dielectric(double idx, const Color &albedo) : ir(idx), albedo(std::make_shared<SolidColor>(albedo)) {

}

bool Dielectric::scatter(const Ray &r_in, const HitRecord &record, AppleMath::Vector3 &attenuation,
						 Ray &scattered) const {
	attenuation = albedo->value(record.u, record.v, record.p);
	double ref_ratio = record.front_face ? (1.0 / ir) : ir;
	auto unit = r_in.dir().normalized();

	double cos = fmin((-unit).dot(record.normal), 1.0);
	double sin = sqrt(1 - cos * cos);

	bool can_refr = ref_ratio * sin < 1.0;
	AppleMath::Vector3 dir;
	if(can_refr && reflectance(cos, ref_ratio) < randomDouble())
		dir = refract(r_in.dir().normalized(), record.normal, ref_ratio);
	else
		dir = reflect(r_in.dir().normalized(), record.normal);

	scattered = Ray(record.p, dir, r_in.time());
	return true;
}

double Dielectric::reflectance(double cosine, double refr_idx) {
	auto r0 = (1 - refr_idx) / (1 + refr_idx);
	r0 *= r0;
	return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}
