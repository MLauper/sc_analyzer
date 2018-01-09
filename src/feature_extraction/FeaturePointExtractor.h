#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"

namespace feature_extraction
{
	class FeaturePointExtractor
	{
	public:
		FeaturePointExtractor();
		~FeaturePointExtractor();
		void extractFeaturePoints(dto::Track& track, dto::Camera& camera);
	private:
		cv::FeatureDetector orb_detector;
		cv::Ptr<cv::xfeatures2d::SURF> surf_detector_;
		cv::Ptr<cv::xfeatures2d::SIFT> sift_detector_;
	};
}
