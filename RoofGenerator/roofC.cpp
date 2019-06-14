#include "roofC.h"
#include "drawRotatedRect.h"
#include "Utils.h"
#include "roofTypes.h"

cv::Mat RoofC::generateRoof(int width, int height, const Config& roof_paras_main, const Config& roof_paras_sub_v1, const Config& roof_paras_sub_v2, const cv::Scalar& bg_color, const cv::Scalar& fg_color, int type, bool bDebug){
	if (roof_paras_main.roofAspect * roof_paras_main.roofWidth_ratio >= 1.0)
		return cv::Mat();
	if (roof_paras_sub_v1.roofAspect * roof_paras_sub_v1.roofWidth_ratio >= 1.0)
		return cv::Mat();
	if (roof_paras_sub_v2.roofAspect * roof_paras_sub_v2.roofWidth_ratio >= 1.0)
		return cv::Mat();
	
	cv::Mat result(height, width, CV_8UC3, bg_color);
	int imageRoofWidth_main = roof_paras_main.roofWidth_ratio * width;
	int imageRoofHeight_main = imageRoofWidth_main * roof_paras_main.roofAspect;
	int center_w_main = width * roof_paras_main.center_x_ratio;
	int center_h_main = height * roof_paras_main.center_y_ratio;
	double rotate_main = roof_paras_main.rotate;

	int imageRoofWidth_sub_v1 = roof_paras_sub_v1.roofWidth_ratio * width;
	int imageRoofHeight_sub_v1 = imageRoofWidth_sub_v1 * roof_paras_sub_v1.roofAspect;
	int center_w_sub_v1 = width * roof_paras_sub_v1.center_x_ratio;
	int center_h_sub_v1 = height * roof_paras_sub_v1.center_y_ratio;
	double rotate_sub_v1 = roof_paras_sub_v1.rotate;

	int imageRoofWidth_sub_v2 = roof_paras_sub_v2.roofWidth_ratio * width;
	int imageRoofHeight_sub_v2 = imageRoofWidth_sub_v2 * roof_paras_sub_v2.roofAspect;
	int center_w_sub_v2 = width * roof_paras_sub_v2.center_x_ratio;
	int center_h_sub_v2 = height * roof_paras_sub_v2.center_y_ratio;
	double rotate_sub_v2 = roof_paras_sub_v2.rotate;

	if (bDebug){
		std::cout << "main (" << center_w_main << ", " << center_h_main << ", " << imageRoofWidth_main << ", " << imageRoofHeight_main << ", " << rotate_main << ")" << std::endl;
	}

	if (imageRoofWidth_main * imageRoofHeight_main > 5 * imageRoofWidth_sub_v1 * imageRoofHeight_sub_v1 || imageRoofWidth_main * imageRoofHeight_main > 5 * imageRoofWidth_sub_v2 * imageRoofHeight_sub_v2)
		return cv::Mat();
	if (imageRoofWidth_sub_v1 * imageRoofHeight_sub_v1 > 5 * imageRoofWidth_main * imageRoofHeight_main || imageRoofWidth_sub_v1 * imageRoofHeight_sub_v1 > 5 * imageRoofWidth_sub_v2 * imageRoofHeight_sub_v2)
		return cv::Mat();
	if (imageRoofWidth_sub_v2 * imageRoofHeight_sub_v2 > 5 * imageRoofWidth_main * imageRoofHeight_main || imageRoofWidth_sub_v2 * imageRoofHeight_sub_v2 > 5 * imageRoofWidth_sub_v1 * imageRoofHeight_sub_v1)
		return cv::Mat();

	// first check out of boundary
	if (!utils::rectInsideRect(width, height, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main)){
		return cv::Mat();
	}
	if (!utils::rectInsideRect(width, height, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1)){
		return cv::Mat();
	}
	if (!utils::rectInsideRect(width, height, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2)){
		return cv::Mat();
	}

	// second check connectivity 
	if (type == 0) // two independent nodes
	{
		if (utils::rectIntersecRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1))
			return cv::Mat();
		if (utils::rectIntersecRect(center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();

		if (utils::rectIntersecRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2))
			return cv::Mat();
		if (utils::rectIntersecRect(center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();

		if (utils::rectIntersecRect(center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1))
			return cv::Mat();
		if (utils::rectIntersecRect(center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2))
			return cv::Mat();

	}
	else if (type == 1) // connected
	{
		if (utils::rectIntersecRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2))
			return cv::Mat();
		if (utils::rectIntersecRect(center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();

		if (utils::rectIntersecRect(center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1))
			return cv::Mat();
		if (utils::rectIntersecRect(center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2))
			return cv::Mat();

		if (!utils::rectIntersecRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1))
			return cv::Mat();
		if (!utils::rectIntersecRect(center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();
		if (utils::rectInsideRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1))
			return cv::Mat();
		if (utils::rectInsideRect(center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();

		// special L or T
		if (!utils::rectSideBySideRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1))
			return cv::Mat();
		if (!utils::rectSideBySideRect(center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();
	}
	else if (type == 2){
		if (utils::rectIntersecRect(center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1))
			return cv::Mat();
		if (utils::rectIntersecRect(center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2))
			return cv::Mat();

		if (!utils::rectIntersecRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1))
			return cv::Mat();
		if (!utils::rectIntersecRect(center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();
		if (!utils::rectIntersecRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2))
			return cv::Mat();
		if (!utils::rectIntersecRect(center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();

		if (utils::rectInsideRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1))
			return cv::Mat();
		if (utils::rectInsideRect(center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();

		// special L or T
		if (!utils::rectSideBySideRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1))
			return cv::Mat();
		if (!utils::rectSideBySideRect(center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();


		if (utils::rectInsideRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2))
			return cv::Mat();
		if (utils::rectInsideRect(center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();

		// special L or T
		if (!utils::rectSideBySideRect(center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2))
			return cv::Mat();
		if (!utils::rectSideBySideRect(center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2, center_w_main, center_h_main, imageRoofWidth_main, imageRoofHeight_main, rotate_main))
			return cv::Mat();

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

	// add sub v1 roof
	if (roof_paras_sub_v1.selected_roof_type == RoofTypes::FLAT){
		DrawRotatedRect::generateRect(result, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, roof_paras_sub_v1.selected_roof_type, bg_color, fg_color);
	}
	else if (roof_paras_sub_v1.selected_roof_type == RoofTypes::GABLE || roof_paras_sub_v1.selected_roof_type == RoofTypes::HIP){
		int ridge_length = roof_paras_sub_v1.ridgeRatio * imageRoofWidth_sub_v1;
		DrawRotatedRect::generateRect(result, center_w_sub_v1, center_h_sub_v1, imageRoofWidth_sub_v1, imageRoofHeight_sub_v1, rotate_sub_v1, roof_paras_sub_v1.selected_roof_type, roof_paras_sub_v1.bRidgeDis, roof_paras_sub_v1.ridgeDisRatio, ridge_length, bg_color, fg_color);
	}
	else{
		// do nothing
	}

	// add sub v2 roof
	if (roof_paras_sub_v2.selected_roof_type == RoofTypes::FLAT){
		DrawRotatedRect::generateRect(result, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2, roof_paras_sub_v2.selected_roof_type, bg_color, fg_color);
	}
	else if (roof_paras_sub_v1.selected_roof_type == RoofTypes::GABLE || roof_paras_sub_v1.selected_roof_type == RoofTypes::HIP){
		int ridge_length = roof_paras_sub_v2.ridgeRatio * imageRoofWidth_sub_v2;
		DrawRotatedRect::generateRect(result, center_w_sub_v2, center_h_sub_v2, imageRoofWidth_sub_v2, imageRoofHeight_sub_v2, rotate_sub_v2, roof_paras_sub_v2.selected_roof_type, roof_paras_sub_v2.bRidgeDis, roof_paras_sub_v2.ridgeDisRatio, ridge_length, bg_color, fg_color);
	}
	else{
		// do nothing
	}

	return result;
}