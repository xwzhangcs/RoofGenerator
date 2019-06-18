#include "Utils.h"
#include <math.h>
#include <random>
#include <iostream>

namespace utils {
	const float M_PI = 3.1415926535;

	double genRand() {
		return (double)(rand() % 1000) / 1000.0;
	}

	double genRand(double a, double b) {
		return genRand() * (b - a) + a;
	}

	float gause(float u, float sigma) {
		return 1.0f / 2.0f / M_PI / sigma / sigma * expf(-u * u / 2.0f / sigma / sigma);
	}

	float stddev(std::vector<float> list) {
		if (list.size() <= 1) return 0.0f;

		float avg = mean(list);

		float total = 0.0f;
		for (int i = 0; i < list.size(); ++i) {
			total += (list[i] - avg) * (list[i] - avg);
		}

		return sqrt(total / (list.size() - 1));
	}

	float mean(std::vector<float> list) {
		float sum = 0.0f;
		for (int i = 0; i < list.size(); ++i) {
			sum += list[i];
		}
		return sum / list.size();
	}

	void grayScale(const cv::Mat& img, cv::Mat& grayImg) {
		if (img.channels() == 1) {
			grayImg = img.clone();
		}
		else if (img.channels() == 3) {
			cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
		}
		else if (img.channels() == 4) {
			cv::cvtColor(img, grayImg, cv::COLOR_BGRA2GRAY);
		}
	}

	void scaleToFit(const cv::Mat& src, cv::Mat& dst, const cv::Size& size) {
		cv::Scalar bg_color;
		if (src.channels() == 1) {
			bg_color = cv::Scalar(255);
		}
		else if (src.channels() == 3) {
			bg_color = cv::Scalar(255, 255, 255);
		}
		else if (src.channels() == 4) {
			bg_color = cv::Scalar(255, 255, 255, 255);
		}
		dst = cv::Mat(size, src.type(), bg_color);

		float scale = std::min((float)dst.cols / src.cols, (float)dst.rows / src.rows);
		cv::Size roi_size(src.cols * scale, src.rows * scale);

		cv::Mat roi(dst, cv::Rect((dst.cols - roi_size.width) * 0.5, (dst.rows - roi_size.height) * 0.5, roi_size.width, roi_size.height));
		cv::resize(src, roi, roi_size);
	}

	void distanceMap(const cv::Mat& img, cv::Mat& distMap) {
		cv::Mat gray_img;
		grayScale(img, gray_img);

		cv::threshold(gray_img, gray_img, 254, 255, CV_THRESH_BINARY);
		cv::distanceTransform(gray_img, distMap, CV_DIST_L2, 3);
		distMap.convertTo(distMap, CV_32F);
	}

	void output_vector(const std::vector<float>& values) {
		for (int i = 0; i < values.size(); ++i) {
			if (i > 0) std::cout << ", ";
			std::cout << values[i];
		}
		std::cout << std::endl;
	}

	cv::Point2f RotatePoint(const cv::Point2f& p, float rad)
	{
		const float x = std::cos(rad) * p.x - std::sin(rad) * p.y;
		const float y = std::sin(rad) * p.x + std::cos(rad) * p.y;

		const cv::Point2f rot_p(x, y);
		return rot_p;
	}

	cv::Point2f RotatePoint(const cv::Point2f& cen_pt, const cv::Point2f& p, float rad)
	{
		const cv::Point2f trans_pt = p - cen_pt;
		const cv::Point2f rot_pt = RotatePoint(trans_pt, rad);
		const cv::Point2f fin_pt = rot_pt + cen_pt;

		return fin_pt;
	}

	bool lineLineIntersection(const cv::Point2f& a, const cv::Point2f& b, const cv::Point2f& c, const cv::Point2f& d, double *tab, double *tcd, bool segment_only, cv::Point2f& int_pt) {
		cv::Point2f u = b - a;
		cv::Point2f v = d - c;

		if (cv::norm(u) < 0.0000001 || cv::norm(v) < 0.0000001) {
			return false;
		}

		double numer = v.x * (c.y - a.y) + v.y * (a.x - c.x);
		double denom = u.y * v.x - u.x * v.y;

		if (denom == 0.0)  {
			// they are parallel
			return false;
		}

		double t0 = numer / denom;

		cv::Point2f ipt = a + t0*u;
		cv::Point2f tmp = ipt - c;
		double t1;
		if (tmp.dot(v) > 0.0) {
			t1 = cv::norm(tmp) / cv::norm(v);
		}
		else {
			t1 = -1.0 * cv::norm(tmp) / cv::norm(d - c);
		}

		// check if intersection is within the segments
		if (segment_only && !((t0 >= 0.0000001) && (t0 <= 1.0 - 0.0000001) && (t1 >= 0.0000001) && (t1 <= 1.0 - 0.0000001))) {
			return false;
		}

		*tab = t0;
		*tcd = t1;
		cv::Point2f dirVec = b - a;

		int_pt = a + t0 * dirVec;

		return true;
	}

	bool rectInsideRect(int width, int height, int center_x, int center_y, int rect_width, int rect_height, double rotate){
		cv::Point2f center(center_x, center_y);
		cv::RotatedRect rRect = cv::RotatedRect(center, cv::Size2f(rect_width, rect_height), rotate);
		cv::Point2f vertices[4];
		rRect.points(vertices);
		for (int i = 0; i < 4; i++)
			if (!pointInsideRect(width, height, vertices[i]))
				return false;
		return true;
	}

	bool pointInsideRect(int width, int height, cv::Point2f& pt){
		if (pt.x < 0 || pt.x > width)
			return false;
		if (pt.y < 0 || pt.y > height)
			return false;
		return true;
	}

	bool rectIntersecRect(int center_x_v1, int center_y_v1, int rect_width_v1, int rect_height_v1, double rotate_v1, int center_x_v2, int center_y_v2, int rect_width_v2, int rect_height_v2, double rotate_v2){
		for (int i = center_x_v1 - rect_width_v1 * 0.5; i <= center_x_v1 + rect_width_v1 * 0.5; i++){
			for (int j = center_y_v1 - rect_height_v1 * 0.5; j <= center_y_v1 + rect_height_v1 * 0.5; j++){
				cv::Point2f new_pt = utils::RotatePoint(cv::Point2f(center_x_v1, center_y_v1), cv::Point2f(i, j), (rotate_v1)* M_PI / 180.0);
				if (pointInsideRotatedRect(center_x_v2, center_y_v2, rect_width_v2, rect_height_v2, rotate_v2, new_pt))
					return true;
			}
		}
		return false;
	}

	bool pointInsideRotatedRect(int center_x, int center_y, int rect_width, int rect_height, double rotate, cv::Point2f& pt){
		cv::Point2f new_pt = utils::RotatePoint(cv::Point2f(center_x, center_y), pt, (-rotate)* M_PI / 180.0);
		
		if (new_pt.x < center_x - rect_width * 0.5 || new_pt.x > center_x + rect_width * 0.5)
			return false;
		if (new_pt.y < center_y - rect_height * 0.5 || new_pt.y > center_y + rect_height * 0.5)
			return false;
		return true;
	}

	bool rectInsideRect(int center_x_v1, int center_y_v1, int rect_width_v1, int rect_height_v1, double rotate_v1, int center_x_v2, int center_y_v2, int rect_width_v2, int rect_height_v2, double rotate_v2){
		cv::Point2f center(center_x_v2, center_y_v2);
		cv::RotatedRect rRect = cv::RotatedRect(center, cv::Size2f(rect_width_v2, rect_height_v2), rotate_v2);
		cv::Point2f vertices[4];
		rRect.points(vertices);
		for (int i = 0; i < 4; i++)
			if (!pointInsideRotatedRect(center_x_v1, center_y_v1, rect_width_v1, rect_height_v1, rotate_v1, vertices[i]))
				return false;
		return true;
	}

	bool rectSideBySideRect(int center_x_v1, int center_y_v1, int rect_width_v1, int rect_height_v1, double rotate_v1, int center_x_v2, int center_y_v2, int rect_width_v2, int rect_height_v2, double rotate_v2){
		cv::Point2f center_v1(center_x_v1, center_y_v1);
		cv::RotatedRect rRect_v1 = cv::RotatedRect(center_v1, cv::Size2f(rect_width_v1, rect_height_v1), rotate_v1);
		cv::Point2f vertices_v1[4];
		rRect_v1.points(vertices_v1);
		int width_left_v1 = vertices_v1[0].x;
		int width_right_v1 = vertices_v1[3].x;
		int height_top_v1 = vertices_v1[1].y;
		int height_bot_v1 = vertices_v1[3].y;
		
		cv::Point2f center_v2(center_x_v2, center_y_v2);
		cv::RotatedRect rRect_v2 = cv::RotatedRect(center_v2, cv::Size2f(rect_width_v2, rect_height_v2), rotate_v2);
		cv::Point2f vertices_v2[4];
		rRect_v2.points(vertices_v2);
		int width_left_v2 = vertices_v2[0].x;
		int width_right_v2 = vertices_v2[3].x;
		int height_top_v2 = vertices_v2[1].y;
		int height_bot_v2 = vertices_v2[3].y;

		int inside = 0;
		for (int i = 0; i < 4; i++)
			if (pointInsideRotatedRect(center_x_v1, center_y_v1, rect_width_v1, rect_height_v1, rotate_v1, vertices_v2[i]))
				inside++;
		if (inside == 1)
			return false;

		/*if (width_left_v1 == width_right_v2 || width_right_v1 == width_left_v2)
			return true;
		if (height_top_v1 == height_bot_v2 || height_bot_v1 == height_top_v2)
			return true;
		if (center_x_v1 == width_right_v2 || center_x_v1 == width_left_v2)
			return true;
		if (center_y_v1 == height_bot_v2 || center_y_v1 == height_top_v2)
			return true;*/
		if (abs(width_left_v1 - width_right_v2) < 4 || abs(width_right_v1 - width_left_v2) < 4)
		return true;
		if (abs(height_top_v1 - height_bot_v2) < 4 || abs(height_bot_v1 - height_top_v2) < 4)
		return true;
		if (abs(center_x_v1 - width_right_v2) < 4 || abs(center_x_v1 - width_left_v2) < 4)
			return true;
		if (abs(center_y_v1 - height_bot_v2) < 4 || abs(center_y_v1 - height_top_v2) < 4)
			return true;

		return false;

	}

	bool rectControlRect(int center_x_v1, int center_y_v1, int rect_width_v1, int rect_height_v1, double rotate_v1, int center_x_v2, int center_y_v2, int rect_width_v2, int rect_height_v2, double rotate_v2){
		cv::Point2f center_v2(center_x_v2, center_y_v2);
		cv::RotatedRect rRect_v2 = cv::RotatedRect(center_v2, cv::Size2f(rect_width_v2, rect_height_v2), rotate_v2);
		cv::Point2f vertices_v2[4];
		rRect_v2.points(vertices_v2);

		int inside = 0;
		for (int i = 0; i < 4; i++)
			if (pointInsideRotatedRect(center_x_v1, center_y_v1, rect_width_v1, rect_height_v1, rotate_v1, vertices_v2[i]))
				inside++;
		std::cout << "inside is " << inside << std::endl;
		if (inside == 1)
			return false;
		else
			return true;
				
	}
}