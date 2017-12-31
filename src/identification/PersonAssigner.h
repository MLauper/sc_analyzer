#pragma once
#include <vector>
#include "../dto/Track.h"
#include "../dto/Person.h"

namespace identification {
	class PersonAssigner
	{
	public:
		PersonAssigner();
		~PersonAssigner();
		void assignTracksToPerson(std::vector<dto::Track>& tracks, std::vector<dto::Person>& persons);
	};
}
