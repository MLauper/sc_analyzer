#include "ColorMatcher.h"


identification::ColorMatcher::ColorMatcher()
{
}


identification::ColorMatcher::~ColorMatcher()
{
}

void identification::ColorMatcher::matchAllColors(std::vector<dto::Track>& tracks) const
{
	for (auto& t : tracks)
	{
		for (auto& comp : tracks)
		{
			// Upper Bodycolor Suggestion
			float diffColorUpperBody_1 = abs(t.primary_color_ids.upperBody - comp.primary_color_ids.upperBody);
			float diffColorUpperBody_2 = abs(t.primary_color_ids.upperBody - comp.primary_color_ids.upperBody - 16);
			const float diffColorUpperBody = diffColorUpperBody_1 < diffColorUpperBody_2
				                                 ? diffColorUpperBody_1
				                                 : diffColorUpperBody_2;

			float probUpperBodyColor = 1 - diffColorUpperBody / 2.0f;
			if (probUpperBodyColor < 0.0f) probUpperBodyColor = 0.0f;

			dto::Track::suggestion suggestionColorUpperBody;
			suggestionColorUpperBody.track = &comp;
			suggestionColorUpperBody.likelihood = probUpperBodyColor;
			t.upper_body_color_suggestion.push_back(suggestionColorUpperBody);

			// Lower Bodycolor Suggestion
			float diffColorLowerBody_1 = abs(t.primary_color_ids.lowerBody - comp.primary_color_ids.lowerBody);
			float diffColorLowerBody_2 = abs(t.primary_color_ids.lowerBody - comp.primary_color_ids.lowerBody - 16);
			const float diffColorLowerBody = diffColorLowerBody_1 < diffColorLowerBody_2
				                                 ? diffColorLowerBody_1
				                                 : diffColorLowerBody_2;

			float probLowerBodyColor = 1 - diffColorLowerBody / 2;
			if (probLowerBodyColor < 0.0f) probLowerBodyColor = 0.0f;

			dto::Track::suggestion suggestionColorLowerBody;
			suggestionColorLowerBody.track = &comp;
			suggestionColorLowerBody.likelihood = probLowerBodyColor;
			t.lower_body_color_suggestion.push_back(suggestionColorLowerBody);
		}
	}
}
