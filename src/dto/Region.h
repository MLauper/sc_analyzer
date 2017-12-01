#pragma once
#include <opencv2/core.hpp>
#include <vector>

namespace dto
{
	struct Region
	{
		std::vector<cv::Point> contour;
		
		int minX;
		int minY;
		int maxX;
		int maxY;

		float ratio;
	};
}
