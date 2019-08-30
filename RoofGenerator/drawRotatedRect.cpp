#include "drawRotatedRect.h"
#include "Utils.h"
#include "roofTypes.h"
const float M_PI = 3.1415926535;

void DrawRotatedRect::generateRect(cv::Mat & roof_img, int padding, int center_x, int center_y, int roofWidth, int roofHeight, double rotate, int selected_roof_type, const cv::Scalar& bg_color, const cv::Scalar& fg_color){
	int imageRoofWidth = roofWidth;
	int imageRoofHeight = roofHeight;
	int center_w = center_x;
	int center_h = center_y;
	int thickness = 1;
	int width = roof_img.size().width + padding * 2;
	int height = roof_img.size().height + padding * 2;
	cv::resize(roof_img, roof_img, cv::Size(width, height));
	if (selected_roof_type == RoofTypes::FLAT){
		cv::Point2f center(center_w + padding, center_h + padding);
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

void DrawRotatedRect::generateRect(cv::Mat & roof_img, int padding, std::vector<std::vector<int>> roof_paras, int selected_roof_type, const cv::Scalar& bg_color, const cv::Scalar& fg_color){
	std::vector<cv::Scalar> fg_color_set; // bgr
	fg_color_set.push_back(cv::Scalar(0, 0, 255)); // red
	fg_color_set.push_back(cv::Scalar(0, 255, 0)); // green
	fg_color_set.push_back(cv::Scalar(0, 255, 255)); // yellow
	fg_color_set.push_back(cv::Scalar(0, 165, 255)); // orange
	fg_color_set.push_back(cv::Scalar(128, 128, 128)); // grey
	fg_color_set.push_back(cv::Scalar(255, 255, 0)); // cyan
	fg_color_set.push_back(cv::Scalar(255, 0, 0)); // blue
	fg_color_set.push_back(cv::Scalar(255, 255, 255)); // white
	int thickness = 1;
	int width = roof_img.size().width + padding * 2;
	int height = roof_img.size().height + padding * 2;
	cv::resize(roof_img, roof_img, cv::Size(width, height));
	int step_dis = 2;
	if (selected_roof_type == RoofTypes::FLAT){
		for (int index = 0; index < roof_paras.size(); index++){
			int center_w = roof_paras[index][0] + utils::genRand(-step_dis, step_dis);
			int center_h = roof_paras[index][1] + utils::genRand(-step_dis, step_dis);
			int imageRoofWidth = roof_paras[index][2] + utils::genRand(-step_dis, step_dis);
			int imageRoofHeight = roof_paras[index][3] + utils::genRand(-step_dis, step_dis);
			if (center_w - 0.5 * imageRoofWidth < 0 || center_w + 0.5 * imageRoofWidth > roof_img.size().width){
				center_w = roof_paras[index][0];
				imageRoofWidth = roof_paras[index][2];
			}
			if (center_h - 0.5 * imageRoofHeight < 0 || center_h + 0.5 * imageRoofHeight > roof_img.size().height){
				center_h = roof_paras[index][1];
				imageRoofHeight = roof_paras[index][3];
			}

			cv::Point2f center(center_w + padding, center_h + padding);
			cv::RotatedRect rRect = cv::RotatedRect(center, cv::Size2f(imageRoofWidth, imageRoofHeight), roof_paras[index][4]);
			cv::Point2f vertices[4];
			rRect.points(vertices);
			//// add sides
			/*for (int i = 0; i < 4; i++)
				line(roof_img, vertices[i], vertices[(i + 1) % 4], fg_color_set[index], thickness);*/
			cv::rectangle(roof_img, vertices[1], vertices[3], fg_color_set[7], -1);
			// add noise

			if (false)
			{
				float threshold = 0.50;
				int kernel = 3;
				// collect all points on the boundaries
				std::vector<cv::Point> points;
				for (int i = vertices[1].y; i <= vertices[0].y; i += 2 * kernel)
					points.push_back(cv::Point(vertices[0].x, i));
				for (int i = vertices[2].y; i <= vertices[3].y; i += 2 * kernel)
					points.push_back(cv::Point(vertices[2].x, i));
				for (int i = vertices[1].x; i <= vertices[2].x; i += 2 * kernel)
					points.push_back(cv::Point(i, vertices[1].y));
				for (int i = vertices[0].x; i <= vertices[3].x; i += 2 * kernel)
					points.push_back(cv::Point(i, vertices[0].y));
				for (int i = 0; i < points.size(); i++){
					kernel = utils::genRand(0, 4);
					if (utils::genRand(0.0, 1.0) > threshold){
						if ((int)utils::genRand(0, 2)  % 2 == 0){
							cv::circle(roof_img, points[i], kernel, cv::Scalar(255, 255, 255), -1);
						}
						else{
							cv::circle(roof_img, points[i], kernel, cv::Scalar(0, 0, 0), -1);
						}
					}
				}
				cv::GaussianBlur(roof_img, roof_img, cv::Size(3, 3), 0, 0);
			}
		}
	}
	else{
		// do nothing
	}
}

void DrawRotatedRect::generateRect(cv::Mat & roof_img, int padding, std::vector<std::vector<int>> roof_paras, int relationship, int selected_roof_type, const cv::Scalar& bg_color, const cv::Scalar& fg_color){
	std::vector<cv::Scalar> fg_color_set; // bgr
	fg_color_set.push_back(cv::Scalar(0, 0, 255)); // red
	fg_color_set.push_back(cv::Scalar(0, 255, 0)); // green
	fg_color_set.push_back(cv::Scalar(0, 255, 255)); // yellow
	fg_color_set.push_back(cv::Scalar(0, 165, 255)); // orange
	fg_color_set.push_back(cv::Scalar(128, 128, 128)); // grey
	fg_color_set.push_back(cv::Scalar(255, 255, 0)); // cyan
	fg_color_set.push_back(cv::Scalar(255, 0, 0)); // blue
	fg_color_set.push_back(cv::Scalar(255, 255, 255)); // white
	int thickness = 1;
	int width = roof_img.size().width + padding * 2;
	int height = roof_img.size().height + padding * 2;
	cv::resize(roof_img, roof_img, cv::Size(width, height));

	for (int index = 0; index < roof_paras.size(); index++){
		int center_w = roof_paras[index][0];
		int center_h = roof_paras[index][1];
		int imageRoofWidth = roof_paras[index][2];
		int imageRoofHeight = roof_paras[index][3];
		cv::Point2f center(center_w + padding, center_h + padding);
		cv::RotatedRect rRect = cv::RotatedRect(center, cv::Size2f(imageRoofWidth, imageRoofHeight), roof_paras[index][4]);
		cv::Point2f vertices[4];
		rRect.points(vertices);
		//// add sides
		for (int i = 0; i < 4; i++)
			cv::line(roof_img, vertices[i], vertices[(i + 1) % 4], fg_color_set[7], thickness);
	}
	// clean up edges
	int top_w = roof_paras[0][0] - 0.5 * roof_paras[0][2] + padding;
	int top_h = roof_paras[0][1] - 0.5 * roof_paras[0][3] + padding;
	int bot_w = top_w + roof_paras[0][2];
	int bot_h = top_h + roof_paras[0][3];
	int top_w_v1 = roof_paras[1][0] - 0.5 * roof_paras[1][2] + padding;
	int top_h_v1 = roof_paras[1][1] - 0.5 * roof_paras[1][3] + padding;
	int bot_w_v1 = top_w_v1 + roof_paras[1][2];
	int bot_h_v1 = top_h_v1 + roof_paras[1][3];
	int index_sub = utils::rectSub_L(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
	cv::Point point_1, point_2, point_3, point_4;
	// compute 2 critical points
	if (index_sub == 1){
		int imageRoofWidth = roof_paras[index_sub][2];
		int imageRoofHeight = roof_paras[index_sub][3];
		point_1 = utils::vertInsideRect_L(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
		point_2 = utils::vertOnRect_L(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
		if (imageRoofWidth > imageRoofHeight){
			point_3.x = point_2.x;
			point_3.y = point_1.y;
			point_4.x = point_1.x;
			point_4.y = point_2.y;
		}
		else{
			point_3.x = point_1.x;
			point_3.y = point_2.y;
			point_4.x = point_2.x;
			point_4.y = point_1.y;
		}
		/*std::cout << "point_1 is " << point_1 << std::endl;
		std::cout << "point_2 is " << point_2 << std::endl;
		std::cout << "point_3 is " << point_3 << std::endl;
		std::cout << "point_4 is " << point_4 << std::endl;*/
		cv::line(roof_img, point_1, point_3, bg_color, thickness);
		cv::line(roof_img, point_1, point_4, bg_color, thickness);
		cv::line(roof_img, point_2, point_3, bg_color, thickness);
	}

	if (selected_roof_type == RoofTypes::GABLE){
		if (index_sub == 1){
			cv::Point mid = 0.5 * (point_1 + point_4);
			cv::line(roof_img, mid, point_2, fg_color, thickness);
			cv::line(roof_img, mid, point_3, fg_color, thickness);
		}
		else{
			//
		}
		for (int index = 0; index < roof_paras.size(); index++){
			int center_w = roof_paras[index][0];
			int center_h = roof_paras[index][1];
			int imageRoofWidth = roof_paras[index][2];
			int imageRoofHeight = roof_paras[index][3];
			int rotate = roof_paras[index][4];
			int ridgeLength = roof_paras[index][5];
			// draw rect
			cv::Point2f center(center_w + padding, center_h + padding);
			// add ridge
			int ridge_length = ridgeLength;
			int ridge_x1 = imageRoofWidth > imageRoofHeight ? center.x - ridge_length * 0.5 : center.x;
			int ridge_y1 = imageRoofWidth > imageRoofHeight ? center.y : center.y - ridge_length * 0.5;
			int ridge_x2 = imageRoofWidth > imageRoofHeight ? ridge_x1 + ridge_length : center.x;
			int ridge_y2 = imageRoofWidth > imageRoofHeight ? center.y : ridge_y1 + ridge_length;
			cv::Point2f ridge_left = utils::RotatePoint(center, cv::Point2f(ridge_x1, ridge_y1), (rotate)* M_PI / 180.0);
			cv::Point2f ridge_right = utils::RotatePoint(center, cv::Point2f(ridge_x2, ridge_y2), (rotate)* M_PI / 180.0);
			cv::line(roof_img, ridge_left, ridge_right, fg_color, thickness);
		}
	}
	else if (selected_roof_type == RoofTypes::HIP){
		if (index_sub == 1){
			cv::Point mid = 0.5 * (point_1 + point_4);
			cv::line(roof_img, mid, point_2, fg_color, thickness);
			cv::line(roof_img, mid, point_3, fg_color, thickness);
		}
		for (int index = 0; index < roof_paras.size(); index++){
			int center_w = roof_paras[index][0];
			int center_h = roof_paras[index][1];
			int imageRoofWidth = roof_paras[index][2];
			int imageRoofHeight = roof_paras[index][3];
			int rotate = roof_paras[index][4];
			int ridgeLength = roof_paras[index][5];
			// draw rect
			cv::Point2f center(center_w + padding, center_h + padding);
			cv::RotatedRect rRect = cv::RotatedRect(center, cv::Size2f(imageRoofWidth, imageRoofHeight), roof_paras[index][4]);
			cv::Point2f vertices[4];
			rRect.points(vertices);
			// add ridge
			int ridge_length = ridgeLength;
			int ridge_x1 = imageRoofWidth > imageRoofHeight ? center.x - ridge_length * 0.5 : center.x;
			int ridge_y1 = imageRoofWidth > imageRoofHeight ? center.y : center.y - ridge_length * 0.5;
			int ridge_x2 = imageRoofWidth > imageRoofHeight ? ridge_x1 + ridge_length : center.x;
			int ridge_y2 = imageRoofWidth > imageRoofHeight ? center.y : ridge_y1 + ridge_length;
			cv::Point2f ridge_left = utils::RotatePoint(center, cv::Point2f(ridge_x1, ridge_y1), (rotate)* M_PI / 180.0);
			cv::Point2f ridge_right = utils::RotatePoint(center, cv::Point2f(ridge_x2, ridge_y2), (rotate)* M_PI / 180.0);
			cv::line(roof_img, ridge_left, ridge_right, fg_color, thickness);
			if (imageRoofWidth > imageRoofHeight){
				cv::line(roof_img, ridge_left, vertices[0], fg_color, thickness);
				cv::line(roof_img, ridge_left, vertices[1], fg_color, thickness);
				cv::line(roof_img, ridge_right, vertices[2], fg_color, thickness);
				cv::line(roof_img, ridge_right, vertices[3], fg_color, thickness);
				if (index == index_sub){
					// clean up more
					if (index_sub == 1){
						cv::Point mid = 0.5 * (point_1 + point_4);
						cv::line(roof_img, point_1, abs(ridge_left.x - mid.x) < abs(ridge_right.x - mid.x) ? ridge_left : ridge_right, bg_color, thickness);
						cv::line(roof_img, point_4, abs(ridge_left.x - mid.x) < abs(ridge_right.x - mid.x) ? ridge_left : ridge_right, bg_color, thickness);
						cv::line(roof_img, mid, abs(ridge_left.x - mid.x) < abs(ridge_right.x - mid.x) ? ridge_left : ridge_right, fg_color, thickness);
					}
				}
			}
			else{
				cv::line(roof_img, ridge_left, vertices[1], fg_color, thickness);
				cv::line(roof_img, ridge_left, vertices[2], fg_color, thickness);
				cv::line(roof_img, ridge_right, vertices[0], fg_color, thickness);
				cv::line(roof_img, ridge_right, vertices[3], fg_color, thickness);
				if (index == index_sub){
					// clean up more
					if (index_sub == 1){
						cv::Point mid = 0.5 * (point_1 + point_4);
						cv::line(roof_img, point_1, abs(ridge_left.y - mid.y) < abs(ridge_right.y - mid.y) ? ridge_left : ridge_right, bg_color, thickness);
						cv::line(roof_img, point_4, abs(ridge_left.y - mid.y) < abs(ridge_right.y - mid.y) ? ridge_left : ridge_right, bg_color, thickness);
						cv::line(roof_img, mid, abs(ridge_left.y - mid.y) < abs(ridge_right.y - mid.y) ? ridge_left : ridge_right, fg_color, thickness);
					}
				}
			}
		}
	}
	else{
		// do nothing
	}
}

void DrawRotatedRect::generateRect(cv::Mat & roof_img, int padding, int center_x, int center_y, int roofWidth, int roofHeight, double rotate, int selected_roof_type, bool bRidgeDis, double ridgeDisRatio, int ridgeLength, const cv::Scalar& bg_color, const cv::Scalar& fg_color){
	int imageRoofWidth = roofWidth;
	int imageRoofHeight = roofHeight;
	int center_w = center_x;
	int center_h = center_y;
	int thickness = 1;
	int width = roof_img.size().width + padding * 2;
	int height = roof_img.size().height + padding * 2;
	cv::resize(roof_img, roof_img, cv::Size(width, height));
	if (selected_roof_type == RoofTypes::GABLE || selected_roof_type == RoofTypes::HIP){
		// draw rect
		cv::Point2f center(center_w + padding, center_h + padding);
		cv::RotatedRect rRect = cv::RotatedRect(center, cv::Size2f(imageRoofWidth, imageRoofHeight), rotate);
		cv::Point2f vertices[4];
		rRect.points(vertices);
		// add sides
		for (int i = 0; i < 4; i++)
			line(roof_img, vertices[i], vertices[(i + 1) % 4], fg_color, thickness);
		// add ridge
		int ridge_length = ridgeLength;
		int ridge_x1 = roofWidth > roofHeight ? center.x - ridge_length * 0.5 : center.x;
		int ridge_y1 = roofWidth > roofHeight ? center.y : center.y - ridge_length * 0.5;
		int ridge_x2 = roofWidth > roofHeight ? ridge_x1 + ridge_length : center.x;
		int ridge_y2 = roofWidth > roofHeight ? center.y : ridge_y1 + ridge_length;
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
			cv::line(roof_img, roofWidth > roofHeight ? vertices[0] : vertices[2], ridge_left, fg_color, thickness);
			cv::line(roof_img, vertices[1], ridge_left, fg_color, thickness);
			cv::line(roof_img, roofWidth > roofHeight ? vertices[2] : vertices[0], ridge_right, fg_color, thickness);
			cv::line(roof_img, vertices[3], ridge_right, fg_color, thickness);
		}
	}
	else{
		// do nothing
	}
}

void DrawRotatedRect::generateRect(cv::Mat & roof_img, bool bResize, int padding, int center_x, int center_y, int roofWidth, int roofHeight, double rotate, int selected_roof_type, bool bRidgeDis, double ridgeDisRatio, int ridgeLength, const cv::Scalar& bg_color, const cv::Scalar& fg_color){
	int imageRoofWidth = roofWidth;
	int imageRoofHeight = roofHeight;
	int center_w = center_x;
	int center_h = center_y;
	int thickness = 1;
	if (bResize){
		int width = roof_img.size().width + padding * 2;
		int height = roof_img.size().height + padding * 2;
		cv::resize(roof_img, roof_img, cv::Size(width, height));
	}
	if (selected_roof_type == RoofTypes::GABLE || selected_roof_type == RoofTypes::HIP){
		// draw rect
		cv::Point2f center(center_w + padding, center_h + padding);
		cv::RotatedRect rRect = cv::RotatedRect(center, cv::Size2f(imageRoofWidth, imageRoofHeight), rotate);
		cv::Point2f vertices[4];
		rRect.points(vertices);
		// add sides
		for (int i = 0; i < 4; i++)
			line(roof_img, vertices[i], vertices[(i + 1) % 4], fg_color, thickness);
		// add ridge
		int ridge_length = ridgeLength;
		int ridge_x1 = roofWidth > roofHeight ? center.x - ridge_length * 0.5 : center.x;
		int ridge_y1 = roofWidth > roofHeight ? center.y : center.y - ridge_length * 0.5;
		int ridge_x2 = roofWidth > roofHeight ? ridge_x1 + ridge_length : center.x;
		int ridge_y2 = roofWidth > roofHeight ? center.y : ridge_y1 + ridge_length;
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
			cv::line(roof_img, roofWidth > roofHeight ? vertices[0] : vertices[2], ridge_left, fg_color, thickness);
			cv::line(roof_img, vertices[1], ridge_left, fg_color, thickness);
			cv::line(roof_img, roofWidth > roofHeight ? vertices[2] : vertices[0], ridge_right, fg_color, thickness);
			cv::line(roof_img, vertices[3], ridge_right, fg_color, thickness);
		}
	}
	else{
		// do nothing
	}
}
