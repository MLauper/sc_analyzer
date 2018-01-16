#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"

/*! \file FrameSelector.h
*	\brief Contains FrameSelector class.
*
* This file contains the FrameSelector class.
* It is used to choose an optimal frame for 
* further processing.
*/

/*! \brief The feature_extraction namespace contains all classes used for feature extraction.*/
namespace feature_extraction
{
	/*! \brief This class is used to select an optimal frame in a track.*/
	class FrameSelector
	{
	public:
		/*! \brief Default Constructor.*/
		FrameSelector();
		/*! \brief Select optimal frame in track.*/
		void SelectFrame(dto::Track& track, const dto::Camera& camera) const;
		/*! \brief Select optimal region in track.*/
		void SaveRegion(dto::Track& track, const dto::Camera& camera) const;
	};
}
