#pragma once
#include <string>
#include "opencv2/core.hpp"

/*! \file Camera.h
*	\brief Contains DTO for the camera
*
* This file contains the camera dto with all required properties.
*/

/*! \brief The dto namespace contains all DTOs*/
namespace dto
{
	/*! \brief The camera dto cotnains all camera properties.*/
	struct Camera
	{
		/*! \brief This enum lists the available options for the entry side.*/
		enum entrySide
		{
			/*! \brief The person enters the room from the left.*/
			entry_left,
			/*! \brief The person enters the room from the right*/
			entry_right,
			/*! \brief The person enters the room from top.*/
			entry_top,
			/*! \brief The person enters the room from bottom.*/
			entry_bottom,
			/*! \brief There is no entry side.*/
			none
		};

		/*! \brief The mode a gate operates in.*/
		enum gateMode
		{
			/*! \brief Measure based on distance from the left side.*/
			minLeft,
			/*! \brief Measure based on distance from the rigth side.*/
			minRight,
			/*! \brief Measure based on distance from top.*/
			minTop,
			/*! \brief Measure based on distance from bottom.*/
			minBottom
		};

		/*! \brief Configure how persons should be counted.*/
		enum personCountUpWhen
		{
			/*! \brief Count persons up if they are recognized in the room and go to entry side.*/
			in_to_entry,
			/*! \brief Count persons up if they enter thorugh entry side and go in the room.*/
			entry_to_in
		};

		/*! \brief Directory where the input files are.*/
		std::string directory;
		/*! \brief Prefix of the input files.*/
		std::string prefix;

		/*! \brief Unique scene identifier*/
		int scene;

		/*! \brief Entry side where persons enter or leave the room.*/
		entrySide entry_side;
		/*! \brief How to count persons.*/
		personCountUpWhen personCountMode;
		/*! \brief Image width.*/
		int width;
		/*! \brief Image height.*/
		int height;
		/*! \brief Frames per second of the camera.*/
		int fps;

		/*! \brief Configured gate mode.*/
		gateMode gateMode;
		/*! \brief Configured second gate mode.*/
		enum gateMode secondGateMode;
		/*! \brief Number of pixels where the gate is.*/
		int gateValue;
		/*! \brief Number of pixels where the second gate is.*/
		int secondGateValue;
		/*! \brief Optimal person location.*/
		cv::Point optimalPersonLocation;
		/*! \brief Threshold to subtract the background.*/
		double backgroundThreshold;

		/*! \brief Camera matrix for calibration.*/
		cv::Mat cameraMatrix;
		/*! \brief Distortion Coefficients for calibration.*/
		cv::Mat distCoeffs;

		/*! \brief Pixel to Centimeter ratio.*/
		double pixelToCentimeterRatio;

		/*! \brief If RTPS is used, the connection string.*/
		std::string rtspConnectionString;
		/*! \brief If Image URL is used, the url to the image.*/
		std::string urlConnectionString;
		/*! \brief If Image URL is used, the username for authentication.*/
		std::string urlUsername;
		/*! \brief If Image URL is used, the password for authentication.*/
		std::string urlPassword;

		/*! \brief If video file is used, the path to the video file.*/
		std::string videoFilePath;
	};
}
