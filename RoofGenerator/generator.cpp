#include "generator.h"
#include "drawRotatedRect.h"
#include "Utils.h"
#include "roofTypes.h"
#include "drawRotatedRect.h"

void Generator::generate_one_edge(int width, int height, int step_size, int padding, std::string output_path){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(255, 255, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(1);
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
					roof_paras[0].clear();
					roof_paras[0].push_back(center_w);
					roof_paras[0].push_back(center_h);
					roof_paras[0].push_back(roof_w);
					roof_paras[0].push_back(roof_h);
					roof_paras[0].push_back(0);
					cv::Mat roof_img(height, width, CV_8UC3, bg_color);
					DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
					if (!roof_img.empty()){
						char buffer[50];
						sprintf(buffer, "roof_image_%08d.png", index);
						std::string img_filename = output_path + "/" + std::string(buffer);
						std::cout << img_filename << std::endl;
						cv::imwrite(img_filename, roof_img);
						index++;
					}
				}
			}
		}
	}
}

void Generator::generate_two_edges(int width, int height, int step_size, int padding, std::string output_path){
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
					// add first 
					roof_paras[0].clear();
					roof_paras[0].push_back(center_w);
					roof_paras[0].push_back(center_h);
					roof_paras[0].push_back(roof_w);
					roof_paras[0].push_back(roof_h);
					roof_paras[0].push_back(0);
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
									// add second
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									cv::Mat roof_img(height, width, CV_8UC3, bg_color);
									DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
									if (!roof_img.empty()){
										char buffer[50];
										sprintf(buffer, "roof_image_%08d.png", index);
										std::string img_filename = output_path + "/" + std::string(buffer);
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

void Generator::generate_three_edges_v1(int width, int height, int step_size, int padding, std::string output_path){
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
					// add first 
					roof_paras[0].clear();
					roof_paras[0].push_back(center_w);
					roof_paras[0].push_back(center_h);
					roof_paras[0].push_back(roof_w);
					roof_paras[0].push_back(roof_h);
					roof_paras[0].push_back(0);
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
									bool bTouch = utils::relation_T(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
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
									// add second
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									cv::Mat roof_img(height, width, CV_8UC3, bg_color);
									DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
									if (!roof_img.empty()){
										char buffer[50];
										sprintf(buffer, "roof_image_%08d.png", index);
										std::string img_filename = output_path + "/" + std::string(buffer);
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

void Generator::generate_three_edges_v2(int width, int height, int step_size, int padding, std::string output_path){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(3);
	std::vector<int> imageRoofsWidth;
	imageRoofsWidth.resize(roof_paras.size());
	std::vector<int> imageRoofsHeight;
	imageRoofsHeight.resize(roof_paras.size());
	std::vector<int> centers_w;
	centers_w.resize(roof_paras.size());
	std::vector<int> centers_h;
	centers_h.resize(roof_paras.size());
	std::vector<double> rotates;
	rotates.resize(roof_paras.size());
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += 0.5 * step_size){
				for (int top_h = 0; top_h < height; top_h += 0.5 * step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					roof_paras[0].clear();
					roof_paras[0].push_back(center_w);
					roof_paras[0].push_back(center_h);
					roof_paras[0].push_back(roof_w);
					roof_paras[0].push_back(roof_h);
					roof_paras[0].push_back(0);
					imageRoofsWidth[0] = roof_w;
					imageRoofsHeight[0] = roof_h;
					centers_w[0] = center_w;
					centers_h[0] = center_h;
					rotates[0] = 0;
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
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									imageRoofsWidth[1] = roof_w_v1;
									imageRoofsHeight[1] = roof_h_v1;
									centers_w[1] = center_w_v1;
									centers_h[1] = center_h_v1;
									rotates[1] = 0;
									if (centers_w[0] == centers_w[1] && centers_h[0] == centers_h[1] && imageRoofsWidth[0] == imageRoofsWidth[1] && imageRoofsHeight[0] == imageRoofsHeight[1])
										continue;
									// touch
									int bot_h = top_h + roof_h;
									int bot_w = top_w + roof_w;
									int bot_h_v1 = top_h_v1 + roof_h_v1;
									int bot_w_v1 = top_w_v1 + roof_w_v1;
									bool bTouch_0_1 = utils::relation_L(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
									if (!bTouch_0_1)
										continue;
									// third rectangle
									for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
										for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
											for (int top_w_v2 = 0; top_w_v2 < width; top_w_v2 += 0.5 * step_size){
												for (int top_h_v2 = 0; top_h_v2 < height; top_h_v2 += 0.5 * step_size){
													int center_w_v2 = top_w_v2 + 0.5 * roof_w_v2;
													int center_h_v2 = top_h_v2 + 0.5 * roof_h_v2;
													if (!utils::rectInsideRect(width, height, center_w_v2, center_h_v2, roof_w_v2, roof_h_v2))
														continue;
													roof_paras[2].clear();
													roof_paras[2].push_back(center_w_v2);
													roof_paras[2].push_back(center_h_v2);
													roof_paras[2].push_back(roof_w_v2);
													roof_paras[2].push_back(roof_h_v2);
													roof_paras[2].push_back(0);
													imageRoofsWidth[2] = roof_w_v2;
													imageRoofsHeight[2] = roof_h_v2;
													centers_w[2] = center_w_v2;
													centers_h[2] = center_h_v2;
													rotates[2] = 0;
													// condition 1
													if (centers_w[2] == centers_w[1] && centers_h[2] == centers_h[1] && imageRoofsWidth[2] == imageRoofsWidth[1] && imageRoofsHeight[2] == imageRoofsHeight[1])
														continue;
													// 
													bool bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													// same size of v1 and v2
													if ((imageRoofsWidth[1] != imageRoofsWidth[2]) || (imageRoofsHeight[1] != imageRoofsHeight[2]))
														continue;
													//
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bool bTouch_0_2 = utils::relation_L(top_w, top_h, bot_w, bot_h, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch_0_2)
														continue;
													// condition 3
													int dis_left = 100000;
													int dis_right = 100000;
													int dis_top = 100000;
													int dis_bot = 100000;
													for (int i = 0; i < roof_paras.size(); i++){
														if (dis_left >(centers_w[i] - 0.5 * imageRoofsWidth[i]))
															dis_left = centers_w[i] - 0.5 * imageRoofsWidth[i];
														if (dis_right > (width - centers_w[i] - 0.5 * imageRoofsWidth[i]))
															dis_right = width - centers_w[i] - 0.5 * imageRoofsWidth[i];
														if (dis_top > (centers_h[i] - 0.5 * imageRoofsHeight[i]))
															dis_top = centers_h[i] - 0.5 * imageRoofsHeight[i];
														if (dis_bot > (height - centers_h[i] - 0.5 * imageRoofsHeight[i]))
															dis_bot = height - centers_h[i] - 0.5 * imageRoofsHeight[i];

													}
													if (dis_left != dis_right || dis_top != dis_bot)
														continue;
													if (dis_left * dis_top != 0)
														continue;
													cv::Mat roof_img(height, width, CV_8UC3, bg_color);
													DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
													if (!roof_img.empty()){
														char buffer[50];
														sprintf(buffer, "roof_image_%08d.png", index);
														std::string img_filename = output_path + "/" + std::string(buffer);
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
}