/**
 * @file GraphicObjects.cpp
 * @author ayano
 * @date 1/16/24
 * @brief
 */

#include "GraphicObjects.h"
#include "Eigen/Core"
#include "Material.h"
#include "MathUtil.h"
#include <memory>

Sphere::Sphere(float radius, Eigen::Vector3d position,
               std::shared_ptr<IMaterial> mat)
    : radius(radius), position(std::move(position)), material(std::move(mat)) {
    auto rvec = Eigen::Vector3d{radius, radius, radius};
    bbox = AABB(this->position - rvec, this->position + rvec);
}

bool Sphere::hit(const Ray &r, Interval interval, HitRecord &record) const {
    auto sphere_center = getPosition(r.time());
    std::shared_ptr<Eigen::Vector3d> oc =
        std::make_shared<Eigen::Vector3d>(r.pos() - sphere_center);
    auto a = r.dir().squaredNorm();
    auto h = oc->dot(r.dir());
    auto c = oc->squaredNorm() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;
    auto discri_sqrt = std::sqrt(discriminant);

    auto root = (-h - discri_sqrt) / a;
    if (!interval.surround(root)) {
        root = (-h + discri_sqrt) / a;
        if (!interval.surround(root)) {
            return false;
        }
    }
    record.t = root;
    record.p = r.at(record.t);
    auto out_normal = (record.p - sphere_center) / radius;
    record.setFaceNormal(r, out_normal);
    getSphereUV(out_normal, record.u, record.v);
    record.material = material;
    return true;
}

void Sphere::getSphereUV(const Point3 &p, float &u, float &v) {
    float theta = std::acos(-p[1]);
    float phi = std::atan2(-p[2], p[0]) + PI;
    u = phi / (2 * PI);
    v = theta / PI;
}

void HitRecord::setFaceNormal(const Ray &r, const Eigen::Vector3d &normal_out) {
    front_face = normal_out.dot(r.dir()) < 0;
    normal = front_face ? normal_out : -normal_out;
}

auto HittableList::end() { return objects.end(); }

bool HittableList::hit(const Ray &r, Interval interval,
                       HitRecord &record) const {
    HitRecord temp_rec;
    bool if_hit = false;
    auto closest_t = interval.max;
    for (const auto &i : objects) {
        if (i->hit(r, Interval(interval.min, closest_t), temp_rec)) {
            if_hit = true;
            closest_t = temp_rec.t;
            record = temp_rec;
        }
    }
    return if_hit;
}

HittableList::HittableList(const std::shared_ptr<IHittable> &obj) {
    add(obj);
    bbox = obj->boundingBox();
}

void HittableList::add(const std::shared_ptr<IHittable> &obj) {
    objects.push_back(obj);
    bbox = AABB(bbox, obj->boundingBox());
}

void HittableList::clear() {
    objects.clear();
    bbox = AABB();
}

auto HittableList::begin() { return objects.begin(); }
AABB HittableList::boundingBox() const { return bbox; }

Sphere::Sphere(float radius, const Point3 &init_position,
               const Point3 &final_position, std::shared_ptr<IMaterial> mat)
    : radius(radius), position(init_position), material(std::move(mat)) {
    direction_vec = final_position - init_position;
    is_moving = true;
    auto rvec = Eigen::Vector3d{radius, radius, radius};
    auto bbox1 = AABB(init_position - rvec, init_position + rvec);
    auto bbox2 = AABB(final_position - rvec, final_position + rvec);
    bbox = AABB(bbox1, bbox2);
}

Eigen::Vector3d Sphere::getPosition(float time) const {
    return is_moving ? position + time * direction_vec : position;
}

AABB Sphere::boundingBox() const { return bbox; }

bool BVHNode::hit(const Ray &r, Interval interval, HitRecord &record) const {
    if (!bbox.hit(r, interval)) {
        return false;
    }
    bool hit_left = left->hit(r, interval, record);
    bool hit_right = right->hit(
        r, Interval(interval.min, hit_left ? record.t : interval.max), record);
    return hit_left || hit_right;
}

AABB BVHNode::boundingBox() const { return bbox; }

bool BVHNode::compare(const std::shared_ptr<IHittable> &a,
                      const std::shared_ptr<IHittable> &b, int axis) {
    return a->boundingBox().axis(axis).min < b->boundingBox().axis(axis).min;
}

bool BVHNode::compareX(const std::shared_ptr<IHittable> &a,
                       const std::shared_ptr<IHittable> &b) {
    return compare(a, b, 0);
}

bool BVHNode::compareY(const std::shared_ptr<IHittable> &a,
                       const std::shared_ptr<IHittable> &b) {
    return compare(a, b, 1);
}

bool BVHNode::compareZ(const std::shared_ptr<IHittable> &a,
                       const std::shared_ptr<IHittable> &b) {
    return compare(a, b, 2);
}

BVHNode::BVHNode(const HittableList &list)
    : BVHNode(list.objects, 0, list.objects.size()) {}

BVHNode::BVHNode(const std::vector<std::shared_ptr<IHittable>> &objects,
                 size_t start, size_t end) {
    auto modifiable_objects = objects;
    auto axis = randomInt(0, 2);
    auto comparator = (axis == 0)   ? compareX
                      : (axis == 1) ? compareY
                                    : compareZ;
    auto object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    } else {
        std::sort(modifiable_objects.begin() + start,
                  modifiable_objects.begin() + end, comparator);
        auto mid = start + object_span / 2;
        left = std::make_shared<BVHNode>(modifiable_objects, start, mid);
        right = std::make_shared<BVHNode>(modifiable_objects, mid, end);
    }

    bbox = AABB(left->boundingBox(), right->boundingBox());
}

AABB Quad::boundingBox() const { return bbox; }

void Quad::setBoundingBox() { bbox = AABB(Q, Q + u + v).pad(); }

Quad::Quad(const Eigen::Vector3d &Q, const Eigen::Vector3d &u,
           const Eigen::Vector3d &v, std::shared_ptr<IMaterial> mat)
    : Q(Q), u(u), v(v), mat(mat) {
    auto n = u.cross(v);
    normal = n.normalized();
    D = normal.dot(Q);
    w = n / n.dot(n);
    setBoundingBox();
}

bool Quad::inside(float a, float b, HitRecord &rec) const {
    if (a < 0 || a > 1 || b < 0 || b > 1)
        return false;
    rec.u = a;
    rec.v = b;
    return true;
}

bool Quad::hit(const Ray &r, Interval interval, HitRecord &record) const {
    float denom = normal.dot(r.dir());
    if (fabs(denom) < 1e-8) {
        return false;
    }
    auto t = (D - normal.dot(r.pos())) / denom;
    if (!interval.surround(t)) {
        return false;
    }
    auto intersection = r.at(t);
    auto plane_hit_vect = intersection - Q;
    auto alpha = w.dot(plane_hit_vect.cross(v));
    auto beta = w.dot(u.cross(plane_hit_vect));
    if (!inside(alpha, beta, record)) {
        return false;
    }
    record.t = t;
    record.p = intersection;
    record.material = mat;
    record.setFaceNormal(r, normal);

    return true;
}

Triangle::Triangle(const Eigen::Vector3d &Q, const Eigen::Vector3d &u,
                   const Eigen::Vector3d &v, std::shared_ptr<IMaterial> mat)
    : Q(Q), u(u), v(v), mat(mat) {
    auto n = v.cross(u);
    normal = n.normalized();
    w = n / n.dot(n);
    setBoundingBox();
}

void Triangle::setBoundingBox() { box = AABB(Q, Q + u + v).pad(); }

AABB Triangle::boundingBox() const { return box; }

bool Triangle::inside(const Eigen::Vector3d &intersection) const {
    auto side1 = v;
    auto side2 = u - v;
    auto side3 = -u;
    auto dis1 = intersection - Q;
    auto dis2 = intersection - (Q + side1);
    auto dis3 = intersection - (Q + side1 + side2);
    return (side1.cross(dis1).dot(normal) > 0 &&
            side2.cross(dis2).dot(normal) > 0 &&
            side3.cross(dis3).dot(normal) > 0);
}

bool Triangle::hit(const Ray &r, Interval interval, HitRecord &record) const {
    float denom = normal.dot(r.dir());
    if (fabs(denom) < 1e-8) {
        return false;
    }

    auto t = (D - normal.dot(r.pos())) / denom;

    if (!interval.surround(t)) {
        return false;
    }

    auto intersection = r.at(t);
    if (!inside(intersection)) {
        return false;
    }
    auto plane_hit_vec = intersection - Q;
    auto alpha = w.dot(plane_hit_vec.cross(u));
    auto beta = w.dot(u.cross(plane_hit_vec));
    record.t = t;
    record.p = intersection;
    record.material = mat;
    record.setFaceNormal(r, normal);
    record.u = alpha;
    record.v = beta;
    return true;
}

Translate::Translate(std::shared_ptr<IHittable> obj,
                     const Eigen::Vector3d &displacement)
    : object(obj), offset(displacement) {
    bbox.x = Interval(bbox.x.min + offset.x(), bbox.x.max + offset.x());
    bbox.y = Interval(bbox.y.min + offset.y(), bbox.y.max + offset.y());
    bbox.z = Interval(bbox.z.min + offset.z(), bbox.z.max + offset.z());
}

AABB Translate::boundingBox() const { return bbox; }

bool Translate::hit(const Ray &r, Interval interval, HitRecord &record) const {
    Ray offset_r = Ray(r.pos() - offset, r.dir(), r.time());

    if (!object->hit(offset_r, interval, record)) {
        return false;
    }

    record.p += offset;

    return true;
}

Rotation::Rotation(std::shared_ptr<IHittable> obj, float psi, float theta,
                   float phi, Point3 about_pt)
    : object(obj), rotation_matrix(makeEulerRotationMatrixAboutPt(about_pt, psi,
                                                                  theta, phi)),
      inverse_rotation_matrix(
          makeEulerRotationMatrixAboutPt(about_pt, -psi, -theta, -phi)) {
    auto bbox_temp = obj->boundingBox();
    auto min_vec =
        Eigen::Vector4d{bbox_temp.x.min, bbox_temp.y.min, bbox_temp.z.min, 1};
    auto max_vec =
        Eigen::Vector4d{bbox_temp.x.max, bbox_temp.y.max, bbox_temp.z.max, 1};
    min_vec = rotation_matrix * min_vec;
    max_vec = rotation_matrix * max_vec;
    auto new_min_vec = deHomo(min_vec);
    auto new_max_vec = deHomo(max_vec);
    bbox.x = Interval(std::min(bbox_temp.x.min, new_min_vec.x()),
                      std::max(bbox_temp.x.max, new_max_vec.x()));
    bbox.y = Interval(std::min(bbox_temp.y.min, new_min_vec.y()),
                      std::max(bbox_temp.y.max, new_max_vec.y()));
    bbox.z = Interval(std::min(bbox_temp.z.min, new_min_vec.z()),
                      std::max(bbox_temp.z.max, new_max_vec.z()));
}

AABB Rotation::boundingBox() const { return bbox; }

bool Rotation::hit(const Ray &r, Interval interval, HitRecord &record) const {
    auto origin = r.pos();
    auto dir = r.dir();

    origin = deHomo(rotation_matrix * makeHomo(origin));
    dir = deHomo(rotation_matrix * makeHomo(dir));
    Ray rotated(origin, dir, r.time());

    if (!object->hit(rotated, interval, record)) {
        return false;
    }
    auto inverse_transpose =
        Eigen::Matrix4d(inverse_rotation_matrix.inverse().transpose());
    record.normal = deHomo(inverse_transpose * makeHomo(record.normal));
    record.p = deHomo(inverse_rotation_matrix * makeHomo(record.p));
    return true;
}
