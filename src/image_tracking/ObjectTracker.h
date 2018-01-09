#pragma once
#include <vector>
#include "../dto/Image.h"
#include "../dto/Track.h"
#include <random>
#include "../feature_extraction/Controller.h"

namespace image_tracking
{
	class ObjectTracker
	{
	public:
		ObjectTracker();
		void apply(dto::Image& image);
		bool hasFinishedTracks();
		dto::Track getFinishedTrack();
		void SendFinishedTracksTo(feature_extraction::Controller& controller, dto::Camera& camera);
	private:
		std::vector<dto::Track> currentTracks;
		int maxNumberOfMissingFramesInTrack;

		std::random_device rd;
		std::mt19937 rng;
		int latestTrackId = 0;
	};
}
