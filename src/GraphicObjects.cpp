/**
 * @file GraphicObjects.cpp
 * @author ayano
 * @date 1/16/24
 * @brief
*/

#include "GraphicObjects.h"

void Camera::Render(const IHittable& world) {
	auto img = std::vector<std::vector<ImageUtil::Color>>();
	auto now = std::chrono::system_clock::now();
	for (int i = 0; i < getHeight(); ++i) {
		auto v = std::vector<ImageUtil::Color>();
		if (i % 10 == 0 || i > getHeight() - 10) spdlog::info("line remaining: {}", (getHeight() - i + 1));
		for (int j = 0; j < getWidth(); ++j) {
			ImageUtil::Color pixel_color = {0, 0, 0};
			for (int k = 0; k < sample_count; ++k) {
				auto ray_dir = getPixRayDir(j, i);
				auto ray = MathUtil::Ray(getPosition(), ray_dir);
				pixel_color += rayColor(ray, world);
			}
			pixel_color /= sample_count;
			v.emplace_back(pixel_color);
		}
		img.emplace_back(v);
	}
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
	spdlog::info("finished! rendering time: {}s", static_cast<double>(elapsed.count()) / 1000.0);
	ImageUtil::makePPM(getWidth(), getHeight(), img, "out", "test.ppm");
}

Camera::Camera(int width, double aspect_ratio, double viewport_width, double focal_len,
               MathUtil::Point3 position) : width(width), aspect_ratio(aspect_ratio),
                                                   viewport_width(viewport_width), focal_len(focal_len),
                                                   position(std::move(position)),
                                                   height(static_cast<int>(width / aspect_ratio)),viewport_height(viewport_width / (static_cast<double>(width) / static_cast<double>(height))) {
    updateVectors();

}

void Camera::updateVectors() {
    hori_vec = MathUtil::Vec3(viewport_width, 0, 0);
    vert_vec = MathUtil::Vec3(0, -viewport_height, 0);
    pix_delta_x = hori_vec / width;
    pix_delta_y = vert_vec / height;
    viewport_ul = position - MathUtil::Vec3(0, 0, focal_len) - (vert_vec + hori_vec) / 2;
    pixel_00 = viewport_ul + (pix_delta_y + pix_delta_x) * 0.5;
}

int Camera::getWidth() const {
    return width;
}

int Camera::getHeight() const {
    return height;
}

double Camera::getAspectRatio() const {
    return aspect_ratio;
}

double Camera::getViewportWidth() const {
    return viewport_width;
}

double Camera::getViewportHeight() const {
    return viewport_height;
}

double Camera::getFocalLen() const {
    return focal_len;
}

const MathUtil::Point3 &Camera::getPosition() const {
    return position;
}

const MathUtil::Vec3 &Camera::getHoriVec() const {
    return hori_vec;
}

const MathUtil::Vec3 &Camera::getVertVec() const {
    return vert_vec;
}

const MathUtil::Vec3 &Camera::getPixDeltaX() const {
    return pix_delta_x;
}

const MathUtil::Vec3 &Camera::getPixDeltaY() const {
    return pix_delta_y;
}

const MathUtil::Point3 &Camera::getViewportUl() const {
    return viewport_ul;
}

const MathUtil::Point3 &Camera::getPixel00() const {
    return pixel_00;
}

void Camera::setWidth(int width) {
    Camera::width = width;
    height = static_cast<int>(width / aspect_ratio);
    updateVectors();
}

void Camera::setAspectRatio(double aspect_ratio) {
    Camera::aspect_ratio = aspect_ratio;
    height = static_cast<int>(width / aspect_ratio);
    viewport_height = viewport_width / aspect_ratio;
    updateVectors();
}

void Camera::setFocalLen(double focal_len) {
    Camera::focal_len = focal_len;
    updateVectors();
}

void Camera::setPosition(const MathUtil::Point3 &position) {
    Camera::position = position;
    updateVectors();
}

void Camera::setSampleCount(int sample_count) {
	Camera::sample_count = sample_count;
}

MathUtil::Vec3 Camera::getPixelVec(int x, int y) const {
    return pixel_00 + pix_delta_x * x + pix_delta_y * y + randomDisplacement();
//	return pixel_00 + pix_delta_x * x + pix_delta_y * y;
}


MathUtil::Vec3 Camera::getPixRayDir(int x, int y) const {
    return getPixelVec(x, y) - position;
}
ImageUtil::Color Camera::rayColor(const MathUtil::Ray &ray, const IHittable &object) {
	ImageUtil::Color color;
	HitRecord record;
	if (object.hit(ray, MathUtil::Interval(0, INF), record)) {
		auto refl = MathUtil::Vec3::randomUnitVec3InHemiSphere(record.normal);
		color = 0.5 * rayColor(MathUtil::Ray(record.p, refl), object);
	}
	else {
		auto unit = ray.dir().unit();
		color = ImageUtil::Color(std::min(0.5 * unit.x + 1, 1.0), std::min(0.5 * unit.y + 1, 1.0), 1);
	}
	return color;
}
int Camera::getSampleCount() const {
	return sample_count;
}
MathUtil::Vec3 Camera::randomDisplacement() const {
	auto delta_x = pix_delta_x * (MathUtil::randomDouble() - 0.5);
	auto delta_y = pix_delta_y * (MathUtil::randomDouble() - 0.5);
	return delta_x + delta_y;
}

Sphere::Sphere(double radius, MathUtil::Vec3 position) : radius(radius), position(std::move(position)) {
}

bool Sphere::hit(const MathUtil::Ray &r, MathUtil::Interval interval, HitRecord& record) const {
    MathUtil::Point3 oc = r.pos() - position;
    auto a = r.dir().lengthSq();
    auto h = oc.dot(r.dir());
    auto c = oc.lengthSq() - radius * radius;

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
    return true;
}

void HitRecord::setFaceNormal(const MathUtil::Ray &r, const MathUtil::Vec3 &normal_out) {
    front_face = normal_out.dot(r.dir()) < 0;
    normal = front_face ? normal_out : -normal_out;
}

auto HittableList::end() {
    return objects.end();
}

bool HittableList::hit(const MathUtil::Ray &r, MathUtil::Interval interval, HitRecord &record) const {
    HitRecord temp_rec;
    bool if_hit = false;
    auto closest_t = interval.max;

    for (const auto& i : objects) {
        if (i->hit(r, interval, temp_rec)) {
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
