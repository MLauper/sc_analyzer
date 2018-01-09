#pragma once
#include <vector>
#include "dto/Track.h"

namespace identification
{
	class ColorMatcher
	{
	public:
		ColorMatcher();
		~ColorMatcher();
		void matchAllColors(std::vector<dto::Track>& tracks) const;
	};
}
