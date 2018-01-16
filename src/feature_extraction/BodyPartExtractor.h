#pragma once

#include "../dto/Camera.h"
#include "../dto/Track.h"

/*! \file BodyPartExtractor.h
*	\brief Contains BodyPartExtractor class.
*
* This file contains the BodyPartExtractor class.
* It is used to split the body parts of an input
* image in different parts.
*/

/*! \brief The feature_extraction namespace contains all classes used for feature extraction.*/
namespace feature_extraction
{
	/*! \brief This class is used to extract body parts from an input image.*/
	class BodyPartExtractor
	{
	public:
		/*! \brief Default constructor.*/
		BodyPartExtractor();
		/*! \brief Default destructor.*/
		~BodyPartExtractor();
		/*! \brief Extracts body parts from an input image and saves the result in the track dto.*/
		static void extractBodyParts(dto::Track& track, dto::Camera& camera);
	};
}
