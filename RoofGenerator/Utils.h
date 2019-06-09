#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace utils {

	double genRand();
	double genRand(double a, double b);
	float gause(float u, float sigma);
	float stddev(std::vector<float> list);
	float mean(std::vector<float> list);

	void grayScale(const cv::Mat& img, cv::Mat& grayImg);
	void scaleToFit(const cv::Mat& src, cv::Mat& dst, const cv::Size& size);
	void distanceMap(const cv::Mat& img, cv::Mat& distMap);

	void output_vector(const std::vector<float>& values);
	cv::Point2f RotatePoint(const cv::Point2f& p, float rad);
	cv::Point2f RotatePoint(const cv::Point2f& cen_pt, const cv::Point2f& p, float rad);
	bool lineLineIntersection(const cv::Point2f& a, const cv::Point2f& b, const cv::Point2f& c, const cv::Point2f& d, double *tab, double *tcd, bool segment_only, cv::Point2f& int_pt);

}