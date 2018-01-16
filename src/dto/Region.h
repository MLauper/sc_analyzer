#pragma once
#include <opencv2/core.hpp>
#include <vector>

/*! \file Region.h
*	\brief Contains DTO for the Region
*
* This file contains the region dto with all required properties.
*/

/*! \brief The dto namespace contains all DTOs*/
namespace dto
{
	/*! \brief The region dto struct cotnains all region properties.*/
	struct Region
	{
		/*! \brief The contour of the object as a list of points.*/
		std::vector<cv::Point> contour;

		/*! \brief Minimal x values of all points of the region.*/
		int minX;
		/*! \brief Minimal y values of all points of the region.*/
		int minY;
		/*! \brief Maximum x values of all points of the region.*/
		int maxX;
		/*! \brief Maximum y values of all points of the region.*/
		int maxY;

		/*! \brief The side ratio of the region.*/
		float ratio;
	};
}
