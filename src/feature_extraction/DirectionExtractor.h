#pragma once
#include "../dto/Track.h"
#include "../dto/Camera.h"

namespace feature_extraction
{
	class DirectionExtractor
	{
	public:
		DirectionExtractor();
		void extractDirection(dto::Track& track, dto::Camera& camera);
	};
}
