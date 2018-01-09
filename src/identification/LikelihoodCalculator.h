#pragma once
#include "dto/Track.h"

namespace identification
{
	class LikelihoodCalculator
	{
	public:
		LikelihoodCalculator();
		~LikelihoodCalculator();
		void calculateAllLikelihoods(std::vector<dto::Track>& tracks) const;
	};
}
