#include "FeaturePointMatcher.h"
#include "dto/Track.h"
#include "dto/Configuration.h"


identification::FeaturePointMatcher::FeaturePointMatcher()
{

	int minHessian = 400;
	this->surf_detector_ = cv::xfeatures2d::SURF::create(minHessian);

	this->sift_detector_ = cv::xfeatures2d::SIFT::create();

	this->bf_matcher = cv::BFMatcher::create();
}


identification::FeaturePointMatcher::~FeaturePointMatcher()
{
}

void identification::FeaturePointMatcher::matchAllFeaturePoints(std::vector<dto::Track>& tracks)
{
	for (auto& t : tracks)
	{
		// Load images
		std::stringstream image_out_path;
		image_out_path << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "scene-" << t.camera.scene << "\\" << t.camera.prefix << "\\" << "Track-" << t.trackId << "_optimalImage-cut.jpg";
		t.cv_optimalPersonCut = cv::imread(image_out_path.str().c_str());

		std::stringstream image_out_path_full;
		image_out_path_full << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "scene-" << t.camera.scene << "\\" << t.camera.prefix << "\\" << "Track-" << t.trackId << "_optimalImage-cut_Full.jpg";
		t.cv_optimalPersonCut_Full = cv::imread(image_out_path_full.str().c_str());

		if (dto::Configuration::USE_FG_IMAGE_FOR_FEATURE_POINTS) {
			// Detect SURF Keypoints
			this->surf_detector_->detectAndCompute(t.cv_optimalPersonCut, cv::Mat(), t.surf_keyPoints, t.surf_descriptors);

			// Detect SIFT Keypoints
			this->sift_detector_->detectAndCompute(t.cv_optimalPersonCut, cv::Mat(), t.sift_keyPoints, t.sift_descriptors);
		}
		else
		{
			// Detect SURF Keypoints
			this->surf_detector_->detectAndCompute(t.cv_optimalPersonCut_Full, cv::Mat(), t.surf_keyPoints, t.surf_descriptors);

			// Detect SIFT Keypoints
			this->sift_detector_->detectAndCompute(t.cv_optimalPersonCut, cv::Mat(), t.sift_keyPoints, t.sift_descriptors);
		}

	}

	for (auto& t : tracks)
	{
		for (auto& comp : tracks) {
			// Match SURF
			std::vector<cv::DMatch> surfMatches;
			bf_matcher->match(t.surf_descriptors, comp.surf_descriptors, surfMatches);

			std::vector<cv::DMatch > good_surf_matches;
			for (int i = 0; i < t.surf_descriptors.rows; i++)
			{
				if (surfMatches[i].distance <= 0.25)
				{
					good_surf_matches.push_back(surfMatches[i]);
				}
			}

			dto::Track::suggestion surf_suggestion;
			surf_suggestion.track = &comp;
			surf_suggestion.likelihood = ((float)good_surf_matches.size() / (float)t.surf_keyPoints.size());
			t.surf_keypoint_suggestion.push_back(surf_suggestion);

			// Match SIFT
			std::vector<cv::DMatch> siftMatches;
			bf_matcher->match(t.sift_descriptors, comp.sift_descriptors, siftMatches);

			std::vector<cv::DMatch > good_sift_matches;
			for (int i = 0; i < t.sift_descriptors.rows; i++)
			{
				if (siftMatches[i].distance <= 0.25)
				{
					good_sift_matches.push_back(siftMatches[i]);
				}
			}

			dto::Track::suggestion sift_suggestion;
			sift_suggestion.track = &comp;
			sift_suggestion.likelihood = ((float)good_sift_matches.size() / (float)t.sift_keyPoints.size());
			t.sift_keypoint_suggestion.push_back(sift_suggestion);

		}
	}
	
}