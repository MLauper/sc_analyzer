#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"

/*! \file FeaturePointExtractor.h
*	\brief Contains FeaturePointExtractor class.
*
* This file contains the FeaturePointExtractor class.
* It is used to detect sift and surf feature
* points.
*/

/*! \brief The feature_extraction namespace contains all classes used for feature extraction.*/
namespace feature_extraction
{
	/*! \brief This class is used to generate sift and surf feature point descriptors.*/
	class FeaturePointExtractor
	{
	public:
		/*! \brief Default Constructor.*/
		FeaturePointExtractor();
		/*! \brief Default Destructor.*/
		~FeaturePointExtractor();
		/*! \brief Extract sift and surf feature points.*/
		void extractFeaturePoints(dto::Track& track, dto::Camera& camera) const;
	private:
		cv::FeatureDetector orb_detector;
		cv::Ptr<cv::xfeatures2d::SURF> surf_detector_;
		cv::Ptr<cv::xfeatures2d::SIFT> sift_detector_;
	};
}
