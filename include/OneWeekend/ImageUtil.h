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
using Color = Vec3;

double gammaCorrect(double c);

void makePPM(int width, int height, std::vector<std::vector<Color>> img, const std::string &path,
			 const std::string &name);

#endif //ONEWEEKEND_IMAGEUTIL_H
