#pragma once
#include "Track.h"

namespace dto
{
	struct Person
	{
		int person_id;
		std::vector<dto::Track> tracks;
	};
}
