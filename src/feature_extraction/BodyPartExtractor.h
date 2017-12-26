#pragma once

#include "../dto/Camera.h"
#include "../dto/Track.h"

namespace feature_extraction {
	class BodyPartExtractor
	{
	public:
		BodyPartExtractor();
		~BodyPartExtractor();
		void extractBodyParts(dto::Track& track, dto::Camera& camera);
	};
}
