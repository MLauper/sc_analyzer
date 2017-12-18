#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <arrayfire.h>
#include "Region.h"
#include <yolo/yolo_v2_class.hpp>

namespace dto
{
	struct Image
	{
	public:
		std::string path;
		std::string filename;
		af::array af_image_color;
		SYSTEMTIME filetime;

		cv::Mat cv_image_original;
		cv::Mat cv_image_distorted;
		cv::cuda::GpuMat cv_gpu_image;
		cv::Mat cv_fgmask;
		cv::cuda::GpuMat cv_gpu_fgmask;
		cv::Mat cv_fgimg;
		cv::cuda::GpuMat cv_gpu_fgimg;
		cv::Mat cv_bgimg;
		cv::cuda::GpuMat cv_gpu_bgimg;

		std::vector<dto::Region> regions;

		std::vector<bbox_t> yoloPersons;

	};
}
