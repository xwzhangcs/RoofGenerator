#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/opencv.hpp>
#include <vector>

class Generator {

protected:
	Generator() {}

public:
	static void generate_one_edge(int width, int height, int step_size, int padding, std::string output_path);
	static void generate_two_edges(int width, int height, int step_size, int padding, std::string output_path);
	static void generate_three_edges_v1(int width, int height, int step_size, int padding, std::string output_path);
	static void generate_three_edges_v2(int width, int height, int step_size, int padding, std::string output_path);

};