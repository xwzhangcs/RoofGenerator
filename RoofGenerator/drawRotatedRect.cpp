#include "drawRotatedRect.h"
#include "Utils.h"
#include "roofTypes.h"
const float M_PI = 3.1415926535;

void DrawRotatedRect::generateRect(cv::Mat & roof_img, int center_x, int center_y, int roofWidth, int roofHeight, double rotate, int selected_roof_type, const cv::Scalar& bg_color, const cv::Scalar& fg_color){
	int imageRoofWidth = roofWidth;
	int imageRoofHeight = roofHeight;
	int center_w = center_x;
	int center_h = center_y;
	int thickness = 1;
	if (selected_roof_type == RoofTypes::FLAT){
		cv::Point2f center(center_w, center_h);
		cv::RotatedRect rRect = cv::RotatedRect(center, cv::Size2f(imageRoofWidth, imageRoofHeight), rotate);
		cv::Point2f vertices[4];
		rRect.points(vertices);
		// add sides
		for (int i = 0; i < 4; i++)
			line(roof_img, vertices[i], vertices[(i + 1) % 4], fg_color, thickness);
	}
	else{
		// do nothing
	}
}

void DrawRotatedRect::generateRect(cv::Mat & roof_img, int center_x, int center_y, int roofWidth, int roofHeight, double rotate, int selected_roof_type, bool bRidgeDis, double ridgeDisRatio, int ridgeLength, const cv::Scalar& bg_color, const cv::Scalar& fg_color){
	int imageRoofWidth = roofWidth;
	int imageRoofHeight = roofHeight;
	int center_w = center_x;
	int center_h = center_y;
	int thickness = 1;
	if (selected_roof_type == RoofTypes::GABLE || selected_roof_type == RoofTypes::HIP){
		// draw rect
		cv::Point2f center(center_w, center_h);
		cv::RotatedRect rRect = cv::RotatedRect(center, cv::Size2f(imageRoofWidth, imageRoofHeight), rotate);
		cv::Point2f vertices[4];
		rRect.points(vertices);
		// add sides
		for (int i = 0; i < 4; i++)
			line(roof_img, vertices[i], vertices[(i + 1) % 4], fg_color, thickness);
		// add ridge
		int ridge_length = ridgeLength;
		int ridge_x1 = center_w - ridge_length * 0.5;
		int ridge_y1 = center_h;
		int ridge_x2 = ridge_x1 + ridge_length;
		int ridge_y2 = center_h;
		if (bRidgeDis) {
			ridge_y1 += utils::genRand(-imageRoofHeight * ridgeDisRatio, imageRoofHeight * ridgeDisRatio);
			ridge_y2 += utils::genRand(-imageRoofHeight * ridgeDisRatio, imageRoofHeight * ridgeDisRatio);
		}
		cv::Point2f ridge_left = utils::RotatePoint(center, cv::Point2f(ridge_x1, ridge_y1), (rotate)* M_PI / 180.0);
		cv::Point2f ridge_right = utils::RotatePoint(center, cv::Point2f(ridge_x2, ridge_y2), (rotate)* M_PI / 180.0);
		cv::line(roof_img, ridge_left, ridge_right, fg_color, thickness);
		if (selected_roof_type == RoofTypes::HIP)
		{
			// connect other roof edges
			cv::line(roof_img, vertices[0], ridge_left, fg_color, thickness);
			cv::line(roof_img, vertices[1], ridge_left, fg_color, thickness);
			cv::line(roof_img, vertices[2], ridge_right, fg_color, thickness);
			cv::line(roof_img, vertices[3], ridge_right, fg_color, thickness);
		}
	}
	else{
		// do nothing
	}
}
