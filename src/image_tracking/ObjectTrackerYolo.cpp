#include "ObjectTrackerYolo.h"
#include "../dto/Configuration.h"


image_tracking::ObjectTrackerYolo::ObjectTrackerYolo()
{
	this->maxNumberOfMissingFramesInTrack = dto::Configuration::MAX_NUMBER_OF_MISSING_FRAMES_IN_TRACK;
}

void image_tracking::ObjectTrackerYolo::apply(dto::Image& image)
{

	std::vector<int> updatedTracks;
	for (int x = 0; x < image.yoloPersons.size(); x++)
	{
		auto& p = image.yoloPersons.at(x);

		bool personAssigned = false;

		const int minX = p.x;
		const int minY = p.y;
		const int maxX = p.x + p.w;
		const int maxY = p.y + p.h;

		// Try to find a previous region that fits
		for (int i = 0; i < this->currentTracks.size(); i++)
		{
			auto& prevPerson = this->currentTracks.at(i).persons.back();

			const int prevMinX = prevPerson.x;
			const int prevMinY = prevPerson.y;
			const int prevMaxX = prevPerson.x + prevPerson.w;
			const int prevMaxY = prevPerson.y + prevPerson.h;
			
			if (minX >= prevMinX && minX <= prevMaxX) { this->currentTracks.at(i).persons.push_back(p); this->currentTracks.at(i).images.push_back(image); updatedTracks.push_back(i); personAssigned = true; break; }
			if (minY >= prevMinY && minY <= prevMaxY) { this->currentTracks.at(i).persons.push_back(p); this->currentTracks.at(i).images.push_back(image); updatedTracks.push_back(i); personAssigned = true; break; }
			if (maxX >= prevMinX && maxX <= prevMaxX) { this->currentTracks.at(i).persons.push_back(p); this->currentTracks.at(i).images.push_back(image); updatedTracks.push_back(i); personAssigned = true; break; }
			if (maxY >= prevMinY && maxY <= prevMaxY) { this->currentTracks.at(i).persons.push_back(p); this->currentTracks.at(i).images.push_back(image); updatedTracks.push_back(i); personAssigned = true; break; }
		}

		// Create new Track if region cannot be assigned
		if (personAssigned == false)
		{
			dto::Track newTrack;
			newTrack.trackId = this->latestTrackId++;
			newTrack.persons.push_back(p);
			newTrack.images.push_back(image);
			newTrack.numImagesWithoutRegion = 0;
			this->currentTracks.push_back(newTrack);
		}
	}

	for (int track_id : updatedTracks)
	{
		this->currentTracks.at(track_id).numImagesWithoutRegion = 0;
	}

	for (int i = 0; i < this->currentTracks.size(); i++)
	{
		this->currentTracks.at(i).numImagesWithoutRegion++;

	}
}

bool image_tracking::ObjectTrackerYolo::hasFinishedTracks()
{
	for (int i = 0; i < this->currentTracks.size(); i++)
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
	for (int i = 0; i < this->currentTracks.size(); i++)
	{
		if (this->currentTracks.at(i).numImagesWithoutRegion >= this->maxNumberOfMissingFramesInTrack)
		{

			if (dto::Configuration::SAVE_TRACK_IMAGES || dto::Configuration::SHOW_TRACK_IMAGES)
			{

				cv::Mat drawingAll = cv::Mat::zeros(this->currentTracks.at(i).images.at(0).cv_image_original.size(), CV_8UC3);
				cv::RNG rng(12345);

				for (int j = 0; j < this->currentTracks.at(i).persons.size(); j++)
				{
					auto& p = this->currentTracks.at(i).persons.at(j);
					cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
					cv::rectangle(drawingAll, cv::Point(p.x, p.y), cv::Point(p.x + p.w, p.y + p.h), color);
				}

				if (dto::Configuration::SAVE_TRACK_IMAGES) {
					std::stringstream image_out_path;
					image_out_path << dto::Configuration::TRACK_IMAGES_DIRECTORY << "Track-" << this->currentTracks.at(i).trackId << "_yoloBB.jpg";
					cv::imwrite(image_out_path.str().c_str(), drawingAll);
				}
				if (dto::Configuration::SHOW_TRACK_IMAGES)
				{
					cv::imshow("Track Bounding Boxes", drawingAll);
					cv::waitKey(1);
				}
			}

			controller.processTrack(this->currentTracks.at(i), camera);
			this->currentTracks.erase(this->currentTracks.begin() + i);
		}
	}
}

