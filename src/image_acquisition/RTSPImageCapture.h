#pragma once
//#include <string>
#include <image_segmentation/Controller.h>
//#include <set>
#include "dto/Camera.h"

/*! \file RTSPImageCapture.h
*	\brief Contains RTSPImageCapture class.
*
* This file contains the RTSPImageCapture class.
* It is used to connect to an rtsp source and
* stream each frame in the pipeline.
*/

/*! \brief The image_acquisition namespace contains all classes used for image acquisition.*/
namespace image_acquisition
{
	/*! \brief This class is used to stream frames from a RTSP source.*/
	class RTSPImageCapture
	{
	public:
		/*! \brief Start reading images from rtsp stream.*/
		void startCapturing();
		/*! \brief Constructor.*/
		RTSPImageCapture(dto::Camera& camera, image_segmentation::Controller* segmentation_controller);
		/*! \brief Default destructor.*/
		~RTSPImageCapture();
	private:
		dto::Camera camera;
		image_segmentation::Controller* segmentation_controller;
		cv::Mat dist_map1;
		cv::Mat dist_map2;
		int frameNumber;
		cv::VideoCapture* cv_videoCapture;
	};
}
