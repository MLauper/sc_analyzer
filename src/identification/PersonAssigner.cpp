#include "PersonAssigner.h"
#include "dto/Person.h"
#include "dto/Configuration.h"


identification::PersonAssigner::PersonAssigner()
{
}


identification::PersonAssigner::~PersonAssigner()
{
}

void identification::PersonAssigner::assignTracksToPerson(std::vector<dto::Track>& tracks,
                                                          std::vector<dto::Person>& persons)
{
	// Reset all assigned person ids
	for (auto& t : tracks) t.assignedPersonId = -1;

	int i = 0;

	for (auto& t : tracks)
	{
		if (tracks.size() > 1)
		{
			auto& match = t.overall_suggestion.at(1).track;

			// Create new person if match does not have any
			if (match->assignedPersonId = -1)
			{
				dto::Person person;
				person.person_id = i++;
				person.tracks.push_back(t);
				person.tracks.push_back(*match);

				t.assignedPersonId = person.person_id;
				match->assignedPersonId = person.person_id;

				persons.push_back(person);
			}
				// Use person assignment of match if available
			else
			{
				persons.at(match->assignedPersonId).tracks.push_back(t);
				t.assignedPersonId = persons.at(match->assignedPersonId).person_id;
			}
		} 
		// Edge case: If only one track exist, create new person 
		else
		{
			dto::Person person;
			person.person_id = i++;
			person.tracks.push_back(t);

			t.assignedPersonId = person.person_id;

			persons.push_back(person);
		}
	}
	
}
