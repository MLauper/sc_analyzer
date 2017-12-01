#pragma once
#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>

namespace image_segmentation
{
	class PersonDetectorHog
	{
	public:
		PersonDetectorHog();
		void detectPerson(cv::cuda::GpuMat image);
	private:

		cv::Mat frame;
	};
}
