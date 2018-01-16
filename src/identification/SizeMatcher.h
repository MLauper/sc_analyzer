#pragma once
#include <vector>
#include "dto/Track.h"

/*! \file SizeMatcher.h
*	\brief Contains SizeMatcher class.
*
* This file contains the SizeMatcher class.
* It is used to compare the person sizes
* of two tracks and assign them a likelihood
* that they contain the same person.
*/

/*! \brief The identification namespace contains all classes used for identification.*/
namespace identification
{
	/*! \brief This class is used to compare sizes.*/
	class SizeMatcher
	{
	public:
		/*! \brief Default Constructor.*/
		SizeMatcher();
		/*! \brief Default Destructor.*/
		~SizeMatcher();
		/*! \brief Compare sizes between all tracks.*/
		void matchAllSizes(std::vector<dto::Track>& tracks) const;
	};
}
