#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"

namespace feature_extraction {
	class ColorExtractor
	{
	public:
		ColorExtractor();
		~ColorExtractor();
		void extractMaxHue(cv::Mat& hsv_image, int& maxBucketId);
		void extractPrimaryColors(dto::Track& track, dto::Camera& camera);
	};
}
