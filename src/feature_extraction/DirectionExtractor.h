#pragma once
#include "../dto/Track.h"
#include "../dto/Camera.h"

/*! \file DirectionExtractor.h
*	\brief Contains DirectionExtractor class.
*
* This file contains the DirectionExtractor class.
* It is used to analyze if a person is entering
* or leaving the room.
*/

/*! \brief The feature_extraction namespace contains all classes used for feature extraction.*/
namespace feature_extraction
{
	/*! \brief This class is used to estimate the walking direction of a person.*/
	class DirectionExtractor
	{
	public:
		/*! \brief Default Constructor.*/
		DirectionExtractor();
		/*! \brief Extract walking direction.*/
		static void extractDirection(dto::Track& track, dto::Camera& camera);
	};
}
