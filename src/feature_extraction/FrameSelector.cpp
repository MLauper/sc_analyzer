#include "FrameSelector.h"
#include "../dto/Track.h"
#include "../dto/Camera.h"
#include "../dto/Configuration.h"

feature_extraction::FrameSelector::FrameSelector()
{
}

void feature_extraction::FrameSelector::SelectFrame(dto::Track& track, const dto::Camera& camera)
{
	if (dto::Configuration::USE_POINT_FOR_OPTIMAL_TRACK)
	{
		struct personCandidate
		{
			int personID;
			int distance_x;
			int distance_y;
		};
		std::vector<personCandidate> optimalPersonCandidates;

		// Find any candidate that lies within the configured region on y-axis
		for (int i = 0; i < track.persons.size(); i++)
		{
			int candidateDistance = abs(
				(
					static_cast<int>(track.persons.at(i).y) + static_cast<int>(track.persons.at(i).h)
				) - camera.optimalPersonLocation.y);
			if (candidateDistance < dto::Configuration::MAX_OPTIMAL_DISTANCE)
			{
				personCandidate c;
				c.personID = i;
				c.distance_y = candidateDistance;
				c.distance_x = abs(
					(static_cast<int>(track.persons.at(i).x) + static_cast<int>(track.persons.at(i).w)) - camera.optimalPersonLocation.
					                                                                                             x);
				optimalPersonCandidates.push_back(c);

				if (dto::Configuration::PRINT_FRAME_SELECTION_STEPS)
				{
					std::cout << "Found candidate with ID: " << i << std::endl;
					std::cout << " y-Position: " << static_cast<int>(track.persons.at(i).y) << " + " << static_cast<int>(track.persons.
					                                                                                                           at(i).h)
						<< " = " << static_cast<int>(track.persons.at(i).y) + static_cast<int>(track.persons.at(i).h) << std::endl;
					std::cout << " y-Distance to " << camera.optimalPersonLocation.y << ": " << candidateDistance << std::endl;
					std::cout << " x-Position: " << static_cast<int>(track.persons.at(i).x) << " + " << static_cast<int>(track.persons.
					                                                                                                           at(i).w)
						<< " = " << static_cast<int>(track.persons.at(i).x) + static_cast<int>(track.persons.at(i).w) << std::endl;
					std::cout << " x-Distance to " << camera.optimalPersonLocation.x << ": " << c.distance_x << std::endl;
				}
			}
		}

		// Select nearest person on x-axis
		track.optimalPersonId = -1;
		int optimalRegionDistance = INT_MAX;
		for (int i = 0; i < optimalPersonCandidates.size(); i++)
		{
			if (optimalPersonCandidates.at(i).distance_x < optimalRegionDistance)
			{
				if (dto::Configuration::PRINT_FRAME_SELECTION_STEPS)
				{
					std::cout << "Select better candidate ID: " << optimalPersonCandidates.at(i).personID << std::endl;
				}
				track.optimalPersonId = optimalPersonCandidates.at(i).personID;
				optimalRegionDistance = optimalPersonCandidates.at(i).distance_x;
			}
		}
	}


	if (dto::Configuration::SAVE_TRACK_STATISTICS)
	{
		std::ofstream fileStream;

		std::stringstream filePath;
		filePath << dto::Configuration::STATISTICS_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix << "\\" <<
			"Track-" << track.trackId << "_statistics.txt";
		fileStream.open(filePath.str().c_str(), std::fstream::app);

		fileStream << "Selected Track Picture: " << track.optimalPersonId << std::endl;
		if (track.optimalPersonId != -1)
		{
			auto& selectedPerson = track.persons.at(track.optimalPersonId);
			fileStream << "Selected Track Location Point: " << (selectedPerson.x + (selectedPerson.w / 2)) << "x" << (
				selectedPerson.y + (selectedPerson.h / 2)) << std::endl;
		}
		fileStream.close();
	}

	if (dto::Configuration::SAVE_OPTIMAL_TRACK_IMAGE && track.optimalPersonId != -1)
	{
		cv::Mat drawingAll;
		if (track.optimalPersonId != -1)
		{
			drawingAll = track.images.at(track.optimalPersonId).cv_image_original.clone();
			cv::Scalar color = cv::Scalar(0, 255, 0);

			auto& person = track.persons.at(track.optimalPersonId);
			rectangle(drawingAll, cv::Point(person.x, person.y), cv::Point(person.x + person.w, person.y + person.h), color, 3);
		}

		std::stringstream image_out_path;
		image_out_path << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix <<
			"\\" << "Track-" << track.trackId << "_optimalImage-original.jpg";
		imwrite(image_out_path.str().c_str(), drawingAll);
	}
}

void feature_extraction::FrameSelector::SaveRegion(dto::Track& track, const dto::Camera& camera)
{
	auto& optimalPerson = track.persons.at(track.optimalPersonId);
	cv::Rect roi;
	roi.x = optimalPerson.x;
	roi.y = optimalPerson.y;
	roi.width = optimalPerson.w;
	roi.height = optimalPerson.h;

	track.cv_optimalPersonCut = track.images.at(track.optimalPersonId).cv_fgimg(roi);
	track.cv_optimalPersonCut_Full = track.images.at(track.optimalPersonId).cv_image_original(roi);

	if (dto::Configuration::SAVE_OPTIMAL_TRACK_IMAGE_CUT)
	{
		std::stringstream image_out_path;
		image_out_path << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix <<
			"\\" << "Track-" << track.trackId << "_optimalImage-cut.jpg";
		imwrite(image_out_path.str().c_str(), track.cv_optimalPersonCut);

		std::stringstream image_out_path_full;
		image_out_path_full << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "scene-" << camera.scene << "\\" << camera.
			prefix << "\\" << "Track-" << track.trackId << "_optimalImage-cut_Full.jpg";
		imwrite(image_out_path_full.str().c_str(), track.cv_optimalPersonCut_Full);
	}
}
