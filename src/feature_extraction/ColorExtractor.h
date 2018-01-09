#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"

namespace feature_extraction
{
	class ColorExtractor
	{
	public:
		ColorExtractor();
		~ColorExtractor();
		void extractMaxHue(dto::Track& track, dto::Camera& camera, cv::Mat& hsv_image, int& maxBucketId,
		                   bool isUpperBody) const;
		void extractPrimaryColors(dto::Track& track, dto::Camera& camera) const;
	};
}
