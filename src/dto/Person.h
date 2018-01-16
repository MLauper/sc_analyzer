#pragma once
#include "Track.h"

/*! \file Person.h
*	\brief Contains DTO for the Person
*
* This file contains the person dto with all required properties.
*/

/*! \brief The dto namespace contains all DTOs*/
namespace dto
{
	/*! \brief The person dto struct cotnains all person properties.*/
	struct Person
	{
		/*! \brief A unique person id.*/
		int person_id;
		/*! \brief All tracks that are assigned to this person.*/
		std::vector<Track> tracks;
	};
}
