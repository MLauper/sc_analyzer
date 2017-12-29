#pragma once
#include <string>
#include "opencv2/core.hpp"

namespace dto
{
	struct Camera
	{
		enum entrySide
		{
			entry_left,
			entry_right,
			entry_top,
			entry_bottom,
			none
		};
		enum gateMode
		{
			minLeft,
			minRight,
			minTop,
			minBottom
		};
		enum personCountUpWhen
		{
			in_to_entry,
			entry_to_in
		};
	public:
		std::string directory;
		std::string prefix;
		entrySide entry_side;
		personCountUpWhen personCountMode;
		int width;
		int height;
		int fps;

		gateMode gateMode;
		enum gateMode secondGateMode;
		int gateValue;
		int secondGateValue;
		cv::Point optimalPersonLocation;
		double backgroundThreshold;

		cv::Mat cameraMatrix;
		cv::Mat distCoeffs;

		double pixelToCentimeterRatio;

		std::string rtspConnectionString;
		std::string urlConnectionString;
		std::string urlUsername;
		std::string urlPassword;

		std::string videoFilePath;
	};
}