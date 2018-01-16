#pragma once
#include "../dto/Track.h"
#include "DirectionExtractor.h"
#include "FrameSelector.h"
#include "ColorExtractor.h"
#include "BodyPartExtractor.h"
#include "SizeExtractor.h"
#include "FeaturePointExtractor.h"
#include "TrackPersistor.h"

/*! \file Controller.h
*	\brief Contains Controller class.
*
* This file contains the Controller class.
* It is used to control all feature extraction
* steps.
*/

/*! \brief The feature_extraction namespace contains all classes used for feature extraction.*/
namespace feature_extraction
{
	/*! \brief This class is used to control all feature extraction steps.*/
	class Controller
	{
	public:
		/*! \brief Default Constructor.*/
		Controller();
		/*! \brief Start feature extraction of a track.*/
		void processTrack(dto::Track& track, dto::Camera& camera);
	private:
		DirectionExtractor direction_extractor_;
		FrameSelector frame_selector_;
		ColorExtractor color_extractor_;
		BodyPartExtractor body_part_extractor_;
		SizeExtractor size_extractor_;
		FeaturePointExtractor feature_point_extractor_;
		TrackPersistor track_persistor_;
		int personsInTheRoom;
	};
}
