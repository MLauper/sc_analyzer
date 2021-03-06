#include "ObjectTrackerYolo.h"
#include "../dto/Configuration.h"


image_tracking::ObjectTrackerYolo::ObjectTrackerYolo()
{
	this->maxNumberOfMissingFramesInTrack = dto::Configuration::MAX_NUMBER_OF_MISSING_FRAMES_IN_TRACK;
}

void image_tracking::ObjectTrackerYolo::apply(dto::Image& image)
{
	std::vector<dto::Track*> updatedTracks;
	for (auto x = 0; x < image.yoloPersons.size(); x++)
	{
		auto& p = image.yoloPersons.at(x);

		const cv::Rect curBB(p.x, p.y, p.w, p.h);

		// Try to find a previous region that fits
		std::vector<dto::Track*> candidateTracks;

		for (auto i = 0; i < this->currentTracks.size(); i++)
		{
			auto& prevPerson = this->currentTracks.at(i).persons.back();

			const cv::Rect prevBB(prevPerson.x, prevPerson.y, prevPerson.w, prevPerson.h);

			const auto intersectingBB = (curBB & prevBB).area() > 0;

			if (intersectingBB) { candidateTracks.push_back(&this->currentTracks.at(i)); }
		}

		//this->currentTracks.at(i).persons.push_back(p);
		//this->currentTracks.at(i).images.push_back(image);
		//updatedTracks.push_back(i);

		// Create new Track if region cannot be assigned
		if (candidateTracks.size() == 0)
		{
			dto::Track newTrack;
			newTrack.trackId = this->latestTrackId++;
			newTrack.persons.push_back(p);
			newTrack.images.push_back(image);
			newTrack.numImagesWithoutRegion = 0;
			this->currentTracks.push_back(newTrack);
		}
			// if exactly one candidate exist, use it
		else if (candidateTracks.size() == 1)
		{
			candidateTracks.at(0)->persons.push_back(p);
			candidateTracks.at(0)->images.push_back(image);
			updatedTracks.push_back(candidateTracks.at(0));
		}
			// if multiple candidates exist, choose closest
		else if (candidateTracks.size() > 1)
		{
			const cv::Point center_current(p.x + p.w / 2, p.y + p.h / 2);
			auto selectedTrack = candidateTracks.at(0);
			auto selectedTrackDistance = DBL_MAX;
			for (auto ct : candidateTracks)
			{
				const auto& prevPerson = ct->persons.back();
				const cv::Point center_previous(prevPerson.x + prevPerson.w / 2, prevPerson.y + prevPerson.h / 2);

				const auto candidateDistance = norm(center_previous - center_current);

				if (candidateDistance < selectedTrackDistance)
				{
					selectedTrack = ct;
					selectedTrackDistance = candidateDistance;
				}
			}

			if (selectedTrackDistance < dto::Configuration::MAX_TRACKING_DISTANCE)
			{
				selectedTrack->persons.push_back(p);
				selectedTrack->images.push_back(image);
				updatedTracks.push_back(selectedTrack);
			}
			else
			{
				dto::Track newTrack;
				newTrack.trackId = this->latestTrackId++;
				newTrack.persons.push_back(p);
				newTrack.images.push_back(image);
				newTrack.numImagesWithoutRegion = 0;
				this->currentTracks.push_back(newTrack);
			}
		}
	}

	for (auto t : updatedTracks)
	{
		t->numImagesWithoutRegion = 0;
	}

	for (auto i = 0; i < this->currentTracks.size(); i++)
	{
		this->currentTracks.at(i).numImagesWithoutRegion++;
	}
}

bool image_tracking::ObjectTrackerYolo::hasFinishedTracks()
{
	for (auto i = 0; i < this->currentTracks.size(); i++)
	{
		if (this->currentTracks.at(i).numImagesWithoutRegion >= this->maxNumberOfMissingFramesInTrack)
		{
			return true;
		}
	}

	return false;
}

void image_tracking::ObjectTrackerYolo::SendFinishedTracksTo(feature_extraction::Controller& controller,
                                                             dto::Camera& camera)
{
	for (auto i = 0; i < this->currentTracks.size(); i++)
	{
		if (this->currentTracks.at(i).numImagesWithoutRegion >= this->maxNumberOfMissingFramesInTrack)
		{
			if (this->currentTracks.at(i).images.size() >= dto::Configuration::MIN_NUMBERS_OF_FRAMES_IN_TRACK)
			{
				if (dto::Configuration::SAVE_TRACK_IMAGES)
				{
					// Save all Track Images
					if (dto::Configuration::SAVE_TRACK_IMAGES)
					{
						for (auto j = 0; j < this->currentTracks.at(i).persons.size(); j++)
						{
							std::stringstream image_out_path;
							image_out_path << dto::Configuration::TRACK_IMAGES_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix
								<< "\\" << "Track-" << this->currentTracks.at(i).trackId << "-" << j << "_original.jpg";
							//cv::imwrite(image_out_path.str().c_str(), this->currentTracks.at(i).images.at(j).cv_image_original);
						}
						for (auto j = 0; j < this->currentTracks.at(i).persons.size(); j++)
						{
							auto& p = this->currentTracks.at(i).persons.at(j);
							auto drawing = this->currentTracks.at(i).images.at(j).cv_image_original.clone();
							const auto color = cv::Scalar(0, 255, 0);
							rectangle(drawing, cv::Point(p.x, p.y), cv::Point(p.x + p.w, p.y + p.h), color, 3);

							std::stringstream image_out_path;
							image_out_path << dto::Configuration::TRACK_IMAGES_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix
								<< "\\" << "Track-" << this->currentTracks.at(i).trackId << "-" << j << "_yoloBB.jpg";
							imwrite(image_out_path.str().c_str(), drawing);
						}
					}

					// Collect all BBs on one Image
					cv::Mat drawingAll = cv::Mat::zeros(this->currentTracks.at(i).images.at(0).cv_image_original.size(), CV_8UC3);
					cv::RNG rng(12345);
					for (auto j = 0; j < this->currentTracks.at(i).persons.size(); j++)
					{
						auto& p = this->currentTracks.at(i).persons.at(j);
						const auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
						rectangle(drawingAll, cv::Point(p.x, p.y), cv::Point(p.x + p.w, p.y + p.h), color);
					}

					if (dto::Configuration::SAVE_TRACK_IMAGES)
					{
						std::stringstream image_out_path;
						image_out_path << dto::Configuration::TRACK_IMAGES_DIRECTORY << "Track-" << this->currentTracks.at(i).trackId <<
							"_yoloBB.jpg";
						imwrite(image_out_path.str().c_str(), drawingAll);
					}
				}

				controller.processTrack(this->currentTracks.at(i), camera);
			}
			this->currentTracks.erase(this->currentTracks.begin() + i);
		}
	}
}
