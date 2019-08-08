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
void test_three_nodes_v2(int width, int height, int step_size, int type);

int main(int argc, char** argv)
{
	int padding = 5;
	Config roof_flat_paras(0.5, 0.5, 1.0, 0.5, 0, RoofTypes::FLAT, false, 0.0, 1.0);
	cv::Mat roof_img = RoofA::generateRoof(224, 224, roof_flat_paras, padding, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 255));
	if (!roof_img.empty())
	cv::imwrite("../data/roofA_flat.png", roof_img);
	//test_one_nodes(64, 64, 8);
	//test_two_nodes(64, 64, 8, 1);
	//test_three_nodes_v2(64, 64, 8, 1);
	/*std::cout << utils::rectControlRect(16, 32, 22, 13, 0, 24, 20, 26, 10, 0) << std::endl;
	std::cout << utils::rectIntersecRect(16, 32, 22, 13, 0, 24, 20, 26, 10, 0) << std::endl;
	std::cout << utils::rectIntersecRect(24, 20, 26, 10, 0, 16, 32, 22, 13, 0) << std::endl;*/
	system("pause");

	return 0;
}

// reduce the resolution to 56 * 56 
void test_one_nodes(int width, int height, int step_size){
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
	int roof_w = width * 1.0;
	int roof_min_size = 2 * step_size;
	int top_w = 0;
	for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
		for (int top_h = 0; top_h < 0.5 * height; top_h += 0.5 * step_size){
			// first check out of boundary
			if (!utils::rectInsideRect(width, height, top_w + 0.5 * roof_w, top_h + 0.5 * roof_h, roof_w, roof_h, 0)){
				continue;
			}
			// symmetry
			if (top_h != height - top_h - roof_h)
				continue;
			{
				std::cout << "top_w is " << top_w<<", top_h is " << top_h << std::endl;
				std::cout << "roof_w is " << roof_w << ", roof_h is " << roof_h << std::endl;
			}
			cv::Mat roof_img(height + padding_size, width + padding_size, CV_8UC3, cv::Scalar(0, 0, 0));
			DrawRotatedRect::generateRect(roof_img, 5, top_w + 0.5 * roof_w + 0.5 * padding_size, top_h + 0.5 * roof_h + 0.5 * padding_size, roof_w, roof_h, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[0]);
			char buffer[50];
			sprintf(buffer, "roof_image_%08d.png", index);
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
	int roof_w = width * 1.0;
	int roof_min_size = 2 * step_size;
	int top_w = 0;
	for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
		for (int top_h = 0; top_h < 0.5 * height; top_h += step_size){
			// first check out of boundary
			if (!utils::rectInsideRect(width, height, top_w + 0.5 * roof_w, top_h + 0.5 * roof_h, roof_w, roof_h, 0)){
				continue;
			}
			// symmetry
			if (top_h + 0.5 * roof_h > 0.5 * height)
				continue;
			// add the second rectangle
			for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
				for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
					for (int top_w_v1 = 0; top_w_v1 < 0.5 * width; top_w_v1 += step_size){
						for (int top_h_v1 = 0; top_h_v1 <= height; top_h_v1 += step_size){
							// first check out of boundary
							if (!utils::rectInsideRect(width, height, top_w_v1 + 0.5 * roof_w_v1, top_h_v1 + 0.5 * roof_h_v1, roof_w_v1, roof_h_v1, 0)){
								continue;
							}
							if (top_w_v1 + 0.5 * roof_w_v1 > 0.5 * width)
								continue;
							int bot_h_v1 = top_h_v1 + roof_h_v1;
							int bot_h = top_h + roof_h;
							if ((top_h_v1 > top_h && top_h_v1 < bot_h) || (bot_h_v1 > top_h && bot_h_v1 < bot_h))
								continue;
							if (top_h_v1 > bot_h || bot_h_v1 < top_h)
								continue;
							// make sure equal
							int top_dis = top_h < top_h_v1 ? top_h : top_h_v1;
							int bot_dis = bot_h < bot_h_v1 ? height - bot_h_v1 : height - bot_h;
							if (top_dis == bot_dis){
								cv::Mat roof_img(height + padding_size, width + padding_size, CV_8UC3, cv::Scalar(0, 0, 0));
								DrawRotatedRect::generateRect(roof_img, 5, top_w + 0.5 * roof_w + 0.5 * padding_size, top_h + 0.5 * roof_h + 0.5 * padding_size, roof_w, roof_h, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[0]);
								DrawRotatedRect::generateRect(roof_img, 5, top_w_v1 + 0.5 * roof_w_v1 + 0.5 * padding_size, top_h_v1 + 0.5 * roof_h_v1 + 0.5 * padding_size, roof_w_v1, roof_h_v1, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[1]);
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
	}
}

void test_three_nodes(int width, int height, int step_size, int type){
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
	int roof_w = width * 1.0;
	int roof_min_size = 2 * step_size;
	int top_w = 0;
	for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
		for (int top_h = 0; top_h < 0.5 * height; top_h += step_size){
			// first check out of boundary
			if (!utils::rectInsideRect(width, height, top_w + 0.5 * roof_w, top_h + 0.5 * roof_h, roof_w, roof_h, 0)){
				continue;
			}
			// symmetry
			if (top_h + 0.5 * roof_h > 0.5 * height)
				continue;
			// add the second rectangle
			for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
				for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
					for (int top_w_v1 = 0; top_w_v1 < 0.5 * width; top_w_v1 += step_size){
						for (int top_h_v1 = 0; top_h_v1 <= height; top_h_v1 += step_size){
							// first check out of boundary
							if (!utils::rectInsideRect(width, height, top_w_v1 + 0.5 * roof_w_v1, top_h_v1 + 0.5 * roof_h_v1, roof_w_v1, roof_h_v1, 0)){
								continue;
							}
							if (top_w_v1 + 0.5 * roof_w_v1 > 0.5 * width)
								continue;
							int bot_h_v1 = top_h_v1 + roof_h_v1;
							int bot_h = top_h + roof_h;
							if ((top_h_v1 > top_h && top_h_v1 < bot_h) || (bot_h_v1 > top_h && bot_h_v1 < bot_h))
								continue;
							if (top_h_v1 > bot_h || bot_h_v1 < top_h)
								continue;	

							// add the third rectangle
							for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
								for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
									for (int top_w_v2 = width; top_w_v2 > 0.5 * width; top_w_v2 -= step_size){
										for (int top_h_v2 = 0; top_h_v2 <= height; top_h_v2 += step_size){
											// first check out of boundary
											if (!utils::rectInsideRect(width, height, top_w_v2 - 0.5 * roof_w_v2, top_h_v2 + 0.5 * roof_h_v2, roof_w_v2, roof_h_v2, 0)){
												continue;
											}
											if (top_w_v2 - 0.5 * roof_w_v2 < 0.5 * width)
												continue;
											int bot_h_v2 = top_h_v2 + roof_h_v2;
											if ((top_h_v2 > top_h && top_h_v2 < bot_h) || (bot_h_v2 > top_h && bot_h_v2 < bot_h))
												continue;
											if (top_h_v2 > bot_h || bot_h_v2 < top_h)
												continue;
											// make sure v1 and v2 do not insect
											if (utils::rectIntersecRect(top_w_v1 + 0.5 * roof_w_v1, top_h_v1 + 0.5 * roof_h_v1, roof_w_v1, roof_h_v1, 0, top_w_v2 - 0.5 * roof_w_v2, top_h_v2 + 0.5 * roof_h_v2, roof_w_v2, roof_h_v2, 0))
												continue;
											if (utils::rectIntersecRect(top_w_v2 - 0.5 * roof_w_v2, top_h_v2 + 0.5 * roof_h_v2, roof_w_v2, roof_h_v2, 0, top_w_v1 + 0.5 * roof_w_v1, top_h_v1 + 0.5 * roof_h_v1, roof_w_v1, roof_h_v1, 0))
												continue;

											// identical
											if (roof_w_v1 != roof_w_v2 || roof_h_v1 != roof_h_v2)
												continue;

											// make sure equal
											std::vector<int> top_h_values;
											top_h_values.push_back(top_h);
											top_h_values.push_back(top_h_v1);
											top_h_values.push_back(top_h_v2);

											std::vector<int> bot_h_values;
											bot_h_values.push_back(bot_h);
											bot_h_values.push_back(bot_h_v1);
											bot_h_values.push_back(bot_h_v2);

											int top_dis = *std::min_element(top_h_values.begin(), top_h_values.end());
											//std::cout << top_dis << std::endl;
											int bot_dis = height - *std::max_element(bot_h_values.begin(), bot_h_values.end());
											//std::cout << bot_dis << std::endl;
											if (top_dis == bot_dis){
												cv::Mat roof_img(height + padding_size, width + padding_size, CV_8UC3, cv::Scalar(0, 0, 0));
												DrawRotatedRect::generateRect(roof_img, 5, top_w + 0.5 * roof_w + 0.5 * padding_size, top_h + 0.5 * roof_h + 0.5 * padding_size, roof_w, roof_h, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[0]);
												DrawRotatedRect::generateRect(roof_img, 5, top_w_v1 + 0.5 * roof_w_v1 + 0.5 * padding_size, top_h_v1 + 0.5 * roof_h_v1 + 0.5 * padding_size, roof_w_v1, roof_h_v1, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[1]);
												DrawRotatedRect::generateRect(roof_img, 5, top_w_v2 - 0.5 * roof_w_v2 + 0.5 * padding_size, top_h_v2 + 0.5 * roof_h_v2 + 0.5 * padding_size, roof_w_v2, roof_h_v2, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[2]);

												char buffer[50];
												sprintf(buffer, "roof_image_%08d.png", index);
												std::string img_filename = "../data/node_three/" + std::string(buffer);
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
	}
}

void test_three_nodes_v2(int width, int height, int step_size, int type){
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
	int roof_w = width * 1.0;
	int roof_min_size = 2 * step_size;
	int top_w = 0;
	for (int roof_w = roof_min_size; roof_w < width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_h = 0; top_h < 0.5 * height; top_h += step_size){
				top_w = (width - roof_w) * 0.5;
				// first check out of boundary
				if (!utils::rectInsideRect(width, height, top_w + 0.5 * roof_w, top_h + 0.5 * roof_h, roof_w, roof_h, 0)){
					continue;
				}
				// symmetry
				if (top_h + 0.5 * roof_h > 0.5 * height)
					continue;
				// add the second rectangle
				for (int roof_w_v1 = roof_min_size; roof_w_v1 < width; roof_w_v1 += step_size){
					for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
						for (int top_w_v1 = top_w; top_w_v1 < 0.5 * width; top_w_v1 += step_size){
							for (int top_h_v1 = 0; top_h_v1 <= height; top_h_v1 += step_size){
								// first check out of boundary
								if (!utils::rectInsideRect(width, height, top_w_v1 + 0.5 * roof_w_v1, top_h_v1 + 0.5 * roof_h_v1, roof_w_v1, roof_h_v1, 0)){
									continue;
								}
								if (top_w_v1 + 0.5 * roof_w_v1 > 0.5 * width)
									continue;
								int bot_h_v1 = top_h_v1 + roof_h_v1;
								int bot_h = top_h + roof_h;
								if ((top_h_v1 > top_h && top_h_v1 < bot_h) || (bot_h_v1 > top_h && bot_h_v1 < bot_h))
									continue;
								if (top_h_v1 > bot_h || bot_h_v1 < top_h)
									continue;

								// add the third rectangle
								for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
									for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
										for (int top_w_v2 = top_w + roof_w; top_w_v2 > 0.5 * width; top_w_v2 -= step_size){
											for (int top_h_v2 = 0; top_h_v2 <= height; top_h_v2 += step_size){
												// first check out of boundary
												if (!utils::rectInsideRect(width, height, top_w_v2 - 0.5 * roof_w_v2, top_h_v2 + 0.5 * roof_h_v2, roof_w_v2, roof_h_v2, 0)){
													continue;
												}
												if (top_w_v2 - 0.5 * roof_w_v2 < 0.5 * width)
													continue;
												int bot_h_v2 = top_h_v2 + roof_h_v2;
												if ((top_h_v2 > top_h && top_h_v2 < bot_h) || (bot_h_v2 > top_h && bot_h_v2 < bot_h))
													continue;
												if (top_h_v2 > bot_h || bot_h_v2 < top_h)
													continue;
												// make sure v1 and v2 do not insect
												if (utils::rectIntersecRect(top_w_v1 + 0.5 * roof_w_v1, top_h_v1 + 0.5 * roof_h_v1, roof_w_v1, roof_h_v1, 0, top_w_v2 - 0.5 * roof_w_v2, top_h_v2 + 0.5 * roof_h_v2, roof_w_v2, roof_h_v2, 0))
													continue;
												if (utils::rectIntersecRect(top_w_v2 - 0.5 * roof_w_v2, top_h_v2 + 0.5 * roof_h_v2, roof_w_v2, roof_h_v2, 0, top_w_v1 + 0.5 * roof_w_v1, top_h_v1 + 0.5 * roof_h_v1, roof_w_v1, roof_h_v1, 0))
													continue;

												// identical
												if (roof_w_v1 != roof_w_v2 || roof_h_v1 != roof_h_v2)
													continue;
												// make sure equal
												std::vector<int> top_h_values;
												top_h_values.push_back(top_h);
												top_h_values.push_back(top_h_v1);
												top_h_values.push_back(top_h_v2);

												std::vector<int> bot_h_values;
												bot_h_values.push_back(bot_h);
												bot_h_values.push_back(bot_h_v1);
												bot_h_values.push_back(bot_h_v2);

												int top_dis = *std::min_element(top_h_values.begin(), top_h_values.end());
												//std::cout << top_dis << std::endl;
												int bot_dis = height - *std::max_element(bot_h_values.begin(), bot_h_values.end());
												//std::cout << bot_dis << std::endl;
												if (top_dis == 0 && bot_dis == 0){
													cv::Mat roof_img(height + padding_size, width + padding_size, CV_8UC3, cv::Scalar(0, 0, 0));
													DrawRotatedRect::generateRect(roof_img, 5, top_w + 0.5 * roof_w + 0.5 * padding_size, top_h + 0.5 * roof_h + 0.5 * padding_size, roof_w, roof_h, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[0]);
													DrawRotatedRect::generateRect(roof_img, 5, top_w_v1 + 0.5 * roof_w_v1 + 0.5 * padding_size, top_h_v1 + 0.5 * roof_h_v1 + 0.5 * padding_size, roof_w_v1, roof_h_v1, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[1]);
													DrawRotatedRect::generateRect(roof_img, 5, top_w_v2 - 0.5 * roof_w_v2 + 0.5 * padding_size, top_h_v2 + 0.5 * roof_h_v2 + 0.5 * padding_size, roof_w_v2, roof_h_v2, 0, RoofTypes::FLAT, cv::Scalar(0, 0, 0), fg_color_set[2]);

													char buffer[50];
													sprintf(buffer, "roof_image_%08d.png", index);
													std::string img_filename = "../data/node_three_v2/" + std::string(buffer);
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
		}
	}
}


