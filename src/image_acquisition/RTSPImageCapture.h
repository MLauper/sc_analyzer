#pragma once
#include <string>
#include <image_segmentation/Controller.h>
#include <set>
#include "dto/Camera.h"

namespace image_acquisition
{
	class RTSPImageCapture
	{
	public:
		void startCapturing();
		RTSPImageCapture(dto::Camera& camera, image_segmentation::Controller* segmentation_controller);
		~RTSPImageCapture();
	private:
		dto::Camera camera;
		image_segmentation::Controller* segmentation_controller;
		cv::Mat dist_map1;
		cv::Mat dist_map2;
		int frameNumber;
		cv::VideoCapture* cv_videoCapture;
	};
}
