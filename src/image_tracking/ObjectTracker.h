#pragma once
#include <opencv2/core/base.hpp>
#include <vector>
#include "../dto/Image.h"
#include "../dto/Track.h"
#include <random>

namespace image_tracking
{
	class ObjectTracker
	{
	public:
		ObjectTracker();
		void apply(dto::Image image);
		bool hasFinishedTracks();
		dto::Track getFinishedTrack();
	private:
		std::vector<dto::Track> currentTracks;
		int maxNumberOfMissingFramesInTrack;

		std::random_device rd;
		std::mt19937 rng;
		int latestTrackId = 0;
		cv::Mat drawingAll;
	};
}
