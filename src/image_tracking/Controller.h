#pragma once
#include "ObjectTracker.h"
#include "../dto/Image.h"
#include "../feature_extraction/Controller.h"
#include "ObjectTrackerYolo.h"

/*! \file Controller.h
*	\brief Contains Controller class.
*
* This file contains the Controller class.
* It is used to controll all image tracking
* steps.
*/

/*! \brief The image_tracking namespace contains all classes used for image tracking.*/
namespace image_tracking
{
	/*! \brief This class is used to controll image tracking steps.*/
	class Controller
	{
	public:
		/*! \brief Default constructor.*/
		Controller();
		/*! \brief Process image.*/
		void ProcessImage(dto::Image& image, dto::Camera& camera);
	private:
		ObjectTracker ot;
		ObjectTrackerYolo ot_yolo;
		feature_extraction::Controller feature_extraction_controller;
	};
}
