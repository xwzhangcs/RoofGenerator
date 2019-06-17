#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "roofA.h"
#include "roofB.h"
#include "roofC.h"
#include "roofD.h"
#include "roofTypes.h"
#include "RoofUtils.h"
#include "Utils.h"

void test_one_nodes(int width, int height, int step_size);
void test_two_nodes(int width, int height, int step_size, int type);
void test_three_nodes(int width, int height, int step_size, int type);
void test_four_nodes(int width, int height, int step_size, int type);

int main(int argc, char** argv)
{
	//test_one_nodes(56, 56, 8);
	test_two_nodes(56, 56, 8, 1);
	//test_two_nodes(56, 56, 1);
	//test_three_nodes(56, 56, 2);
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

void test_two_nodes(int width, int height, int step_size, int type){
	int index = 0;
	int padding_size = 8;
	double center_x_ratio = 0.5;
	double center_y_ratio = 0.5;
	double roof_w_ratio = width * 1.0 / (width + padding_size);
	double roof_start_ratio = (0.2 * width) / (width + padding_size);
	double step_aspect_ratio = step_size * 1.0 / (roof_w_ratio * (width + padding_size));
	for (double aspect_ratio = 0.25; aspect_ratio <= 4; aspect_ratio += step_aspect_ratio){
		// height control
		if (aspect_ratio * roof_w_ratio > height * 1.0 / (height + padding_size))
			continue;
		Config roof_flat_paras(center_x_ratio, center_y_ratio, roof_w_ratio, aspect_ratio, 0, RoofTypes::FLAT, false, 0.0, 1.0);
		cv::Mat roof_img_test = RoofA::generateRoof(width + padding_size, height + padding_size, roof_flat_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
		if (roof_img_test.empty()){
			continue;
		}
		// add the second rectangle
		double step_ratio = step_size * 1.0 / (width + padding_size);
		// cross type
		for (double dis_ratio = 0.0; dis_ratio <= 0.5; dis_ratio += step_ratio){
			for (double roof_w_ratio_v1 = roof_start_ratio; roof_w_ratio_v1 < 1.0; roof_w_ratio_v1 += step_ratio){
				double step_aspect_ratio_v1 = step_size * 1.0 / (roof_w_ratio_v1 * (width + padding_size));
				for (double aspect_ratio_v1 = 0.25; aspect_ratio_v1 < 4; aspect_ratio_v1 += step_aspect_ratio_v1){
					if (aspect_ratio_v1 * roof_w_ratio_v1 > height * 1.0 / (height + padding_size) || aspect_ratio_v1 * roof_w_ratio_v1 < roof_start_ratio)
						continue;
					if (aspect_ratio_v1 * roof_w_ratio_v1 - aspect_ratio * roof_w_ratio < 2 * step_ratio)
						continue;
					double center_x_ratio_v1 = center_x_ratio - 0.5 * roof_w_ratio + dis_ratio + 0.5 * roof_w_ratio_v1;
					//double center_y_ratio_v1 = center_y_ratio + 0.5 * (roof_w_ratio * aspect_ratio) + 0.5 * (roof_w_ratio_v1 * aspect_ratio_v1);
					double center_y_ratio_v1 = center_y_ratio;
					Config roof_flat_paras_v1(center_x_ratio_v1, center_y_ratio_v1, roof_w_ratio_v1, aspect_ratio_v1, 0, RoofTypes::FLAT, false, 0.0, 1.0);
					cv::Mat roof_img_test_v1 = RoofA::generateRoof(width + padding_size, height + padding_size, roof_flat_paras_v1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
					if (roof_img_test_v1.empty()){
						continue;
					}
					std::vector<Config> roof_paras;
					roof_paras.push_back(roof_flat_paras);
					roof_paras.push_back(roof_flat_paras_v1);
					cv::Mat roof_img = RoofB::generateRoof(width + padding_size, height + padding_size, roof_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255), type, false);
					if (!roof_img.empty()){
						char buffer[50];
						sprintf(buffer, "roof_image_%08d.png", index);
						std::string img_filename = "../data/node_two/" + std::string(buffer);
						std::cout << img_filename << std::endl;
						cv::imwrite(img_filename, roof_img);
						index++;
					}
				}
			}
		}

		// T or L shape
		for (double dis_ratio = 0.0; dis_ratio <= 0.5; dis_ratio += step_ratio){
			for (double roof_w_ratio_v1 = roof_start_ratio; roof_w_ratio_v1 < 1.0; roof_w_ratio_v1 += step_ratio){
				double step_aspect_ratio_v1 = step_size * 1.0 / (roof_w_ratio_v1 * (width + padding_size));
				for (double aspect_ratio_v1 = 0.25; aspect_ratio_v1 < 4; aspect_ratio_v1 += step_aspect_ratio_v1){
					if (aspect_ratio_v1 * roof_w_ratio_v1 > height * 1.0 / (height + padding_size) || aspect_ratio_v1 * roof_w_ratio_v1 < roof_start_ratio)
						continue;
					double center_x_ratio_v1 = center_x_ratio - 0.5 * roof_w_ratio + dis_ratio + 0.5 * roof_w_ratio_v1;
					double center_y_ratio_v1 = center_y_ratio + 0.5 * (roof_w_ratio * aspect_ratio) + 0.5 * (roof_w_ratio_v1 * aspect_ratio_v1);
					Config roof_flat_paras_v1(center_x_ratio_v1, center_y_ratio_v1, roof_w_ratio_v1, aspect_ratio_v1, 0, RoofTypes::FLAT, false, 0.0, 1.0);
					cv::Mat roof_img_test_v1 = RoofA::generateRoof(width + padding_size, height + padding_size, roof_flat_paras_v1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
					if (roof_img_test_v1.empty()){
						continue;
					}
					std::vector<Config> roof_paras;
					roof_paras.push_back(roof_flat_paras);
					roof_paras.push_back(roof_flat_paras_v1);
					cv::Mat roof_img = RoofB::generateRoof(width + padding_size, height + padding_size, roof_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255), type, false);
					if (!roof_img.empty()){
						char buffer[50];
						sprintf(buffer, "roof_image_%08d.png", index);
						std::string img_filename = "../data/node_two/" + std::string(buffer);
						std::cout << img_filename << std::endl;
						cv::imwrite(img_filename, roof_img);
						index++;
					}
				}
			}
		}
	}
}


