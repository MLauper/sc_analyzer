#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"

namespace feature_extraction {
	class ColorExtractor
	{
	public:
		ColorExtractor();
		~ColorExtractor();
		void extractMaxHue(dto::Track& track, cv::Mat& hsv_image, int& maxBucketId, bool isUpperBody);
		void extractPrimaryColors(dto::Track& track, dto::Camera& camera);
	};
}