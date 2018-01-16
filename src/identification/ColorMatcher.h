#pragma once
#include <vector>
#include "dto/Track.h"

/*! \file ColorMatcher.h
*	\brief Contains ColorMatcher class.
*
* This file contains the ColorMatcher class.
* It is used to compare the color of two persons
* and assign a likelihood that they contain the 
* same person.
*/

/*! \brief The identification namespace contains all classes used for identification.*/
namespace identification
{
	/*! \brief This class is used to compare the color of two tracks.*/
	class ColorMatcher
	{
	public:
		/*! \brief Default Constructor.*/
		ColorMatcher();
		/*! \brief Default Destructor.*/
		~ColorMatcher();
		/*! \brief Compare colors between all tracks.*/
		void matchAllColors(std::vector<dto::Track>& tracks) const;
	};
}
