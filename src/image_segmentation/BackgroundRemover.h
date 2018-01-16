#pragma once
//#include <string>
//#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
//#include <opencv2/core/utility.hpp>
//#include <opencv2/core/cuda.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/cudabgsegm.hpp>
#include "../dto/Image.h"
#include "../dto/Camera.h"

/*! \file BackgroundRemover.h
*	\brief Contains BackgroundRemover class.
*
* This file contains the BackgroundRemover class.
* It is used to remove the image background.
*/

/*! \brief The image_segmentation namespace contains all classes used for image segmentation.*/
namespace image_segmentation
{
	/*! \brief This class is used to remove image background.*/
	class BackgroundRemover
	{
	public:
		/*! \brief Constructor.*/
		explicit BackgroundRemover(dto::Camera& camera);
		/*! \brief Remove background from image.*/
		void removeBackground(dto::Image& image, dto::Camera& camera) const;
	private:

		cv::Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
		cv::Ptr<cv::BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
		char keyboard; //input from keyboard
		cv::Ptr<cv::cuda::BackgroundSubtractorMOG2> gpuMOG2;

		int fps;
	};
}
