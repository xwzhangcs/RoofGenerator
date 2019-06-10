#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/opencv.hpp>
#include <vector>
#include "Config.h"

class RoofA {

protected:
	RoofA() {}

public:
	static cv::Mat generateRoof(int width, int height, const Config& roof_paras, const cv::Scalar& bg_color, const cv::Scalar& fg_color);
	//static int generateRoofImages(std::string roofImagesPath, int imageNum, int start_index, int type, int width, int height, std::pair<double, double> center_x_ratio, std::pair<double, double> center_y_ratio, std::pair<double, double> roofWidth, std::pair<double, double> roofAspect, int selected_roof_type, std::pair<double, double> ridgeRatio, const cv::Scalar& bg_color, const cv::Scalar& fg_color);

};