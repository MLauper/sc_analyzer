#pragma once
#include "../dto/Image.h"
#include <yolo/yolo_v2_class.hpp>
#include "../dto/Camera.h"

/*! \file PersonDetector.h
*	\brief Contains PersonDetector class.
*
* This file contains the PersonDetector class.
* It is used to detect persons based on yolo.
*/

/*! \brief The image_segmentation namespace contains all classes used for image segmentation.*/
namespace image_segmentation
{
	/*! \brief This class is used to detect persons on an image.*/
	class PersonDetector
	{
	public:
		/*! \brief Default constructor.*/
		PersonDetector();
		/*! \brief Extract the contours of all persons.*/
		void extractPersonContours(dto::Image& Image, dto::Camera& camera) const;
		/*! \brief Extract person bounding boxes using Yolo.*/
		void detectPersonsYolo(dto::Image& Image, dto::Camera& camera) const;
	private:
		int minRegionWidth;
		int minRegionHeight;
		int maxRegionWidth;
		int maxRegionHeight;
		float minRatio;
		float maxRatio;
		int imageHeight;
		int imageWidth;
		int minRegionX;
		int maxRegionX;
		int minRegionY;
		int maxRegionY;
		Detector* yoloDetector;
	};
}
