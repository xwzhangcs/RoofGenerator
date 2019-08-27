#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "Utils.h"
#include "generator.h"

int main(int argc, char** argv)
{
	int padding = 16;
	int type = 0;
	int total = 8;
	int start_index = 0;
	int resolution = 192;
	int step = 12;
	start_index = Generator::generate_one_edge(start_index, 0, total, resolution, resolution, step, padding, "../data/roof_data");
	start_index = Generator::generate_two_edges(start_index, 1, total, resolution, resolution, step, padding, "../data/roof_data");
	start_index = Generator::generate_three_edges_v1(start_index, 2, total, resolution, resolution, step, padding, "../data/roof_data");
	start_index = Generator::generate_three_edges_v2(start_index, 3, total, resolution, resolution, step, padding, "../data/roof_data");
	start_index = Generator::generate_four_edges_v1(start_index, 4, total, resolution, resolution, step, padding, "../data/roof_data");
	start_index = Generator::generate_four_edges_v2(start_index, 5, total, resolution, resolution, 16, padding, "../data/roof_data");
	start_index = Generator::generate_four_edges_v3(start_index, 6, total, resolution, resolution, 24, padding, "../data/roof_data");
	start_index = Generator::generate_four_edges_v4(start_index, 7, total, resolution, resolution, step, padding, "../data/roof_data");

	//Generator::generate_five_edges_v1(200, 200, 20, padding, "../data/five_edges_v1");
	//Generator::generate_five_edges_v2(200, 200, 20, padding, "../data/five_edges_v2");
	//Generator::generate_five_edges_v3(200, 200, 20, padding, "../data/five_edges_v3");
	//Generator::generate_five_edges_v4(200, 200, 20, padding, "../data/five_edges_v4");
	//Generator::generate_five_edges_v5(200, 200, 20, padding, "../data/five_edges_v5");
	//Generator::generate_five_edges_v6(200, 200, 20, padding, "../data/five_edges_v6");
	system("pause");

	return 0;
}