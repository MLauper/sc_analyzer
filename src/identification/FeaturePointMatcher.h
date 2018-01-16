#pragma once
#include <opencv2/features2d.hpp>
#include "dto/Track.h"
#include <opencv2/xfeatures2d/nonfree.hpp>

/*! \file FeaturePointMatcher.h
*	\brief Contains FeaturePointMatcher class.
*
* This file contains the FeaturePointMatcher class.
* It is used to compare feature points (sift and surf)
* and assign them a likelihood that they contain 
* the same person.
*/

/*! \brief The identification namespace contains all classes used for identification.*/
namespace identification
{
	/*! \brief This class is used to compare feature point descriptors.*/
	class FeaturePointMatcher
	{
	public:
		/*! \brief Default Constructor.*/
		FeaturePointMatcher();
		/*! \brief Default Destructor.*/
		~FeaturePointMatcher();
		/*! \brief Compare feature points between all tracks.*/
		void matchAllFeaturePoints(std::vector<dto::Track>& tracks);
	private:
		cv::Ptr<cv::BFMatcher> bf_matcher;
		cv::Ptr<cv::xfeatures2d::SURF> surf_detector_;
		cv::Ptr<cv::xfeatures2d::SIFT> sift_detector_;
	};
}
