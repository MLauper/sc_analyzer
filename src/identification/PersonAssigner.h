#pragma once
#include <vector>
#include "../dto/Track.h"
#include "../dto/Person.h"

/*! \file PersonAssigner.h
*	\brief Contains PersonAssigner class.
*
* This file contains the PersonAssigner class.
* It is used to assign each track to a unique
* person.
*/

/*! \brief The identification namespace contains all classes used for identification.*/
namespace identification
{
	/*! \brief This class is used to assign a person to each track.*/
	class PersonAssigner
	{
	public:
		/*! \brief Default Constructor.*/
		PersonAssigner();
		/*! \brief Default Destructor.*/
		~PersonAssigner();
		/*! \brief Assign a person to each track.*/
		static void assignTracksToPerson(std::vector<dto::Track>& tracks, std::vector<dto::Person>& persons);
	};
}
