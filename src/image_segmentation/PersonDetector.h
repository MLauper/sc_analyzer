#pragma once
#include "../dto/Image.h"
#include <yolo/yolo_v2_class.hpp>
#include "../dto/Camera.h"

namespace image_segmentation
{
	class PersonDetector
	{
	public:
		PersonDetector();
		void extractPersonContours(dto::Image& Image, dto::Camera& camera) const;
		void detectPersonsYolo(dto::Image& Image, dto::Camera& camera) const;
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
