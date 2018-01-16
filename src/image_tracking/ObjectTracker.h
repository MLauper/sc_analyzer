#pragma once
#include <vector>
#include "../dto/Image.h"
#include "../dto/Track.h"
#include <random>
#include "../feature_extraction/Controller.h"

/*! \file ObjectTracker.h
*	\brief Contains ObjectTracker class.
*
* This file contains the ObjectTracker class.
* It is used to track objects based on the extracted
* regions.
*/

/*! \brief The image_tracking namespace contains all classes used for image tracking.*/
namespace image_tracking
{
	/*! \brief This class is used to track objects.*/
	class ObjectTracker
	{
	public:
		/*! \brief Default constructor.*/
		ObjectTracker();
		/*! \brief Remove background from image.*/
		void apply(dto::Image& image);
		/*! \brief Asks if there are any finished tracks.*/
		bool hasFinishedTracks();
		/*! \brief Retrieve a finished track.*/
		dto::Track getFinishedTrack();
		/*! \brief Send all finished tracks to feature extraction controller.*/
		void SendFinishedTracksTo(feature_extraction::Controller& controller, dto::Camera& camera);
	private:
		std::vector<dto::Track> currentTracks;
		int maxNumberOfMissingFramesInTrack;

		std::random_device rd;
		std::mt19937 rng;
		int latestTrackId = 0;
	};
}
