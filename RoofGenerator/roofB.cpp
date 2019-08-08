#include "roofB.h"
#include "drawRotatedRect.h"
#include "Utils.h"
#include "roofTypes.h"

cv::Mat RoofB::generateRoof(int width, int height, const std::vector<Config>& roof_paras, const cv::Scalar& bg_color, int padding, const cv::Scalar& fg_color, int type, bool bDebug){
	std::vector<cv::Scalar> fg_color_set; // bgr
	fg_color_set.push_back(cv::Scalar(0, 0, 255)); // red
	fg_color_set.push_back(cv::Scalar(0, 255, 0)); // green
	fg_color_set.push_back(cv::Scalar(0, 255, 255)); // yellow
	fg_color_set.push_back(cv::Scalar(0, 165, 255)); // orange
	fg_color_set.push_back(cv::Scalar(128, 128, 128)); // grey
	fg_color_set.push_back(cv::Scalar(255, 255, 0)); // cyan
	fg_color_set.push_back(cv::Scalar(255, 0, 0)); // blue

	for (int i = 0; i < roof_paras.size(); i++)
		if (roof_paras[i].roofAspect * roof_paras[i].roofWidth_ratio > 1.0)
			return cv::Mat();

	cv::Mat result(height, width, CV_8UC3, bg_color);
	std::vector<int> imageRoofWidth;
	imageRoofWidth.resize(roof_paras.size());
	std::vector<int> imageRoofHeight;
	imageRoofHeight.resize(roof_paras.size());
	std::vector<int> center_w;
	center_w.resize(roof_paras.size());
	std::vector<int> center_h;
	center_h.resize(roof_paras.size());
	std::vector<double> rotate;
	rotate.resize(roof_paras.size());
	for (int i = 0; i < roof_paras.size(); i++){
		imageRoofWidth[i] = roof_paras[i].roofWidth_ratio * width;
		imageRoofHeight[i] = imageRoofWidth[i] * roof_paras[i].roofAspect;
		center_w[i] = width * roof_paras[i].center_x_ratio;
		center_h[i] = height * roof_paras[i].center_y_ratio;
		rotate[i] = roof_paras[i].rotate;
	}

	// it's better that each rectangle is not 5 times bigger than other rectangles
	for (int i = 0; i < roof_paras.size(); i++){
		for (int j = i + 1; j < roof_paras.size(); j++){
			if (imageRoofWidth[i] * imageRoofHeight[i] > 5 * imageRoofWidth[j] * imageRoofHeight[j] || imageRoofWidth[i] * imageRoofHeight[i] < 0.2 * imageRoofWidth[j] * imageRoofHeight[j])
				return cv::Mat();
		}
	}

	// first check out of boundary
	for (int i = 0; i < roof_paras.size(); i++){
		if (!utils::rectInsideRect(width, height, center_w[i], center_h[i], imageRoofWidth[i], imageRoofHeight[i], rotate[i])){
			return cv::Mat();
		}
	}

	// second check connectivity 
	if (type == 0) // two independent nodes
	{
		// don't consider for now
	}
	else if (type == 1) // connected
	{
		for (int i = 0; i < roof_paras.size(); i++){
			for (int j = i + 1; j < roof_paras.size(); j++){
				//std::cout << center_w[i] << ", " << center_h[i] << ", " << ", " << imageRoofWidth[i] << ", " << imageRoofHeight[i] << ", " << rotate[i] << std::endl;
				//std::cout << center_w[j] << ", " << center_h[j] << ", " << ", " << imageRoofWidth[j] << ", " << imageRoofHeight[j] << ", " << rotate[j] << std::endl;
				bool intersec_a = !utils::rectIntersecRect(center_w[i], center_h[i], imageRoofWidth[i], imageRoofHeight[i], rotate[i], center_w[j], center_h[j], imageRoofWidth[j], imageRoofHeight[j], rotate[j]);
				bool intersec_b = !utils::rectIntersecRect(center_w[j], center_h[j], imageRoofWidth[j], imageRoofHeight[j], rotate[j], center_w[i], center_h[i], imageRoofWidth[i], imageRoofHeight[i], rotate[i]);
				if (intersec_a && intersec_b)
					return cv::Mat();
				if (utils::rectInsideRect(center_w[i], center_h[i], imageRoofWidth[i], imageRoofHeight[i], rotate[i], center_w[j], center_h[j], imageRoofWidth[j], imageRoofHeight[j], rotate[j]))
					return cv::Mat();
				if (utils::rectInsideRect(center_w[j], center_h[j], imageRoofWidth[j], imageRoofHeight[j], rotate[j], center_w[i], center_h[i], imageRoofWidth[i], imageRoofHeight[i], rotate[i]))
					return cv::Mat();
			}
		}
	}
	else{

	}
	
	int thickness = 2;
	for (int i = 0; i < roof_paras.size(); i++){
		if (roof_paras[i].selected_roof_type == RoofTypes::FLAT){
			DrawRotatedRect::generateRect(result, padding, center_w[i], center_h[i], imageRoofWidth[i], imageRoofHeight[i], rotate[i], roof_paras[i].selected_roof_type, bg_color, fg_color_set[i]);
		}
		else if (roof_paras[i].selected_roof_type == RoofTypes::GABLE || roof_paras[i].selected_roof_type == RoofTypes::HIP){
			// don't consider for now
		}
		else{
			// do nothing
		}
	}

	return result;
}