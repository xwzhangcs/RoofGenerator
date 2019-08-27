#include "generator.h"
#include "drawRotatedRect.h"
#include "Utils.h"
#include "roofTypes.h"
#include "drawRotatedRect.h"

int Generator::generate_one_edge(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(255, 255, 255); // bgr
	int index = start_index;
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
					if (roof_w > 0.7 * width && roof_h > 0.7 * height)
						continue;
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
					for (int iter = 0; iter < 200; iter++){
						cv::Mat roof_img(height, width, CV_8UC3, bg_color);
						DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
						if (!roof_img.empty()){
							char buffer[50];
							sprintf(buffer, "roof_image_%06d.png", index);
							std::string img_filename = output_path + "/" + std::string(buffer);
							std::cout << img_filename << std::endl;
							cv::imwrite(img_filename, roof_img);
							{
								out_param << std::string(buffer);
								for (int cluster = 0; cluster < total; cluster++){
									out_param << ",";
									if (cluster == type){
										out_param << 1;
									}
									else
										out_param << 0;
								}
								out_param << "\n";
							}
							index++;
						}
					}
				}
			}
		}
	}
	return index;
}

int Generator::generate_two_edges(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(255, 255, 255); // bgr
	int index = start_index;
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
					if (roof_w > 0.7 * width && roof_h > 0.7 * height)
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
									if (roof_w_v1 > 0.7 * width && roof_h_v1 > 0.7 * height)
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
									if (dis_left != 0 || dis_top != 0)
										continue;
									//
									// add second
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									for (int iter = 0; iter < 10; iter++){
										cv::Mat roof_img(height, width, CV_8UC3, bg_color);
										DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
										if (!roof_img.empty()){
											char buffer[50];
											sprintf(buffer, "roof_image_%06d.png", index);
											std::string img_filename = output_path + "/" + std::string(buffer);
											std::cout << img_filename << std::endl;
											cv::imwrite(img_filename, roof_img);
											{
												out_param << std::string(buffer);
												for (int cluster = 0; cluster < total; cluster++){
													out_param << ",";
													if (cluster == type){
														out_param << 1;
													}
													else
														out_param << 0;
												}
												out_param << "\n";
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
		}
	}
	return index;
}

int Generator::generate_three_edges_v1(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(255, 255, 255); // bgr
	int index = start_index;
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
					if (roof_w > 0.7 * width && roof_h > 0.7 * height)
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
									if (roof_w_v1 > 0.7 * width && roof_h_v1 > 0.7 * height)
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
									if (dis_left != 0 || dis_top != 0)
										continue;
									//
									// add second
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									for (int iter = 0; iter < 1; iter++){
										cv::Mat roof_img(height, width, CV_8UC3, bg_color);
										DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
										if (!roof_img.empty()){
											char buffer[50];
											sprintf(buffer, "roof_image_%06d.png", index);
											std::string img_filename = output_path + "/" + std::string(buffer);
											std::cout << img_filename << std::endl;
											cv::imwrite(img_filename, roof_img);
											{
												out_param << std::string(buffer);
												for (int cluster = 0; cluster < total; cluster++){
													out_param << ",";
													if (cluster == type){
														out_param << 1;
													}
													else
														out_param << 0;
												}
												out_param << "\n";
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
		}
	}
	return index;
}

int Generator::generate_three_edges_v2(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = start_index;
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
					if (roof_w > 0.7 * width && roof_h > 0.7 * height)
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
									if (roof_w_v1 > 0.7 * width && roof_h_v1 > 0.7 * height)
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
													if (roof_w_v2 > 0.7 * width && roof_h_v2 > 0.7 * height)
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
													// same size of v1 and v2
													if ((imageRoofsWidth[1] != imageRoofsWidth[2]) || (imageRoofsHeight[1] != imageRoofsHeight[2]))
														continue;
													//
													bool bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
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
													if (dis_left != 0 || dis_top != 0)
														continue;
													for (int iter = 0; iter < 1; iter++){
														cv::Mat roof_img(height, width, CV_8UC3, bg_color);
														DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
														if (!roof_img.empty()){
															char buffer[50];
															sprintf(buffer, "roof_image_%06d.png", index);
															std::string img_filename = output_path + "/" + std::string(buffer);
															std::cout << img_filename << std::endl;
															cv::imwrite(img_filename, roof_img);
															{
																out_param << std::string(buffer);
																for (int cluster = 0; cluster < total; cluster++){
																	out_param << ",";
																	if (cluster == type){
																		out_param << 1;
																	}
																	else
																		out_param << 0;
																}
																out_param << "\n";
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
						}
					}
				}
			}
		}
	}
	return index;
}

int Generator::generate_four_edges_v2(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = start_index;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(4);
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
			for (int top_w = 0; top_w < width; top_w += step_size){
				for (int top_h = 0; top_h < height; top_h += step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					if (roof_w > 0.7 * width && roof_h > 0.7 * height)
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
					int bot_h = top_h + roof_h;
					int bot_w = top_w + roof_w;
					// check 
					// second rectangle
					for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
						for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1))
										continue;
									if (roof_w_v1 > 0.7 * width && roof_h_v1 > 0.7 * height)
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
									int bot_h_v1 = top_h_v1 + roof_h_v1;
									int bot_w_v1 = top_w_v1 + roof_w_v1;
									bool bTouch_0_1 = utils::relation_L(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
									if (!bTouch_0_1)
										continue;
									// third rectangle
									for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
										for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
											for (int top_w_v2 = 0; top_w_v2 < width; top_w_v2 += step_size){
												for (int top_h_v2 = 0; top_h_v2 < height; top_h_v2 += step_size){
													int center_w_v2 = top_w_v2 + 0.5 * roof_w_v2;
													int center_h_v2 = top_h_v2 + 0.5 * roof_h_v2;
													if (!utils::rectInsideRect(width, height, center_w_v2, center_h_v2, roof_w_v2, roof_h_v2))
														continue;
													if (roof_w_v2 > 0.7 * width && roof_h_v2 > 0.7 * height)
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
													if (centers_w[1] == centers_w[2] && centers_h[1] == centers_h[2] && imageRoofsWidth[1] == imageRoofsWidth[2] && imageRoofsHeight[1] == imageRoofsHeight[2])
														continue;
													bool bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bool bTouch_0_2 = utils::relation_L(top_w, top_h, bot_w, bot_h, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch_0_2)
														continue;
													// fourth rectangle
													for (int roof_w_v3 = roof_min_size; roof_w_v3 <= width; roof_w_v3 += step_size){
														for (int roof_h_v3 = roof_min_size; roof_h_v3 <= height; roof_h_v3 += step_size){
															for (int top_w_v3 = 0; top_w_v3 < width; top_w_v3 += step_size){
																for (int top_h_v3 = 0; top_h_v3 < height; top_h_v3 += step_size){
																	int center_w_v3 = top_w_v3 + 0.5 * roof_w_v3;
																	int center_h_v3 = top_h_v3 + 0.5 * roof_h_v3;
																	if (!utils::rectInsideRect(width, height, center_w_v3, center_h_v3, roof_w_v3, roof_h_v3))
																		continue;
																	if (roof_w_v3 > 0.7 * width && roof_h_v3 > 0.7 * height)
																		continue;
																	roof_paras[3].clear();
																	roof_paras[3].push_back(center_w_v3);
																	roof_paras[3].push_back(center_h_v3);
																	roof_paras[3].push_back(roof_w_v3);
																	roof_paras[3].push_back(roof_h_v3);
																	roof_paras[3].push_back(0);
																	imageRoofsWidth[3] = roof_w_v3;
																	imageRoofsHeight[3] = roof_h_v3;
																	centers_w[3] = center_w_v3;
																	centers_h[3] = center_h_v3;
																	rotates[3] = 0;
																	if (centers_w[2] == centers_w[3] && centers_h[2] == centers_h[3] && imageRoofsWidth[2] == imageRoofsWidth[3] && imageRoofsHeight[2] == imageRoofsHeight[3])
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	int bot_h_v3 = top_h_v3 + roof_h_v3;
																	int bot_w_v3 = top_w_v3 + roof_w_v3;
																	bool bTouch_3_1 = utils::relation_L(top_w_v3, top_h_v3, bot_w_v3, bot_h_v3, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
																	if (!bTouch_3_1)
																		continue;
																	bool bTouch_3_2 = utils::relation_L(top_w_v3, top_h_v3, bot_w_v3, bot_h_v3, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
																	if (!bTouch_3_2)
																		continue;
																	// condition 3
																	int dis_left = 100000;
																	int dis_right = 100000;
																	int dis_top = 100000;
																	int dis_bot = 100000;
																	for (int i = 0; i < roof_paras.size(); i++){
																		if (dis_left >(centers_w[i] - 0.5 * imageRoofsWidth[i]))
																			dis_left = centers_w[i] - 0.5 * imageRoofsWidth[i];
																		if (dis_right >(width - centers_w[i] - 0.5 * imageRoofsWidth[i]))
																			dis_right = width - centers_w[i] - 0.5 * imageRoofsWidth[i];
																		if (dis_top > (centers_h[i] - 0.5 * imageRoofsHeight[i]))
																			dis_top = centers_h[i] - 0.5 * imageRoofsHeight[i];
																		if (dis_bot > (height - centers_h[i] - 0.5 * imageRoofsHeight[i]))
																			dis_bot = height - centers_h[i] - 0.5 * imageRoofsHeight[i];

																	}
																	if (dis_left != dis_right || dis_top != dis_bot)
																		continue;
																	if (dis_left != 0 || dis_top != 0)
																		continue;
																	for (int iter = 0; iter < 9; iter++){
																		cv::Mat roof_img(height, width, CV_8UC3, bg_color);
																		DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
																		if (!roof_img.empty()){
																			char buffer[50];
																			sprintf(buffer, "roof_image_%06d.png", index);
																			std::string img_filename = output_path + "/" + std::string(buffer);
																			std::cout << img_filename << std::endl;
																			cv::imwrite(img_filename, roof_img);
																			{
																				out_param << std::string(buffer);
																				for (int cluster = 0; cluster < total; cluster++){
																					out_param << ",";
																					if (cluster == type){
																						out_param << 1;
																					}
																					else
																						out_param << 0;
																				}
																				out_param << "\n";
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
	return index;
}

int Generator::generate_four_edges_v1(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(255, 255, 255); // bgr
	int index = start_index;
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
					if (roof_w > 0.7 * width && roof_h > 0.7 * height)
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
									if (roof_w_v1 > 0.7 * width && roof_h_v1 > 0.7 * height)
										continue;
									// condition 1
									if (center_w == center_w_v1 && center_h == center_h_v1 && roof_w == roof_w_v1 && roof_h == roof_h_v1)
										continue;
									// touch
									int bot_h = top_h + roof_h;
									int bot_w = top_w + roof_w;
									int bot_h_v1 = top_h_v1 + roof_h_v1;
									int bot_w_v1 = top_w_v1 + roof_w_v1;
									bool bTouch = utils::relation_Cross(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
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
									if (dis_left != 0 || dis_top != 0)
										continue;
									//
									// add second
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									for (int iter = 0; iter < 1; iter++){
										cv::Mat roof_img(height, width, CV_8UC3, bg_color);
										DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
										if (!roof_img.empty()){
											char buffer[50];
											sprintf(buffer, "roof_image_%06d.png", index);
											std::string img_filename = output_path + "/" + std::string(buffer);
											std::cout << img_filename << std::endl;
											cv::imwrite(img_filename, roof_img);
											{
												out_param << std::string(buffer);
												for (int cluster = 0; cluster < total; cluster++){
													out_param << ",";
													if (cluster == type){
														out_param << 1;
													}
													else
														out_param << 0;
												}
												out_param << "\n";
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
		}
	}
	return index;
}

int Generator::generate_four_edges_v3(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = start_index;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(4);
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
			for (int top_w = 0; top_w < width; top_w += step_size){
				for (int top_h = 0; top_h < height; top_h += step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					if (roof_w > 0.7 * width && roof_h > 0.7 * height)
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
					int bot_h = top_h + roof_h;
					int bot_w = top_w + roof_w;
					// check 
					// second rectangle
					for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
						for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1))
										continue;
									if (roof_w_v1 > 0.7 * width && roof_h_v1 > 0.7 * height)
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
									int bot_h_v1 = top_h_v1 + roof_h_v1;
									int bot_w_v1 = top_w_v1 + roof_w_v1;
									bool bTouch_0_1 = utils::relation_L(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
									if (!bTouch_0_1)
										continue;
									// third rectangle
									for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
										for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
											for (int top_w_v2 = 0; top_w_v2 < width; top_w_v2 += step_size){
												for (int top_h_v2 = 0; top_h_v2 < height; top_h_v2 += step_size){
													int center_w_v2 = top_w_v2 + 0.5 * roof_w_v2;
													int center_h_v2 = top_h_v2 + 0.5 * roof_h_v2;
													if (!utils::rectInsideRect(width, height, center_w_v2, center_h_v2, roof_w_v2, roof_h_v2))
														continue;
													if (roof_w_v2 > 0.7 * width && roof_h_v2 > 0.7 * height)
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
													if (centers_w[1] == centers_w[2] && centers_h[1] == centers_h[2] && imageRoofsWidth[1] == imageRoofsWidth[2] && imageRoofsHeight[1] == imageRoofsHeight[2])
														continue;
													bool bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bool bTouch_1_2 = utils::relation_L(top_w_v1, top_h_v1, bot_w_v1, bot_h_v1, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch_1_2)
														continue;
													// fourth rectangle
													for (int roof_w_v3 = roof_min_size; roof_w_v3 <= width; roof_w_v3 += step_size){
														for (int roof_h_v3 = roof_min_size; roof_h_v3 <= height; roof_h_v3 += step_size){
															for (int top_w_v3 = 0; top_w_v3 < width; top_w_v3 += step_size){
																for (int top_h_v3 = 0; top_h_v3 < height; top_h_v3 += step_size){
																	int center_w_v3 = top_w_v3 + 0.5 * roof_w_v3;
																	int center_h_v3 = top_h_v3 + 0.5 * roof_h_v3;
																	if (!utils::rectInsideRect(width, height, center_w_v3, center_h_v3, roof_w_v3, roof_h_v3))
																		continue;
																	if (roof_w_v3 > 0.7 * width && roof_h_v3 > 0.7 * height)
																		continue;
																	roof_paras[3].clear();
																	roof_paras[3].push_back(center_w_v3);
																	roof_paras[3].push_back(center_h_v3);
																	roof_paras[3].push_back(roof_w_v3);
																	roof_paras[3].push_back(roof_h_v3);
																	roof_paras[3].push_back(0);
																	imageRoofsWidth[3] = roof_w_v3;
																	imageRoofsHeight[3] = roof_h_v3;
																	centers_w[3] = center_w_v3;
																	centers_h[3] = center_h_v3;
																	rotates[3] = 0;
																	if (centers_w[2] == centers_w[3] && centers_h[2] == centers_h[3] && imageRoofsWidth[2] == imageRoofsWidth[3] && imageRoofsHeight[2] == imageRoofsHeight[3])
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	int bot_h_v3 = top_h_v3 + roof_h_v3;
																	int bot_w_v3 = top_w_v3 + roof_w_v3;			
																	bool bTouch_2_3 = utils::relation_L(top_w_v2, top_h_v2, bot_w_v2, bot_h_v2, top_w_v3, top_h_v3, bot_w_v3, bot_h_v3);
																	if (!bTouch_2_3)
																		continue;
																	// condition 3
																	int dis_left = 100000;
																	int dis_right = 100000;
																	int dis_top = 100000;
																	int dis_bot = 100000;
																	for (int i = 0; i < roof_paras.size(); i++){
																		if (dis_left >(centers_w[i] - 0.5 * imageRoofsWidth[i]))
																			dis_left = centers_w[i] - 0.5 * imageRoofsWidth[i];
																		if (dis_right >(width - centers_w[i] - 0.5 * imageRoofsWidth[i]))
																			dis_right = width - centers_w[i] - 0.5 * imageRoofsWidth[i];
																		if (dis_top > (centers_h[i] - 0.5 * imageRoofsHeight[i]))
																			dis_top = centers_h[i] - 0.5 * imageRoofsHeight[i];
																		if (dis_bot > (height - centers_h[i] - 0.5 * imageRoofsHeight[i]))
																			dis_bot = height - centers_h[i] - 0.5 * imageRoofsHeight[i];

																	}
																	if (dis_left != dis_right || dis_top != dis_bot)
																		continue;
																	if (dis_left != 0 || dis_top != 0)
																		continue;
																	for (int iter = 0; iter < 20; iter++){
																		cv::Mat roof_img(height, width, CV_8UC3, bg_color);
																		DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
																		if (!roof_img.empty()){
																			char buffer[50];
																			sprintf(buffer, "roof_image_%06d.png", index);
																			std::string img_filename = output_path + "/" + std::string(buffer);
																			std::cout << img_filename << std::endl;
																			cv::imwrite(img_filename, roof_img);
																			{
																				out_param << std::string(buffer);
																				for (int cluster = 0; cluster < total; cluster++){
																					out_param << ",";
																					if (cluster == type){
																						out_param << 1;
																					}
																					else
																						out_param << 0;
																				}
																				out_param << "\n";
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
	return index;
}

int Generator::generate_four_edges_v4(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = start_index;
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
			for (int top_w = 0; top_w < width; top_w += step_size){
				for (int top_h = 0; top_h < height; top_h += step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					if (roof_w > 0.7 * width && roof_h > 0.7 * height)
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
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1))
										continue;
									if (roof_w_v1 > 0.7 * width && roof_h_v1 > 0.7 * height)
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
									bool bTouch_0_1 = utils::relation_T(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
									if (!bTouch_0_1)
										continue;
									// third rectangle
									for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
										for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
											for (int top_w_v2 = 0; top_w_v2 < width; top_w_v2 += step_size){
												for (int top_h_v2 = 0; top_h_v2 < height; top_h_v2 += step_size){
													int center_w_v2 = top_w_v2 + 0.5 * roof_w_v2;
													int center_h_v2 = top_h_v2 + 0.5 * roof_h_v2;
													if (!utils::rectInsideRect(width, height, center_w_v2, center_h_v2, roof_w_v2, roof_h_v2))
														continue;
													if (roof_w_v2 > 0.7 * width && roof_h_v2 > 0.7 * height)
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
													bool bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													// same size of v1 and v2
													if ((imageRoofsWidth[0] != imageRoofsWidth[2]) || (imageRoofsHeight[0] != imageRoofsHeight[2]))
														continue;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bool bTouch_1_2 = utils::relation_L(top_w_v1, top_h_v1, bot_w_v1, bot_h_v1, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch_1_2)
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
													if (dis_left != 0 || dis_top != 0)
														continue;
													for (int iter = 0; iter < 90; iter++){
														cv::Mat roof_img(height, width, CV_8UC3, bg_color);
														DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
														if (!roof_img.empty()){
															char buffer[50];
															sprintf(buffer, "roof_image_%06d.png", index);
															std::string img_filename = output_path + "/" + std::string(buffer);
															std::cout << img_filename << std::endl;
															cv::imwrite(img_filename, roof_img);
															{
																out_param << std::string(buffer);
																for (int cluster = 0; cluster < total; cluster++){
																	out_param << ",";
																	if (cluster == type){
																		out_param << 1;
																	}
																	else
																		out_param << 0;
																}
																out_param << "\n";
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
						}
					}
				}
			}
		}
	}
	return index;
}

int Generator::generate_five_edges_v1(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = start_index;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(4);
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
									bool bTouch_0_1 = utils::relation_T(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
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
													bool bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bool bTouch_1_2 = utils::relation_L(top_w_v1, top_h_v1, bot_w_v1, bot_h_v1, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch_1_2)
														continue;
													// fourth rectangle
													for (int roof_w_v3 = roof_min_size; roof_w_v3 <= width; roof_w_v3 += step_size){
														for (int roof_h_v3 = roof_min_size; roof_h_v3 <= height; roof_h_v3 += step_size){
															for (int top_w_v3 = 0; top_w_v3 < width; top_w_v3 += 0.5 * step_size){
																for (int top_h_v3 = 0; top_h_v3 < height; top_h_v3 += 0.5 * step_size){
																	int center_w_v3 = top_w_v3 + 0.5 * roof_w_v3;
																	int center_h_v3 = top_h_v3 + 0.5 * roof_h_v3;
																	if (!utils::rectInsideRect(width, height, center_w_v3, center_h_v3, roof_w_v3, roof_h_v3))
																		continue;
																	roof_paras[3].clear();
																	roof_paras[3].push_back(center_w_v3);
																	roof_paras[3].push_back(center_h_v3);
																	roof_paras[3].push_back(roof_w_v3);
																	roof_paras[3].push_back(roof_h_v3);
																	roof_paras[3].push_back(0);
																	imageRoofsWidth[3] = roof_w_v3;
																	imageRoofsHeight[3] = roof_h_v3;
																	centers_w[3] = center_w_v3;
																	centers_h[3] = center_h_v3;
																	rotates[3] = 0;
																	if (centers_w[2] == centers_w[3] && centers_h[2] == centers_h[3] && imageRoofsWidth[2] == imageRoofsWidth[3] && imageRoofsHeight[2] == imageRoofsHeight[3])
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	int bot_h_v3 = top_h_v3 + roof_h_v3;
																	int bot_w_v3 = top_w_v3 + roof_w_v3;
																	bool bTouch_0_3 = utils::relation_L(top_w, top_h, bot_w, bot_h, top_w_v3, top_h_v3, bot_w_v3, bot_h_v3);
																	if (!bTouch_0_3)
																		continue;
																	bool bTouch_2_3 = utils::relation_L(top_w_v2, top_h_v2, bot_w_v2, bot_h_v2, top_w_v3, top_h_v3, bot_w_v3, bot_h_v3);
																	if (!bTouch_2_3)
																		continue;
																	// condition 3
																	int dis_left = 100000;
																	int dis_right = 100000;
																	int dis_top = 100000;
																	int dis_bot = 100000;
																	for (int i = 0; i < roof_paras.size(); i++){
																		if (dis_left > (centers_w[i] - 0.5 * imageRoofsWidth[i]))
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
																		{
																			out_param << std::string(buffer);
																			for (int cluster = 0; cluster < total; cluster++){
																				out_param << ",";
																				if (cluster == type){
																					out_param << 1;
																				}
																				else
																					out_param << 0;
																			}
																			out_param << "\n";
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
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return index;
}

int Generator::generate_five_edges_v2(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(255, 255, 255); // bgr
	int index = start_index;
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
					// add first 
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
									bool bTouch = utils::relation_Cross(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
									if (!bTouch)
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
													bool bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bool bTouch_1_2 = utils::relation_L(top_w_v1, top_h_v1, bot_w_v1, bot_h_v1, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch_1_2)
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
														{
															out_param << std::string(buffer);
															for (int cluster = 0; cluster < total; cluster++){
																out_param << ",";
																if (cluster == type){
																	out_param << 1;
																}
																else
																	out_param << 0;
															}
															out_param << "\n";
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
					}
				}
			}
		}
	}
	return index;
}

int Generator::generate_five_edges_v3(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = start_index;
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
									bool bTouch_0_1 = utils::relation_T(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
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
													bool bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													// same size of v1 and v2
													if ((imageRoofsWidth[0] != imageRoofsWidth[2]) || (imageRoofsHeight[0] != imageRoofsHeight[2]))
														continue;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bool bTouch_2_1 = utils::relation_T(top_w_v2, top_h_v2, bot_w_v2, bot_h_v2, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
													if (!bTouch_2_1)
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
														{
															out_param << std::string(buffer);
															for (int cluster = 0; cluster < total; cluster++){
																out_param << ",";
																if (cluster == type){
																	out_param << 1;
																}
																else
																	out_param << 0;
															}
															out_param << "\n";
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
					}
				}
			}
		}
	}
	return index;
}

int Generator::generate_five_edges_v4(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = start_index;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(4);
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
									bool bTouch_0_1 = utils::relation_T(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
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
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bool bTouch_0_2 = utils::relation_L(top_w, top_h, bot_w, bot_h, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch_0_2)
														continue;
													// fourth rectangle
													for (int roof_w_v3 = roof_min_size; roof_w_v3 <= width; roof_w_v3 += step_size){
														for (int roof_h_v3 = roof_min_size; roof_h_v3 <= height; roof_h_v3 += step_size){
															for (int top_w_v3 = 0; top_w_v3 < width; top_w_v3 += 0.5 * step_size){
																for (int top_h_v3 = 0; top_h_v3 < height; top_h_v3 += 0.5 * step_size){
																	int center_w_v3 = top_w_v3 + 0.5 * roof_w_v3;
																	int center_h_v3 = top_h_v3 + 0.5 * roof_h_v3;
																	if (!utils::rectInsideRect(width, height, center_w_v3, center_h_v3, roof_w_v3, roof_h_v3))
																		continue;
																	roof_paras[3].clear();
																	roof_paras[3].push_back(center_w_v3);
																	roof_paras[3].push_back(center_h_v3);
																	roof_paras[3].push_back(roof_w_v3);
																	roof_paras[3].push_back(roof_h_v3);
																	roof_paras[3].push_back(0);
																	imageRoofsWidth[3] = roof_w_v3;
																	imageRoofsHeight[3] = roof_h_v3;
																	centers_w[3] = center_w_v3;
																	centers_h[3] = center_h_v3;
																	rotates[3] = 0;
																	if (centers_w[2] == centers_w[3] && centers_h[2] == centers_h[3] && imageRoofsWidth[2] == imageRoofsWidth[3] && imageRoofsHeight[2] == imageRoofsHeight[3])
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	// same size of v1 and v2
																	if ((imageRoofsWidth[3] != imageRoofsWidth[2]) || (imageRoofsHeight[3] != imageRoofsHeight[2]))
																		continue;
																	int bot_h_v3 = top_h_v3 + roof_h_v3;
																	int bot_w_v3 = top_w_v3 + roof_w_v3;
																	bool bTouch_0_3 = utils::relation_L(top_w, top_h, bot_w, bot_h, top_w_v3, top_h_v3, bot_w_v3, bot_h_v3);
																	if (!bTouch_0_3)
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
																		{
																			out_param << std::string(buffer);
																			for (int cluster = 0; cluster < total; cluster++){
																				out_param << ",";
																				if (cluster == type){
																					out_param << 1;
																				}
																				else
																					out_param << 0;
																			}
																			out_param << "\n";
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
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return index;
}

int Generator::generate_five_edges_v5(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = start_index;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(5);
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
					int bot_h = top_h + roof_h;
					int bot_w = top_w + roof_w;
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
													if (centers_w[1] == centers_w[2] && centers_h[1] == centers_h[2] && imageRoofsWidth[1] == imageRoofsWidth[2] && imageRoofsHeight[1] == imageRoofsHeight[2])
														continue;
													bool bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bool bTouch_1_2 = utils::relation_L(top_w_v1, top_h_v1, bot_w_v1, bot_h_v1, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch_1_2)
														continue;
													// fourth rectangle
													for (int roof_w_v3 = roof_min_size; roof_w_v3 <= width; roof_w_v3 += step_size){
														for (int roof_h_v3 = roof_min_size; roof_h_v3 <= height; roof_h_v3 += step_size){
															for (int top_w_v3 = 0; top_w_v3 < width; top_w_v3 += 0.5 * step_size){
																for (int top_h_v3 = 0; top_h_v3 < height; top_h_v3 += 0.5 * step_size){
																	int center_w_v3 = top_w_v3 + 0.5 * roof_w_v3;
																	int center_h_v3 = top_h_v3 + 0.5 * roof_h_v3;
																	if (!utils::rectInsideRect(width, height, center_w_v3, center_h_v3, roof_w_v3, roof_h_v3))
																		continue;
																	roof_paras[3].clear();
																	roof_paras[3].push_back(center_w_v3);
																	roof_paras[3].push_back(center_h_v3);
																	roof_paras[3].push_back(roof_w_v3);
																	roof_paras[3].push_back(roof_h_v3);
																	roof_paras[3].push_back(0);
																	imageRoofsWidth[3] = roof_w_v3;
																	imageRoofsHeight[3] = roof_h_v3;
																	centers_w[3] = center_w_v3;
																	centers_h[3] = center_h_v3;
																	rotates[3] = 0;
																	if (centers_w[2] == centers_w[3] && centers_h[2] == centers_h[3] && imageRoofsWidth[2] == imageRoofsWidth[3] && imageRoofsHeight[2] == imageRoofsHeight[3])
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	int bot_h_v3 = top_h_v3 + roof_h_v3;
																	int bot_w_v3 = top_w_v3 + roof_w_v3;
																	bool bTouch_2_3 = utils::relation_L(top_w_v2, top_h_v2, bot_w_v2, bot_h_v2, top_w_v3, top_h_v3, bot_w_v3, bot_h_v3);
																	if (!bTouch_2_3)
																		continue;
																	// fifth rectangle
																	for (int roof_w_v4 = roof_min_size; roof_w_v4 <= width; roof_w_v4 += step_size){
																		for (int roof_h_v4 = roof_min_size; roof_h_v4 <= height; roof_h_v4 += step_size){
																			for (int top_w_v4 = 0; top_w_v4 < width; top_w_v4 += 0.5 * step_size){
																				for (int top_h_v4 = 0; top_h_v4 < height; top_h_v4 += 0.5 * step_size){
																					int center_w_v4 = top_w_v4 + 0.5 * roof_w_v4;
																					int center_h_v4 = top_h_v4 + 0.5 * roof_h_v4;
																					if (!utils::rectInsideRect(width, height, center_w_v4, center_h_v4, roof_w_v4, roof_h_v4))
																						continue;
																					roof_paras[4].clear();
																					roof_paras[4].push_back(center_w_v4);
																					roof_paras[4].push_back(center_h_v4);
																					roof_paras[4].push_back(roof_w_v4);
																					roof_paras[4].push_back(roof_h_v4);
																					roof_paras[4].push_back(0);
																					imageRoofsWidth[4] = roof_w_v4;
																					imageRoofsHeight[4] = roof_h_v4;
																					centers_w[4] = center_w_v4;
																					centers_h[4] = center_h_v4;
																					rotates[4] = 0;
																					if (centers_w[4] == centers_w[3] && centers_h[4] == centers_h[3] && imageRoofsWidth[4] == imageRoofsWidth[3] && imageRoofsHeight[4] == imageRoofsHeight[3])
																						continue;
																					bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[4], centers_h[4], imageRoofsWidth[4], imageRoofsHeight[4]);
																					if (bIntersect)
																						continue;
																					bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[4], centers_h[4], imageRoofsWidth[4], imageRoofsHeight[4]);
																					if (bIntersect)
																						continue;
																					bIntersect = utils::rectIntersecRect(centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2], centers_w[4], centers_h[4], imageRoofsWidth[4], imageRoofsHeight[4]);
																					if (bIntersect)
																						continue;
																					int bot_h_v4 = top_h_v4 + roof_h_v4;
																					int bot_w_v4 = top_w_v4 + roof_w_v4;
																					bool bTouch_3_4 = utils::relation_L(top_w_v3, top_h_v3, bot_w_v3, bot_h_v3, top_w_v4, top_h_v4, bot_w_v4, bot_h_v4);
																					if (!bTouch_3_4)
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
																						{
																							out_param << std::string(buffer);
																							for (int cluster = 0; cluster < total; cluster++){
																								out_param << ",";
																								if (cluster == type){
																									out_param << 1;
																								}
																								else
																									out_param << 0;
																							}
																							out_param << "\n";
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
	return index;
}

int Generator::generate_five_edges_v6(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path){
	std::ofstream out_param(output_path + "/parameters.txt", std::ios::app);
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = start_index;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(4);
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
									bool bTouch_0_1 = utils::relation_T(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
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
													bool bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bool bTouch_1_2 = utils::relation_L(top_w_v1, top_h_v1, bot_w_v1, bot_h_v1, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch_1_2)
														continue;
													// fourth rectangle
													for (int roof_w_v3 = roof_min_size; roof_w_v3 <= width; roof_w_v3 += step_size){
														for (int roof_h_v3 = roof_min_size; roof_h_v3 <= height; roof_h_v3 += step_size){
															for (int top_w_v3 = 0; top_w_v3 < width; top_w_v3 += 0.5 * step_size){
																for (int top_h_v3 = 0; top_h_v3 < height; top_h_v3 += 0.5 * step_size){
																	int center_w_v3 = top_w_v3 + 0.5 * roof_w_v3;
																	int center_h_v3 = top_h_v3 + 0.5 * roof_h_v3;
																	if (!utils::rectInsideRect(width, height, center_w_v3, center_h_v3, roof_w_v3, roof_h_v3))
																		continue;
																	roof_paras[3].clear();
																	roof_paras[3].push_back(center_w_v3);
																	roof_paras[3].push_back(center_h_v3);
																	roof_paras[3].push_back(roof_w_v3);
																	roof_paras[3].push_back(roof_h_v3);
																	roof_paras[3].push_back(0);
																	imageRoofsWidth[3] = roof_w_v3;
																	imageRoofsHeight[3] = roof_h_v3;
																	centers_w[3] = center_w_v3;
																	centers_h[3] = center_h_v3;
																	rotates[3] = 0;
																	if (centers_w[2] == centers_w[3] && centers_h[2] == centers_h[3] && imageRoofsWidth[2] == imageRoofsWidth[3] && imageRoofsHeight[2] == imageRoofsHeight[3])
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	int bot_h_v3 = top_h_v3 + roof_h_v3;
																	int bot_w_v3 = top_w_v3 + roof_w_v3;
																	bool bTouch_2_3 = utils::relation_L(top_w_v2, top_h_v2, bot_w_v2, bot_h_v2, top_w_v3, top_h_v3, bot_w_v3, bot_h_v3);
																	if (!bTouch_2_3)
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
																		{
																			out_param << std::string(buffer);
																			for (int cluster = 0; cluster < total; cluster++){
																				out_param << ",";
																				if (cluster == type){
																					out_param << 1;
																				}
																				else
																					out_param << 0;
																			}
																			out_param << "\n";
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
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return index;
}