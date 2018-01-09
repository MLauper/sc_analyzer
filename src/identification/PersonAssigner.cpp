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
	return (i.overall_suggestion.at(1).likelihood > j.overall_suggestion.at(1).likelihood);
}

void identification::PersonAssigner::assignTracksToPerson(std::vector<dto::Track>& tracks,
                                                          std::vector<dto::Person>& persons)
{
	// Reset all assigned person ids
	for (auto& t : tracks) t.assignedPersonId = -1;

	//if (tracks.size() > 1) std::sort(tracks.begin(), tracks.end(), compare_tracks);

	int i = 0;

	for (auto& t : tracks)
	{
		if (tracks.size() > 1)
		{
			auto& match = t.overall_suggestion.at(1).track;

			// Create new person if track and match does not have any
			if (t.assignedPersonId == -1 && match->assignedPersonId == -1)
			{
				if (dto::Configuration::PRINT_PERSON_SELECTOR_STEPS) std::cout << "Create new Person " << (i) << " for Tracks " << t
					.camera.scene << "." << t.trackId << " and " << match->camera.scene << "." << match->trackId << std::endl;
				int assignment = i++;
				t.assignedPersonId = assignment;
				match->assignedPersonId = assignment;
			}
				// Use person assignment of match if only match has one
			else if (t.assignedPersonId == -1 && match->assignedPersonId != -1)
			{
				if (dto::Configuration::PRINT_PERSON_SELECTOR_STEPS) std::cout << "Assign ID " << match->assignedPersonId <<
					" to Track " << t.camera.scene << "." << t.trackId << " from Track " << match->camera.scene << "." << match->
					trackId << std::endl;
				t.assignedPersonId = match->assignedPersonId;
			}
				// Use person assignment of track if only track has one
			else if (t.assignedPersonId != -1 && match->assignedPersonId == -1)
			{
				if (dto::Configuration::PRINT_PERSON_SELECTOR_STEPS) std::cout << "Assign ID " << t.assignedPersonId << " to Track "
					<< match->camera.scene << "." << match->trackId << " from Track " << t.camera.scene << "." << t.trackId << std::
					endl;
				match->assignedPersonId = t.assignedPersonId;
			}
			else
			{
				if (t.overall_suggestion.at(1).likelihood > dto::Configuration::ALWAYS_MATCH_LIKELIHOOD)
				{
					int replace_id = t.assignedPersonId;
					int replacement_id = match->assignedPersonId;
					if (replace_id != replacement_id)
					{
						if (dto::Configuration::PRINT_PERSON_SELECTOR_STEPS) std::cout << "Merge Tracks " << t.camera.scene << "." << t.
							trackId << " and " << match->camera.scene << "." << match->trackId << ", IDs: " << t.assignedPersonId << " and "
							<< match->assignedPersonId << std::endl;
						for (auto& x : tracks) if (x.assignedPersonId == replace_id) x.assignedPersonId = replacement_id;
					}
					else
					{
						if (dto::Configuration::PRINT_PERSON_SELECTOR_STEPS) std::cout << "Tracks " << t.camera.scene << "." << t.trackId
							<< " and " << match->camera.scene << "." << match->trackId << ", IDs: " << t.assignedPersonId << " and " << match
							->assignedPersonId << " already in same group, do nothing." << std::endl;
					}
				}
				else
				{
					if (dto::Configuration::PRINT_PERSON_SELECTOR_STEPS) std::cout << "Do not touch id on tracks " << t.camera.scene <<
						"." << t.trackId << " and " << match->camera.scene << "." << match->trackId << std::endl;
				}
			}
		}
			// Edge case: If only one track exist, create one new person 
		else
		{
			t.assignedPersonId = 0;
		}
	}

	int j = -1;
	int current_id = -1;
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
