#pragma once
#include <string>
#include <opencv2/opencv.hpp>
//#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
//#include <opencv2/video.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
#include <arrayfire.h>
#include "Region.h"
#include <yolo/yolo_v2_class.hpp>

/*! \file Image.h
*	\brief Contains DTO for the Image
*
* This file contains the image dto with all required properties.
*/

/*! \brief The dto namespace contains all DTOs*/
namespace dto
{
	/*! \brief The image dto struct cotnains all image properties.*/
	struct Image
	{
		/*! \brief The path to the original image.*/
		std::string path;
		/*! \brief The filename of the original image.*/
		std::string filename;
		/*! \brief The ArrayFire representation of the image.*/
		af::array af_image_color;
		/*! \brief The time the image was recorded.*/
		SYSTEMTIME filetime;

		/*! \brief The OpenCV representation of the original image.*/
		cv::Mat cv_image_original;
		/*! \brief The OpenCV representation of the distorted image.*/
		cv::Mat cv_image_distorted;
		/*! \brief The OpenCV representation of the image with local contrast correction,.*/
		cv::Mat cv_image_high_contrast;
		/*! \brief The OpenCV cuda representation of the image.*/
		cv::cuda::GpuMat cv_gpu_image;
		/*! \brief The foreground mask.*/
		cv::Mat cv_fgmask;
		/*! \brief The OpenCV cuda representation of the foreground mask.*/
		cv::cuda::GpuMat cv_gpu_fgmask;
		/*! \brief The OpenCV representation of the foreground image.*/
		cv::Mat cv_fgimg;
		/*! \brief The OpenCV cuda representation of the foreground image.*/
		cv::cuda::GpuMat cv_gpu_fgimg;
		/*! \brief The OpenCV repesentation of background image.*/
		cv::Mat cv_bgimg;
		/*! \brief The OpenCV cuda representation of the background image.*/
		cv::cuda::GpuMat cv_gpu_bgimg;

		/*! \brief The person regions found in that image.*/
		std::vector<Region> regions;

		/*! \brief The yolo bounding boxes found in that image.*/
		std::vector<bbox_t> yoloPersons;
	};
}
