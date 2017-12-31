#include "LikelihoodCalculator.h"
#include "../dto/Configuration.h"


identification::LikelihoodCalculator::LikelihoodCalculator()
{
}


identification::LikelihoodCalculator::~LikelihoodCalculator()
{
}

bool compare_suggestions(dto::Track::suggestion i, dto::Track::suggestion j) { return (i.likelihood > j.likelihood); }

void identification::LikelihoodCalculator::calculateAllLikelihoods(std::vector<dto::Track>& tracks)
{
	for (auto& t : tracks)
	{
		for (int i = 0; i < tracks.size(); i++)
		{
			float overall_likelihood = 0.0f;
			overall_likelihood += t.size_width_suggestion.at(i).likelihood * dto::Configuration::size_width_suggestion_weight;
			overall_likelihood += t.size_height_suggestion.at(i).likelihood * dto::Configuration::size_height_suggestion_weight;
			overall_likelihood += t.surf_keypoint_suggestion.at(i).likelihood * dto::Configuration::surf_keypoint_suggestion_weight;
			overall_likelihood += t.sift_keypoint_suggestion.at(i).likelihood * dto::Configuration::sift_keypoint_suggestion_weight;
			overall_likelihood += t.upper_body_color_suggestion.at(i).likelihood * dto::Configuration::upper_body_color_suggestion_weight;
			overall_likelihood += t.lower_body_color_suggestion.at(i).likelihood * dto::Configuration::lower_body_color_suggestion_weight;

			dto::Track::suggestion overallSuggestion;
			overallSuggestion.track = t.size_width_suggestion.at(i).track;
			overallSuggestion.likelihood = overall_likelihood;

			t.overall_suggestion.push_back(overallSuggestion);
		}

		// Sort all vectors for output
		std::sort(t.size_width_suggestion.begin(), t.size_width_suggestion.end(), compare_suggestions);
		std::sort(t.size_height_suggestion.begin(), t.size_height_suggestion.end(), compare_suggestions);
		std::sort(t.surf_keypoint_suggestion.begin(), t.surf_keypoint_suggestion.end(), compare_suggestions);
		std::sort(t.sift_keypoint_suggestion.begin(), t.sift_keypoint_suggestion.end(), compare_suggestions);
		std::sort(t.upper_body_color_suggestion.begin(), t.upper_body_color_suggestion.end(), compare_suggestions);
		std::sort(t.lower_body_color_suggestion.begin(), t.lower_body_color_suggestion.end(), compare_suggestions);
		std::sort(t.overall_suggestion.begin(), t.overall_suggestion.end(), compare_suggestions);

		if (dto::Configuration::SAVE_TRACK_STATISTICS)
		{
			std::ofstream fileStream;
			std::stringstream filePath;
			filePath << dto::Configuration::STATISTICS_DIRECTORY << "scene-" << t.camera.scene << "\\" << t.camera.prefix << "\\" << "Track-" << t.trackId << "_statistics.txt";
			fileStream.open(filePath.str().c_str(), std::fstream::app);
			fileStream << "Identification: " << std::endl;
			
			fileStream << " size_width_suggestions:" << std::endl;
			for (auto& s : t.size_width_suggestion)
			{
				fileStream << "  " << s.likelihood << ": Track " << s.track->trackId << ", Scene " << s.track->camera.scene << ", Prefix " << s.track->camera.prefix << std::endl;
			}

			fileStream << " size_height_suggestion:" << std::endl;
			for (auto& s : t.size_height_suggestion)
			{
				fileStream << "  " << s.likelihood << ": Track " << s.track->trackId << ", Scene " << s.track->camera.scene << ", Prefix " << s.track->camera.prefix << std::endl;
			}

			fileStream << " surf_keypoint_suggestion:" << std::endl;
			for (auto& s : t.surf_keypoint_suggestion)
			{
				fileStream << "  " << s.likelihood << ": Track " << s.track->trackId << ", Scene " << s.track->camera.scene << ", Prefix " << s.track->camera.prefix << std::endl;
			}

			/*
			fileStream << " sift_keypoint_suggestion:" << std::endl;
			for (auto& s : t.sift_keypoint_suggestion)
			{
				fileStream << "  " << s.likelihood << ": Track " << s.track->trackId << ", Scene " << s.track->camera.scene << ", Prefix " << s.track->camera.prefix << std::endl;
			}
			*/

			fileStream << " upper_body_color_suggestion:" << std::endl;
			for (auto& s : t.upper_body_color_suggestion)
			{
				fileStream << "  " << s.likelihood << ": Track " << s.track->trackId << ", Scene " << s.track->camera.scene << ", Prefix " << s.track->camera.prefix << std::endl;
			}

			fileStream << " lower_body_color_suggestion:" << std::endl;
			for (auto& s : t.lower_body_color_suggestion)
			{
				fileStream << "  " << s.likelihood << ": Track " << s.track->trackId << ", Scene " << s.track->camera.scene << ", Prefix " << s.track->camera.prefix << std::endl;
			}

			fileStream << " overall_suggestion:" << std::endl;
			for (auto& s : t.overall_suggestion)
			{
				fileStream << "  " << s.likelihood << ": Track " << s.track->trackId << ", Scene " << s.track->camera.scene << ", Prefix " << s.track->camera.prefix << std::endl;
			}
			
			fileStream.close();
		}
	}
}
