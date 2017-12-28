#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"

namespace feature_extraction {
	class SizeExtractor
	{
	public:
		SizeExtractor();
		~SizeExtractor();
		void extractBodySizes(dto::Track& track, dto::Camera camera);
	};
}