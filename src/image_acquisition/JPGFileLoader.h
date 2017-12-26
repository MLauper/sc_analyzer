#pragma once
#include <string>
#include <image_segmentation/Controller.h>
#include <set>
#include "dto/Camera.h"

namespace image_acquisition
{
	class FileLoader
	{
	public:
		void SetPrefix(std::string prefix);
		void WatchDirectory();
		void ProcessFiles();
		FileLoader(std::string directory, std::string prefix, image_segmentation::Controller* segmentation_controller);
		FileLoader(dto::Camera& camera, image_segmentation::Controller* segmentation_controller);
		void SetDirectory(std::string directory);
		void SetImageSegmentationController(image_segmentation::Controller* segmentation_controller);
		static bool isPrefix(const char* s1, const char* s2);
		~FileLoader();
		static std::string extract_filename(char const* path_c);
	private:
		dto::Camera camera;
		std::string directory;
		std::string prefix;
		image_segmentation::Controller* segmentation_controller;
		std::string path_prefix;
		std::string::const_pointer prefix_c;
		std::string::const_pointer path_prefix_c;
		cv::Mat dist_map1;
		cv::Mat dist_map2;
	};
}
