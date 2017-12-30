#pragma once
#include <opencv2/features2d.hpp>
#include "dto/Track.h"
#include <opencv2/xfeatures2d/nonfree.hpp>

namespace identification {
	class FeaturePointMatcher
	{
	public:
		FeaturePointMatcher();
		~FeaturePointMatcher();
		void matchAllFeaturePoints(std::vector<dto::Track>& tracks);
	private:
		cv::Ptr<cv::BFMatcher> bf_matcher;
		cv::Ptr<cv::xfeatures2d::SURF> surf_detector_;
		cv::Ptr<cv::xfeatures2d::SIFT> sift_detector_;
	};
}
