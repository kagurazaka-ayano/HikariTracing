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
	int worker_cnt;
	if(render_thread_count == 0) {
		worker_cnt = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	}
	else {
		worker_cnt = render_thread_count;
	}
	std::vector<std::vector<Color>> image;
	image.resize(height);
	for(auto& i : image) {
		i.resize(width);
	}
	int part = partition();
	auto m = KawaiiMQ::MessageQueueManager::Instance();
	auto result_queue = KawaiiMQ::makeQueue("result");
	auto result_topic = KawaiiMQ::Topic("renderResult");
	m->relate(result_topic, result_queue);
	auto th = std::vector<std::thread>();
	spdlog::info("rendering started!");
	spdlog::info("using {} threads to render {} blocks", worker_cnt, part);
	auto begin = std::chrono::system_clock::now();
	for(int i = 0; i < worker_cnt; i++) {
		th.emplace_back(&Camera::RenderWorker, this, std::ref(world));
	}
	for(auto& i : th) {
		i.join();
	}
	auto end = std::chrono::system_clock::now();
	auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	spdlog::info("render completed! taken {}s", static_cast<double>(time_elapsed.count()) / 1000.0);
	auto consumer = KawaiiMQ::Consumer("resultConsumer");
	consumer.subscribe(result_topic);
	while(!result_queue->empty()) {
		auto chunk = KawaiiMQ::getMessage<ImageChunk>(consumer.fetchSingleTopic(result_topic)[0]);
		for(int i = chunk.starty; i < chunk.starty + chunk.height; i++) {
			for(int j = chunk.startx; j < chunk.startx + chunk.width; j++) {
				image[i][j] = chunk.partial[i - chunk.starty][j - chunk.startx];
			}
		}
	}
#ifndef ASCII_ART
	makePPM(width, height, image, path, name);
#else
	makeGrayscaleTxt(width, height, image, path, name);
#endif
}

void Camera::RenderWorker(const IHittable &world) {
	std::stringstream ss;
	ss << std::this_thread::get_id();
	auto m = KawaiiMQ::MessageQueueManager::Instance();
	auto result_topic = KawaiiMQ::Topic("renderResult");
	auto result_producer = KawaiiMQ::Producer("producer");
	result_producer.subscribe(result_topic);
	auto task_topic = KawaiiMQ::Topic("renderTask");
	auto task_fetcher = KawaiiMQ::Consumer({task_topic});
	auto task_queue = m->getAllRelatedQueue(task_topic)[0];
	spdlog::info("thread {} started", ss.str());
	while (!task_queue->empty()) {
		auto chunk = KawaiiMQ::getMessage<ImageChunk>(task_fetcher.fetchSingleTopic(task_topic)[0]);
		spdlog::info("chunk {} (start from ({}, {}), dimension {} * {}) started by thread {}", chunk.chunk_idx,
					 chunk.startx, chunk.starty, chunk.width, chunk.height, ss.str());
		for (int i = chunk.starty; i < chunk.starty + chunk.height; i++) {
			auto hori = std::vector<Color>();
			hori.reserve(chunk.width);
			for (int j = chunk.startx; j < chunk.startx + chunk.width; j++) {
				Color pixel_color = Color{0, 0, 0};
				for (int k = 0; k < sample_count; ++k) {
					auto ray = getRay(j, i);
					pixel_color += rayColor(ray, world, render_depth);
				}
				pixel_color /= sample_count;
				pixel_color = Color{gammaCorrect(pixel_color[0]), gammaCorrect(pixel_color[1]), gammaCorrect(pixel_color[2])};
				hori.emplace_back(pixel_color);
			}
			chunk.partial.emplace_back(hori);
		}
		auto message = KawaiiMQ::makeMessage(chunk);
		result_producer.broadcastMessage(message);
	}
}

int Camera::partition() const {
	auto manager = KawaiiMQ::MessageQueueManager::Instance();
	auto queue = KawaiiMQ::makeQueue("renderTaskQueue");
	auto topic = KawaiiMQ::Topic("renderTask");
	manager->relate(topic, queue);
	KawaiiMQ::Producer prod("chunkPusher");
	prod.subscribe(topic);
	int upperleft_x = 0;
	int upperleft_y = 0;
	int idx = 0;
	while(upperleft_y < height) {
		while(upperleft_x < width) {
			ImageChunk chunk;
			chunk.chunk_idx = idx;
			++idx;
			chunk.startx = upperleft_x;
			chunk.starty = upperleft_y;
			if(upperleft_x + chunk_dimension > width) {
				chunk.width = width % chunk_dimension;
			}
			else {
				chunk.width = chunk_dimension;
			}
			if(upperleft_y + chunk_dimension > height) {
				chunk.height = height % chunk_dimension;
			}
			else {
				chunk.height = chunk_dimension;
			}
			auto message = KawaiiMQ::makeMessage(chunk);
			prod.broadcastMessage(message);
			upperleft_x += chunk_dimension;
		}
		upperleft_x = 0;
		upperleft_y += chunk_dimension;
	}
	return idx;
}

Camera::Camera(int width, double aspect_ratio, double fov, AppleMath::Vector3 target, Point3 position, double dof_angle) : width(width), aspect_ratio(aspect_ratio),
	fov(fov), target(std::move(target)),
	position(std::move(position)),
	height(static_cast<int>(width / aspect_ratio)), dof_angle(dof_angle){
	render_thread_count = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	updateVectors();

}

void Camera::updateVectors() {
	auto theta = deg2Rad(fov);
	auto h = tan(theta / 2);
	focal_len = (position - target).length();
	viewport_height = 2 * h * focal_len;
	viewport_width = viewport_height * (static_cast<double>(width) / height);
	w = (position - target).normalized();
	u = UP.cross(w).normalized();
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

const AppleMath::Vector3 &Camera::getHoriVec() const {
	return hori_vec;
}

const AppleMath::Vector3 &Camera::getVertVec() const {
	return vert_vec;
}

const AppleMath::Vector3 &Camera::getPixDeltaX() const {
	return pix_delta_x;
}

const AppleMath::Vector3 &Camera::getPixDeltaY() const {
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
	if (render_thread_count > width || render_thread_count > height) {
		render_thread_count = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	}
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

AppleMath::Vector3 Camera::dofDiskSample() const {
	auto p = randomVec3InUnitDisk();
	return position + (p[0] * dof_disk_h) + (p[1] * dof_disk_v);
}

Color Camera::rayColor(const Ray &ray, const IHittable &object, int depth) {
	HitRecord record;
	if(depth <= 0) return Color{0, 0, 0};

	if (object.hit(ray, Interval(EPS, INF), record)) {
		Ray scattered;
		Color attenuation;

		if (record.material->scatter(ray, record, attenuation, scattered)) {
			return attenuation.componentProd(rayColor(scattered, object, depth - 1));
		}
		return Color{0, 0, 0};
	}
	auto unit = ray.dir().normalized();
	return Color{std::min(0.5 * unit[0] + 1, 1.0), std::min(0.5 * unit[1] + 1, 1.0), 1};

}

int Camera::getSampleCount() const {
	return sample_count;
}
AppleMath::Vector3 Camera::randomDisplacement() const {
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
	if (renderThreadCount == 0 || renderThreadCount > width || renderThreadCount > height) {
		render_thread_count = std::thread::hardware_concurrency() == 0 ? 12 : std::thread::hardware_concurrency();
	}
	else {
		render_thread_count = renderThreadCount;
	}
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
	auto time = randomDouble(0, shutter_speed);
	return Ray(origin, direction, time);
}
int Camera::getChunkDimension() const {
	return chunk_dimension;
}

void Camera::setChunkDimension(int dimension) {
	chunk_dimension = (dimension > width || dimension > height) ? width / render_thread_count : dimension;
}

double Camera::getShutterSpeed() const {
	return shutter_speed;
}

void Camera::setShutterSpeed(double shutterSpeed) {
	shutter_speed = shutterSpeed;
}
