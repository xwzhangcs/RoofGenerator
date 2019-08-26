#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "Utils.h"
#include "generator.h"

int main(int argc, char** argv)
{
	int padding = 12;
	int type = 0;
	int total = 8;
	int start_index = 0;
	start_index = Generator::generate_one_edge(start_index, 0, total, 200, 200, 4, padding, "../data/roof_data");
	start_index = Generator::generate_two_edges(start_index, 1, total, 200, 200, 10, padding, "../data/roof_data");
	start_index = Generator::generate_three_edges_v1(start_index, 2, total, 200, 200, 10, padding, "../data/roof_data");
	start_index = Generator::generate_three_edges_v2(start_index, 3, total, 200, 200, 20, padding, "../data/roof_data");
	start_index = Generator::generate_four_edges_v2(start_index, 4, total, 200, 200, 10, padding, "../data/roof_data");
	start_index = Generator::generate_four_edges_v1(start_index, 5, total, 200, 200, 20, padding, "../data/roof_data");
	start_index = Generator::generate_four_edges_v3(start_index, 6, total, 200, 200, 20, padding, "../data/roof_data");
	start_index = Generator::generate_four_edges_v4(start_index, 7, total, 200, 200, 20, padding, "../data/roof_data");
	//Generator::generate_five_edges_v1(200, 200, 20, padding, "../data/five_edges_v1");
	//Generator::generate_five_edges_v2(200, 200, 20, padding, "../data/five_edges_v2");
	//Generator::generate_five_edges_v3(200, 200, 20, padding, "../data/five_edges_v3");
	//Generator::generate_five_edges_v4(200, 200, 20, padding, "../data/five_edges_v4");
	//Generator::generate_five_edges_v5(200, 200, 20, padding, "../data/five_edges_v5");
	//Generator::generate_five_edges_v6(200, 200, 20, padding, "../data/five_edges_v6");
	system("pause");

	return 0;
}