#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "roofA.h"
#include "roofB.h"
#include "roofTypes.h"
#include "RoofUtils.h"
#include "Utils.h"

int main(int argc, char** argv)
{
	/*Config roof_flat_paras(0.5, 0.2, 0.4, 0.5, -30, RoofTypes::FLAT, false, 0.0, 1.0);
	cv::Mat roof_img = RoofA::generateRoof(224, 224, roof_flat_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
	if (!roof_img.empty())
		cv::imwrite("../data/roofA_flat.png", roof_img);
	Config roof_gable_paras(0.3, 0.7, 0.4, 0.5, 30, RoofTypes::GABLE, true, 0.1, 1.0);
	roof_img = RoofA::generateRoof(224, 224, roof_gable_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
	if (!roof_img.empty())
		cv::imwrite("../data/roofA_gable.png", roof_img);
	Config roof_hip_paras(0.5, 0.5, 0.4, 0.5, 30, RoofTypes::HIP, true, 0.1, 0.6);
	roof_img = RoofA::generateRoof(224, 224, roof_hip_paras, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
	if (!roof_img.empty())
		cv::imwrite("../data/roofA_hip.png", roof_img);*/
	Config roof_flat_paras_main(0.5, 0.2, 0.4, 0.4, 0, RoofTypes::GABLE, false, 0.0, 1.0);
	Config roof_flat_paras_sub(0.5, 0.4, 0.6, 0.4, 0, RoofTypes::GABLE, false, 0.0, 1.0);
	cv::Mat roof_img = RoofB::generateRoof(224, 224, roof_flat_paras_main, roof_flat_paras_sub, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
	if (!roof_img.empty())
		cv::imwrite("../data/roofB_flat.png", roof_img);
	system("pause");
	return 0;
	{
		std::cout << utils::rectIntersecRect(50, 55, 40, 40, 20, 50, 100, 40, 40, 0) << std::endl;
		cv::Point2f new_pt = utils::RotatePoint(cv::Point2f(50, 55), cv::Point2f(70, 75), (20)* M_PI / 180.0);
		std::cout << new_pt << std::endl;
		std::cout << utils::RotatePoint(cv::Point2f(50, 100), new_pt, 0) << std::endl;
		cv::Mat result(224, 224, CV_8UC3, cv::Scalar(0, 0, 0));
		cv::Point2f center(50, 55);
		cv::RotatedRect rRect = cv::RotatedRect(center, cv::Size2f(40, 40), 20);
		cv::Point2f vertices[4];
		rRect.points(vertices);
		// add sides
		for (int i = 0; i < 4; i++)
			line(result, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 255, 255), 1);

		cv::Point2f center_v2(50, 100);
		cv::RotatedRect rRect_v2 = cv::RotatedRect(center_v2, cv::Size2f(40, 40), 0);
		cv::Point2f vertices_v2[4];
		rRect_v2.points(vertices_v2);
		// add sides
		for (int i = 0; i < 4; i++)
			line(result, vertices_v2[i], vertices_v2[(i + 1) % 4], cv::Scalar(255, 255, 255), 1);
		cv::imwrite("../data/test.png", result);
		system("pause");
	}
	return 0;
}