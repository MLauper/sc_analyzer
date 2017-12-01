#pragma once
#include <opencv2/core/base.hpp>
#include "ObjectTracker.h"
#include <opencv2/core/mat.hpp>
#include "../dto/Image.h"
#include "../feature_extraction/Controller.h"

namespace image_tracking
{
	class Controller
	{
	public:
		Controller();
		void ProcessImage(dto::Image image, dto::Camera camera);
	private:
		ObjectTracker ot;
		feature_extraction::Controller feature_extraction_controller;
	};
}