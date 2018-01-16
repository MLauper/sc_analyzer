#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"

/*! \file SizeExtractor.h
*	\brief Contains SizeExtractor class.
*
* This file contains the SizeExtractor class.
* It is used to estimate body height and width
* based on an input image.
*/

/*! \brief The feature_extraction namespace contains all classes used for feature extraction.*/
namespace feature_extraction
{
	/*! \brief This class is used to estimate body height and width.*/
	class SizeExtractor
	{
	public:
		/*! \brief Default constructor.*/
		SizeExtractor();
		/*! \brief Default destructor.*/
		~SizeExtractor();
		/*! \brief Extract body sizes from an input track and stores the result in the track dto.*/
		static void extractBodySizes(dto::Track& track, dto::Camera camera);
	};
}
