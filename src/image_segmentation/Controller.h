#pragma once
#include <arrayfire.h>
#include <fstream>
#include "BackgroundRemover.h"
#include "PersonDetector.h"
#include "PersonDetectorHog.h"
#include "../image_tracking/Controller.h"
#include "../dto/Image.h"
#define NOMINMAX

namespace image_segmentation
{
	class Controller
	{
	public:
		Controller(dto::Camera& camera);
		static void ProcessImage(SYSTEMTIME* time, af::array& image, std::string path, std::string filename);
		void ProcessImage(dto::Image& image);
	private:
		af::array current_image;
		SYSTEMTIME* current_image_time;
		int imageHeight;
		int imageWidth;
		float minPixelChangedPercentageThreshold;
		float maxPixelChangedPercentageThreshold;
		int minPixelDeltaValue;
		af::array current_image_grayscale;
		af::array background;
		af::array mask;
		long int numImagePixels;
		af::array morphMask;
		af::array morphMask2;
		af::array current_image_hsv;
		std::ofstream stat_out;
		dto::Camera camera;
		static af::array binThreshold(const af::array& in, float thresholdValue);
		BackgroundRemover* br;
		PersonDetector pd;
		image_tracking::Controller image_tracking_controller;
		//PersonDetector pd;
	};
}
