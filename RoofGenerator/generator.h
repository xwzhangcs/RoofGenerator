#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/opencv.hpp>
#include <vector>

class Generator {

protected:
	Generator() {}

public:
	static int generate_one_edge(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);

	static int generate_two_edges(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);

	static int generate_three_edges_v1(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);
	static int generate_three_edges_v2(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);

	static int generate_four_edges_v1(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);
	static int generate_four_edges_v2(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);
	static int generate_four_edges_v3(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);
	static int generate_four_edges_v4(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);

	static int generate_five_edges_v1(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);
	static int generate_five_edges_v2(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);
	static int generate_five_edges_v3(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);
	static int generate_five_edges_v4(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);
	static int generate_five_edges_v5(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);
	static int generate_five_edges_v6(int start_index, int type, int total, int width, int height, int step_size, int padding, std::string output_path);

};