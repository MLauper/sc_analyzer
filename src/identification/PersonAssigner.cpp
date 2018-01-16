#include "PersonAssigner.h"
#include "dto/Person.h"
#include "dto/Configuration.h"


identification::PersonAssigner::PersonAssigner()
{
}


identification::PersonAssigner::~PersonAssigner()
{
}

bool compare_tracks(dto::Track i, dto::Track j)
{
	return i.overall_suggestion.at(1).likelihood > j.overall_suggestion.at(1).likelihood;
}

void identification::PersonAssigner::assignTracksToPerson(std::vector<dto::Track>& tracks,
                                                          std::vector<dto::Person>& persons)
{
	// Reset all assigned person ids
	for (auto& t : tracks) t.assignedPersonId = -1;

	//if (tracks.size() > 1) std::sort(tracks.begin(), tracks.end(), compare_tracks);

	auto i = 0;

	for (auto& t : tracks)
	{
		if (tracks.size() > 1)
		{
			auto& match = t.overall_suggestion.at(1).track;

			// Create new person if track and match does not have any
			if (t.assignedPersonId == -1 && match->assignedPersonId == -1)
			{
				const auto assignment = i++;
				t.assignedPersonId = assignment;
				match->assignedPersonId = assignment;
			}
				// Use person assignment of match if only match has one
			else if (t.assignedPersonId == -1 && match->assignedPersonId != -1)
			{
				t.assignedPersonId = match->assignedPersonId;
			}
				// Use person assignment of track if only track has one
			else if (t.assignedPersonId != -1 && match->assignedPersonId == -1)
			{
				match->assignedPersonId = t.assignedPersonId;
			}
			else
			{
				if (t.overall_suggestion.at(1).likelihood > dto::Configuration::ALWAYS_MATCH_LIKELIHOOD)
				{
					const auto replace_id = t.assignedPersonId;
					const auto replacement_id = match->assignedPersonId;
					if (replace_id != replacement_id)
					{
						for (auto& x : tracks) if (x.assignedPersonId == replace_id) x.assignedPersonId = replacement_id;
					}
					else
					{
					}
				}
				else
				{
				}
			}
		}
			// Edge case: If only one track exist, create one new person 
		else
		{
			t.assignedPersonId = 0;
		}
	}

	auto j = -1;
	auto current_id = -1;
	for (auto& t : tracks)
	{
		// New id, create new person
		if (t.assignedPersonId != current_id)
		{
			current_id = t.assignedPersonId;
			t.assignedPersonId = ++j;
			dto::Person person;
			person.person_id = j;
			person.tracks.push_back(t);
			persons.push_back(person);
		}
			// Same id, add to persons
		else
		{
			t.assignedPersonId = j;
			persons.at(j).tracks.push_back(t);
		}
	}
}
