#include "ObjectTracker.h"
#include <opencv2/core.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "../dto/Configuration.h"


image_tracking::ObjectTracker::ObjectTracker()
{
	this->maxNumberOfMissingFramesInTrack = dto::Configuration::MAX_NUMBER_OF_MISSING_FRAMES_IN_TRACK;
}

void image_tracking::ObjectTracker::apply(dto::Image& image)
{
	std::vector<int> updatedTracks;
	for (auto region : image.regions)
	{
		auto regionAssigned = false;

		// Try to find a previous region that fits
		for (auto i = 0; i < this->currentTracks.size(); i++)
		{
			const auto prevRegion = this->currentTracks.at(i).regions.back();
			if (region.minX >= prevRegion.minX && region.minX <= prevRegion.maxX)
			{
				this->currentTracks.at(i).regions.push_back(region);
				this->currentTracks.at(i).images.push_back(image);
				updatedTracks.push_back(i);
				regionAssigned = true;
				break;
			}
			if (region.minY >= prevRegion.minY && region.minY <= prevRegion.maxY)
			{
				this->currentTracks.at(i).regions.push_back(region);
				this->currentTracks.at(i).images.push_back(image);
				updatedTracks.push_back(i);
				regionAssigned = true;
				break;
			}
			if (region.maxX >= prevRegion.minX && region.maxX <= prevRegion.maxX)
			{
				this->currentTracks.at(i).regions.push_back(region);
				this->currentTracks.at(i).images.push_back(image);
				updatedTracks.push_back(i);
				regionAssigned = true;
				break;
			}
			if (region.maxY >= prevRegion.minY && region.maxY <= prevRegion.maxY)
			{
				this->currentTracks.at(i).regions.push_back(region);
				this->currentTracks.at(i).images.push_back(image);
				updatedTracks.push_back(i);
				regionAssigned = true;
				break;
			}
		}

		// Create new Track if region cannot be assigned
		if (regionAssigned == false)
		{
			dto::Track newTrack;
			newTrack.trackId = this->latestTrackId++;
			newTrack.regions.push_back(region);
			newTrack.images.push_back(image);
			newTrack.numImagesWithoutRegion = 0;
			this->currentTracks.push_back(newTrack);
		}
	}

	for (auto track_id : updatedTracks)
	{
		this->currentTracks.at(track_id).numImagesWithoutRegion = 0;
	}

	for (auto i = 0; i < this->currentTracks.size(); i++)
	{
		this->currentTracks.at(i).numImagesWithoutRegion++;

		//if (this->currentTracks.at(i).numImagesWithoutRegion >= this->maxNumberOfMissingFramesInTrack)
		//{
		//	// Track is finished
		//	std::cout << "============== TRACK IS FINISHED";
		//
		//	int y = 0;
		//	
		//	cv::Vec3b color(0, 255 * this->currentTracks.at(i).trackId, 255);
		//
		//	for (int j = 0; j < this->currentTracks.at(i).regions.size(); j ++)
		//	{
		//		dto::Image im = this->currentTracks.at(i).images.at(j);
		//		dto::Region re = this->currentTracks.at(i).regions.at(j);
		//
		//		for (int n = 0; n < re.contour.size(); n++)
		//		{
		//			drawingAll.at<cv::Vec3b>(re.contour.at(n).y, re.contour.at(n).x) = color;
		//		}
		//
		//		std::stringstream image_out_path;
		//		image_out_path << "c:\\temp\\\extracted_persons\\track-" << this->currentTracks.at(i).trackId << "_image-" << y << ".jpg";
		//		y += 1;
		//		cv::imwrite(image_out_path.str().c_str(), drawingAll);
		//	}
		//
		//}
	}

	//this->currentTracks.erase(std::remove_if(this->currentTracks.begin(), this->currentTracks.end(),
	//	[](dto::Track t) { return t.numImagesWithoutRegion >= 8; }), this->currentTracks.end());

	//cv::Mat drawing = cv::Mat::zeros(image.cv_fgmask.size(), CV_8UC3);
	//cv::RNG rng(12345);
	//for (size_t i = 0; i < this->currentTracks.at(0).size(); i++)
	//{
	//	cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	cv::drawContours(drawing, this->currentTracks.at(0), (int)i, color, 2, 8, 0, 0, cv::Point());
	//}
	//cv::imshow("Tracking", drawing);
	//
	//if (this->currentTracks.at(0).size() > 0) {
	//	std::stringstream image_out_path_passed;
	//	image_out_path_passed << "c:\\temp\\\extracted_persons\\contourSeries_" << this->currentTracks.at(0).size() << ".jpg";
	//	cv::imwrite(image_out_path_passed.str().c_str(), drawing);
	//}
}

bool image_tracking::ObjectTracker::hasFinishedTracks()
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

dto::Track image_tracking::ObjectTracker::getFinishedTrack()
{
	for (auto i = 0; i < this->currentTracks.size(); i++)
	{
		if (this->currentTracks.at(i).numImagesWithoutRegion >= this->maxNumberOfMissingFramesInTrack)
		{
			auto t = this->currentTracks.at(i);
			this->currentTracks.erase(this->currentTracks.begin() + i);
			return t;
		}
	}
	return {};
}

void image_tracking::ObjectTracker::SendFinishedTracksTo(feature_extraction::Controller& controller,
                                                         dto::Camera& camera)
{
	for (auto i = 0; i < this->currentTracks.size(); i++)
	{
		if (this->currentTracks.at(i).numImagesWithoutRegion >= this->maxNumberOfMissingFramesInTrack)
		{
			if (dto::Configuration::SAVE_TRACK_IMAGES)
			{
				cv::Mat drawingAll = cv::Mat::zeros(this->currentTracks.at(i).images.at(0).cv_image_original.size(), CV_8UC3);
				cv::RNG rng(12345);

				for (auto j = 0; j < this->currentTracks.at(i).regions.size(); j++)
				{
					const auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
					drawContours(drawingAll, std::vector<std::vector<cv::Point>>(1, this->currentTracks.at(i).regions.at(j).contour),
					             0, color, 2, 8, 0, 0, cv::Point());
				}

				if (dto::Configuration::SAVE_TRACK_IMAGES)
				{
					std::stringstream image_out_path;
					image_out_path << dto::Configuration::TRACK_IMAGES_DIRECTORY << "Track-" << this->currentTracks.at(i).trackId <<
						"_contours.jpg";
					imwrite(image_out_path.str().c_str(), drawingAll);
				}
			}

			controller.processTrack(this->currentTracks.at(i), camera);
			this->currentTracks.erase(this->currentTracks.begin() + i);
		}
	}
}

//void image_tracking::ObjectTracker::apply(cv::Mat image, cv::Mat mask)
//{
//	std::vector<std::vector<cv::Point>> contours;
//	std::vector<cv::Vec4i> hierarchy;
//	
//	try {
//		cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
//	}
//	catch (std::exception e)
//	{
//		std::cerr << e.what();
//	}
//	cv::Mat drawing = cv::Mat::zeros(mask.size(), CV_8UC3);
//	cv::RNG rng(12345);
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
//		cv::drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, cv::Point());
//	}
//	cv::imshow("Contours", drawing);
//	std::cout << "Found " << contours.size() << " contours.\n";
//
//}
