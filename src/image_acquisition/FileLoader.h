#pragma once
#include <string>
#include <image_segmentation/Controller.h>

namespace image_acquisition
{
	class FileLoader
	{
	public:
		void SetPrefix(std::string prefix);
		void WatchDirectory();
		void ProcessFiles();
		FileLoader(std::string directory, std::string prefix, image_segmentation::Controller* segmentation_controller);
		void SetDirectory(std::string directory);
		void SetImageSegmentationController(image_segmentation::Controller* segmentation_controller);
		~FileLoader();
	private:
		std::string directory;
		std::string prefix;
		image_segmentation::Controller* segmentation_controller;
	};
}
