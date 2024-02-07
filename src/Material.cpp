/**
 * @file Material.cpp
 * @author ayano
 * @date 2/4/24
 * @brief
 */

#include "Material.h"


Lambertian::Lambertian(const Color &albedo) : albedo(albedo) {

}
bool Lambertian::scatter(const Ray &r_in, const HitRecord &record, Vec3 &attenuation,
						 Ray &scattered) const {
	auto ray_dir = record.normal + Vec3::randomUnitVec3();
	if (ray_dir.verySmall()) {
		ray_dir = record.normal;
	}
	scattered = Ray(record.p, ray_dir);
	attenuation = albedo;
	return true;
}

Metal::Metal(const Color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {}
bool Metal::scatter(const Ray &r_in, const HitRecord &record, Vec3 &attenuation,
					Ray &scattered) const {
	auto ray_dir = Vec3::reflect(r_in.dir().unit() + Vec3::randomUnitVec3() * fuzz, record.normal);
	scattered = Ray(record.p, ray_dir);
	attenuation = albedo;
	return true;
}

Dielectric::Dielectric(double idx, const Color &albedo): ir(idx), albedo(albedo) {}

bool Dielectric::scatter(const Ray &r_in, const HitRecord &record, Vec3 &attenuation,
						 Ray &scattered) const {
	attenuation = albedo;
	double ref_ratio = record.front_face ? (1.0 / ir) : ir;
	auto unit = r_in.dir().unit();

	double cos = fmin((-unit).dot(record.normal), 1.0);
	double sin = sqrt(1 - cos * cos);

	bool can_refr = ref_ratio * sin < 1.0;
	Vec3 dir;
	if(can_refr && reflectance(cos, ref_ratio) < randomDouble())
		dir = Vec3::refract(r_in.dir().unit(), record.normal, ref_ratio);
	else
		dir = Vec3::reflect(r_in.dir().unit(), record.normal);

	scattered = Ray(record.p, dir);
	return true;
}
double Dielectric::reflectance(double cosine, double refr_idx) {
	auto r0 = (1 - refr_idx) / (1 + refr_idx);
	r0 *= r0;
	return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}
