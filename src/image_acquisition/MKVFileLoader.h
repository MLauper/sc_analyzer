#pragma once
#include <string>
#include "../image_segmentation/Controller.h"
#include "../dto/Camera.h"

namespace image_acquisition {
	class MKVFileLoader
	{
	public:
		MKVFileLoader(dto::Camera& camera, image_segmentation::Controller* segmentation_controller);
		~MKVFileLoader();
		void process_file();
		std::string extract_filename(char const* path_c);
	private:
		dto::Camera camera;
		image_segmentation::Controller* segmentation_controller;
	};
}

