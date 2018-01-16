#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"

/*! \file ColorExtractor.h
*	\brief Contains ColorExtractor class.
*
* This file contains the ColorExtractor class.
* It is used to analyze the primary color
* of the different body parts.
*/

/*! \brief The feature_extraction namespace contains all classes used for feature extraction.*/
namespace feature_extraction
{
	/*! \brief  This class is used to extract the primary body colors.*/
	class ColorExtractor
	{
	public:
		/*! \brief Default Constructor.*/
		ColorExtractor();
		/*! \brief Default Destructor.*/
		~ColorExtractor();
		/*! \brief Extract primary color and save it in maxBucketId.*/
		void extractMaxHue(dto::Track& track, dto::Camera& camera, cv::Mat& hsv_image, int& maxBucketId,
		                   bool isUpperBody) const;
		/*! \brief Extract primary colors of all body parts.*/
		void extractPrimaryColors(dto::Track& track, dto::Camera& camera) const;
	};
}
