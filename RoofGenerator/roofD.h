#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/opencv.hpp>
#include <vector>
#include "Config.h"

class RoofD {

protected:
	RoofD() {}

public:
	static cv::Mat generateRoof(int width, int height, const std::vector<Config>& roof_paras, const cv::Scalar& bg_color, const cv::Scalar& fg_color, int type, bool bDebug);
};