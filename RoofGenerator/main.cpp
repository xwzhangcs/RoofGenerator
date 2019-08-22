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

void test_one_nodes(int width, int height, int step_size, int padding);
void test_two_nodes(int width, int height, int step_size, int type, int padding);

void print_vector(std::vector<int> v){
	for (int i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	int padding = 5;
	//test_one_nodes(64, 64, 8, padding);
	test_two_nodes(64, 64, 8, 1, padding);
	system("pause");

	return 0;
}

// reduce the resolution to 56 * 56 
void test_one_nodes(int width, int height, int step_size, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(255, 255, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += 0.5 * step_size){
				for (int top_h = 0; top_h < height; top_h += 0.5 * step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					int dis_left = top_w;
					int dis_right = width - top_w - roof_w;
					int dis_top = top_h;
					int dis_bot = height - top_h - roof_h;
					// condition 1
					if (dis_left != dis_right || dis_top != dis_bot)
						continue;
					// condition 2
					if (dis_left * dis_top != 0)
						continue;
					// condition 3
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					cv::Mat roof_img(height, width, CV_8UC3, bg_color);
					DrawRotatedRect::generateRect(roof_img, true, padding, center_w, center_h, roof_w, roof_h, 0, RoofTypes::HIP, false, 0, roof_w > roof_h? roof_w * 0.8: roof_h * 0.8, bg_color, fg_color);
					if (!roof_img.empty()){
						char buffer[50];
						sprintf(buffer, "roof_image_%08d.png", index);
						std::string img_filename = "../data/node_one/" + std::string(buffer);
						std::cout << img_filename << std::endl;
						cv::imwrite(img_filename, roof_img);
						index++;
					}
				}
			}
		}
	}
}

void test_two_nodes(int width, int height, int step_size, int type, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(255, 255, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(2);
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += 0.5 * step_size){
				for (int top_h = 0; top_h < height; top_h += 0.5 * step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					// check 
					// second rectangle
					for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
						for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += 0.5 * step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += 0.5 * step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1))
										continue;
									// condition 1
									if (center_w == center_w_v1 && center_h == center_h_v1 && roof_w == roof_w_v1 && roof_h == roof_h_v1)
										continue;
									// touch
									int bot_h = top_h + roof_h;
									int bot_w = top_w + roof_w;
									int bot_h_v1 = top_h_v1 + roof_h_v1;
									int bot_w_v1 = top_w_v1 + roof_w_v1;
									bool bTouch = utils::relation_L(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
									if (!bTouch)
										continue;
									int dis_left = top_w < top_w_v1 ? top_w : top_w_v1;
									int dis_right = (top_w + roof_w) >(top_w_v1 + roof_w_v1) ? (width - top_w - roof_w) : (width - top_w_v1 - roof_w_v1);
									int dis_top = top_h < top_h_v1 ? top_h : top_h_v1;;
									int dis_bot = (top_h + roof_h) >(top_h_v1 + roof_h_v1) ? (height - top_h - roof_h) : (height - top_h_v1 - roof_h_v1);
									// condition 2
									if (dis_left != dis_right || dis_top != dis_bot)
										continue;
									// condition 3
									if (dis_left * dis_top != 0)
										continue;
									//
									// add first 
									roof_paras[0].clear();
									roof_paras[0].push_back(center_w);
									roof_paras[0].push_back(center_h);
									roof_paras[0].push_back(roof_w);
									roof_paras[0].push_back(roof_h);
									roof_paras[0].push_back(0);
									roof_paras[0].push_back(roof_w > roof_h ? roof_w : roof_h);
									// add second
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									roof_paras[1].push_back(roof_w_v1 > roof_h_v1 ? roof_w_v1 : roof_h_v1);
									cv::Mat roof_img(height, width, CV_8UC3, bg_color);
									DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofRelationship::L, RoofTypes::GABLE, bg_color, fg_color);
									if (!roof_img.empty()){
										char buffer[50];
										sprintf(buffer, "roof_image_%08d.png", index);
										std::string img_filename = "../data/two_edges/" + std::string(buffer);
										std::cout << img_filename << std::endl;
										cv::imwrite(img_filename, roof_img);
										index++;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}