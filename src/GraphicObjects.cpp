/**
 * @file GraphicObjects.cpp
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#include <utility>

#include "GraphicObjects.h"
#include "Material.h"


Sphere::Sphere(double radius, Vec3 position, std::shared_ptr<IMaterial> mat) : radius(radius), position(std::move(position)), material(mat) {
}

bool Sphere::hit(const Ray &r, Interval interval, HitRecord& record) const {
    std::shared_ptr<Vec3> oc = std::make_shared<Vec3>(r.pos() - position);
    auto a = r.dir().lengthSq();
    auto h = oc->dot(r.dir());
    auto c = oc->lengthSq() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0) return false;
    auto discri_sqrt = std::sqrt(discriminant);

    auto root = (-h - discri_sqrt) / a;
    if (!interval.surround(root)) {
        root = (-h + discri_sqrt) / a;
        if (!interval.surround(root)){
            return false;
        }
    }
    record.t = root;
    record.p = r.at(root);
    auto out_normal = (record.p - position) / radius;
    record.setFaceNormal(r, out_normal);
	record.material = material;
    return true;
}

void HitRecord::setFaceNormal(const Ray &r, const Vec3 &normal_out) {
    front_face = normal_out.dot(r.dir()) < 0;
    normal = front_face ? normal_out : -normal_out;
}

auto HittableList::end() {
    return objects.end();
}

bool HittableList::hit(const Ray &r, Interval interval, HitRecord &record) const {
    HitRecord temp_rec;
    bool if_hit = false;
    auto closest_t = interval.max;

    for (const auto& i : objects) {
        if (i->hit(r, Interval(interval.min, closest_t), temp_rec)) {
            if_hit = true;
            closest_t = temp_rec.t;
            record = temp_rec;
        }
    }
    return if_hit;
}

HittableList::HittableList(const std::shared_ptr<IHittable>& obj) {
    add(obj);
}

void HittableList::add(const std::shared_ptr<IHittable>& obj) {
    objects.push_back(obj);
}

void HittableList::clear() {
    objects.clear();
}

auto HittableList::begin() {
    return objects.begin();
}
