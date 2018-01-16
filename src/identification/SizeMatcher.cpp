#include "SizeMatcher.h"


identification::SizeMatcher::SizeMatcher()
{
}


identification::SizeMatcher::~SizeMatcher()
{
}

void identification::SizeMatcher::matchAllSizes(std::vector<dto::Track>& tracks) const
{
	for (auto& t : tracks)
	{
		for (auto& comp : tracks)
		{
			const auto diffHeight = abs(t.estimatedPersonSize.height - comp.estimatedPersonSize.height);
			const auto diffWidth = abs(t.estimatedPersonSize.width - comp.estimatedPersonSize.width);

			auto probHeight = 1 - diffHeight / 50;
			if (probHeight < 0.0f) probHeight = 0.0f;

			auto probWidth = 1 - diffWidth / 20;
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
