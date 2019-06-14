#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/opencv.hpp>
#include <vector>
#include "Config.h"

class RoofB {

protected:
	RoofB() {}

public:
	static cv::Mat generateRoof(int width, int height, const Config& roof_paras_main, const Config& roof_paras_sub, const cv::Scalar& bg_color, const cv::Scalar& fg_color, int type, bool bDebug);
};