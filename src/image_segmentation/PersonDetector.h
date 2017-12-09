#pragma once
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include "../dto/Image.h"
#include <yolo/yolo_v2_class.hpp>

namespace image_segmentation
{
	class PersonDetector
	{
	public:
		PersonDetector();
		void extractPersonContours(dto::Image& Image);
		void detectPersonsYolo(dto::Image& Image);
	private:
		int minRegionWidth;
		int minRegionHeight;
		int maxRegionWidth;
		int maxRegionHeight;
		float minRatio;
		float maxRatio;
		int imageHeight;
		int imageWidth;
		int minRegionX;
		int maxRegionX;
		int minRegionY;
		int maxRegionY;
		Detector* yoloDetector;
	};
}
