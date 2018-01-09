#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"

namespace feature_extraction
{
	class FrameSelector
	{
	public:
		FrameSelector();
		void SelectFrame(dto::Track& track, const dto::Camera& camera) const;
		void SaveRegion(dto::Track& track, const dto::Camera& camera) const;
	};
}
