#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "Utils.h"
#include "generator.h"

int main(int argc, char** argv)
{
	int padding = 12;
	//Generator::generate_one_edge(200, 200, 20, padding, "../data/one_edge");
	//Generator::generate_two_edges(200, 200, 20, padding, "../data/two_edges");
	//Generator::generate_three_edges_v1(200, 200, 20, padding, "../data/three_edges_v1");
	Generator::generate_three_edges_v2(200, 200, 20, padding, "../data/three_edges_v2");
	system("pause");

	return 0;
}