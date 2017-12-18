#pragma once
#include <opencv2/core/base.hpp>
#include "ObjectTracker.h"
#include <opencv2/core/mat.hpp>
#include "../dto/Image.h"
#include "../feature_extraction/Controller.h"
#include "ObjectTrackerYolo.h"

namespace image_tracking
{
	class Controller
	{
	public:
		Controller();
		void ProcessImage(dto::Image& image, dto::Camera& camera);
	private:
		ObjectTracker ot;
		ObjectTrackerYolo ot_yolo;
		feature_extraction::Controller feature_extraction_controller;
	};
}
