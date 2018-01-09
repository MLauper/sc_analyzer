#pragma once
#include <vector>
#include "dto/Track.h"

namespace identification
{
	class SizeMatcher
	{
	public:
		SizeMatcher();
		~SizeMatcher();
		void matchAllSizes(std::vector<dto::Track>& tracks);
	};
}
