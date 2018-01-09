#include "FeaturePointExtractor.h"
#include "../dto//Configuration.h"


feature_extraction::FeaturePointExtractor::FeaturePointExtractor()
{
	int minHessian = 400;
	this->surf_detector_ = cv::xfeatures2d::SURF::create(minHessian);

	this->sift_detector_ = cv::xfeatures2d::SIFT::create();
}


feature_extraction::FeaturePointExtractor::~FeaturePointExtractor()
{
}

void feature_extraction::FeaturePointExtractor::extractFeaturePoints(dto::Track& track, dto::Camera& camera)
{
	if (dto::Configuration::USE_FG_IMAGE_FOR_FEATURE_POINTS)
	{
		// Detect SURF Keypoints
		this->surf_detector_->detectAndCompute(track.cv_optimalPersonCut, cv::Mat(), track.surf_keyPoints,
		                                       track.surf_descriptors);

		// Detect SIFT Keypoints
		this->sift_detector_->detectAndCompute(track.cv_optimalPersonCut, cv::Mat(), track.sift_keyPoints,
		                                       track.sift_descriptors);
	}
	else
	{
		// Detect SURF Keypoints
		this->surf_detector_->detectAndCompute(track.cv_optimalPersonCut_Full, cv::Mat(), track.surf_keyPoints,
		                                       track.surf_descriptors);

		// Detect SIFT Keypoints
		this->sift_detector_->detectAndCompute(track.cv_optimalPersonCut, cv::Mat(), track.sift_keyPoints,
		                                       track.sift_descriptors);
	}

	if (dto::Configuration::SAVE_FEATURE_POINT_IMAGES)
	{
		cv::Mat drawing_surf;
		cv::Mat drawing_sift;
		if (dto::Configuration::USE_FG_IMAGE_FOR_FEATURE_POINTS)
		{
			drawKeypoints(track.cv_optimalPersonCut, track.surf_keyPoints, drawing_surf, cv::Scalar::all(-1),
			              cv::DrawMatchesFlags::DEFAULT);
			drawKeypoints(track.cv_optimalPersonCut, track.sift_keyPoints, drawing_sift, cv::Scalar::all(-1),
			              cv::DrawMatchesFlags::DEFAULT);
		}
		else
		{
			drawKeypoints(track.cv_optimalPersonCut_Full, track.surf_keyPoints, drawing_surf, cv::Scalar::all(-1),
			              cv::DrawMatchesFlags::DEFAULT);
			drawKeypoints(track.cv_optimalPersonCut_Full, track.sift_keyPoints, drawing_sift, cv::Scalar::all(-1),
			              cv::DrawMatchesFlags::DEFAULT);
		}

		std::stringstream sift_image_path;
		sift_image_path << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix <<
			"\\" << "Track-" << track.trackId << "_sift_keypoints.jpg";
		imwrite(sift_image_path.str().c_str(), drawing_sift);

		std::stringstream surf_image_path;
		surf_image_path << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix <<
			"\\" << "Track-" << track.trackId << "_surf_keypoints.jpg";
		imwrite(surf_image_path.str().c_str(), drawing_surf);
	}
}
