#pragma once
#include <string>
#include "opencv2/core.hpp"

namespace dto
{
	struct Camera
	{
		enum side
		{
			left,
			right,
			top,
			bottom,
			none
		};
		enum gateMode
		{
			minLeft,
			minRight,
			minTop,
			minBottom
		};
	public:
		std::string directory;
		std::string prefix;
		side entrance_side;
		int width;
		int height;
		int fps;

		gateMode gateMode;
		int gateValue;
		double backgroundThreshold;

		cv::Mat cameraMatrix;
		cv::Mat distCoeffs;

		std::string rtspConnectionString;
		std::string urlConnectionString;
		std::string urlUsername;
		std::string urlPassword;
	};
}