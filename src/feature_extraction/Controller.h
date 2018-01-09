#pragma once
#include "../dto/Track.h"
#include "DirectionExtractor.h"
#include "FrameSelector.h"
#include "ColorExtractor.h"
#include "BodyPartExtractor.h"
#include "SizeExtractor.h"
#include "FeaturePointExtractor.h"
#include "TrackPersistor.h"

namespace feature_extraction
{
	class Controller
	{
	public:
		Controller();
		void processTrack(dto::Track& track, dto::Camera& camera);
	private:
		DirectionExtractor direction_extractor_;
		FrameSelector frame_selector_;
		ColorExtractor color_extractor_;
		BodyPartExtractor body_part_extractor_;
		SizeExtractor size_extractor_;
		FeaturePointExtractor feature_point_extractor_;
		TrackPersistor track_persistor_;
		int personsInTheRoom;
	};
}
