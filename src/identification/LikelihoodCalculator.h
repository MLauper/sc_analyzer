#pragma once
#include "dto/Track.h"

/*! \file LikelihoodCalculator.h
*	\brief Contains LikelihoodCalculator class.
*
* This file contains the LikelihoodCalculator class.
* It is used to calculate the overall likelihood
* that two tracks contain the same person.
*/

/*! \brief The identification namespace contains all classes used for identification.*/
namespace identification
{
	/*! \brief This class is used to calculate the overall likelihood two tracks match.*/
	class LikelihoodCalculator
	{
	public:
		/*! \brief Default Constructor.*/
		LikelihoodCalculator();
		/*! \brief Default Destructor.*/
		~LikelihoodCalculator();
		/*! \brief Calculate overall likelihood of tracks.*/
		void calculateAllLikelihoods(std::vector<dto::Track>& tracks) const;
	};
}
