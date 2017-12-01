#pragma once
#include "../dto/Track.h"
#include "DirectionExtractor.h"


namespace feature_extraction
{
	class Controller
	{
	public:
		Controller();
		void processTrack(dto::Track& track, dto::Camera& camera);
	private:
		feature_extraction::DirectionExtractor de;
	};


}
