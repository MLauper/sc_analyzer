#pragma once
#include <string>
#include <image_segmentation/Controller.h>
//#include <set>
#include "dto/Camera.h"

/*! \file JPGFileLoader.h
*	\brief Contains JPGFileLoader class.
*
* This file contains the JPGFileLoader class.
* It is used to load all JPG files in a local
* folder based on their prefix.
*/

/*! \brief The image_acquisition namespace contains all classes used for image acquisition.*/
namespace image_acquisition
{
	/*! \brief This class is used to stream files in a local folder.*/
	class FileLoader
	{
	public:
		/*! \brief Start watching a local director fro changes.*/
		void WatchDirectory() const;
		/*! \brief Process all files in the configured directory.*/
		void ProcessFiles() const;
		/*! \brief Constructor.*/
		FileLoader(std::string directory, std::string prefix, image_segmentation::Controller* segmentation_controller);
		/*! \brief Constructor.*/
		FileLoader(dto::Camera& camera, image_segmentation::Controller* segmentation_controller);
		/*! \brief Set directory for processing files.*/
		void SetDirectory(std::string directory);
		/*! \brief Set a new image segmentation controller.*/
		void SetImageSegmentationController(image_segmentation::Controller* segmentation_controller);
		/*! \brief Ask if one string is prefix of another string.*/
		static bool isPrefix(const char* s1, const char* s2);
		/*! \brief Default Destructor.*/
		~FileLoader();
		/*! \brief Extract filename from file path.*/
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
