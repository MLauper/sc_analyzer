#include "SizeMatcher.h"


identification::SizeMatcher::SizeMatcher()
{
}


identification::SizeMatcher::~SizeMatcher()
{
}

void identification::SizeMatcher::matchAllSizes(std::vector<dto::Track>& tracks)
{
	for (auto& t : tracks)
	{
		for (auto& comp : tracks)
		{
			float diffHeight = abs(t.estimatedPersonSize.height - comp.estimatedPersonSize.height);
			float diffWidth = abs(t.estimatedPersonSize.width - comp.estimatedPersonSize.width);

			float probHeight = 1 - (diffHeight / 50);
			if (probHeight < 0.0f) probHeight = 0.0f;

			float probWidth = 1 - (diffWidth / 20);
			if (probWidth < 0.0f) probWidth = 0.0f;

			dto::Track::suggestion suggestionHeight;
			suggestionHeight.track = &comp;
			suggestionHeight.likelihood = probHeight;
			t.size_height_suggestion.push_back(suggestionHeight);

			dto::Track::suggestion suggestionWidth;
			suggestionWidth.track = &comp;
			suggestionWidth.likelihood = probWidth;
			t.size_width_suggestion.push_back(suggestionWidth);
		}
	}
}
