#pragma once
#include "../dto/Track.h"
#include "../dto/Image.h"
#include "../feature_extraction/Controller.h"
#include <random>

namespace image_tracking
{
	class ObjectTrackerYolo
	{
	public:
		ObjectTrackerYolo();
		void apply(dto::Image& image);
		bool hasFinishedTracks();
		void SendFinishedTracksTo(feature_extraction::Controller& controller, dto::Camera& camera);
	private:
		std::vector<dto::Track> currentTracks;
		int maxNumberOfMissingFramesInTrack;

		std::random_device rd;
		std::mt19937 rng;
		int latestTrackId = 0;
		cv::Mat drawingAll;
	};
}
