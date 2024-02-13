/**
 * @file ImageUtil.cpp
 * @author ayano
 * @date 1/15/24
 * @brief
*/

#include "ImageUtil.h"

std::string getGreyScaleCharacter(double r, double g, double b) {
	double sum = r * 255.0 * 0.299 + g * 255.0 * 0.587 + b * 255.0 * 0.114;
	std::stringstream ret;
	ret << grayscale[static_cast<int>((sum - 1.0) / (255.0 / static_cast<double>(grayscale.size())))];
	return ret.str();
}

double gammaCorrect(double c) {
	return std::pow(c, 1.0 / 2.0);
}

void makePPM(int width, int height, const std::vector<std::vector<Color>>& img, const std::string &path,
			 const std::string &name) {
	auto fout = std::ofstream();
	mkdir(path, name);
	fout.open((path.ends_with("/") ? path : path + "/") + name);
	fout << "P3\n" << width << ' ' << height << "\n255\n";
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			fout << img[i][j].makeColor();
		}
	}
	fout.close();
}

std::string makeGrayscaleString(int width, int height, std::vector<std::vector<Color>> img) {
	std::stringstream ss;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			ss << getGreyScaleCharacter(img[i][j].x, img[i][j].y, img[i][j].z);
		}
		ss << std::endl;
	}
	return ss.str();
}


void makeGrayscaleTxt(int width, int height, const std::vector<std::vector<Color>>& img, const std::string &path, const std::string &name) {
	auto fout = std::ofstream();
	mkdir(path, name);
	fout.open((path.ends_with("/") ? path : path + "/") + name);
	fout << makeGrayscaleString(width, height, img);
	fout.close();
}


void mkdir(const std::string& path, const std::string& name) {
	auto dir = path.ends_with("/") ? path : path + "/";
	struct stat st;
	if (stat(dir.c_str(), &st) != 0 && mkdirat(AT_FDCWD, dir.c_str(), 0755) == -1) {
		spdlog::critical("directory create failed at {}", path);
		exit(2);
	}
}

std::vector<std::string> split(const std::string &str, const std::string &delimeter) {
	std::vector<std::string> ret;
	std::size_t pos = 0;
	std::string token;
	std::string str_op = str;
	while ((pos = str_op.find(delimeter)) != std::string::npos) {
		token = str_op.substr(0, pos);
		ret.push_back(token);
		str_op.erase(0, pos + delimeter.length());
	}
	return ret;
}
