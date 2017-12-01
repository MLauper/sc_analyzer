#pragma once
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include "../dto/Image.h"

namespace image_segmentation
{
	class PersonDetector
	{
	public:
		PersonDetector();
		void extractPersonContours(dto::Image& Image);
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
	};
}
