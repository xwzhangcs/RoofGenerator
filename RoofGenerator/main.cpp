#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "Utils.h"
#include "generator.h"

int main(int argc, char** argv)
{
	/*cv::Mat image = cv::imread("../data/roof_image_00000023.png", CV_LOAD_IMAGE_UNCHANGED);
	cv::Mat src_gray;
	/// Convert image to gray and blur it
	if (image.channels() != 1)
		cv::cvtColor(image, src_gray, CV_BGR2GRAY);
	else
		src_gray = image.clone();
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours(src_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	/// Draw contours
	cv::Mat drawing = cv::Mat::zeros(src_gray.size(), CV_8UC3);
	cv::RNG rng(12345);
	for (int i = 0; i< contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
	}
	float threshold = 0.50;
	int kernel = 2;
	cv::Mat dst = image.clone();
	std::cout << "contours size is " << contours[0].size() << std::endl;
	for (int i = 0; i < contours[0].size(); i++){
		if (utils::genRand(0.0, 1.0) > threshold){
			cv::Point p1(contours[0][i].x - kernel, contours[0][i].y - kernel);
			cv::Point p2(contours[0][i].x + kernel, contours[0][i].y + kernel);
			cv::rectangle(dst, p1, p2,  cv::Scalar(255, 255, 255), -1);
		}
		else{
			cv::Point p1(contours[0][i].x - kernel, contours[0][i].y - kernel);
			cv::Point p2(contours[0][i].x + kernel, contours[0][i].y + kernel);
			cv::rectangle(dst, p1, p2, cv::Scalar(0, 0, 0), -1);
		}
	}
	cv::imwrite("../data/roof_test_noise.png", dst);
	system("pause");
	return 0;*/
	int padding = 12;
	int type = 0;
	int total = 14;
	int start_index = 0;
	start_index = Generator::generate_one_edge(start_index, 0, total, 200, 200, 20, padding, "../data/roof_data");
	//start_index = Generator::generate_two_edges(start_index, 1, total, 200, 200, 2, padding, "../data/roof_data");
	//Generator::generate_three_edges_v1(200, 200, 20, padding, "../data/three_edges_v1");
	//Generator::generate_three_edges_v2(200, 200, 20, padding, "../data/three_edges_v2");
	//Generator::generate_four_edges_v2(200, 200, 20, padding, "../data/four_edges_v2");
	//Generator::generate_four_edges_v1(200, 200, 20, padding, "../data/four_edges_v1");
	//Generator::generate_four_edges_v3(200, 200, 20, padding, "../data/four_edges_v3");
	//Generator::generate_four_edges_v4(200, 200, 20, padding, "../data/four_edges_v4");
	//Generator::generate_five_edges_v1(200, 200, 20, padding, "../data/five_edges_v1");
	//Generator::generate_five_edges_v2(200, 200, 20, padding, "../data/five_edges_v2");
	//Generator::generate_five_edges_v3(200, 200, 20, padding, "../data/five_edges_v3");
	//Generator::generate_five_edges_v4(200, 200, 20, padding, "../data/five_edges_v4");
	//Generator::generate_five_edges_v5(200, 200, 20, padding, "../data/five_edges_v5");
	//Generator::generate_five_edges_v6(200, 200, 20, padding, "../data/five_edges_v6");
	system("pause");

	return 0;
}