#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"

namespace feature_extraction
{
	class RegionSelector
	{
	public:
		RegionSelector();
		void SelectRegion(dto::Track& track, const dto::Camera& camera);
	};
}
