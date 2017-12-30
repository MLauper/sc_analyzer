#include "ColorMatcher.h"



identification::ColorMatcher::ColorMatcher()
{
}


identification::ColorMatcher::~ColorMatcher()
{
}

void identification::ColorMatcher::matchAllColors(std::vector<dto::Track>& tracks)
{
	for (auto& t : tracks)
	{
		for (auto& comp : tracks) {

			// Upper Bodycolor Suggestion
			float diffColorUpperBody = abs(t.primary_color_ids.upperBody - comp.primary_color_ids.upperBody);
			
			float probUpperBodyColor = 1 - (diffColorUpperBody / 2.0f);
			if (probUpperBodyColor < 0.0f) probUpperBodyColor = 0.0f;

			dto::Track::suggestion suggestionColorUpperBody;
			suggestionColorUpperBody.track = &comp;
			suggestionColorUpperBody.likelihood = probUpperBodyColor;
			t.upper_body_color_suggestion.push_back(suggestionColorUpperBody);

			// Lower Bodycolor Suggestion
			float diffColorLowerBody = abs(t.primary_color_ids.lowerBody - comp.primary_color_ids.lowerBody);

			float probLowerBodyColor = 1 - (diffColorLowerBody / 2);
			if (probLowerBodyColor < 0.0f) probLowerBodyColor = 0.0f;

			dto::Track::suggestion suggestionColorLowerBody;
			suggestionColorLowerBody.track = &comp;
			suggestionColorLowerBody.likelihood = probLowerBodyColor;
			t.lower_body_color_suggestion.push_back(suggestionColorLowerBody);
		}
	}
}
