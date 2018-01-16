#pragma once
//#include <string>
#include <image_segmentation/Controller.h>
//#include <set>
#include "dto/Camera.h"

/*! \file URLImageLoader.h
*	\brief Contains URLImageLoader class.
*
* This file contains the URLImageLoader class.
* It is used to load images based on a URL. 
* Each time a frame was processed, a new image 
* is requested at the configured URL.
*/

/*! \brief The image_acquisition namespace contains all classes used for image acquisition.*/
namespace image_acquisition
{
	/*! \brief This class is used to stream frames from an WebURL.*/
	class URLImageLoader
	{
	public:
		/*! \brief Publish Results.*/
		static void publishResults(int size);
		/*! \brief Start loading images from URL.*/
		void startCapturing();
		/*! \brief Constructor.*/
		URLImageLoader(dto::Camera& camera, image_segmentation::Controller* segmentation_controller);
		/*! \brief Default Destructor*/
		~URLImageLoader();
	private:
		dto::Camera camera;
		image_segmentation::Controller* segmentation_controller;
		cv::Mat dist_map1;
		cv::Mat dist_map2;
		int frameNumber;
	};
}
