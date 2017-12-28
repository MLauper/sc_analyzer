#pragma once
#include "../dto/Track.h"
#include "DirectionExtractor.h"
#include "FrameSelector.h"
#include "ColorExtractor.h"
#include "BodyPartExtractor.h"
#include "SizeExtractor.h"
#include "FeaturePointExtractor.h"

namespace feature_extraction
{
	class Controller
	{
	public:
		Controller();
		void processTrack(dto::Track& track, dto::Camera& camera);
	private:
		feature_extraction::DirectionExtractor direction_extractor_;
		feature_extraction::FrameSelector frame_selector_;
		feature_extraction::ColorExtractor color_extractor_;
		feature_extraction::BodyPartExtractor body_part_extractor_;
		feature_extraction::SizeExtractor size_extractor_;
		feature_extraction::FeaturePointExtractor feature_point_extractor_;
		int personsInTheRoom;
	};


}
