#include "roofA.h"
#include "drawRotatedRect.h"
#include "Utils.h"
#include "roofTypes.h"

cv::Mat RoofA::generateRoof(int width, int height, const Config& roof_paras, int padding, const cv::Scalar& bg_color, const cv::Scalar& fg_color){
	if (roof_paras.roofAspect * roof_paras.roofWidth_ratio >= 1.0)
		return cv::Mat();
	cv::Mat result(height, width, CV_8UC3, bg_color);
	int imageRoofWidth = roof_paras.roofWidth_ratio * width;
	double imageRoofAspect = roof_paras.roofAspect;
	int imageRoofHeight = imageRoofWidth * imageRoofAspect;
	int center_w = width * roof_paras.center_x_ratio;
	int center_h = height * roof_paras.center_y_ratio;
	double ridgeDisRatio = roof_paras.ridgeDisRatio;
	// check 
	if (roof_paras.center_x_ratio + roof_paras.roofWidth_ratio * 0.5 > 1.0 || roof_paras.center_x_ratio < roof_paras.roofWidth_ratio * 0.5)
		return cv::Mat();
	if (roof_paras.center_y_ratio + roof_paras.roofWidth_ratio * roof_paras.roofAspect * 0.5 > 1.0 || roof_paras.center_y_ratio < roof_paras.roofWidth_ratio * roof_paras.roofAspect * 0.5)
		return cv::Mat();

	if (roof_paras.selected_roof_type == RoofTypes::FLAT){
		DrawRotatedRect::generateRect(result, padding, center_w, center_h, imageRoofWidth, imageRoofHeight, roof_paras.rotate, roof_paras.selected_roof_type, bg_color, fg_color);
	}
	else if (roof_paras.selected_roof_type == RoofTypes::GABLE || roof_paras.selected_roof_type == RoofTypes::HIP){
		int ridge_length = roof_paras.ridgeRatio * imageRoofWidth;
		DrawRotatedRect::generateRect(result, padding, center_w, center_h, imageRoofWidth, imageRoofHeight, roof_paras.rotate, roof_paras.selected_roof_type, roof_paras.bRidgeDis, 0.10, ridge_length, bg_color, fg_color);
	}
	else{
		// do nothing
	}
	return result;
}