#pragma once
#include <opencv2/core.hpp>

namespace image_segmentation
{
	class PersonDetectorHog
	{
	public:
		PersonDetectorHog();
		void detectPerson(cv::cuda::GpuMat image) const;
	private:

		cv::Mat frame;
	};
}
