#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "roofA.h"
#include "roofB.h"
#include "roofC.h"
#include "roofD.h"
#include "roofTypes.h"
#include "RoofUtils.h"
#include "Utils.h"
#include "drawRotatedRect.h"


#include <boost/config.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/io.hpp>

void test_one_nodes(int width, int height, int step_size, int padding);
void test_two_nodes(int width, int height, int step_size, int type, int padding);
void test_three_nodes(int width, int height, int step_size, int type, int padding);
void test_four_nodes(int width, int height, int step_size, int type, int padding);
void test_four_nodes_c(int width, int height, int step_size, int type, int padding);
void test_four_nodes_d(int width, int height, int step_size, int type, int padding);
void test_bgl(int num_nodes);
void test_drawing();
void findCombinationsUtil(int arr[], int index, int num, int reducedNum, int nodes);
std::vector<int> DecimalToBinary(int n, int num_digits);
void test_intersection(int width, int height, int step_size, int type, int padding);
void test_tri_touch(int width, int height, int step_size, int type, int padding);
bool test_triangle_circle(int num_nodes, int numb_edges, int c);
void test_intersection_angle(int width, int height, int step_size, int type, int padding);

//a class to hold the coordinates of the straight line embedding
struct coord_t
{
	std::size_t x;
	std::size_t y;
};

struct cycle_detector : public boost::dfs_visitor<>
{
	cycle_detector(bool& has_cycle)
		: _has_cycle(has_cycle) { }

	template <class Edge, class Graph>
	void back_edge(Edge, Graph&) {
		_has_cycle = true;
	}
protected:
	bool& _has_cycle;
};

void print_vector(std::vector<int> v){
	for (int i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	int padding = 5;
	//test_one_nodes(64, 64, 8, padding);
	//test_two_nodes(64, 64, 8, 1, padding);
	//test_three_nodes(64, 64, 8, 1, padding);
	//test_four_nodes(64, 64, 8, 1, padding);
	//test_four_nodes_d(64, 64, 8, 1, padding);
	/*std::cout << utils::rectControlRect(16, 32, 22, 13, 0, 24, 20, 26, 10, 0) << std::endl;
	std::cout << utils::rectIntersecRect(16, 32, 22, 13, 0, 24, 20, 26, 10, 0) << std::endl;
	std::cout << utils::rectIntersecRect(24, 20, 26, 10, 0, 16, 32, 22, 13, 0) << std::endl;*/
	test_intersection_angle(64, 64, 8, 1, padding);
	//test_bgl(atoi(argv[1]));
	system("pause");

	return 0;
}

std::vector<int> DecimalToBinary(int n, int num_digits) {
	std::vector<int> binaryNumber;
	if (pow(2, num_digits) <= n)
		return binaryNumber;
	binaryNumber.resize(num_digits);
	int num = n;
	int i = num_digits - 1;
	while (n > 0 && i >=0 ) {
		binaryNumber[i] = n % 2;
		n = n / 2;
		i--;
	}
	return binaryNumber;
}

void test_drawing(){
	enum files_e {
		dax_h, yow_h, boz_h, zow_h, foo_cpp,
		foo_o, bar_cpp, bar_o, libfoobar_a,
		zig_cpp, zig_o, zag_cpp, zag_o,
		libzigzag_a, killerapp, N
	};
	const char* name[] = { "dax.h", "yow.h", "boz.h", "zow.h", "foo.cpp",
		"foo.o", "bar.cpp", "bar.o", "libfoobar.a",
		"zig.cpp", "zig.o", "zag.cpp", "zag.o",
		"libzigzag.a", "killerapp" };
	typedef std::pair<int, int> Edge;
	Edge used_by[] = {
		Edge(dax_h, foo_cpp), Edge(dax_h, bar_cpp), Edge(dax_h, yow_h),
		Edge(yow_h, bar_cpp), Edge(yow_h, zag_cpp),
		Edge(boz_h, bar_cpp), Edge(boz_h, zig_cpp), Edge(boz_h, zag_cpp),
		Edge(zow_h, foo_cpp),
		Edge(foo_cpp, foo_o),
		Edge(foo_o, libfoobar_a),
		Edge(bar_cpp, bar_o),
		Edge(bar_o, libfoobar_a),
		Edge(libfoobar_a, libzigzag_a),
		Edge(zig_cpp, zig_o),
		Edge(zig_o, libzigzag_a),
		Edge(zag_cpp, zag_o),
		Edge(zag_o, libzigzag_a),
		Edge(libzigzag_a, killerapp)
	};
	const int nedges = sizeof(used_by) / sizeof(Edge);
	int weights[nedges];
	std::fill(weights, weights + nedges, 1);

	using namespace boost;

	typedef adjacency_list< vecS, vecS, directedS,
		property< vertex_color_t, default_color_type >,
		property< edge_weight_t, int >
	> Graph;
	Graph g(used_by, used_by + nedges, weights, N);

	write_graphviz(std::cout, g, make_label_writer(name));
}

void findCombinationsUtil(int arr[], int index,
	int num, int reducedNum, int nodes)
{
	// Base condition 
	if (reducedNum < 0)
		return;

	// If combination is found, print it 
	if (reducedNum == 0)
	{
		if (index == nodes){
			for (int i = 0; i < index; i++)
				std::cout << arr[i] << " ";
			std::cout << std::endl;
		}
		return;
	}

	// Find the previous number stored in arr[] 
	// It helps in maintaining increasing order 
	int prev = (index == 0) ? 1 : arr[index - 1];

	// note loop starts from previous number 
	// i.e. at array location index - 1 
	for (int k = prev; k <= num; k++)
	{
		// next element of array is k 
		arr[index] = k;
		// call recursively with reduced number 
		findCombinationsUtil(arr, index + 1, num,
			reducedNum - k, nodes);
	}
}

void test_bgl(int num_nodes){
	boost::numeric::ublas::matrix<int> m(num_nodes, num_nodes, 0);
	int numb_edges = num_nodes * (num_nodes - 1) / 2;
	int num_cases = pow(2, numb_edges);
	int num_connected = 0;
	//std::vector<int> degrees;
	//degrees.resize(num_nodes);
	//std::vector<std::vector<int>> results_degrees;
	std::vector<int> results_index;
	// assignment
	int num_test = 0;
	for (int c = 0; c < num_cases; c++){
		num_test++;
		std::cout << "num_test is " << num_test << std::endl;
		std::vector<int> values = DecimalToBinary(c, numb_edges);
		int valid_edges = 0;
		for (int i = 0; i < values.size(); i++)
			if (values[i] == 1)
				valid_edges++;
		if (valid_edges < num_nodes - 1)
			continue;
		typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::undirectedS> Graph;
		Graph G(num_nodes);
		int index_e = 0;
		for (int i = 0; i < m.size1(); i++){
			for (int j = i + 1; j < m.size2(); j++){
				m(i, j) = values[index_e];
				if (m(i, j) == 1){
					add_edge(i, j, G);
				}
				index_e++;
			}
		}
		bool bConnected = false;
		// check connectivity
		std::vector<int> component(num_vertices(G));
		int num = connected_components(G, &component[0]);
		if (num == 1){
			bConnected = true;
			num_connected++;
		}
		if (bConnected){
			//std::cout << "------------------" << std::endl;
			// generate undirected graph mat
			/*for (int i = 0; i < m.size1(); i++){
				for (int j = 0; j < i; j++){
					m(i, j) = m(j, i);
				}
			}*/
			// compute degrees
			/*for (int i = 0; i < m.size1(); i++){
				degrees[i] = 0;
				for (int j = 0; j < m.size2(); j++){
					if (m(i, j) == 1)
						degrees[i]++;
				}
			}*/
			bool bNew = true;
			for (int i = 0; i < results_index.size(); i++){
				// g1
				Graph g1(num_nodes);
				int index_e = 0;
				std::vector<int> values = DecimalToBinary(results_index[i], numb_edges);
				for (int i = 0; i < m.size1(); i++){
					for (int j = i + 1; j < m.size2(); j++){
						m(i, j) = values[index_e];
						if (m(i, j) == 1){
							add_edge(i, j, g1);
						}
						index_e++;
					}
				}
				if (boost::isomorphism(g1, G)){
					bNew = false;
					break;
				}
			}
			if (bNew){
				results_index.push_back(c);
			}
		}
	}
	std::wcout << "here" << std::endl;
	int num_cycles = 0;
	for (int i = 0; i < results_index.size(); i++){
		// check triangle cycles
		if (test_triangle_circle(num_nodes, numb_edges, results_index[i])){
			num_cycles++;
			continue;
		}

		std::vector<int> values = DecimalToBinary(results_index[i], numb_edges);
		int index_e = 0;
		for (int i = 0; i < m.size1(); i++){
			for (int j = i + 1; j < m.size2(); j++){
				m(i, j) = values[index_e];
				index_e++;
			}
		}
		// generate undirected graph mat
		for (int i = 0; i < m.size1(); i++){
			for (int j = 0; j < i; j++){
				m(i, j) = m(j, i);
			}
		}
		std::cout << "------------------" << std::endl;
		// display mat
		for (int i = 0; i < m.size1(); i++){
			for (int j = 0; j < m.size2(); j++){
				std::cout << m(i, j);
				if (j < m.size2() - 1)
					std::cout << ",";
			}
			std::cout << std::endl;
		}
	}
	std::cout << "The number of types is " << results_index.size() - num_cycles << " out of " << results_index.size() << std::endl;
}

bool test_triangle_circle(int num_nodes, int numb_edges, int c){
	boost::numeric::ublas::matrix<int> m(num_nodes, num_nodes, 0);
	std::vector<int> values = DecimalToBinary(c, numb_edges);
	int index_e = 0;
	for (int i = 0; i < m.size1(); i++){
		for (int j = i + 1; j < m.size2(); j++){
			m(i, j) = values[index_e];
			index_e++;
		}
	}
	// complete m
	for (int i = 0; i < m.size1(); i++){
		for (int j = 0; j < i; j++){
			m(i, j) = m(j, i);
		}
	}
	for (int i = 0; i < m.size1(); i++){
		for (int j = 0; j < m.size2(); j++){
			for (int k = 0; k < m.size2(); k++){
				if (m(i, j) == 1 && m(j, k) == 1 && m(k, i) == 1){ // i -> j, j -> k and k -> i 
					return true;
				}
			}
		}
	}
	return false;
}

void test_intersection(int width, int height, int step_size, int type, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(2);
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += step_size){
				for (int top_h = 0; top_h < height; top_h += step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					// check 
					// second rectangle
					for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
						for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1))
										continue;
									// condition 1
									if (center_w == center_w_v1 && center_h == center_h_v1 && roof_w == roof_w_v1 && roof_h == roof_h_v1)
										continue;
									// touch
									int bot_h = top_h + roof_h;
									int bot_w = top_w + roof_w;
									int bot_h_v1 = top_h_v1 + roof_h_v1;
									int bot_w_v1 = top_w_v1 + roof_w_v1;
									bool bTouch = utils::rectTouch(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
									if (!bTouch)
										continue;
									/*bool bCross = utils::rectCross(center_w, center_h, roof_w, roof_h, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1);
									if (!bCross)
										continue;*/
									int dis_left = top_w < top_w_v1 ? top_w : top_w_v1;
									int dis_right = (top_w + roof_w) >(top_w_v1 + roof_w_v1) ? (width - top_w - roof_w) : (width - top_w_v1 - roof_w_v1);
									int dis_top = top_h < top_h_v1 ? top_h : top_h_v1;;
									int dis_bot = (top_h + roof_h) >(top_h_v1 + roof_h_v1) ? (height - top_h - roof_h) : (height - top_h_v1 - roof_h_v1);
									// condition 2
									if (dis_left != dis_right || dis_top != dis_bot)
										continue;
									// condition 3
									if (dis_left * dis_top != 0)
										continue;
									// add first 
									roof_paras[0].clear();
									roof_paras[0].push_back(center_w);
									roof_paras[0].push_back(center_h);
									roof_paras[0].push_back(roof_w);
									roof_paras[0].push_back(roof_h);
									roof_paras[0].push_back(0);
									// add second
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									//
									cv::Mat roof_img(height, width, CV_8UC3, bg_color);
									DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
									if (!roof_img.empty()){
										char buffer[50];
										sprintf(buffer, "roof_image_%08d.png", index);
										std::string img_filename = "../data/node_test/" + std::string(buffer);
										std::cout << img_filename << std::endl;
										cv::imwrite(img_filename, roof_img);
										index++;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void test_intersection_angle(int width, int height, int step_size, int type, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(2);
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= roof_w; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += step_size){
				for (int top_h = 0; top_h < height; top_h += step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					// check 
					// second rectangle
					int roof_w_v1 = roof_w;
					int roof_h_v1 = roof_h;
					int top_w_v1 = top_w + roof_w;
					int top_h_v1 = top_h;
					int bottom_right_w_v1 = top_w_v1 + roof_w_v1;
					int bottom_right_h_v1 = top_h_v1 + roof_h_v1;
					int rotate_v1 = -30;
					cv::Point2f top_left_v1(top_w_v1, top_h_v1);
					cv::Point2f bottom_right_v1 = utils::RotatePoint(top_left_v1, cv::Point2f(bottom_right_w_v1, bottom_right_h_v1), (rotate_v1)* M_PI / 180.0);
					cv::Point2f center((top_left_v1.x + bottom_right_v1.x) * 0.5, (top_left_v1.y + bottom_right_v1.y) * 0.5);

					int center_w_v1 = center.x;
					int center_h_v1 = center.y;
					if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1, rotate_v1))
						continue;
					int dis_left = top_w < top_w_v1 ? top_w : top_w_v1;
					int dis_right = (top_w + roof_w) > bottom_right_v1.x ? (width - top_w - roof_w) : (width - bottom_right_v1.x);
					int dis_top = top_h < top_h_v1 ? top_h : top_h_v1;;
					int dis_bot = (top_h + roof_h) > bottom_right_h_v1 ? (height - top_h - roof_h) : (height - bottom_right_h_v1);
					// condition 2
					/*if (dis_left != dis_right || dis_top != dis_bot)
						continue;*/
					// condition 3
					/*if (dis_left * dis_top != 0)
						continue;*/
					// add first 
					roof_paras[0].clear();
					roof_paras[0].push_back(center_w);
					roof_paras[0].push_back(center_h);
					roof_paras[0].push_back(roof_w);
					roof_paras[0].push_back(roof_h);
					roof_paras[0].push_back(0);
					// add second
					roof_paras[1].clear();
					roof_paras[1].push_back(center_w_v1);
					roof_paras[1].push_back(center_h_v1);
					roof_paras[1].push_back(roof_w_v1);
					roof_paras[1].push_back(roof_h_v1);
					roof_paras[1].push_back(rotate_v1);
					//
					cv::Mat roof_img(height, width, CV_8UC3, bg_color);
					DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
					if (!roof_img.empty()){
						char buffer[50];
						sprintf(buffer, "roof_image_%08d.png", index);
						std::string img_filename = "../data/node_rotate/" + std::string(buffer);
						std::cout << img_filename << std::endl;
						cv::imwrite(img_filename, roof_img);
						index++;
					}
				}
			}
		}
	}
}

// reduce the resolution to 56 * 56 
void test_one_nodes(int width, int height, int step_size, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += 0.5 * step_size){
				for (int top_h = 0; top_h < height; top_h += 0.5 * step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					int dis_left = top_w;
					int dis_right = width - top_w - roof_w;
					int dis_top = top_h;
					int dis_bot = height - top_h - roof_h;
					// condition 1
					if (dis_left != dis_right || dis_top != dis_bot)
						continue;
					// condition 2
					if (dis_left * dis_top != 0)
						continue;
					// condition 3
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h, 0))
						continue;
					cv::Mat roof_img(height, width, CV_8UC3, bg_color);
					DrawRotatedRect::generateRect(roof_img, padding, center_w, center_h, roof_w, roof_h, 0, RoofTypes::FLAT, bg_color, fg_color);
					if (!roof_img.empty()){
						char buffer[50];
						sprintf(buffer, "roof_image_%08d.png", index);
						std::string img_filename = "../data/node_one/" + std::string(buffer);
						std::cout << img_filename << std::endl;
						cv::imwrite(img_filename, roof_img);
						index++;
					}
				}
			}
		}
	}
}

void test_two_nodes(int width, int height, int step_size, int type, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(2);
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += 0.5 * step_size){
				for (int top_h = 0; top_h < height; top_h += 0.5 * step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h, 0))
						continue;
					// check 
					// second rectangle
					for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
						for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += 0.5 * step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += 0.5 * step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1, 0))
										continue;
									// condition 1
									if (center_w == center_w_v1 && center_h == center_h_v1 && roof_w == roof_w_v1 && roof_h == roof_h_v1)
										continue;
									int bot_h_v1 = top_h_v1 + roof_h_v1;
									int bot_h = top_h + roof_h;
									int bot_w_v1 = top_w_v1 + roof_w_v1;
									int bot_w = top_w + roof_w;
									if ((top_w > top_w_v1 && top_w < bot_w_v1) || (bot_w > top_w_v1 && bot_w < bot_w_v1))
										continue;
									if ((top_h_v1 > top_h && top_h_v1 < bot_h) || (bot_h_v1 > top_h && bot_h_v1 < bot_h))
										continue;

									int dis_left = top_w < top_w_v1 ? top_w : top_w_v1;
									int dis_right = (top_w + roof_w) >(top_w_v1 + roof_w_v1) ? (width - top_w - roof_w) : (width - top_w_v1 - roof_w_v1);
									int dis_top = top_h < top_h_v1 ? top_h : top_h_v1;;
									int dis_bot = (top_h + roof_h) >(top_h_v1 + roof_h_v1) ? (height - top_h - roof_h) : (height - top_h_v1 - roof_h_v1);
									// condition 2
									if (dis_left != dis_right || dis_top != dis_bot)
										continue;
									// condition 3
									if (dis_left * dis_top != 0)
										continue;
									// connected
									bool intersec_a = !utils::rectIntersecRect(center_w, center_h, roof_w, roof_h, 0, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1, 0);
									bool intersec_b = !utils::rectIntersecRect(center_w_v1, center_h_v1, roof_w_v1, roof_h_v1, 0, center_w, center_h, roof_w, roof_h, 0);
									if (intersec_a && intersec_b)
										continue;
									// add first 
									roof_paras[0].clear();
									roof_paras[0].push_back(center_w);
									roof_paras[0].push_back(center_h);
									roof_paras[0].push_back(roof_w);
									roof_paras[0].push_back(roof_h);
									roof_paras[0].push_back(0);
									// add second
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									//
									cv::Mat roof_img(height, width, CV_8UC3, bg_color);
									DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
									if (!roof_img.empty()){
										char buffer[50];
										sprintf(buffer, "roof_image_%08d.png", index);
										std::string img_filename = "../data/node_two/" + std::string(buffer);
										std::cout << img_filename << std::endl;
										cv::imwrite(img_filename, roof_img);
										index++;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void test_tri_touch(int width, int height, int step_size, int type, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(3);
	std::vector<int> imageRoofsWidth;
	imageRoofsWidth.resize(roof_paras.size());
	std::vector<int> imageRoofsHeight;
	imageRoofsHeight.resize(roof_paras.size());
	std::vector<int> centers_w;
	centers_w.resize(roof_paras.size());
	std::vector<int> centers_h;
	centers_h.resize(roof_paras.size());
	std::vector<double> rotates;
	rotates.resize(roof_paras.size());
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += 0.5 * step_size){
				for (int top_h = 0; top_h < height; top_h += 0.5 * step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					roof_paras[0].clear();
					roof_paras[0].push_back(center_w);
					roof_paras[0].push_back(center_h);
					roof_paras[0].push_back(roof_w);
					roof_paras[0].push_back(roof_h);
					roof_paras[0].push_back(0);
					imageRoofsWidth[0] = roof_w;
					imageRoofsHeight[0] = roof_h;
					centers_w[0] = center_w;
					centers_h[0] = center_h;
					rotates[0] = 0;
					// check 
					// second rectangle
					for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
						for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1))
										continue;
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									imageRoofsWidth[1] = roof_w_v1;
									imageRoofsHeight[1] = roof_h_v1;
									centers_w[1] = center_w_v1;
									centers_h[1] = center_h_v1;
									rotates[1] = 0;
									// condition 1
									if (center_w == center_w_v1 && center_h == center_h_v1 && roof_w == roof_w_v1 && roof_h == roof_h_v1)
										continue;
									// touch
									int bot_h = top_h + roof_h;
									int bot_w = top_w + roof_w;
									int bot_h_v1 = top_h_v1 + roof_h_v1;
									int bot_w_v1 = top_w_v1 + roof_w_v1;
									bool bTouch = utils::rectTouch(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
									if (!bTouch)
										continue;
									// third rectangle
									for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
										for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
											for (int top_w_v2 = 0; top_w_v2 < width; top_w_v2 += step_size){
												for (int top_h_v2 = 0; top_h_v2 < height; top_h_v2 += step_size){
													int center_w_v2 = top_w_v2 + 0.5 * roof_w_v2;
													int center_h_v2 = top_h_v2 + 0.5 * roof_h_v2;
													if (!utils::rectInsideRect(width, height, center_w_v2, center_h_v2, roof_w_v2, roof_h_v2))
														continue;
													roof_paras[2].clear();
													roof_paras[2].push_back(center_w_v2);
													roof_paras[2].push_back(center_h_v2);
													roof_paras[2].push_back(roof_w_v2);
													roof_paras[2].push_back(roof_h_v2);
													roof_paras[2].push_back(0);
													imageRoofsWidth[2] = roof_w_v2;
													imageRoofsHeight[2] = roof_h_v2;
													centers_w[2] = center_w_v2;
													centers_h[2] = center_h_v2;
													rotates[2] = 0;	
													// condition 1
													if (center_w_v1 == center_w_v2 && center_h_v1 == center_h_v2 && roof_w_v1 == roof_w_v2 && roof_h_v1 == roof_h_v2)
														continue;
													if (center_w == center_w_v2 && center_h == center_h_v2 && roof_w == roof_w_v2 && roof_h == roof_h_v2)
														continue;
													// touch
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bTouch = utils::rectTouch(top_w_v1, top_h_v1, bot_w_v1, bot_h_v1, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch)
														continue;
													bTouch = utils::rectTouch(top_w, top_h, bot_w, bot_h, top_w_v2, top_h_v2, bot_w_v2, bot_h_v2);
													if (!bTouch)
														continue;
													// condition 3
													int dis_left = 100000;
													int dis_right = 100000;
													int dis_top = 100000;
													int dis_bot = 100000;
													for (int i = 0; i < roof_paras.size(); i++){
														if (dis_left >(centers_w[i] - 0.5 * imageRoofsWidth[i]))
															dis_left = centers_w[i] - 0.5 * imageRoofsWidth[i];
														if (dis_right > (width - centers_w[i] - 0.5 * imageRoofsWidth[i]))
															dis_right = width - centers_w[i] - 0.5 * imageRoofsWidth[i];
														if (dis_top > (centers_h[i] - 0.5 * imageRoofsHeight[i]))
															dis_top = centers_h[i] - 0.5 * imageRoofsHeight[i];
														if (dis_bot > (height - centers_h[i] - 0.5 * imageRoofsHeight[i]))
															dis_bot = height - centers_h[i] - 0.5 * imageRoofsHeight[i];

													}
													if (dis_left != dis_right || dis_top != dis_bot)
														continue;
													if (dis_left * dis_top != 0)
														continue;
													
													cv::Mat roof_img(height, width, CV_8UC3, bg_color);
													DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
													if (!roof_img.empty()){
														char buffer[50];
														sprintf(buffer, "roof_image_%08d.png", index);
														std::string img_filename = "../data/node_test_tri/" + std::string(buffer);
														std::cout << img_filename << std::endl;
														cv::imwrite(img_filename, roof_img);
														index++;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void test_three_nodes(int width, int height, int step_size, int type, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(3);
	std::vector<int> imageRoofsWidth;
	imageRoofsWidth.resize(roof_paras.size());
	std::vector<int> imageRoofsHeight;
	imageRoofsHeight.resize(roof_paras.size());
	std::vector<int> centers_w;
	centers_w.resize(roof_paras.size());
	std::vector<int> centers_h;
	centers_h.resize(roof_paras.size());
	std::vector<double> rotates;
	rotates.resize(roof_paras.size());
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += 0.5 * step_size){
				for (int top_h = 0; top_h < height; top_h += 0.5 * step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h, 0))
						continue;
					roof_paras[0].clear();
					roof_paras[0].push_back(center_w);
					roof_paras[0].push_back(center_h);
					roof_paras[0].push_back(roof_w);
					roof_paras[0].push_back(roof_h);
					roof_paras[0].push_back(0);
					imageRoofsWidth[0] = roof_w;
					imageRoofsHeight[0] = roof_h;
					centers_w[0] = center_w;
					centers_h[0] = center_h;
					rotates[0] = 0;
					// check 
					// second rectangle
					for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
						for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += 0.5 * step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += 0.5 * step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1, 0))
										continue;
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									imageRoofsWidth[1] = roof_w_v1;
									imageRoofsHeight[1] = roof_h_v1;
									centers_w[1] = center_w_v1;
									centers_h[1] = center_h_v1;
									rotates[1] = 0;
									// third rectangle
									for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
										for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
											for (int top_w_v2 = 0; top_w_v2 < width; top_w_v2 += 0.5 * step_size){
												for (int top_h_v2 = 0; top_h_v2 < height; top_h_v2 += 0.5 * step_size){
													int center_w_v2 = top_w_v2 + 0.5 * roof_w_v2;
													int center_h_v2 = top_h_v2 + 0.5 * roof_h_v2;
													if (!utils::rectInsideRect(width, height, center_w_v2, center_h_v2, roof_w_v2, roof_h_v2, 0))
														continue;
													roof_paras[2].clear();
													roof_paras[2].push_back(center_w_v2);
													roof_paras[2].push_back(center_h_v2);
													roof_paras[2].push_back(roof_w_v2);
													roof_paras[2].push_back(roof_h_v2);
													roof_paras[2].push_back(0);
													imageRoofsWidth[2] = roof_w_v2;
													imageRoofsHeight[2] = roof_h_v2;
													centers_w[2] = center_w_v2;
													centers_h[2] = center_h_v2;
													rotates[2] = 0;
													// condition 1
													bool bValid = true;
													for (int i = 0; i < roof_paras.size(); i++){
														for (int j = i + 1; j < roof_paras.size(); j++){
															if (centers_w[i] == centers_w[j] && centers_h[i] == centers_h[j] && imageRoofsWidth[i] == imageRoofsWidth[j] && imageRoofsHeight[i] == imageRoofsHeight[j]){
																bValid = false;
																break;
															}
														}
														if (!bValid)
															break;
													}
													if (!bValid)
														continue;
													// same size of v1 and v2
													if ((imageRoofsWidth[1] != imageRoofsWidth[2]) || (imageRoofsHeight[1] != imageRoofsHeight[2]))
														continue;
													
													// condition 3
													int dis_left = 100000;
													int dis_right = 100000;
													int dis_top = 100000;
													int dis_bot = 100000;
													for (int i = 0; i < roof_paras.size(); i++){
														if (dis_left >(centers_w[i] - 0.5 * imageRoofsWidth[i]))
															dis_left = centers_w[i] - 0.5 * imageRoofsWidth[i];
														if (dis_right > (width - centers_w[i] - 0.5 * imageRoofsWidth[i]))
															dis_right = width - centers_w[i] - 0.5 * imageRoofsWidth[i];
														if (dis_top > (centers_h[i] - 0.5 * imageRoofsHeight[i]))
															dis_top = centers_h[i] - 0.5 * imageRoofsHeight[i];
														if (dis_bot > (height - centers_h[i] - 0.5 * imageRoofsHeight[i]))
															dis_bot = height - centers_h[i] - 0.5 * imageRoofsHeight[i];

													}
													if (dis_left != dis_right || dis_top != dis_bot)
														continue;
													if (dis_left * dis_top != 0)
														continue;
													// condition 2
													bValid = true;
													for (int i = 0; i < roof_paras.size(); i++){
														for (int j = i + 1; j < roof_paras.size(); j++){
															if (i == 1 && j == 2){
																int intersec_a = utils::rectIntersecRect(centers_w[i], centers_h[i], imageRoofsWidth[i], imageRoofsHeight[i], rotates[i], centers_w[j], centers_h[j], imageRoofsWidth[j], imageRoofsHeight[j], rotates[j]);
																int intersec_b = utils::rectIntersecRect(centers_w[j], centers_h[j], imageRoofsWidth[j], imageRoofsHeight[j], rotates[j], centers_w[i], centers_h[i], imageRoofsWidth[i], imageRoofsHeight[i], rotates[i]);
																if (intersec_a || intersec_b){
																	bValid = false;
																	break;
																}
															}
															else{
																int intersec_a = !utils::rectIntersecRect(centers_w[i], centers_h[i], imageRoofsWidth[i], imageRoofsHeight[i], rotates[i], centers_w[j], centers_h[j], imageRoofsWidth[j], imageRoofsHeight[j], rotates[j]);
																int intersec_b = !utils::rectIntersecRect(centers_w[j], centers_h[j], imageRoofsWidth[j], imageRoofsHeight[j], rotates[j], centers_w[i], centers_h[i], imageRoofsWidth[i], imageRoofsHeight[i], rotates[i]);
																if (intersec_a && intersec_b){
																	bValid = false;
																	break;
																}
															}
														}
														if (!bValid)
															break;
													}
													if (!bValid)
														continue;
													// condition 4
													int bot_h_v1 = top_h_v1 + roof_h_v1;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_h = top_h + roof_h;
													int bot_w_v1 = top_w_v1 + roof_w_v1;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													int bot_w = top_w + roof_w;
													if ((top_w > top_w_v1 && top_w < bot_w_v1) || (bot_w > top_w_v1 && bot_w < bot_w_v1))
														continue;
													if ((top_w > top_w_v2 && top_w < bot_w_v2) || (bot_w > top_w_v2 && bot_w < bot_w_v2))
														continue;
													if ((top_h_v1 > top_h && top_h_v1 < bot_h) || (bot_h_v1 > top_h && bot_h_v1 < bot_h))
														continue;
													if ((top_h_v2 > top_h && top_h_v2 < bot_h) || (bot_h_v2 > top_h && bot_h_v2 < bot_h))
														continue;


													cv::Mat roof_img(height, width, CV_8UC3, bg_color);
													DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
													if (!roof_img.empty()){
														char buffer[50];
														sprintf(buffer, "roof_image_%08d.png", index);
														std::string img_filename = "../data/node_three/" + std::string(buffer);
														std::cout << img_filename << std::endl;
														cv::imwrite(img_filename, roof_img);
														index++;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void test_four_nodes(int width, int height, int step_size, int type, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(4);
	std::vector<int> imageRoofsWidth;
	imageRoofsWidth.resize(roof_paras.size());
	std::vector<int> imageRoofsHeight;
	imageRoofsHeight.resize(roof_paras.size());
	std::vector<int> centers_w;
	centers_w.resize(roof_paras.size());
	std::vector<int> centers_h;
	centers_h.resize(roof_paras.size());
	std::vector<double> rotates;
	rotates.resize(roof_paras.size());
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += 0.5 * step_size){
				for (int top_h = 0; top_h < height; top_h += 0.5 * step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					roof_paras[0].clear();
					roof_paras[0].push_back(center_w);
					roof_paras[0].push_back(center_h);
					roof_paras[0].push_back(roof_w);
					roof_paras[0].push_back(roof_h);
					roof_paras[0].push_back(0);
					imageRoofsWidth[0] = roof_w;
					imageRoofsHeight[0] = roof_h;
					centers_w[0] = center_w;
					centers_h[0] = center_h;
					rotates[0] = 0;
					// check 
					// second rectangle
					for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
						for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += 0.5 * step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += 0.5 * step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1))
										continue;
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									imageRoofsWidth[1] = roof_w_v1;
									imageRoofsHeight[1] = roof_h_v1;
									centers_w[1] = center_w_v1;
									centers_h[1] = center_h_v1;
									rotates[1] = 0;
									if (centers_w[0] == centers_w[1] && centers_h[0] == centers_h[1] && imageRoofsWidth[0] == imageRoofsWidth[1] && imageRoofsHeight[0] == imageRoofsHeight[1])
										continue;
									int bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1]);
									if (!bIntersect)
										continue;
									// third rectangle
									for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
										for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
											for (int top_w_v2 = 0; top_w_v2 < width; top_w_v2 += 0.5 * step_size){
												for (int top_h_v2 = 0; top_h_v2 < height; top_h_v2 += 0.5 * step_size){
													int center_w_v2 = top_w_v2 + 0.5 * roof_w_v2;
													int center_h_v2 = top_h_v2 + 0.5 * roof_h_v2;
													if (!utils::rectInsideRect(width, height, center_w_v2, center_h_v2, roof_w_v2, roof_h_v2))
														continue;
													roof_paras[2].clear();
													roof_paras[2].push_back(center_w_v2);
													roof_paras[2].push_back(center_h_v2);
													roof_paras[2].push_back(roof_w_v2);
													roof_paras[2].push_back(roof_h_v2);
													roof_paras[2].push_back(0);
													imageRoofsWidth[2] = roof_w_v2;
													imageRoofsHeight[2] = roof_h_v2;
													centers_w[2] = center_w_v2;
													centers_h[2] = center_h_v2;
													rotates[2] = 0;
													if (centers_w[1] == centers_w[2] && centers_h[1] == centers_h[2] && imageRoofsWidth[1] == imageRoofsWidth[2] && imageRoofsHeight[1] == imageRoofsHeight[2])
														continue;
													bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (!bIntersect)
														continue;
													bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													// same size of v1 and v2
													if ((imageRoofsWidth[1] != imageRoofsWidth[2]) || (imageRoofsHeight[1] != imageRoofsHeight[2]))
														continue;
													// fourth rectangle
													for (int roof_w_v3 = roof_min_size; roof_w_v3 <= width; roof_w_v3 += step_size){
														for (int roof_h_v3 = roof_min_size; roof_h_v3 <= height; roof_h_v3 += step_size){
															for (int top_w_v3 = 0; top_w_v3 < width; top_w_v3 += 0.5 * step_size){
																for (int top_h_v3 = 0; top_h_v3 < height; top_h_v3 += 0.5 * step_size){
																	int center_w_v3 = top_w_v3 + 0.5 * roof_w_v3;
																	int center_h_v3 = top_h_v3 + 0.5 * roof_h_v3;
																	if (!utils::rectInsideRect(width, height, center_w_v3, center_h_v3, roof_w_v3, roof_h_v3))
																		continue;
																	roof_paras[3].clear();
																	roof_paras[3].push_back(center_w_v3);
																	roof_paras[3].push_back(center_h_v3);
																	roof_paras[3].push_back(roof_w_v3);
																	roof_paras[3].push_back(roof_h_v3);
																	roof_paras[3].push_back(0);
																	imageRoofsWidth[3] = roof_w_v3;
																	imageRoofsHeight[3] = roof_h_v3;
																	centers_w[3] = center_w_v3;
																	centers_h[3] = center_h_v3;
																	rotates[3] = 0;													
																	if (centers_w[2] == centers_w[3] && centers_h[2] == centers_h[3] && imageRoofsWidth[2] == imageRoofsWidth[3] && imageRoofsHeight[2] == imageRoofsHeight[3])
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (!bIntersect)
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	// same size of v1 and v2
																	if ((imageRoofsWidth[2] != imageRoofsWidth[3]) || (imageRoofsHeight[2] != imageRoofsHeight[3]))
																		continue;
																	// condition 3
																	int dis_left = 100000;
																	int dis_right = 100000;
																	int dis_top = 100000;
																	int dis_bot = 100000;
																	for (int i = 0; i < roof_paras.size(); i++){
																		if (dis_left >(centers_w[i] - 0.5 * imageRoofsWidth[i]))
																			dis_left = centers_w[i] - 0.5 * imageRoofsWidth[i];
																		if (dis_right > (width - centers_w[i] - 0.5 * imageRoofsWidth[i]))
																			dis_right = width - centers_w[i] - 0.5 * imageRoofsWidth[i];
																		if (dis_top > (centers_h[i] - 0.5 * imageRoofsHeight[i]))
																			dis_top = centers_h[i] - 0.5 * imageRoofsHeight[i];
																		if (dis_bot > (height - centers_h[i] - 0.5 * imageRoofsHeight[i]))
																			dis_bot = height - centers_h[i] - 0.5 * imageRoofsHeight[i];

																	}
																	if (dis_left != dis_right || dis_top != dis_bot)
																		continue;
																	if (dis_left * dis_top != 0)
																		continue;

																	// condition 4
																	int bot_h_v1 = top_h_v1 + roof_h_v1;
																	int bot_h_v2 = top_h_v2 + roof_h_v2;
																	int bot_h_v3 = top_h_v3 + roof_h_v3;
																	int bot_h = top_h + roof_h;
																	int bot_w_v1 = top_w_v1 + roof_w_v1;
																	int bot_w_v2 = top_w_v2 + roof_w_v2;
																	int bot_w_v3 = top_w_v3 + roof_w_v3;
																	int bot_w = top_w + roof_w;
																	if ((top_w > top_w_v1 && top_w < bot_w_v1) || (bot_w > top_w_v1 && bot_w < bot_w_v1))
																		continue;
																	if ((top_w > top_w_v2 && top_w < bot_w_v2) || (bot_w > top_w_v2 && bot_w < bot_w_v2))
																		continue;
																	if ((top_w > top_w_v3 && top_w < bot_w_v3) || (bot_w > top_w_v3 && bot_w < bot_w_v3))
																		continue;
																	if ((top_h_v1 > top_h && top_h_v1 < bot_h) || (bot_h_v1 > top_h && bot_h_v1 < bot_h))
																		continue;
																	if ((top_h_v2 > top_h && top_h_v2 < bot_h) || (bot_h_v2 > top_h && bot_h_v2 < bot_h))
																		continue;
																	if ((top_h_v3 > top_h && top_h_v3 < bot_h) || (bot_h_v3 > top_h && bot_h_v3 < bot_h))
																		continue;

																	cv::Mat roof_img(height, width, CV_8UC3, bg_color);
																	DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
																	if (!roof_img.empty()){
																		char buffer[50];
																		sprintf(buffer, "roof_image_%08d.png", index);
																		std::string img_filename = "../data/node_four/" + std::string(buffer);
																		std::cout << img_filename << std::endl;
																		cv::imwrite(img_filename, roof_img);
																		index++;
																	}

																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void test_four_nodes_c(int width, int height, int step_size, int type, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(4);
	std::vector<int> imageRoofsWidth;
	imageRoofsWidth.resize(roof_paras.size());
	std::vector<int> imageRoofsHeight;
	imageRoofsHeight.resize(roof_paras.size());
	std::vector<int> centers_w;
	centers_w.resize(roof_paras.size());
	std::vector<int> centers_h;
	centers_h.resize(roof_paras.size());
	std::vector<double> rotates;
	rotates.resize(roof_paras.size());
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += 0.5 * step_size){
				for (int top_h = 0; top_h < height; top_h += 0.5 * step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					roof_paras[0].clear();
					roof_paras[0].push_back(center_w);
					roof_paras[0].push_back(center_h);
					roof_paras[0].push_back(roof_w);
					roof_paras[0].push_back(roof_h);
					roof_paras[0].push_back(0);
					imageRoofsWidth[0] = roof_w;
					imageRoofsHeight[0] = roof_h;
					centers_w[0] = center_w;
					centers_h[0] = center_h;
					rotates[0] = 0;
					int bot_h = top_h + roof_h;
					int bot_w = top_w + roof_w;
					// check 
					// second rectangle
					for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
						for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += 0.5 * step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += 0.5 * step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1))
										continue;
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									imageRoofsWidth[1] = roof_w_v1;
									imageRoofsHeight[1] = roof_h_v1;
									centers_w[1] = center_w_v1;
									centers_h[1] = center_h_v1;
									rotates[1] = 0;
									if (centers_w[0] == centers_w[1] && centers_h[0] == centers_h[1] && imageRoofsWidth[0] == imageRoofsWidth[1] && imageRoofsHeight[0] == imageRoofsHeight[1])
										continue;
									int bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1]);
									if (!bIntersect)
										continue;
									// same size of v1 and v2
									if ((imageRoofsWidth[0] != imageRoofsWidth[1]) || (imageRoofsHeight[0] != imageRoofsHeight[1]))
										continue;
									int bot_h_v1 = top_h_v1 + roof_h_v1;
									int bot_w_v1 = top_w_v1 + roof_w_v1;
									if ((top_w > top_w_v1 && top_w < bot_w_v1) || (bot_w > top_w_v1 && bot_w < bot_w_v1))
										continue;
									if ((top_h_v1 > top_h && top_h_v1 < bot_h) || (bot_h_v1 > top_h && bot_h_v1 < bot_h))
										continue;
									// third rectangle
									for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
										for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
											for (int top_w_v2 = 0; top_w_v2 < width; top_w_v2 += 0.5 * step_size){
												for (int top_h_v2 = 0; top_h_v2 < height; top_h_v2 += 0.5 * step_size){
													int center_w_v2 = top_w_v2 + 0.5 * roof_w_v2;
													int center_h_v2 = top_h_v2 + 0.5 * roof_h_v2;
													if (!utils::rectInsideRect(width, height, center_w_v2, center_h_v2, roof_w_v2, roof_h_v2))
														continue;
													roof_paras[2].clear();
													roof_paras[2].push_back(center_w_v2);
													roof_paras[2].push_back(center_h_v2);
													roof_paras[2].push_back(roof_w_v2);
													roof_paras[2].push_back(roof_h_v2);
													roof_paras[2].push_back(0);
													imageRoofsWidth[2] = roof_w_v2;
													imageRoofsHeight[2] = roof_h_v2;
													centers_w[2] = center_w_v2;
													centers_h[2] = center_h_v2;
													rotates[2] = 0;
													if (centers_w[1] == centers_w[2] && centers_h[1] == centers_h[2] && imageRoofsWidth[1] == imageRoofsWidth[2] && imageRoofsHeight[1] == imageRoofsHeight[2])
														continue;
													bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (!bIntersect)
														continue;
													// same size of v1 and v2
													if ((imageRoofsWidth[1] != imageRoofsWidth[2]) || (imageRoofsHeight[1] != imageRoofsHeight[2]))
														continue;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													if ((top_w_v1 > top_w_v2 && top_w_v1 < bot_w_v2) || (bot_w_v1 > top_w_v2 && bot_w_v1 < bot_w_v2))
														continue;
													if ((top_h_v2 > top_h_v1 && top_h_v2 < bot_h_v1) || (bot_h_v2 > top_h_v1 && bot_h_v2 < bot_h_v1))
														continue;
													// fourth rectangle
													for (int roof_w_v3 = roof_min_size; roof_w_v3 <= width; roof_w_v3 += step_size){
														for (int roof_h_v3 = roof_min_size; roof_h_v3 <= height; roof_h_v3 += step_size){
															for (int top_w_v3 = 0; top_w_v3 < width; top_w_v3 += 0.5 * step_size){
																for (int top_h_v3 = 0; top_h_v3 < height; top_h_v3 += 0.5 * step_size){
																	int center_w_v3 = top_w_v3 + 0.5 * roof_w_v3;
																	int center_h_v3 = top_h_v3 + 0.5 * roof_h_v3;
																	if (!utils::rectInsideRect(width, height, center_w_v3, center_h_v3, roof_w_v3, roof_h_v3))
																		continue;
																	roof_paras[3].clear();
																	roof_paras[3].push_back(center_w_v3);
																	roof_paras[3].push_back(center_h_v3);
																	roof_paras[3].push_back(roof_w_v3);
																	roof_paras[3].push_back(roof_h_v3);
																	roof_paras[3].push_back(0);
																	imageRoofsWidth[3] = roof_w_v3;
																	imageRoofsHeight[3] = roof_h_v3;
																	centers_w[3] = center_w_v3;
																	centers_h[3] = center_h_v3;
																	rotates[3] = 0;
																	if (centers_w[2] == centers_w[3] && centers_h[2] == centers_h[3] && imageRoofsWidth[2] == imageRoofsWidth[3] && imageRoofsHeight[2] == imageRoofsHeight[3])
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (!bIntersect)
																		continue;
																	// same size of v1 and v2
																	if ((imageRoofsWidth[2] != imageRoofsWidth[3]) || (imageRoofsHeight[2] != imageRoofsHeight[3]))
																		continue;
																	int bot_h_v3 = top_h_v3 + roof_h_v3;
																	int bot_w_v3 = top_w_v3 + roof_w_v3;
																	if ((top_w_v2 > top_w_v3 && top_w_v2 < bot_w_v3) || (bot_w_v2 > top_w_v3 && bot_w_v2 < bot_w_v3))
																		continue;
																	if ((top_h_v3 > top_h_v2 && top_h_v3 < bot_h_v2) || (bot_h_v3 > top_h_v2 && bot_h_v3 < bot_h_v2))
																		continue;
																	// condition 3
																	int dis_left = 100000;
																	int dis_right = 100000;
																	int dis_top = 100000;
																	int dis_bot = 100000;
																	for (int i = 0; i < roof_paras.size(); i++){
																		if (dis_left >(centers_w[i] - 0.5 * imageRoofsWidth[i]))
																			dis_left = centers_w[i] - 0.5 * imageRoofsWidth[i];
																		if (dis_right > (width - centers_w[i] - 0.5 * imageRoofsWidth[i]))
																			dis_right = width - centers_w[i] - 0.5 * imageRoofsWidth[i];
																		if (dis_top > (centers_h[i] - 0.5 * imageRoofsHeight[i]))
																			dis_top = centers_h[i] - 0.5 * imageRoofsHeight[i];
																		if (dis_bot > (height - centers_h[i] - 0.5 * imageRoofsHeight[i]))
																			dis_bot = height - centers_h[i] - 0.5 * imageRoofsHeight[i];

																	}
																	if (dis_left != dis_right || dis_top != dis_bot)
																		continue;
																	if (dis_left * dis_top != 0)
																		continue;

																	cv::Mat roof_img(height, width, CV_8UC3, bg_color);
																	DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
																	if (!roof_img.empty()){
																		char buffer[50];
																		sprintf(buffer, "roof_image_%08d.png", index);
																		std::string img_filename = "../data/node_four_c/" + std::string(buffer);
																		std::cout << img_filename << std::endl;
																		cv::imwrite(img_filename, roof_img);
																		index++;
																	}

																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void test_four_nodes_d(int width, int height, int step_size, int type, int padding){
	cv::Scalar bg_color(0, 0, 0);
	cv::Scalar fg_color(0, 0, 255); // bgr
	int index = 0;
	int roof_min_size = 2 * step_size;
	std::vector<std::vector<int>> roof_paras;
	roof_paras.resize(4);
	std::vector<int> imageRoofsWidth;
	imageRoofsWidth.resize(roof_paras.size());
	std::vector<int> imageRoofsHeight;
	imageRoofsHeight.resize(roof_paras.size());
	std::vector<int> centers_w;
	centers_w.resize(roof_paras.size());
	std::vector<int> centers_h;
	centers_h.resize(roof_paras.size());
	std::vector<double> rotates;
	rotates.resize(roof_paras.size());
	// first rectangle
	for (int roof_w = roof_min_size; roof_w <= width; roof_w += step_size){
		for (int roof_h = roof_min_size; roof_h <= height; roof_h += step_size){
			for (int top_w = 0; top_w < width; top_w += 0.5 * step_size){
				for (int top_h = 0; top_h < height; top_h += 0.5 * step_size){
					int center_w = top_w + 0.5 * roof_w;
					int center_h = top_h + 0.5 * roof_h;
					if (!utils::rectInsideRect(width, height, center_w, center_h, roof_w, roof_h))
						continue;
					roof_paras[0].clear();
					roof_paras[0].push_back(center_w);
					roof_paras[0].push_back(center_h);
					roof_paras[0].push_back(roof_w);
					roof_paras[0].push_back(roof_h);
					roof_paras[0].push_back(0);
					imageRoofsWidth[0] = roof_w;
					imageRoofsHeight[0] = roof_h;
					centers_w[0] = center_w;
					centers_h[0] = center_h;
					rotates[0] = 0;
					int bot_h = top_h + roof_h;
					int bot_w = top_w + roof_w;
					// check 
					// second rectangle
					for (int roof_w_v1 = roof_min_size; roof_w_v1 <= width; roof_w_v1 += step_size){
						for (int roof_h_v1 = roof_min_size; roof_h_v1 <= height; roof_h_v1 += step_size){
							for (int top_w_v1 = 0; top_w_v1 < width; top_w_v1 += 0.5 * step_size){
								for (int top_h_v1 = 0; top_h_v1 < height; top_h_v1 += 0.5 * step_size){
									int center_w_v1 = top_w_v1 + 0.5 * roof_w_v1;
									int center_h_v1 = top_h_v1 + 0.5 * roof_h_v1;
									if (!utils::rectInsideRect(width, height, center_w_v1, center_h_v1, roof_w_v1, roof_h_v1))
										continue;
									roof_paras[1].clear();
									roof_paras[1].push_back(center_w_v1);
									roof_paras[1].push_back(center_h_v1);
									roof_paras[1].push_back(roof_w_v1);
									roof_paras[1].push_back(roof_h_v1);
									roof_paras[1].push_back(0);
									imageRoofsWidth[1] = roof_w_v1;
									imageRoofsHeight[1] = roof_h_v1;
									centers_w[1] = center_w_v1;
									centers_h[1] = center_h_v1;
									rotates[1] = 0;
									if (centers_w[0] == centers_w[1] && centers_h[0] == centers_h[1] && imageRoofsWidth[0] == imageRoofsWidth[1] && imageRoofsHeight[0] == imageRoofsHeight[1])
										continue;
									int bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1]);
									if (!bIntersect)
										continue;
									// same size of v1 and v2
									/*if ((imageRoofsWidth[0] != imageRoofsWidth[1]) || (imageRoofsHeight[0] != imageRoofsHeight[1]))
										continue;*/
									int bot_h_v1 = top_h_v1 + roof_h_v1;
									int bot_w_v1 = top_w_v1 + roof_w_v1;
									bool bTouch = utils::rectTouch(top_w, top_h, bot_w, bot_h, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
									if (!bTouch)
										continue;
									// third rectangle
									for (int roof_w_v2 = roof_min_size; roof_w_v2 <= width; roof_w_v2 += step_size){
										for (int roof_h_v2 = roof_min_size; roof_h_v2 <= height; roof_h_v2 += step_size){
											for (int top_w_v2 = 0; top_w_v2 < width; top_w_v2 += 0.5 * step_size){
												for (int top_h_v2 = 0; top_h_v2 < height; top_h_v2 += 0.5 * step_size){
													int center_w_v2 = top_w_v2 + 0.5 * roof_w_v2;
													int center_h_v2 = top_h_v2 + 0.5 * roof_h_v2;
													if (!utils::rectInsideRect(width, height, center_w_v2, center_h_v2, roof_w_v2, roof_h_v2))
														continue;
													roof_paras[2].clear();
													roof_paras[2].push_back(center_w_v2);
													roof_paras[2].push_back(center_h_v2);
													roof_paras[2].push_back(roof_w_v2);
													roof_paras[2].push_back(roof_h_v2);
													roof_paras[2].push_back(0);
													imageRoofsWidth[2] = roof_w_v2;
													imageRoofsHeight[2] = roof_h_v2;
													centers_w[2] = center_w_v2;
													centers_h[2] = center_h_v2;
													rotates[2] = 0;
													if (centers_w[1] == centers_w[2] && centers_h[1] == centers_h[2] && imageRoofsWidth[1] == imageRoofsWidth[2] && imageRoofsHeight[1] == imageRoofsHeight[2])
														continue;
													bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (bIntersect)
														continue;
													bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2]);
													if (!bIntersect)
														continue;
													// same size of v1 and v2
													if ((imageRoofsWidth[0] != imageRoofsWidth[2]) || (imageRoofsHeight[0] != imageRoofsHeight[2]))
														continue;
													int bot_h_v2 = top_h_v2 + roof_h_v2;
													int bot_w_v2 = top_w_v2 + roof_w_v2;
													bTouch = utils::rectTouch(top_w_v2, top_h_v2, bot_w_v2, bot_h_v2, top_w_v1, top_h_v1, bot_w_v1, bot_h_v1);
													if (!bTouch)
														continue;
													// fourth rectangle
													for (int roof_w_v3 = roof_min_size; roof_w_v3 <= width; roof_w_v3 += step_size){
														for (int roof_h_v3 = roof_min_size; roof_h_v3 <= height; roof_h_v3 += step_size){
															for (int top_w_v3 = 0; top_w_v3 < width; top_w_v3 += 0.5 * step_size){
																for (int top_h_v3 = 0; top_h_v3 < height; top_h_v3 += 0.5 * step_size){
																	int center_w_v3 = top_w_v3 + 0.5 * roof_w_v3;
																	int center_h_v3 = top_h_v3 + 0.5 * roof_h_v3;
																	if (!utils::rectInsideRect(width, height, center_w_v3, center_h_v3, roof_w_v3, roof_h_v3))
																		continue;
																	roof_paras[3].clear();
																	roof_paras[3].push_back(center_w_v3);
																	roof_paras[3].push_back(center_h_v3);
																	roof_paras[3].push_back(roof_w_v3);
																	roof_paras[3].push_back(roof_h_v3);
																	roof_paras[3].push_back(0);
																	imageRoofsWidth[3] = roof_w_v3;
																	imageRoofsHeight[3] = roof_h_v3;
																	centers_w[3] = center_w_v3;
																	centers_h[3] = center_h_v3;
																	rotates[3] = 0;
																	if (centers_w[2] == centers_w[3] && centers_h[2] == centers_h[3] && imageRoofsWidth[2] == imageRoofsWidth[3] && imageRoofsHeight[2] == imageRoofsHeight[3])
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[0], centers_h[0], imageRoofsWidth[0], imageRoofsHeight[0], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (!bIntersect)
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[1], centers_h[1], imageRoofsWidth[1], imageRoofsHeight[1], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (bIntersect)
																		continue;
																	bIntersect = utils::rectIntersecRect(centers_w[2], centers_h[2], imageRoofsWidth[2], imageRoofsHeight[2], centers_w[3], centers_h[3], imageRoofsWidth[3], imageRoofsHeight[3]);
																	if (!bIntersect)
																		continue;
																	// same size of v1 and v2
																	if ((imageRoofsWidth[1] != imageRoofsWidth[3]) || (imageRoofsHeight[1] != imageRoofsHeight[3]))
																		continue;
																	int bot_h_v3 = top_h_v3 + roof_h_v3;
																	int bot_w_v3 = top_w_v3 + roof_w_v3;
																	bTouch = utils::rectTouch(top_w_v2, top_h_v2, bot_w_v2, bot_h_v2, top_w_v3, top_h_v3, bot_w_v3, bot_h_v3);
																	if (!bTouch)
																		continue;
																	bTouch = utils::rectTouch(top_w, top_h, bot_w, bot_h, top_w_v3, top_h_v3, bot_w_v3, bot_h_v3);
																	if (!bTouch)
																		continue;
																	// condition 3
																	int dis_left = 100000;
																	int dis_right = 100000;
																	int dis_top = 100000;
																	int dis_bot = 100000;
																	for (int i = 0; i < roof_paras.size(); i++){
																		if (dis_left >(centers_w[i] - 0.5 * imageRoofsWidth[i]))
																			dis_left = centers_w[i] - 0.5 * imageRoofsWidth[i];
																		if (dis_right >(width - centers_w[i] - 0.5 * imageRoofsWidth[i]))
																			dis_right = width - centers_w[i] - 0.5 * imageRoofsWidth[i];
																		if (dis_top > (centers_h[i] - 0.5 * imageRoofsHeight[i]))
																			dis_top = centers_h[i] - 0.5 * imageRoofsHeight[i];
																		if (dis_bot > (height - centers_h[i] - 0.5 * imageRoofsHeight[i]))
																			dis_bot = height - centers_h[i] - 0.5 * imageRoofsHeight[i];

																	}
																	if (dis_left != dis_right || dis_top != dis_bot)
																		continue;
																	if (dis_left * dis_top != 0)
																		continue;

																	cv::Mat roof_img(height, width, CV_8UC3, bg_color);
																	DrawRotatedRect::generateRect(roof_img, padding, roof_paras, RoofTypes::FLAT, bg_color, fg_color);
																	if (!roof_img.empty()){
																		char buffer[50];
																		sprintf(buffer, "roof_image_%08d.png", index);
																		std::string img_filename = "../data/node_four_d/" + std::string(buffer);
																		std::cout << img_filename << std::endl;
																		cv::imwrite(img_filename, roof_img);
																		index++;
																	}

																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}