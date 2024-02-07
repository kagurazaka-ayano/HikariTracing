/**
 * @file Camera.cpp
 * @author ayano
 * @date 2/7/24
 * @brief
 */

#include "Camera.h"
#include "Material.h"


void Camera::Render(const IHittable& world, const std::string& path, const std::string& name) {
	auto now = std::chrono::system_clock::now();
	spdlog::info("rendering started!");
	int worker_cnt;
	if(render_thread_count == 0) {
		worker_cnt = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	}
	else {
		worker_cnt = render_thread_count;
	}
	spdlog::info("using {} threads", worker_cnt);
	auto displacement = getHeight() / worker_cnt;
	auto workers = std::vector<std::future<std::vector<std::vector<Color>>>>();
	for (int i = 0; i < worker_cnt - 1; ++i) {
		auto f = std::async(std::launch::async, &Camera::RenderWorker, this, std::ref(world), i * displacement, (i + 1) * displacement);
		workers.push_back(std::move(f));
	}
	auto f = std::async(std::launch::async, &Camera::RenderWorker, this, std::ref(world), (worker_cnt - 1) * displacement, getHeight());
	workers.push_back(std::move(f));
	auto img = std::vector<std::vector<Color>>();
	for (auto& i : workers) {
		auto res = i.get();
		img.insert(img.end(), res.begin(), res.end());
	}
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
	spdlog::info("finished! rendering time: {}s", static_cast<double>(elapsed.count()) / 1000.0);
	makePPM(getWidth(), getHeight(), img, path, name);
}


std::vector<std::vector<Color>> Camera::RenderWorker(const IHittable &world, int start, int end) {
	auto img = std::vector<std::vector<Color>>();
	std::stringstream ss;
	ss << std::this_thread::get_id();
	spdlog::info("thread {} (from {} to {}) started!", ss.str(), start, end - 1);
	auto range = end - start;
	auto begin = std::chrono::system_clock::now();
	for (int i = start; i < end; ++i) {
		auto v = std::vector<Color>();
		if (i % (range / 3) == 0) {
			spdlog::info("line remaining for thread {}: {}", ss.str(), (range - (i - start) + 1));
		}
		for (int j = 0; j < getWidth(); ++j) {
			Color pixel_color = {0, 0, 0};
			for (int k = 0; k < sample_count; ++k) {
				auto ray = getRay(j, i);
				pixel_color += rayColor(ray, world, render_depth);
			}
			pixel_color /= sample_count;
			pixel_color = {gammaCorrect(pixel_color.x), gammaCorrect(pixel_color.y), gammaCorrect(pixel_color.z)};
			v.emplace_back(pixel_color);
		}
		img.emplace_back(v);
	}
	auto finish = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - begin);
	spdlog::info("thread {} (from {} to {}) finished! time: {}s", ss.str(), start, end - 1, static_cast<double>(elapsed.count()) / 1000.0);
	return img;
}

Camera::Camera(int width, double aspect_ratio, double fov, Vec3 target, Point3 position, double dof_angle) : width(width), aspect_ratio(aspect_ratio),
	fov(fov), target(std::move(target)),
	position(std::move(position)),
	height(static_cast<int>(width / aspect_ratio)), dof_angle(dof_angle){
	updateVectors();

}

void Camera::updateVectors() {
	auto theta = deg2Rad(fov);
	auto h = tan(theta / 2);
	focal_len = (position - target).length();
	viewport_height = 2 * h * focal_len;
	viewport_width = viewport_height * (static_cast<double>(width) / height);
	w = (position - target).unit();
	u = UP.cross(w).unit();
	v = w.cross(u);
	hori_vec = viewport_width * u;
	vert_vec = viewport_height * -v;
	pix_delta_x = hori_vec / width;
	pix_delta_y = vert_vec / height;
	viewport_ul = position - focal_len * w - (vert_vec + hori_vec) / 2;
	pixel_00 = viewport_ul + (pix_delta_y + pix_delta_x) * 0.5;
	auto dof_radius = focal_len * tan(deg2Rad(dof_angle / 2));
	dof_disk_h = u * dof_radius;
	dof_disk_v = v * dof_radius;
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

const Point3 &Camera::getPosition() const {
	return position;
}

const Vec3 &Camera::getHoriVec() const {
	return hori_vec;
}

const Vec3 &Camera::getVertVec() const {
	return vert_vec;
}

const Vec3 &Camera::getPixDeltaX() const {
	return pix_delta_x;
}

const Vec3 &Camera::getPixDeltaY() const {
	return pix_delta_y;
}

const Point3 &Camera::getViewportUl() const {
	return viewport_ul;
}

const Point3 &Camera::getPixel00() const {
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
	updateVectors();
}

void Camera::setPosition(const Point3 &position) {
	Camera::position = position;
	updateVectors();
}

void Camera::setSampleCount(int sample_count) {
	Camera::sample_count = sample_count;
}

Vec3 Camera::dofDiskSample() const {
	auto p = Vec3::randomVec3InUnitDisk();
	return position + (p.x * dof_disk_h) + (p.y * dof_disk_v);
}

Color Camera::rayColor(const Ray &ray, const IHittable &object, int depth) {
	HitRecord record;
	if(depth <= 0) return {0, 0, 0};

	if (object.hit(ray, Interval(0.01, INF), record)) {
		Ray scattered;
		Color attenuation;

		if (record.material->scatter(ray, record, attenuation, scattered)) {
			return attenuation * rayColor(scattered, object, depth - 1);
		}
		return {0, 0, 0};
	}
	auto unit = ray.dir().unit();
	return Color(std::min(0.5 * unit.x + 1, 1.0), std::min(0.5 * unit.y + 1, 1.0), 1);

}

int Camera::getSampleCount() const {
	return sample_count;
}
Vec3 Camera::randomDisplacement() const {
	auto delta_x = pix_delta_x * (randomDouble() - 0.5);
	auto delta_y = pix_delta_y * (randomDouble() - 0.5);
	return delta_x + delta_y;
}

int Camera::getRenderDepth() const {
	return render_depth;
}
void Camera::setRenderDepth(int renderDepth) {
	render_depth = renderDepth;
}
int Camera::getRenderThreadCount() const {
	return render_thread_count;
}
void Camera::setRenderThreadCount(int renderThreadCount) {
	render_thread_count = renderThreadCount;
}
void Camera::setFov(double fov) {
	this->fov = fov;
}
double Camera::getFov() const {
	return fov;
}
const Point3 &Camera::getTarget() const {
	return target;
}

void Camera::setTarget(const Point3 &target) {
	Camera::target = target;
}
double Camera::getDofAngle() const {
	return dof_angle;
}
void Camera::setDofAngle(double dofAngle) {
	dof_angle = dofAngle;
	updateVectors();
}
void Camera::setFocalLen(double focalLen) {
	focal_len = focalLen;
	updateVectors();
}
Ray Camera::getRay(int x, int y) {
	auto pixel_vec = pixel_00 + pix_delta_x * x + pix_delta_y * y + randomDisplacement();
	auto origin = dof_angle <= 0 ? position : dofDiskSample();
	auto direction = pixel_vec - origin;
	return Ray(origin, direction);

}
