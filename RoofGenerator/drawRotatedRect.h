#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/opencv.hpp>
#include <vector>

class DrawRotatedRect {

protected:
	DrawRotatedRect() {}

public:
	static void generateRect(cv::Mat & roof_img, int padding, int center_x, int center_y, int roofWidth, int roofHeight, double rotate, int selected_roof_type, const cv::Scalar& bg_color, const cv::Scalar& fg_color);
	static void generateRect(cv::Mat & roof_img, int padding, int center_x, int center_y, int roofWidth, int roofHeight, double rotate, int selected_roof_type, bool bRidgeDis, double ridgeDisRatio, int ridgeLength, const cv::Scalar& bg_color, const cv::Scalar& fg_color);

};