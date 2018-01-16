#pragma once
#include "../dto/Track.h"
#include "../dto/Image.h"
#include "../feature_extraction/Controller.h"
#include <random>

/*! \file ObjectTrackerYolo.h
*	\brief Contains ObjectTrackerYolo class.
*
* This file contains the ObjectTrackerYolo class.
* It is used to track objects based on bounding 
* boxes.
*/

/*! \brief The image_tracking namespace contains all classes used for image tracking.*/
namespace image_tracking
{
	/*! \brief This class is used to track objects.*/
	class ObjectTrackerYolo
	{
	public:
		/*! \brief Default constructor.*/
		ObjectTrackerYolo();
		/*! \brief Remove background from image.*/
		void apply(dto::Image& image);
		/*! \brief Ask if there are any finished tracks.*/
		bool hasFinishedTracks();
		/*! \brief Send finished tracks to feature extraction controller.*/
		void SendFinishedTracksTo(feature_extraction::Controller& controller, dto::Camera& camera);
	private:
		std::vector<dto::Track> currentTracks;
		int maxNumberOfMissingFramesInTrack;

		std::random_device rd;
		std::mt19937 rng;
		int latestTrackId = 0;
		cv::Mat drawingAll;
	};
}
