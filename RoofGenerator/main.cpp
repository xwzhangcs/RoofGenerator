#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "roofA.h"
#include "roofB.h"
#include "roofC.h"
#include "roofD.h"
#include "roofTypes.h"
#include "RoofUtils.h"
#include "Utils.h"

void test_one_nodes(int width, int height);
void test_two_nodes(int width, int height, int type);
void test_three_nodes(int width, int height, int type);
void test_four_nodes(int width, int height, int type);

int main(int argc, char** argv)
{
	//test_one_nodes(56, 56);
	//test_two_nodes(56, 56, 0);
	//test_two_nodes(56, 56, 1);
	//test_three_nodes(56, 56, 2);
	test_four_nodes(56, 56, 0);
	/*std::cout << utils::rectControlRect(16, 32, 22, 13, 0, 24, 20, 26, 10, 0) << std::endl;
	std::cout << utils::rectIntersecRect(16, 32, 22, 13, 0, 24, 20, 26, 10, 0) << std::endl;
	std::cout << utils::rectIntersecRect(24, 20, 26, 10, 0, 16, 32, 22, 13, 0) << std::endl;*/
	system("pause");

	return 0;
}

// reduce the resolution to 56 * 56 
void test_one_nodes(int width, int height){
	double step_width = 6.0 / width;
	double step_height = 6.0 / height;
	double step_aspect = 0.2;
	int index = 0;
	for (double center_x_ratio = 0.3; center_x_ratio < 0.8; center_x_ratio += step_width){
		for (double center_y_ratio = 0.3; center_y_ratio < 0.8; center_y_ratio += step_height){
			// set roof width range to 0.3 ~ 0.7, step is step_width
			// set aspect range to 0.4 ~ 2.5, step is 0.1
			for (double roof_w_ratio = 0.4; roof_w_ratio < 0.8; roof_w_ratio += step_width){
				for (double aspect_ratio = 0.4; aspect_ratio < 2.5; aspect_ratio += step_aspect){
					Config roof_flat_paras(center_x_ratio, center_y_ratio, roof_w_ratio, aspect_ratio, 0, RoofTypes::FLAT, false, 0.0, 1.0);
					cv::Mat roof_img = RoofA::generateRoof(width, height, roof_flat_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
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
		}
	}
}

void test_two_nodes(int width, int height, int type){
	double step_width = 6.0 / width;
	double step_height = 6.0 / height;
	double step_aspect = 0.2;
	int index = 0;
	for (double center_x_ratio_v1 = 0.3; center_x_ratio_v1 < 0.8; center_x_ratio_v1 += step_width){
		for (double center_y_ratio_v1 = 0.3; center_y_ratio_v1 < 0.8; center_y_ratio_v1 += step_height){
			// set roof width range to 0.4 ~ 0.8, step is step_width
			// set aspect range to 0.4 ~ 2.5, step is 0.1
			for (double roof_w_ratio_v1 = 0.4; roof_w_ratio_v1 < 0.8; roof_w_ratio_v1 += step_width){
				for (double aspect_ratio_v1 = 0.4; aspect_ratio_v1 < 2.5; aspect_ratio_v1 += step_aspect){

					Config roof_flat_paras_v1(center_x_ratio_v1, center_y_ratio_v1, roof_w_ratio_v1, aspect_ratio_v1, 0, RoofTypes::FLAT, false, 0.0, 1.0);
					cv::Mat roof_img_test_v1 = RoofA::generateRoof(width, height, roof_flat_paras_v1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
					if (roof_img_test_v1.empty()){
						continue;
					}
					// the second node
					for (double center_x_ratio_v2 = 0.3; center_x_ratio_v2 < 0.8; center_x_ratio_v2 += step_width){
						for (double center_y_ratio_v2 = 0.3; center_y_ratio_v2 < 0.8; center_y_ratio_v2 += step_height){
							// set roof width range to 0.4 ~ 0.8, step is step_width
							// set aspect range to 0.4 ~ 2.5, step is 0.1
							for (double roof_w_ratio_v2 = 0.4; roof_w_ratio_v2 < 0.8; roof_w_ratio_v2 += step_width){
								for (double aspect_ratio_v2 = 0.4; aspect_ratio_v2 < 2.5; aspect_ratio_v2 += step_aspect){
									// the second node
									Config roof_flat_paras_v2(center_x_ratio_v2, center_y_ratio_v2, roof_w_ratio_v2, aspect_ratio_v2, 0, RoofTypes::FLAT, false, 0.0, 1.0);
									cv::Mat roof_img_test_v2 = RoofA::generateRoof(width, height, roof_flat_paras_v2, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
									if (roof_img_test_v2.empty()){
										continue;
									}
									std::vector<Config> roof_paras;
									roof_paras.push_back(roof_flat_paras_v1);
									roof_paras.push_back(roof_flat_paras_v2);
									cv::Mat roof_img = RoofB::generateRoof(width, height, roof_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255), type, false);
									if (!roof_img.empty()){
										char buffer[50];
										sprintf(buffer, "roof_image_%08d.png", index);
										std::string img_filename = "../data/node_two_two/" + std::string(buffer);
										std::cout << img_filename << std::endl;
										cv::imwrite(img_filename, roof_img);
										/*if (index == 1506){
											cv::Mat roof_img = RoofB::generateRoof(width, height, roof_flat_paras_v1, roof_flat_paras_v2, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255), type, true);
											cv::imwrite(img_filename, roof_img);
										}*/
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

void test_three_nodes(int width, int height, int type){
	double step_width = 6.0 / width;
	double step_height = 6.0 / height;
	double step_aspect = 0.2;
	int index = 0;
	for (double center_x_ratio_v1 = 0.3; center_x_ratio_v1 < 0.8; center_x_ratio_v1 += step_width){
		for (double center_y_ratio_v1 = 0.3; center_y_ratio_v1 < 0.8; center_y_ratio_v1 += step_height){
			// set roof width range to 0.4 ~ 0.8, step is step_width
			// set aspect range to 0.4 ~ 2.5, step is 0.1
			for (double roof_w_ratio_v1 = 0.4; roof_w_ratio_v1 < 0.8; roof_w_ratio_v1 += step_width){
				for (double aspect_ratio_v1 = 0.4; aspect_ratio_v1 < 2.5; aspect_ratio_v1 += step_aspect){

					Config roof_flat_paras_v1(center_x_ratio_v1, center_y_ratio_v1, roof_w_ratio_v1, aspect_ratio_v1, 0, RoofTypes::FLAT, false, 0.0, 1.0);
					cv::Mat roof_img_test_v1 = RoofA::generateRoof(width, height, roof_flat_paras_v1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
					if (roof_img_test_v1.empty()){
						continue;
					}
					// the second node
					for (double center_x_ratio_v2 = 0.3; center_x_ratio_v2 < 0.8; center_x_ratio_v2 += step_width){
						for (double center_y_ratio_v2 = 0.3; center_y_ratio_v2 < 0.8; center_y_ratio_v2 += step_height){
							// set roof width range to 0.4 ~ 0.8, step is step_width
							// set aspect range to 0.4 ~ 2.5, step is 0.1
							for (double roof_w_ratio_v2 = 0.4; roof_w_ratio_v2 < 0.8; roof_w_ratio_v2 += step_width){
								for (double aspect_ratio_v2 = 0.4; aspect_ratio_v2 < 2.5; aspect_ratio_v2 += step_aspect){
									Config roof_flat_paras_v2(center_x_ratio_v2, center_y_ratio_v2, roof_w_ratio_v2, aspect_ratio_v2, 0, RoofTypes::FLAT, false, 0.0, 1.0);
									cv::Mat roof_img_test_v2 = RoofA::generateRoof(width, height, roof_flat_paras_v2, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
									if (roof_img_test_v2.empty()){
										continue;
									}

									// the third node
									for (double center_x_ratio_v3 = 0.3; center_x_ratio_v3 < 0.8; center_x_ratio_v3 += step_width){
										for (double center_y_ratio_v3 = 0.3; center_y_ratio_v3 < 0.8; center_y_ratio_v3 += step_height){
											// set roof width range to 0.4 ~ 0.8, step is step_width
											// set aspect range to 0.4 ~ 2.5, step is 0.1
											for (double roof_w_ratio_v3 = 0.4; roof_w_ratio_v3 < 0.8; roof_w_ratio_v3 += step_width){
												for (double aspect_ratio_v3 = 0.4; aspect_ratio_v3 < 2.5; aspect_ratio_v3 += step_aspect){
													Config roof_flat_paras_v3(center_x_ratio_v3, center_y_ratio_v3, roof_w_ratio_v3, aspect_ratio_v3, 0, RoofTypes::FLAT, false, 0.0, 1.0);
													cv::Mat roof_img_test_v3 = RoofA::generateRoof(width, height, roof_flat_paras_v3, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
													if (roof_img_test_v3.empty()){
														continue;
													}

													std::vector<Config> roof_paras;
													roof_paras.push_back(roof_flat_paras_v1);
													roof_paras.push_back(roof_flat_paras_v2);
													roof_paras.push_back(roof_flat_paras_v3);
													cv::Mat roof_img = RoofC::generateRoof(width, height, roof_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255), type, false);
													if (!roof_img.empty()){
														char buffer[50];
														sprintf(buffer, "roof_image_%08d.png", index);
														std::string img_filename = "../data/node_three_three/" + std::string(buffer);
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

void test_four_nodes(int width, int height, int type){
	double step_width = 6.0 / width;
	double step_height = 6.0 / height;
	double step_aspect = 0.2;
	int index = 0;
	for (double center_x_ratio_v1 = 0.3; center_x_ratio_v1 < 0.8; center_x_ratio_v1 += step_width){
		for (double center_y_ratio_v1 = 0.3; center_y_ratio_v1 < 0.8; center_y_ratio_v1 += step_height){
			// set roof width range to 0.4 ~ 0.8, step is step_width
			// set aspect range to 0.4 ~ 2.5, step is 0.1
			for (double roof_w_ratio_v1 = 0.4; roof_w_ratio_v1 < 0.8; roof_w_ratio_v1 += step_width){
				for (double aspect_ratio_v1 = 0.4; aspect_ratio_v1 < 2.5; aspect_ratio_v1 += step_aspect){

					Config roof_flat_paras_v1(center_x_ratio_v1, center_y_ratio_v1, roof_w_ratio_v1, aspect_ratio_v1, 0, RoofTypes::FLAT, false, 0.0, 1.0);
					cv::Mat roof_img_test_v1 = RoofA::generateRoof(width, height, roof_flat_paras_v1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
					if (roof_img_test_v1.empty()){
						continue;
					}
					// the second node
					for (double center_x_ratio_v2 = 0.3; center_x_ratio_v2 < 0.8; center_x_ratio_v2 += step_width){
						for (double center_y_ratio_v2 = 0.3; center_y_ratio_v2 < 0.8; center_y_ratio_v2 += step_height){
							// set roof width range to 0.4 ~ 0.8, step is step_width
							// set aspect range to 0.4 ~ 2.5, step is 0.1
							for (double roof_w_ratio_v2 = 0.4; roof_w_ratio_v2 < 0.8; roof_w_ratio_v2 += step_width){
								for (double aspect_ratio_v2 = 0.4; aspect_ratio_v2 < 2.5; aspect_ratio_v2 += step_aspect){
									Config roof_flat_paras_v2(center_x_ratio_v2, center_y_ratio_v2, roof_w_ratio_v2, aspect_ratio_v2, 0, RoofTypes::FLAT, false, 0.0, 1.0);
									cv::Mat roof_img_test_v2 = RoofA::generateRoof(width, height, roof_flat_paras_v2, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
									if (roof_img_test_v2.empty()){
										continue;
									}

									// the third node
									for (double center_x_ratio_v3 = 0.3; center_x_ratio_v3 < 0.8; center_x_ratio_v3 += step_width){
										for (double center_y_ratio_v3 = 0.3; center_y_ratio_v3 < 0.8; center_y_ratio_v3 += step_height){
											// set roof width range to 0.4 ~ 0.8, step is step_width
											// set aspect range to 0.4 ~ 2.5, step is 0.1
											for (double roof_w_ratio_v3 = 0.4; roof_w_ratio_v3 < 0.8; roof_w_ratio_v3 += step_width){
												for (double aspect_ratio_v3 = 0.4; aspect_ratio_v3 < 2.5; aspect_ratio_v3 += step_aspect){
													Config roof_flat_paras_v3(center_x_ratio_v3, center_y_ratio_v3, roof_w_ratio_v3, aspect_ratio_v3, 0, RoofTypes::FLAT, false, 0.0, 1.0);
													cv::Mat roof_img_test_v3 = RoofA::generateRoof(width, height, roof_flat_paras_v3, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
													if (roof_img_test_v3.empty()){
														continue;
													}



													// the fourth node
													for (double center_x_ratio_v4 = 0.3; center_x_ratio_v4 < 0.8; center_x_ratio_v4 += step_width){
														for (double center_y_ratio_v4 = 0.3; center_y_ratio_v4 < 0.8; center_y_ratio_v4 += step_height){
															// set roof width range to 0.4 ~ 0.8, step is step_width
															// set aspect range to 0.4 ~ 2.5, step is 0.1
															for (double roof_w_ratio_v4 = 0.4; roof_w_ratio_v4 < 0.8; roof_w_ratio_v4 += step_width){
																for (double aspect_ratio_v4 = 0.4; aspect_ratio_v4 < 2.5; aspect_ratio_v4 += step_aspect){
																	Config roof_flat_paras_v4(center_x_ratio_v4, center_y_ratio_v4, roof_w_ratio_v4, aspect_ratio_v4, 0, RoofTypes::FLAT, false, 0.0, 1.0);
																	cv::Mat roof_img_test_v4 = RoofA::generateRoof(width, height, roof_flat_paras_v4, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
																	if (roof_img_test_v4.empty()){
																		continue;
																	}
																	std::vector<Config> roof_paras;
																	roof_paras.push_back(roof_flat_paras_v1);
																	roof_paras.push_back(roof_flat_paras_v2);
																	roof_paras.push_back(roof_flat_paras_v3);
																	roof_paras.push_back(roof_flat_paras_v4);
																	cv::Mat roof_img = RoofD::generateRoof(width, height, roof_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255), type, false);
																	if (!roof_img.empty()){
																		char buffer[50];
																		sprintf(buffer, "roof_image_%08d.png", index);
																		std::string img_filename = "../data/node_four_one/" + std::string(buffer);
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
			}
		}
	}
}


