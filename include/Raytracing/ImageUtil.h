/**
 * @file ImageUtil.h
 * @author ayano
 * @date 1/15/24
 * @brief This is image utils, used to produce images
*/

#ifndef ONEWEEKEND_IMAGEUTIL_H
#define ONEWEEKEND_IMAGEUTIL_H

#include <fstream>
#include <vector>
#include <tuple>
#include <sstream>
#include <sys/stat.h>
#include "MathUtil.h"
#include "spdlog/spdlog.h"
#include "KawaiiMQ/kawaiiMQ.h"
using Color = AppleMath::Vector3;

const std::string grayscale = " .:-=+*#%@";

std::string getGreyScaleCharacter(double r, double g, double b);

void makePPM(int width, int height, const std::vector<std::vector<Color>>& img, const std::string &path,
			 const std::string &name);

void makeGrayscaleTxt(int width, int height, const std::vector<std::vector<Color>>& img, const std::string &path, const std::string &name);

std::string makeGrayscaleString(int width, int height, std::vector<std::vector<Color>> img);

void mkdir(const std::string& path, const std::string& name);

std::vector<std::string> split(const std::string& str, const std::string& delimeter);

double gammaCorrect(double c);

struct ImageChunk : public KawaiiMQ::MessageData {
	int startx;
	int starty;
	int chunk_idx;
	int width;
	int height;
	std::vector<std::vector<Color>> partial;
};



#endif //ONEWEEKEND_IMAGEUTIL_H
