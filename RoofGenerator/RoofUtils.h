#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Windows.h>
#include <boost/shared_ptr.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include "print.h"
#include "gdal_priv.h"
#include <dlib/clustering.h>
#include <dlib/rand.h>

using namespace dlib;


namespace roof_utils {

	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
	typedef K::Point_2                   Point;
	typedef CGAL::Polygon_2<K>           Polygon_2;
	typedef CGAL::Straight_skeleton_2<K> Ss;
	typedef boost::shared_ptr<Ss> SsPtr;

	std::vector<std::vector<float>> read_tiff_float(const char* type_tiff);
	void translate_bld_mask(const char* mask_tiff, std::string output_img_file);
	std::vector<std::vector<int>> read_tiff_int(const char* mask_tiff);
	void crop_img_from_mask(const char* src_tiff, std::vector<std::vector<int>> & type_info, std::string output_img_file);
	void findSkeleton(std::string src_img_file, std::string output_img_file);
	void findSkeleton(std::string src_img_file, std::string mask_img_file, std::string output_img_file);
	bool isClockwise(const std::vector<cv::Point>& polygon);
	std::vector<std::string> get_all_files_names_within_folder(std::string folder);
	void generate_from_mask(std::string aoi_path);
	void generate_from_mask_oriented(std::string aoi_path);
	void generate_from_ndsm(std::string aoi_path);
	void generate_from_entroy(std::string src_path, std::string output_path);
	void generate_from_simplify(std::string aoi_path);
	void translate_bld_ndsm(const char* mask_tiff, std::string output_img_file, float threshold);
	void translate_entropy_imgs(std::string entropy_file, float threshold, std::string output_img_file);
	void transform2roofFit(std::string src_img_file, std::string output_img_file);
	void clustering_kkmeans(std::string src_img_file, std::string output_img_file, int clusters);
	void clustering_kkmeans_mask(std::string src_img_file, std::string mask_img_file, std::string output_img_file, int clusters);
	void findContours_from_colorList(std::string src_img_file, std::vector<cv::Scalar> colorList, std::string output_img_file);

}