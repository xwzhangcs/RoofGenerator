#include "roofB.h"
#include "drawRotatedRect.h"
#include "Utils.h"
#include "roofTypes.h"

cv::Mat RoofB::generateRoof(int width, int height, const Config& roof_paras_main, const Config& roof_paras_sub, const cv::Scalar& bg_color, const cv::Scalar& fg_color, int type, bool bDebug){
	if (roof_paras_main.roofAspect * roof_paras_main.roofWidth_ratio >= 1.0)
		return cv::Mat();
	if (roof_paras_sub.roofAspect * roof_paras_sub.roofWidth_ratio >= 1.0)
		return cv::Mat();
	
	cv::Mat result(height, width, CV_8UC3, bg_color);
	int imageRoofWidth_main = roof_paras_main.roofWidth_ratio * width;
	int imageRoofHeight_main = imageRoofWidth_main * roof_paras_main.roofAspect;
	int center_w_main = width * roof_paras_main.center_x_ratio;
	int center_h_main = height * roof_paras_main.center_y_ratio;
	double rotate_main = roof_paras_main.rotate;

	int imageRoofWidth_sub = roof_paras_sub.roofWidth_ratio * width;
	int imageRoofHeight_sub = imageRoofWidth_sub * roof_paras_sub.roofAspect;
	int center_w_sub = width * roof_paras_sub.center_x_ratio;
	int center_h_sub = height * roof_paras_sub.center_y_ratio;
	double rotate_sub = roof_paras_sub.rotate;

	if (bDebug){
		std::cout << "main (" << center_w_main << ", " << center_h_main << ", " << imageRoofWidth_main << ", " << imageRoofHeight_main << ", " << rotate_main << ")" << std::endl;
		std::cout << "sub (" << center_w_sub << ", " << center_h_sub << ", " << imageRoofWidth_sub << ", " << imageRoofHeight_sub << ", " << rotate_sub << ")" << std::endl;
	}

	if (imageRoofWidth_main * imageRoofHeight_main > 5 * imageRoofWidth_sub * imageRoofHeight_sub)
		return cv::Mat();
	if (imageRoofWidth_sub * imageRoofHeight_sub > 5 * imageRoofWidth_main * imageRoofHeight_main)
		return cv::Mat();

	// first check out of boundary
	if (!utils::rectInsideRect(width, height, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main)){
		return cv::Mat();
	}
	if (!utils::rectInsideRect(width, height, center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub)){
		return cv::Mat();
	}
	// second check connectivity 
	if (type == 0) // two independent nodes
	{
		if (utils::rectIntersecRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub))
			return cv::Mat();
		if (utils::rectIntersecRect(center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();
	}
	else if (type == 1) // connected
	{
		if (!utils::rectIntersecRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub))
			return cv::Mat();
		if (!utils::rectIntersecRect(center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();
		if (utils::rectInsideRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub))
			return cv::Mat();
		if (utils::rectInsideRect(center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();
		
		// special L or T
		if (!utils::rectSideBySideRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub))
			return cv::Mat();
		if (!utils::rectSideBySideRect(center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();
	}
	else{

	}
	
	// add main roof
	int thickness = 2;
	if (roof_paras_main.selected_roof_type == RoofTypes::FLAT){
		DrawRotatedRect::generateRect(result, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, roof_paras_main.selected_roof_type, bg_color, fg_color);
	}
	else if (roof_paras_main.selected_roof_type == RoofTypes::GABLE || roof_paras_main.selected_roof_type == RoofTypes::HIP){
		int ridge_length = roof_paras_main.ridgeRatio * imageRoofWidth_main;
		DrawRotatedRect::generateRect(result, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, roof_paras_main.selected_roof_type, roof_paras_main.bRidgeDis, roof_paras_main.ridgeDisRatio, ridge_length, bg_color, fg_color);
	}
	else{
		// do nothing
	}

	// add sub roof
	if (roof_paras_sub.selected_roof_type == RoofTypes::FLAT){
		DrawRotatedRect::generateRect(result, center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub, roof_paras_sub.selected_roof_type, bg_color, fg_color);
	}
	else if (roof_paras_sub.selected_roof_type == RoofTypes::GABLE || roof_paras_sub.selected_roof_type == RoofTypes::HIP){
		int ridge_length = roof_paras_sub.ridgeRatio * imageRoofWidth_sub;
		DrawRotatedRect::generateRect(result, center_w_sub, center_h_sub, imageRoofWidth_sub, imageRoofHeight_sub, rotate_sub, roof_paras_sub.selected_roof_type, roof_paras_sub.bRidgeDis, roof_paras_sub.ridgeDisRatio, ridge_length, bg_color, fg_color);
	}
	else{
		// do nothing
	}

	return result;
}