#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "roofA.h"
#include "roofB.h"
#include "roofC.h"
#include "roofD.h"
#include "roofTypes.h"
#include "RoofUtils.h"
#include "Utils.h"
#include "drawRotatedRect.h"

void test_one_nodes(int width, int height, int step_size);
void test_two_nodes(int width, int height, int step_size, int type);
void test_three_nodes(int width, int height, int step_size, int type);
void test_four_nodes(int width, int height, int step_size, int type);

int main(int argc, char** argv)
{
	//test_one_nodes(56, 56, 8);
	test_two_nodes(64, 64, 8, 1);
	//test_three_nodes(56, 56, 8, 2);
	/*std::cout << utils::rectControlRect(16, 32, 22, 13, 0, 24, 20, 26, 10, 0) << std::endl;
	std::cout << utils::rectIntersecRect(16, 32, 22, 13, 0, 24, 20, 26, 10, 0) << std::endl;
	std::cout << utils::rectIntersecRect(24, 20, 26, 10, 0, 16, 32, 22, 13, 0) << std::endl;*/
	system("pause");

	return 0;
}

// reduce the resolution to 56 * 56 
void test_one_nodes(int width, int height, int step_size){
	int index = 0;
	int padding_size = 8;
	double center_x_ratio = 0.5;
	double center_y_ratio = 0.5;
	double roof_w_ratio = width * 1.0 / (width + padding_size);
	double step_aspect_ratio = step_size * 1.0 / (roof_w_ratio * (width + padding_size));
	for (double aspect_ratio = 0.25; aspect_ratio <= 4; aspect_ratio += step_aspect_ratio){
		// height control
		if (aspect_ratio * roof_w_ratio > height * 1.0 / (height + padding_size))
			continue;
		Config roof_flat_paras(center_x_ratio, center_y_ratio, roof_w_ratio, aspect_ratio, 0, RoofTypes::FLAT, false, 0.0, 1.0);
		cv::Mat roof_img = RoofA::generateRoof(width + padding_size, height + padding_size, roof_flat_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
		if (!roof_img.empty()){
			char buffer[50];
			sprintf(buffer, "roof_image_%06d.png", index);
			std::string img_filename = "../data/node_one/" + std::string(buffer);
			std::cout << img_filename << std::endl;
			cv::imwrite(img_filename, roof_img);
			index++;
		}
	}
}

// just use integer
void test_two_nodes(int width, int height, int step_size, int type){
	std::vector<cv::Scalar> fg_color_set; // bgr
	fg_color_set.push_back(cv::Scalar(0, 0, 255)); // red
	fg_color_set.push_back(cv::Scalar(0, 255, 0)); // green
	fg_color_set.push_back(cv::Scalar(0, 255, 255)); // yellow
	fg_color_set.push_back(cv::Scalar(0, 165, 255)); // orange
	fg_color_set.push_back(cv::Scalar(128, 128, 128)); // grey
	fg_color_set.push_back(cv::Scalar(255, 255, 0)); // cyan
	fg_color_set.push_back(cv::Scalar(255, 0, 0)); // blue
	int index = 0;
	int padding_size = 8;
	int center_x = width * 0.5;
	int roof_w = width * 1.0;
	int roof_min_size = 2 * step_size;
	for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
		for (int center_y = 0; center_y <= 0.5 * height; center_y += step_size){
			// first check out of boundary
			if (!utils::rectInsideRect(width, height, center_x, center_y, roof_w, roof_h, 0)){
				continue;
			}
			// add the second rectangle
			for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
				for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
					for (int center_x_v1 = 0; center_x_v1 <= 0.5 * width; center_x_v1 += step_size){
						for (int center_y_v1 = 0; center_y_v1 <= height; center_y_v1 += step_size){
							// first check out of boundary
							if (!utils::rectInsideRect(width, height, center_x_v1, center_y_v1, roof_w_v1, roof_h_v1, 0)){
								continue;
							}
							int top_y_v1 = center_y_v1 - 0.5 * roof_h_v1;
							int bot_y_v1 = center_y_v1 + 0.5 * roof_h_v1;
							int top_y = center_y - 0.5 * roof_h;
							int bot_y = center_y + 0.5 * roof_h;
							if ((top_y_v1 > top_y && top_y_v1 < bot_y) || (bot_y_v1 > top_y && bot_y_v1 < bot_y))
								continue;
							if (top_y_v1 > bot_y || bot_y_v1 < top_y)
								continue;
							// make sure equal
							int top_dis = top_y < top_y_v1 ? top_y : top_y_v1;
							int bot_dis = bot_y < bot_y_v1 ? height - bot_y_v1 : height - bot_y;
							if (top_dis == bot_dis){
								cv::Mat roof_img(height + padding_size, width + padding_size, CV_8UC3, cv::Scalar(0, 0, 0));
								DrawRotatedRect::generateRect(roof_img, center_x + 0.5 * padding_size, center_y + 0.5 * padding_size, roof_w, roof_h, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[0]);
								DrawRotatedRect::generateRect(roof_img, center_x_v1 + 0.5 * padding_size, center_y_v1 + 0.5 * padding_size, roof_w_v1, roof_h_v1, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[1]);
								char buffer[50];
								sprintf(buffer, "roof_image_%08d.png", index);
								std::string img_filename = "../data/node_two/" + std::string(buffer);
								std::cout << img_filename << std::endl;
								cv::imwrite(img_filename, roof_img);
								if (index == 40 || index == 56 || index == 0 || index == 7){
									std::cout << center_x << ", " << center_y << ", " << roof_w << ", " << roof_h << std::endl;
									std::cout << center_x_v1 << ", " << center_y_v1 << ", " << roof_w_v1 << ", " << roof_h_v1 << std::endl;
								}
								index++;
							}
						}
					}
				}
			}
		}
	}
}

void test_three_nodes(int width, int height, int step_size, int type){
	
}


