#pragma once
#include <string>
#include <image_segmentation/Controller.h>
#include <set>
#include "dto/Camera.h"

namespace image_acquisition
{
	class URLImageLoader
	{
	public:
		void publishResults(int size);
		void startCapturing();
		URLImageLoader(dto::Camera& camera, image_segmentation::Controller* segmentation_controller);
		~URLImageLoader();
	private:
		dto::Camera camera;
		image_segmentation::Controller* segmentation_controller;
		cv::Mat dist_map1;
		cv::Mat dist_map2;
		int frameNumber;
	};
}
