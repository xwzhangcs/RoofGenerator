#include "RoofUtils.h"
#include <iostream>

namespace roof_utils {

	void clustering_kkmeans(std::string src_img_file, std::string output_img_file, int clusters){
		// Here we declare that our samples will be 2 dimensional column vectors.  
		// (Note that if you don't know the dimensionality of your vectors at compile time
		// you can change the 2 to a 0 and then set the size at runtime)
		typedef matrix<double, 0, 1> sample_type;
		// Now we are making a typedef for the kind of kernel we want to use.  I picked the
		// radial basis kernel because it only has one parameter and generally gives good
		// results without much fiddling.
		typedef radial_basis_kernel<sample_type> kernel_type;


		// Here we declare an instance of the kcentroid object.  It is the object used to 
		// represent each of the centers used for clustering.  The kcentroid has 3 parameters 
		// you need to set.  The first argument to the constructor is the kernel we wish to 
		// use.  The second is a parameter that determines the numerical accuracy with which 
		// the object will perform part of the learning algorithm.  Generally, smaller values 
		// give better results but cause the algorithm to attempt to use more dictionary vectors 
		// (and thus run slower and use more memory).  The third argument, however, is the 
		// maximum number of dictionary vectors a kcentroid is allowed to use.  So you can use
		// it to control the runtime complexity.  
		kcentroid<kernel_type> kc(kernel_type(0.2), 0.01, 16);

		// Now we make an instance of the kkmeans object and tell it to use kcentroid objects
		// that are configured with the parameters from the kc object we defined above.
		kkmeans<kernel_type> test(kc);

		std::vector<sample_type> samples;
		std::vector<sample_type> initial_centers;

		cv::Mat src_img = cv::imread(src_img_file, CV_LOAD_IMAGE_UNCHANGED);
		sample_type m(src_img.channels());

		// only do clustering on the not-background pixes
		for (int i = 0; i < src_img.size().height; i++) {
			for (int j = 0; j < src_img.size().width; j++) {
				if (src_img.channels() == 3) {
					if (src_img.at<cv::Vec3b>(i, j)[0] == 0 && src_img.at<cv::Vec3b>(i, j)[1] == 0 && src_img.at<cv::Vec3b>(i, j)[2] == 0)
						continue;
					m(0) = src_img.at<cv::Vec3b>(i, j)[0] * 1.0 / 255;
					m(1) = src_img.at<cv::Vec3b>(i, j)[1] * 1.0 / 255;
					m(2) = src_img.at<cv::Vec3b>(i, j)[2] * 1.0 / 255;
				}
				else {
					if ((int)src_img.at<uchar>(i, j) == 0)
						continue;
					m(0) = (int)src_img.at<uchar>(i, j) * 1.0 / 255;
				}
				// add this sample to our set of samples we will run k-means 
				samples.push_back(m);
			}
		}

		// tell the kkmeans object we made that we want to run k-means with k set to 3. 
		// (i.e. we want 3 clusters)
		test.set_number_of_centers(clusters);

		// You need to pick some initial centers for the k-means algorithm.  So here
		// we will use the dlib::pick_initial_centers() function which tries to find
		// n points that are far apart (basically).  
		pick_initial_centers(clusters, initial_centers, samples, test.get_kernel());

		// now run the k-means algorithm on our set of samples.  
		test.train(samples, initial_centers);

		std::vector<cv::Scalar> clusters_colors;
		std::vector<int> clusters_points;
		clusters_colors.resize(clusters);
		clusters_points.resize(clusters);
		cv::RNG rng(12345);
		for (int i = 0; i < clusters; i++) {
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			clusters_colors[i] = color;
			clusters_points[i] = 0;
		}

		int count = 0;
		/*
		if (src_img.channels() == 3) {
		count = 0;
		for (int i = 0; i < src_img.size().height; i++) {
		for (int j = 0; j < src_img.size().width; j++) {
		clusters_colors[test(samples[count])][0] += src_img.at<cv::Vec3b>(i, j)[0];
		clusters_colors[test(samples[count])][1] += src_img.at<cv::Vec3b>(i, j)[1];
		clusters_colors[test(samples[count])][2] += src_img.at<cv::Vec3b>(i, j)[2];
		clusters_points[test(samples[count])] ++;
		count++;
		}
		}
		for (int i = 0; i < clusters; i++) {
		clusters_colors[i][0] = clusters_colors[i][0] / clusters_points[i];
		clusters_colors[i][1] = clusters_colors[i][1] / clusters_points[i];
		clusters_colors[i][2] = clusters_colors[i][2] / clusters_points[i];
		}
		}
		else if (src_img.channels() == 1) { //gray image
		int count = 0;
		for (int i = 0; i < src_img.size().height; i++) {
		for (int j = 0; j < src_img.size().width; j++) {
		clusters_colors[test(samples[count])][0] += (int)src_img.at<uchar>(i, j);
		clusters_points[test(samples[count])] ++;
		count++;
		}
		}
		for (int i = 0; i < clusters; i++) {
		clusters_colors[i][0] = clusters_colors[i][0] / clusters_points[i];
		}
		}
		else {
		//do nothing
		}
		*/
		cv::Scalar bg_color(0, 0, 0); // black for background
		cv::Mat out_img(cv::Size(src_img.size().width, src_img.size().height), CV_8UC3, bg_color);
		count = 0;
		for (int i = 0; i < out_img.size().height; i++) {
			for (int j = 0; j < out_img.size().width; j++) {
				/*if (src_img.channels() == 1){
				out_img.at<uchar>(i, j) = (uchar)clusters_colors[test(samples[count])][0];
				}
				else{
				out_img.at<cv::Vec3b>(i, j)[0] = clusters_colors[test(samples[count])][0];
				out_img.at<cv::Vec3b>(i, j)[1] = clusters_colors[test(samples[count])][1];
				out_img.at<cv::Vec3b>(i, j)[2] = clusters_colors[test(samples[count])][2];
				}*/
				if (src_img.channels() == 3) {
					if (src_img.at<cv::Vec3b>(i, j)[0] == 0 && src_img.at<cv::Vec3b>(i, j)[1] == 0 && src_img.at<cv::Vec3b>(i, j)[2] == 0)
						continue;
				}
				else {
					if ((int)src_img.at<uchar>(i, j) == 0)
						continue;
				}
				out_img.at<cv::Vec3b>(i, j)[0] = clusters_colors[test(samples[count])][0];
				out_img.at<cv::Vec3b>(i, j)[1] = clusters_colors[test(samples[count])][1];
				out_img.at<cv::Vec3b>(i, j)[2] = clusters_colors[test(samples[count])][2];
				count++;
			}
		}

		cv::imwrite(output_img_file, out_img);
	}

	void clustering_kkmeans_mask(std::string src_img_file, std::string mask_img_file, std::string output_img_file, int clusters){
		// Here we declare that our samples will be 2 dimensional column vectors.  
		// (Note that if you don't know the dimensionality of your vectors at compile time
		// you can change the 2 to a 0 and then set the size at runtime)
		typedef matrix<double, 0, 1> sample_type;
		// Now we are making a typedef for the kind of kernel we want to use.  I picked the
		// radial basis kernel because it only has one parameter and generally gives good
		// results without much fiddling.
		typedef radial_basis_kernel<sample_type> kernel_type;


		// Here we declare an instance of the kcentroid object.  It is the object used to 
		// represent each of the centers used for clustering.  The kcentroid has 3 parameters 
		// you need to set.  The first argument to the constructor is the kernel we wish to 
		// use.  The second is a parameter that determines the numerical accuracy with which 
		// the object will perform part of the learning algorithm.  Generally, smaller values 
		// give better results but cause the algorithm to attempt to use more dictionary vectors 
		// (and thus run slower and use more memory).  The third argument, however, is the 
		// maximum number of dictionary vectors a kcentroid is allowed to use.  So you can use
		// it to control the runtime complexity.  
		kcentroid<kernel_type> kc(kernel_type(0.15), 0.01, 16);

		// Now we make an instance of the kkmeans object and tell it to use kcentroid objects
		// that are configured with the parameters from the kc object we defined above.
		kkmeans<kernel_type> test(kc);

		std::vector<sample_type> samples;
		std::vector<sample_type> initial_centers;

		cv::Mat src_img = cv::imread(src_img_file, CV_LOAD_IMAGE_UNCHANGED);
		cv::Mat mask_img = cv::imread(mask_img_file, CV_LOAD_IMAGE_UNCHANGED);
		sample_type m(src_img.channels());

		// only do clustering on the not-background pixes
		for (int i = 0; i < src_img.size().height; i++) {
			for (int j = 0; j < src_img.size().width; j++) {
				if (src_img.channels() == 3) {
					if ((int)mask_img.at<uchar>(i, j) == 0)
						continue;
					m(0) = src_img.at<cv::Vec3b>(i, j)[0] * 1.0 / 255;
					m(1) = src_img.at<cv::Vec3b>(i, j)[1] * 1.0 / 255;
					m(2) = src_img.at<cv::Vec3b>(i, j)[2] * 1.0 / 255;
				}
				else {
					if ((int)mask_img.at<uchar>(i, j) == 0)
						continue;
					m(0) = (int)src_img.at<uchar>(i, j) * 1.0 / 255;
				}
				// add this sample to our set of samples we will run k-means 
				samples.push_back(m);
			}
		}

		// tell the kkmeans object we made that we want to run k-means with k set to 3. 
		// (i.e. we want 3 clusters)
		test.set_number_of_centers(clusters);

		// You need to pick some initial centers for the k-means algorithm.  So here
		// we will use the dlib::pick_initial_centers() function which tries to find
		// n points that are far apart (basically).  
		pick_initial_centers(clusters, initial_centers, samples, test.get_kernel());

		// now run the k-means algorithm on our set of samples.  
		test.train(samples, initial_centers);

		std::vector<cv::Scalar> clusters_colors;
		std::vector<int> clusters_points;
		clusters_colors.resize(clusters);
		clusters_points.resize(clusters);
		cv::RNG rng(12345);
		for (int i = 0; i < clusters; i++) {
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			clusters_colors[i] = color;
			clusters_points[i] = 0;
		}

		int count = 0;
		/*
		if (src_img.channels() == 3) {
		count = 0;
		for (int i = 0; i < src_img.size().height; i++) {
		for (int j = 0; j < src_img.size().width; j++) {
		clusters_colors[test(samples[count])][0] += src_img.at<cv::Vec3b>(i, j)[0];
		clusters_colors[test(samples[count])][1] += src_img.at<cv::Vec3b>(i, j)[1];
		clusters_colors[test(samples[count])][2] += src_img.at<cv::Vec3b>(i, j)[2];
		clusters_points[test(samples[count])] ++;
		count++;
		}
		}
		for (int i = 0; i < clusters; i++) {
		clusters_colors[i][0] = clusters_colors[i][0] / clusters_points[i];
		clusters_colors[i][1] = clusters_colors[i][1] / clusters_points[i];
		clusters_colors[i][2] = clusters_colors[i][2] / clusters_points[i];
		}
		}
		else if (src_img.channels() == 1) { //gray image
		int count = 0;
		for (int i = 0; i < src_img.size().height; i++) {
		for (int j = 0; j < src_img.size().width; j++) {
		clusters_colors[test(samples[count])][0] += (int)src_img.at<uchar>(i, j);
		clusters_points[test(samples[count])] ++;
		count++;
		}
		}
		for (int i = 0; i < clusters; i++) {
		clusters_colors[i][0] = clusters_colors[i][0] / clusters_points[i];
		}
		}
		else {
		//do nothing
		}
		*/
		cv::Scalar bg_color(0, 0, 0); // black for background
		cv::Mat out_img(cv::Size(src_img.size().width, src_img.size().height), CV_8UC3, bg_color);
		count = 0;
		for (int i = 0; i < out_img.size().height; i++) {
			for (int j = 0; j < out_img.size().width; j++) {
				/*if (src_img.channels() == 1){
				out_img.at<uchar>(i, j) = (uchar)clusters_colors[test(samples[count])][0];
				}
				else{
				out_img.at<cv::Vec3b>(i, j)[0] = clusters_colors[test(samples[count])][0];
				out_img.at<cv::Vec3b>(i, j)[1] = clusters_colors[test(samples[count])][1];
				out_img.at<cv::Vec3b>(i, j)[2] = clusters_colors[test(samples[count])][2];
				}*/
				if ((int)mask_img.at<uchar>(i, j) == 0)
					continue;
				out_img.at<cv::Vec3b>(i, j)[0] = clusters_colors[test(samples[count])][0];
				out_img.at<cv::Vec3b>(i, j)[1] = clusters_colors[test(samples[count])][1];
				out_img.at<cv::Vec3b>(i, j)[2] = clusters_colors[test(samples[count])][2];
				count++;
			}
		}

		cv::imwrite(output_img_file, out_img);
	}

	void transform2roofFit(std::string src_img_file, std::string output_img_file){
		cv::Mat src = cv::imread(src_img_file, CV_LOAD_IMAGE_UNCHANGED);
		// size of the source img
		int width = src.size().width;
		int height = src.size().height;
		int target = width > height ? 2 * width : 2 * height;
		// add padding
		int borderType = cv::BORDER_CONSTANT;
		cv::Scalar value(0, 0, 0);
		cv::Mat img_padding;
		cv::copyMakeBorder(src, img_padding, (target - height) * 0.5, (target - height) * 0.5, (target - width) * 0.5, (target - width) * 0.5, borderType, value);
		cv::imwrite(output_img_file, img_padding);
	}

	void generate_from_entroy(std::string src_path, std::string output_path){
		std::string path(src_path);
		std::vector<std::string> sub_imgs = get_all_files_names_within_folder(path);
		// normal
		for (int i = 0; i < sub_imgs.size(); i++){
			std::string src_entropy_img = path + "/" + sub_imgs[i];
			std::string output_entropy_img = output_path + "/" + sub_imgs[i];
			std::cout << "output_entropy_img is " << output_entropy_img << std::endl;
			float threshold = 120;
			translate_entropy_imgs(src_entropy_img, threshold, output_entropy_img);
		}
	}

	void generate_from_ndsm(std::string aoi_path){
		std::string path(aoi_path);
		std::vector<std::string> sub_folders = get_all_files_names_within_folder(path);
		// normal
		for (int i = 0; i < sub_folders.size(); i++){
			std::string sub_folder = sub_folders[i];
			std::string bld_mask_tif = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__NDSM.tif";
			std::string bld_mask_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__NDSM.png";
			//std::cout << "bld_mask is " << bld_mask << std::endl;
			std::vector<std::vector<int>> type_info;
			translate_bld_ndsm(bld_mask_tif.c_str(), bld_mask_img, 0.3);
			/*type_info = read_tiff_int(bld_mask_tif.c_str());
			std::string bld_pan_tif = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoPAN.tif";
			std::string bld_pan_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoPAN.png";
			std::string bld_rgb_tif = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoRGB.tif";
			std::string bld_rgb_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoRGB.png";
			crop_img_from_mask(bld_pan_tif.c_str(), type_info, bld_pan_img);
			crop_img_from_mask(bld_rgb_tif.c_str(), type_info, bld_rgb_img);
			std::string skeleton_mask_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__Skeleton_mask.png";
			findSkeleton(bld_mask_img, skeleton_mask_img);
			std::string skeleton_rgb_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__Skeleton.png";
			findSkeleton(bld_mask_img, bld_rgb_img, skeleton_rgb_img);*/
		}
	}

	void generate_from_mask(std::string aoi_path){
		std::string path(aoi_path);
		std::vector<std::string> sub_folders = get_all_files_names_within_folder(path);
		// normal
		for (int i = 0; i < sub_folders.size(); i++){
			std::string sub_folder = sub_folders[i];
			std::string bld_mask_tif = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__segment_mask.tif";
			std::string bld_mask_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__segment_mask.png";
			//std::cout << "bld_mask is " << bld_mask << std::endl;
			std::vector<std::vector<int>> type_info;
			translate_bld_mask(bld_mask_tif.c_str(), bld_mask_img);
			type_info = read_tiff_int(bld_mask_tif.c_str());
			std::string bld_pan_tif = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoPAN.tif";
			std::string bld_pan_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoPAN.png";
			std::string bld_rgb_tif = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoRGB.tif";
			std::string bld_rgb_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoRGB.png";
			crop_img_from_mask(bld_pan_tif.c_str(), type_info, bld_pan_img);
			crop_img_from_mask(bld_rgb_tif.c_str(), type_info, bld_rgb_img);
			std::string skeleton_mask_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__Skeleton_mask.png";
			findSkeleton(bld_mask_img, skeleton_mask_img);
			std::string skeleton_rgb_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__Skeleton.png";
			findSkeleton(bld_mask_img, bld_rgb_img, skeleton_rgb_img);
		}
	}

	void generate_from_mask_oriented(std::string aoi_path){
		std::string path(aoi_path);
		std::vector<std::string> sub_folders = get_all_files_names_within_folder(path);
		// normal
		for (int i = 0; i < sub_folders.size(); i++){
			std::string sub_folder = sub_folders[i];
			std::string bld_mask_tif = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__segment_oriented.tif";
			std::string bld_mask_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__segment_oriented.png";
			//std::cout << "bld_mask is " << bld_mask << std::endl;
			std::vector<std::vector<int>> type_info;
			translate_bld_mask(bld_mask_tif.c_str(), bld_mask_img);
			type_info = read_tiff_int(bld_mask_tif.c_str());
			std::string bld_pan_tif = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoPAN_oriented.tif";
			std::string bld_pan_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoPAN_oriented.png";
			std::string bld_rgb_tif = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoRGB_oriented.tif";
			std::string bld_rgb_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoRGB_oriented.png";
			crop_img_from_mask(bld_pan_tif.c_str(), type_info, bld_pan_img);
			crop_img_from_mask(bld_rgb_tif.c_str(), type_info, bld_rgb_img);
		}
	}

	void generate_from_simplify(std::string aoi_path){
		std::string path(aoi_path);
		std::vector<std::string> sub_folders = get_all_files_names_within_folder(path);
		// normal
		for (int i = 0; i < sub_folders.size(); i++){
			std::string sub_folder = sub_folders[i];
			std::string bld_mask_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__segment_mask_simplify.png";
			std::string bld_pan_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoPAN.png";
			std::string bld_rgb_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__OrthoRGB.png";
			std::string skeleton_mask_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__Skeleton_mask_simplify.png";
			findSkeleton(bld_mask_img, skeleton_mask_img);
			std::string skeleton_rgb_img = path + "/" + sub_folder + "/building_cluster_" + sub_folder + "__Skeleton_simplify.png";
			findSkeleton(bld_mask_img, bld_rgb_img, skeleton_rgb_img);
		}
	}

	void translate_entropy_imgs(std::string entropy_file, float threshold, std::string output_img_file){
		cv::Mat src = cv::imread(entropy_file, CV_LOAD_IMAGE_UNCHANGED);
		cv::Scalar bg_color(0, 0, 0); // black for background
		cv::Mat output_img(src.size(), CV_8UC1, bg_color);
		for (int i = 0; i < src.size().height; i++){
			for (int j = 0; j < src.size().width; j++){
				if ((int)src.at<uchar>(i, j) > threshold)
					output_img.at<uchar>(i, j) = (uchar)255/*src.at<uchar>(i, j)*/;
			}
		}
		// do erosion
		int erosion_type = cv::MORPH_RECT;
		cv::Mat erode_dst;
		int kernel_size = 3;
		cv::Mat element = cv::getStructuringElement(erosion_type, cv::Size(kernel_size, kernel_size), cv::Point(kernel_size / 2, kernel_size / 2));
		/// Apply the dilation operation
		cv::erode(output_img, erode_dst, element);

		cv::imwrite(output_img_file, erode_dst);
	}

	void findContours_from_colorList(std::string src_img_file, std::vector<cv::Scalar> colorList, std::string output_img_file){
		cv::Mat src = cv::imread(src_img_file, CV_LOAD_IMAGE_UNCHANGED);
		std::vector<int> colorChosen;
		colorChosen.resize(colorList.size());
		int clusters = 0;
		for (int i = 0; i < colorList.size(); i++)
			colorChosen[i] = 0;
		for (int i = 0; i < src.size().height; i++){
			for (int j = 0; j < src.size().width; j++){
				for (int c = 0; c < colorList.size(); c++)
					if (src.at<cv::Vec3b>(i, j)[0] == colorList[c][0] &&
						src.at<cv::Vec3b>(i, j)[1] == colorList[c][1] &&
						src.at<cv::Vec3b>(i, j)[2] == colorList[c][2] &&
						colorChosen[c] == 0){
						clusters++;
						colorChosen[c] = 1;
					}
			}
		}
		std::cout << "clusters is " << clusters << std::endl;
		cv::Scalar bg_color(0, 0, 0); // black for background
		cv::Mat output_img(src.size(), CV_8UC3, bg_color);
		int selected_clusters = 0;
		for (int c = 0; c < colorList.size(); c++){
			if (colorChosen[c] == 1){
				cv::Mat cluster_img = src.clone();
				for (int i = 0; i < src.size().height; i++){
					for (int j = 0; j < src.size().width; j++){
						if (cluster_img.at<cv::Vec3b>(i, j)[0] != colorList[c][0] ||
							cluster_img.at<cv::Vec3b>(i, j)[1] != colorList[c][1] ||
							cluster_img.at<cv::Vec3b>(i, j)[2] != colorList[c][2])
						{
							cluster_img.at<cv::Vec3b>(i, j)[0] = bg_color[0];
							cluster_img.at<cv::Vec3b>(i, j)[1] = bg_color[1];
							cluster_img.at<cv::Vec3b>(i, j)[2] = bg_color[2];
						}
					}
				}
				// find contour
				cv::Mat cluster_gray;
				cvtColor(cluster_img, cluster_gray, CV_BGR2GRAY);
				std::vector<std::vector<cv::Point> > contours;
				std::vector<cv::Vec4i> hierarchy;
				float threshod = 0.09;
				cv::findContours(cluster_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
				for (size_t i = 0; i< contours.size(); i++)
				{
					float area_contour = cv::contourArea(contours[i]);
					float ratio = area_contour / (src.size().height * src.size().width);
					std::cout << "ratio is " << ratio << std::endl;
					if (ratio < threshod) continue;
					cv::Scalar color = cv::Scalar(255, 255, 255);
					drawContours(output_img, contours, (int)i, color, 2, 8, hierarchy, 0, cv::Point());
					std::cout << "selected_ratio is " << ratio << std::endl;
					selected_clusters++;
				}
			}
		}
		std::cout << "selected_clusters is " << selected_clusters << std::endl;
		// adjust size to fit training
		int borderType = cv::BORDER_CONSTANT;
		cv::Scalar value(0, 0, 0);
		cv::Mat img_padding;
		int target_width = 224;
		int target_height = 224;
		int target_width_ratio = std::ceil(output_img.size().width * 1.0 / target_width);
		int target_height_ratio = std::ceil(output_img.size().height * 1.0 / target_height);
		std::cout << "target_width_ratio is " << target_width_ratio << std::endl;
		std::cout << "target_height_ratio is " << target_height_ratio << std::endl;
		int left_padding =(target_width_ratio * target_width - output_img.size().width) * 0.5;
		int right_padding = target_width_ratio * target_width - output_img.size().width - left_padding;
		int top_padding = (target_height_ratio * target_height - output_img.size().height) * 0.5;
		int bottom_padding = target_height_ratio * target_height - output_img.size().height - top_padding;
		cv::copyMakeBorder(output_img, img_padding, top_padding, bottom_padding, left_padding, right_padding, borderType, value);
		std::cout << "img_padding size is " << img_padding.size() << std::endl;
		cv::resize(img_padding, img_padding, cv::Size(target_width, target_height));
		// correct the color
		for (int i = 0; i < img_padding.size().height; i++) {
			for (int j = 0; j < img_padding.size().width; j++) {
				//noise
				if (img_padding.at<cv::Vec3b>(i, j)[0] > 128 && img_padding.at<cv::Vec3b>(i, j)[1] > 128 && img_padding.at<cv::Vec3b>(i, j)[2] > 128) {
					img_padding.at<cv::Vec3b>(i, j)[0] = 255;
					img_padding.at<cv::Vec3b>(i, j)[1] = 255;
					img_padding.at<cv::Vec3b>(i, j)[2] = 255;
				}
				else{
					img_padding.at<cv::Vec3b>(i, j)[0] = 0;
					img_padding.at<cv::Vec3b>(i, j)[1] = 0;
					img_padding.at<cv::Vec3b>(i, j)[2] = 0;
				}
			}
		}
		cv::imwrite(output_img_file, img_padding);

	}

	void findSkeleton(std::string src_img_file, std::string output_img_file){
		cv::Mat src = cv::imread(src_img_file, CV_LOAD_IMAGE_UNCHANGED);
		std::vector<std::vector<cv::Point> > contours;
		std::vector<std::vector<cv::Point> > contours_approx;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

		cv::Mat drawing = cv::Mat::zeros(src.size(), CV_8UC3);
		contours_approx.resize(contours.size());
		double epsilon = 4.0;
		for (size_t i = 0; i< contours.size(); i++)
		{
			cv::Scalar color = cv::Scalar(255, 255, 255);
			drawContours(drawing, contours, (int)i, color, 1, 8, hierarchy, 0, cv::Point());
			cv::approxPolyDP(contours[i], contours_approx[i], epsilon, true);
			while (contours_approx[i].size() > 80){
				epsilon += 2;
				contours_approx[i].clear();
				cv::approxPolyDP(contours[i], contours_approx[i], epsilon, true);
			}
			epsilon = 4;
		}

		for (size_t i = 0; i< contours_approx.size(); i++)
		{
			cv::Scalar color = cv::Scalar(255, 0, 0);
			drawContours(drawing, contours_approx, (int)i, color, 1, 8, hierarchy, 0, cv::Point());
		}
		/*cv::namedWindow("Contours", cv::WINDOW_AUTOSIZE);
		imshow("Contours", drawing);
		cv::waitKey(0);*/
		if (isClockwise(contours_approx[0])){
			std::cout << "clockwise" << std::endl;
			std::reverse(contours_approx[0].begin(), contours_approx[0].end());
		}
		else{
			std::cout << "counterClockwise" << std::endl;
		}
		Polygon_2 poly;
		std::cout << "There are " << contours_approx[0].size() << " points." << std::endl;
		for (int i = 0; i < contours_approx[0].size(); i++)
			poly.push_back(Point(contours_approx[0][i].x, contours_approx[0][i].y));

		// You can pass the polygon via an iterator pair
		SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());
		std::vector<std::vector<double>> edges = edges_straight_skeleton(*iss);
		for (int i = 0; i< edges.size(); i++)
		{
			cv::Scalar color = cv::Scalar(0, 0, 255);
			cv::line(drawing, cv::Point(edges[i][0], edges[i][1]), cv::Point(edges[i][2], edges[i][3]), color, 1, 8, 0);
		}
		cv::imwrite(output_img_file, drawing);


	}

	void findSkeleton(std::string src_img_file, std::string mask_img_file, std::string output_img_file){
		cv::Mat src = cv::imread(src_img_file, CV_LOAD_IMAGE_UNCHANGED);
		std::vector<std::vector<cv::Point> > contours;
		std::vector<std::vector<cv::Point> > contours_approx;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

		cv::Mat drawing = cv::imread(mask_img_file, CV_LOAD_IMAGE_UNCHANGED);
		contours_approx.resize(contours.size());
		double epsilon = 4.0;
		for (size_t i = 0; i< contours.size(); i++)
		{
			cv::Scalar color = cv::Scalar(255, 255, 255);
			//drawContours(drawing, contours, (int)i, color, 1, 8, hierarchy, 0, cv::Point());
			cv::approxPolyDP(contours[i], contours_approx[i], epsilon, true);
			while (contours_approx[i].size() > 80){
				epsilon += 2;
				contours_approx[i].clear();
				cv::approxPolyDP(contours[i], contours_approx[i], epsilon, true);
			}
			epsilon = 4;
		}

		if (isClockwise(contours_approx[0])){
			std::cout << "clockwise" << std::endl;
			std::reverse(contours_approx[0].begin(), contours_approx[0].end());
		}
		else{
			std::cout << "counterClockwise" << std::endl;
		}
		Polygon_2 poly;
		std::cout << "There are " << contours_approx[0].size() << " points." << std::endl;
		for (int i = 0; i < contours_approx[0].size(); i++)
			poly.push_back(Point(contours_approx[0][i].x, contours_approx[0][i].y));

		// You can pass the polygon via an iterator pair
		SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());
		std::vector<std::vector<double>> edges = edges_straight_skeleton(*iss);
		for (int i = 0; i< edges.size(); i++)
		{
			cv::Scalar color = cv::Scalar(0, 0, 255);
			cv::line(drawing, cv::Point(edges[i][0], edges[i][1]), cv::Point(edges[i][2], edges[i][3]), color, 1, 8, 0);
		}
		cv::imwrite(output_img_file, drawing);


	}

	bool isClockwise(const std::vector<cv::Point>& polygon) {
		float S = 0;
		for (int i = 0; i < polygon.size(); i++) {
			int next = (i + 1) % polygon.size();
			S += (polygon[next].x - polygon[i].x) * (polygon[next].y + polygon[i].y);
		}
		return S > 0;
	}

	std::vector<std::vector<float>> read_tiff_float(const char* type_tiff){
		// height info
		GDALDataset  *poDataset_height;
		std::vector<std::vector<float>> type_info;
		GDALAllRegister();
		poDataset_height = (GDALDataset *)GDALOpen(type_tiff, GA_ReadOnly);
		if (poDataset_height == NULL)
		{
			std::cout << " Null data" << std::endl;
		}
		else{
			// Getting Dataset Information
			double        adfGeoTransform[6];
			printf("Driver: %s/%s\n",
				poDataset_height->GetDriver()->GetDescription(),
				poDataset_height->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));
			printf("Size is %dx%dx%d\n",
				poDataset_height->GetRasterXSize(), poDataset_height->GetRasterYSize(),
				poDataset_height->GetRasterCount());
			if (poDataset_height->GetProjectionRef() != NULL)
				printf("Projection is `%s'\n", poDataset_height->GetProjectionRef());
			if (poDataset_height->GetGeoTransform(adfGeoTransform) == CE_None)
			{
				printf("Origin = (%.6f,%.6f)\n",
					adfGeoTransform[0], adfGeoTransform[3]);
				printf("Pixel Size = (%.6f,%.6f)\n",
					adfGeoTransform[1], adfGeoTransform[5]);
			}

			//Fetching a Raster Band
			GDALRasterBand  *poBand;
			int             nBlockXSize, nBlockYSize;
			int             bGotMin, bGotMax;
			double          adfMinMax[2];
			poBand = poDataset_height->GetRasterBand(1);
			poBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
			adfMinMax[0] = poBand->GetMinimum(&bGotMin);
			adfMinMax[1] = poBand->GetMaximum(&bGotMax);
			if (!(bGotMin && bGotMax))
				GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);
			printf("Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1]);
			if (poBand->GetOverviewCount() > 0)
				printf("Band has %d overviews.\n", poBand->GetOverviewCount());
			if (poBand->GetColorTable() != NULL)
				printf("Band has a color table with %d entries.\n",
				poBand->GetColorTable()->GetColorEntryCount());
			//Reading Raster Data
			int nXSize = poBand->GetXSize();
			int nYSize = poBand->GetYSize();
			float *pafScanline = new float[nXSize*nYSize];
			printf("Band nXSize is %d.\n", nXSize);
			printf("Band nYSize is %d.\n", nYSize);
			pafScanline = (float *)CPLMalloc(sizeof(float)*nXSize*nYSize);
			poBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize,
				pafScanline, nXSize, nYSize, GDT_Float32,
				0, 0);

			// save to a matrix
			std::vector<float> tmp;
			for (int i = 0; i < nYSize; i++){
				for (int j = 0; j < nXSize; j++){
					tmp.push_back(pafScanline[j + nXSize*i]);
				}
				type_info.push_back(tmp);
				tmp.clear();
			}
		}
		return type_info;
	}

	void translate_bld_mask(const char* type_tiff, std::string output_img_file){
		// height info
		GDALDataset  *poDataset_height;
		std::vector<std::vector<int>> type_info;
		GDALAllRegister();
		poDataset_height = (GDALDataset *)GDALOpen(type_tiff, GA_ReadOnly);
		if (poDataset_height == NULL)
		{
			std::cout << " Null data" << std::endl;
		}
		else{
			// Getting Dataset Information
			double        adfGeoTransform[6];
			printf("Driver: %s/%s\n",
				poDataset_height->GetDriver()->GetDescription(),
				poDataset_height->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));
			printf("Size is %dx%dx%d\n",
				poDataset_height->GetRasterXSize(), poDataset_height->GetRasterYSize(),
				poDataset_height->GetRasterCount());
			if (poDataset_height->GetProjectionRef() != NULL)
				printf("Projection is `%s'\n", poDataset_height->GetProjectionRef());
			if (poDataset_height->GetGeoTransform(adfGeoTransform) == CE_None)
			{
				printf("Origin = (%.6f,%.6f)\n",
					adfGeoTransform[0], adfGeoTransform[3]);
				printf("Pixel Size = (%.6f,%.6f)\n",
					adfGeoTransform[1], adfGeoTransform[5]);
			}

			//Fetching a Raster Band
			GDALRasterBand  *poBand;
			int             nBlockXSize, nBlockYSize;
			int             bGotMin, bGotMax;
			double          adfMinMax[2];
			poBand = poDataset_height->GetRasterBand(1);
			poBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
			adfMinMax[0] = poBand->GetMinimum(&bGotMin);
			adfMinMax[1] = poBand->GetMaximum(&bGotMax);
			if (!(bGotMin && bGotMax))
				GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);
			printf("Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1]);
			if (poBand->GetOverviewCount() > 0)
				printf("Band has %d overviews.\n", poBand->GetOverviewCount());
			if (poBand->GetColorTable() != NULL)
				printf("Band has a color table with %d entries.\n",
				poBand->GetColorTable()->GetColorEntryCount());
			//Reading Raster Data
			int nXSize = poBand->GetXSize();
			int nYSize = poBand->GetYSize();
			int *pafScanline = new int[nXSize*nYSize];
			printf("Band nXSize is %d.\n", nXSize);
			printf("Band nYSize is %d.\n", nYSize);
			pafScanline = (int *)CPLMalloc(sizeof(int)*nXSize*nYSize);
			poBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize,
				pafScanline, nXSize, nYSize, GDT_Int32,
				0, 0);

			// translate to png file
			cv::Scalar bg_color(0, 0, 0); // black for background
			cv::Mat output_img(cv::Size(nXSize, nYSize), CV_8UC1, bg_color);
			for (int i = 0; i < nYSize; i++){
				for (int j = 0; j < nXSize; j++){
					if (pafScanline[j + nXSize*i] == 1)
						output_img.at<uchar>(i, j) = (uchar)255;
				}
			}
			cv::imwrite(output_img_file, output_img);
		}
	}

	void translate_bld_ndsm(const char* ndsm_tiff, std::string output_img_file, float threshold){
		// height info
		GDALDataset  *poDataset_height;
		std::vector<std::vector<float>> type_info;
		GDALAllRegister();
		poDataset_height = (GDALDataset *)GDALOpen(ndsm_tiff, GA_ReadOnly);
		if (poDataset_height == NULL)
		{
			std::cout << " Null data" << std::endl;
		}
		else{
			// Getting Dataset Information
			double        adfGeoTransform[6];
			printf("Driver: %s/%s\n",
				poDataset_height->GetDriver()->GetDescription(),
				poDataset_height->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));
			printf("Size is %dx%dx%d\n",
				poDataset_height->GetRasterXSize(), poDataset_height->GetRasterYSize(),
				poDataset_height->GetRasterCount());
			if (poDataset_height->GetProjectionRef() != NULL)
				printf("Projection is `%s'\n", poDataset_height->GetProjectionRef());
			if (poDataset_height->GetGeoTransform(adfGeoTransform) == CE_None)
			{
				printf("Origin = (%.6f,%.6f)\n",
					adfGeoTransform[0], adfGeoTransform[3]);
				printf("Pixel Size = (%.6f,%.6f)\n",
					adfGeoTransform[1], adfGeoTransform[5]);
			}

			//Fetching a Raster Band
			GDALRasterBand  *poBand;
			int             nBlockXSize, nBlockYSize;
			int             bGotMin, bGotMax;
			double          adfMinMax[2];
			poBand = poDataset_height->GetRasterBand(1);
			poBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
			adfMinMax[0] = poBand->GetMinimum(&bGotMin);
			adfMinMax[1] = poBand->GetMaximum(&bGotMax);
			if (!(bGotMin && bGotMax))
				GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);
			printf("Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1]);
			if (poBand->GetOverviewCount() > 0)
				printf("Band has %d overviews.\n", poBand->GetOverviewCount());
			if (poBand->GetColorTable() != NULL)
				printf("Band has a color table with %d entries.\n",
				poBand->GetColorTable()->GetColorEntryCount());
			//Reading Raster Data
			int nXSize = poBand->GetXSize();
			int nYSize = poBand->GetYSize();
			float *pafScanline = new float[nXSize*nYSize];
			printf("Band nXSize is %d.\n", nXSize);
			printf("Band nYSize is %d.\n", nYSize);
			pafScanline = (float *)CPLMalloc(sizeof(float)*nXSize*nYSize);
			poBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize,
				pafScanline, nXSize, nYSize, GDT_Float32,
				0, 0);

			// translate to png file
			cv::Scalar bg_color(0, 0, 0); // black for background
			cv::Mat output_img(cv::Size(nXSize, nYSize), CV_8UC1, bg_color);
			for (int i = 0; i < nYSize; i++){
				for (int j = 0; j < nXSize; j++){
					if (pafScanline[j + nXSize*i] > threshold * adfMinMax[1])
						output_img.at<uchar>(i, j) = (uchar)255;
				}
			}
			cv::imwrite(output_img_file, output_img);
		}
	}

	std::vector<std::vector<int>> read_tiff_int(const char* mask_tiff){
		// height info
		GDALDataset  *poDataset_height;
		std::vector<std::vector<int>> type_info;
		GDALAllRegister();
		poDataset_height = (GDALDataset *)GDALOpen(mask_tiff, GA_ReadOnly);
		if (poDataset_height == NULL)
		{
			std::cout << " Null data" << std::endl;
		}
		else{
			// Getting Dataset Information
			double        adfGeoTransform[6];
			printf("Driver: %s/%s\n",
				poDataset_height->GetDriver()->GetDescription(),
				poDataset_height->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));
			printf("Size is %dx%dx%d\n",
				poDataset_height->GetRasterXSize(), poDataset_height->GetRasterYSize(),
				poDataset_height->GetRasterCount());
			if (poDataset_height->GetProjectionRef() != NULL)
				printf("Projection is `%s'\n", poDataset_height->GetProjectionRef());
			if (poDataset_height->GetGeoTransform(adfGeoTransform) == CE_None)
			{
				printf("Origin = (%.6f,%.6f)\n",
					adfGeoTransform[0], adfGeoTransform[3]);
				printf("Pixel Size = (%.6f,%.6f)\n",
					adfGeoTransform[1], adfGeoTransform[5]);
			}

			//Fetching a Raster Band
			GDALRasterBand  *poBand;
			int             nBlockXSize, nBlockYSize;
			int             bGotMin, bGotMax;
			double          adfMinMax[2];
			poBand = poDataset_height->GetRasterBand(1);
			poBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
			adfMinMax[0] = poBand->GetMinimum(&bGotMin);
			adfMinMax[1] = poBand->GetMaximum(&bGotMax);
			if (!(bGotMin && bGotMax))
				GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);
			printf("Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1]);
			if (poBand->GetOverviewCount() > 0)
				printf("Band has %d overviews.\n", poBand->GetOverviewCount());
			if (poBand->GetColorTable() != NULL)
				printf("Band has a color table with %d entries.\n",
				poBand->GetColorTable()->GetColorEntryCount());
			//Reading Raster Data
			int nXSize = poBand->GetXSize();
			int nYSize = poBand->GetYSize();
			int *pafScanline = new int[nXSize*nYSize];
			printf("Band nXSize is %d.\n", nXSize);
			printf("Band nYSize is %d.\n", nYSize);
			pafScanline = (int *)CPLMalloc(sizeof(int)*nXSize*nYSize);
			poBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize,
				pafScanline, nXSize, nYSize, GDT_Int32,
				0, 0);

			// save to a matrix
			std::vector<int> tmp;
			for (int i = 0; i < nYSize; i++){
				for (int j = 0; j < nXSize; j++){
					tmp.push_back(pafScanline[j + nXSize*i]);
				}
				type_info.push_back(tmp);
				tmp.clear();
			}
		}
		return type_info;
	}

	void crop_img_from_mask(const char* src_tiff, std::vector<std::vector<int>> & type_info, std::string output_img_file){
		// height info
		GDALDataset  *poDataset_height;
		GDALAllRegister();
		poDataset_height = (GDALDataset *)GDALOpen(src_tiff, GA_ReadOnly);
		if (poDataset_height == NULL)
		{
			std::cout << " Null data" << std::endl;
		}
		else{
			// Getting Dataset Information
			double        adfGeoTransform[6];
			printf("Driver: %s/%s\n",
				poDataset_height->GetDriver()->GetDescription(),
				poDataset_height->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));
			printf("Size is %dx%dx%d\n",
				poDataset_height->GetRasterXSize(), poDataset_height->GetRasterYSize(),
				poDataset_height->GetRasterCount());
			if (poDataset_height->GetProjectionRef() != NULL)
				printf("Projection is `%s'\n", poDataset_height->GetProjectionRef());
			if (poDataset_height->GetGeoTransform(adfGeoTransform) == CE_None)
			{
				printf("Origin = (%.6f,%.6f)\n",
					adfGeoTransform[0], adfGeoTransform[3]);
				printf("Pixel Size = (%.6f,%.6f)\n",
					adfGeoTransform[1], adfGeoTransform[5]);
			}
			std::cout << "There are " << poDataset_height->GetRasterCount() << " Bands." << std::endl;
			cv::Scalar bg_color(0, 0, 0); // black for background
			cv::Mat output_img;
			if (poDataset_height->GetRasterCount() == 1){
				output_img = cv::Mat(cv::Size(type_info[0].size(), type_info.size()), CV_8UC1, bg_color);
			}
			else if (poDataset_height->GetRasterCount() == 3){
				output_img = cv::Mat(cv::Size(type_info[0].size(), type_info.size()), CV_8UC3, bg_color);
			}
			else{
				return;
			}

			for (int layer = 1; layer <= poDataset_height->GetRasterCount(); layer++){
				//Fetching a Raster Band
				GDALRasterBand  *poBand;
				int             nBlockXSize, nBlockYSize;
				int             bGotMin, bGotMax;
				double          adfMinMax[2];
				poBand = poDataset_height->GetRasterBand(layer);
				poBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
				//Reading Raster Data
				int nXSize = poBand->GetXSize();
				int nYSize = poBand->GetYSize();
				int *pafScanline = new int[nXSize*nYSize];
				printf("Band nXSize is %d.\n", nXSize);
				printf("Band nYSize is %d.\n", nYSize);
				pafScanline = (int *)CPLMalloc(sizeof(int)*nXSize*nYSize);
				poBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize,
					pafScanline, nXSize, nYSize, GDT_Int32,
					0, 0);
				for (int i = 0; i < nYSize; i++){
					for (int j = 0; j < nXSize; j++){
						if (type_info[i][j] == 1)
							if (poDataset_height->GetRasterCount() == 1)
								output_img.at<uchar>(i, j) = (uchar)pafScanline[j + nXSize*i];
							else
								output_img.at<cv::Vec3b>(i, j)[layer - 1] = pafScanline[j + nXSize*i];
					}
				}
			}
			if (poDataset_height->GetRasterCount() == 3)
				cvtColor(output_img, output_img, cv::COLOR_RGB2BGR);
			cv::imwrite(output_img_file, output_img);

		}
	}

	std::vector<std::string> get_all_files_names_within_folder(std::string folder)
	{
		std::vector<std::string> names;
		std::string search_path = folder + "/*.*";
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				/*if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
				}*/
				if (std::string(fd.cFileName).compare(".") != 0 && std::string(fd.cFileName).compare("..") != 0)
					names.push_back(fd.cFileName);
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		return names;
	}

}