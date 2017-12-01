#pragma once
#include "../dto/Track.h"
#include "DirectionExtractor.h"
#include "RegionSelector.h"


namespace feature_extraction
{
	class Controller
	{
	public:
		Controller();
		void processTrack(dto::Track& track, dto::Camera& camera);
	private:
		feature_extraction::DirectionExtractor de;
		feature_extraction::RegionSelector rs;
		int personsInTheRoom;
	};


}
