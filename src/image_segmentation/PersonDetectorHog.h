#pragma once
#include <opencv2/core.hpp>

/*! \file PersonDetectorHog.h
*	\brief Contains PersonDetectorHog class.
*
* This file contains the PersonDetectorHog class.
* It is used to detect persons based on the Hog
* algorithm.
*/

/*! \brief The image_segmentation namespace contains all classes used for image segmentation.*/
namespace image_segmentation
{
	/*! \brief This class is used to detect persons on an image.*/
	class PersonDetectorHog
	{
	public:
		/*! \brief Default constructor.*/
		PersonDetectorHog();
		/*! \brief Detect a person on an image using Hog.*/
		void detectPerson(cv::cuda::GpuMat image) const;
	private:

		cv::Mat frame;
	};
}
