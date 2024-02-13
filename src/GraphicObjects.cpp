/**
 * @file GraphicObjects.cpp
 * @author ayano
 * @date 1/16/24
 * @brief
*/


#include "GraphicObjects.h"
#include "Material.h"


Sphere::Sphere(double radius, Vec3 position, std::shared_ptr<IMaterial> mat) : radius(radius), position(std::move(position)), material(std::move(mat)) {
	auto rvec = Vec3{radius, radius, radius};
	bbox = AABB(this->position - rvec, this->position + rvec);
}

bool Sphere::hit(const Ray &r, Interval interval, HitRecord& record) const {
	auto sphere_center = getPosition(r.time());
    std::shared_ptr<Vec3> oc = std::make_shared<Vec3>(r.pos() - sphere_center);
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
    auto out_normal = (record.p - sphere_center) / radius;
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
	bbox = obj->boundingBox();
}

void HittableList::add(const std::shared_ptr<IHittable>& obj) {
    objects.push_back(obj);
	bbox = AABB(bbox, obj->boundingBox());
}

void HittableList::clear() {
    objects.clear();
	bbox = AABB();
}

auto HittableList::begin() {
    return objects.begin();
}
AABB HittableList::boundingBox() const {
	return bbox;
}


Sphere::Sphere(double radius, const Point3& init_position, const Point3& final_position, std::shared_ptr<IMaterial> mat) :
    radius(radius), position(init_position), material(std::move(mat)) {
    direction_vec = final_position - init_position;
    is_moving = true;
	auto rvec = Vec3{radius, radius, radius};
	auto bbox1 = AABB(init_position - rvec, init_position + rvec);
	auto bbox2 = AABB(final_position - rvec, final_position + rvec);
	bbox = AABB(bbox1, bbox2);
}

Vec3 Sphere::getPosition(double time) const
{
    return is_moving ? position + time * direction_vec : position;
}

AABB Sphere::boundingBox() const {
	return bbox;
}

bool BVHNode::hit(const Ray &r, Interval interval, HitRecord &record) const {
	if (!bbox.hit(r, interval)) {
		return false;
	}
	bool hit_left = left->hit(r, interval, record);
	bool hit_right = right->hit(r, Interval(interval.min, hit_left ? record.t : interval.max), record);
	return hit_left || hit_right;
}

AABB BVHNode::boundingBox() const {
	return bbox;
}

bool BVHNode::compare(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b, int axis) {
	return a->boundingBox().axis(axis).min < b->boundingBox().axis(axis).min;
}

bool BVHNode::compareX(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b) {
	return compare(a, b, 0);
}

bool BVHNode::compareY(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b) {
	return compare(a, b, 1);
}

bool BVHNode::compareZ(const std::shared_ptr<IHittable> &a, const std::shared_ptr<IHittable> &b) {
	return compare(a, b, 2);
}

BVHNode::BVHNode(const HittableList& list) : BVHNode(list.objects, 0, list.objects.size()) {

}

BVHNode::BVHNode(const std::vector<std::shared_ptr<IHittable>> &objects, size_t start, size_t end) {
	auto modifiable_objects = objects;
	auto axis = randomInt(0, 2);
	auto comparator = (axis == 0) ? compareX : (axis == 1) ? compareY : compareZ;
	auto object_span = end - start;

	if (object_span == 1) {
		left = right = objects[start];
	}
	else if (object_span == 2) {
		if (comparator(objects[start], objects[start + 1])) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else {
		std::sort(modifiable_objects.begin() + start, modifiable_objects.begin() + end, comparator);
		auto mid = start + object_span / 2;
		left = std::make_shared<BVHNode>(modifiable_objects, start, mid);
		right = std::make_shared<BVHNode>(modifiable_objects, mid, end);
	}

	bbox = AABB(left->boundingBox(), right->boundingBox());
}
