#pragma once
#include <vector>
#include "Region.h"
#include "Image.h"

namespace dto
{
	struct Track
	{
		int trackId;
		std::vector<dto::Region> regions;
		std::vector<dto::Image> images;

		cv::Point walkingDirection;

		int numImagesWithoutRegion;
	};
}
