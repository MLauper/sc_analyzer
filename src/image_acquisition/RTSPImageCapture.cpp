#include "JPGFileLoader.h"
#include <string>
#include <bemapiset.h>
#include <tchar.h>
#include <filesystem>
#include <arrayfire.h>
#include <strsafe.h>
#include <set>
#include "dto/Image.h"
#include "dto/Configuration.h"
#include "RTSPImageCapture.h"

namespace fs = std::experimental::filesystem;

void image_acquisition::RTSPImageCapture::startCapturing()
{
		dto::Image image;
		image_segmentation::PersonDetector personDetector;

		image.path = "rtsp:\\";
		image.filename = this->frameNumber++;

		//image.filetime = ;

		cv::namedWindow("Original", CV_WINDOW_AUTOSIZE);

		while (true) {
			delete this->cv_videoCapture;
			this->cv_videoCapture = new cv::VideoCapture(this->camera.rtspConnectionString);

			if (!this->cv_videoCapture->read(image.cv_image_original)) {
				std::cerr << "RTSP stream is not open. Retry...\n";
				continue;
			}

			cv::imshow("Original", image.cv_image_original);
			
			personDetector.detectPersonsYolo(image);

			cv::waitKey(1);
		}

		image.cv_image_original = cv::imread(image.path);

		if (dto::Configuration::CREATE_DISTORTED_IMAGE)
		{
			cv::remap(image.cv_image_original, image.cv_image_distorted, this->dist_map1, this->dist_map2, cv::INTER_LINEAR);
			if (dto::Configuration::SHOW_DISTORTED_IMAGE)
			{
				cv::imshow("DistortedImage", image.cv_image_distorted);
				cv::waitKey(1);
			}

			if (dto::Configuration::SAVE_DISTORTED_IMAGE)
			{
				std::stringstream image_out_path;
				image_out_path << dto::Configuration::ORIGINAL_IMAGES_DIRECTORY << image.filename << "_distorted.jpg";
				cv::imwrite(image_out_path.str().c_str(), image.cv_image_distorted);
			}
		}

		//Process image
		this->segmentation_controller->ProcessImage(image);
}

image_acquisition::RTSPImageCapture::RTSPImageCapture(dto::Camera& camera, image_segmentation::Controller* segmentation_controller)
{
	this->camera = camera;
	this->frameNumber = 0;

	this->cv_videoCapture = new cv::VideoCapture(this->camera.rtspConnectionString);

	this->segmentation_controller = segmentation_controller;

	if (dto::Configuration::CREATE_DISTORTED_IMAGE)
	{
		initUndistortRectifyMap(
			camera.cameraMatrix, camera.distCoeffs, cv::Mat(),
			cv::getOptimalNewCameraMatrix(camera.cameraMatrix, camera.distCoeffs, cv::Size(camera.width, camera.height), 1, cv::Size(camera.width, camera.height), 0), cv::Size(camera.width, camera.height),
			CV_16SC2, this->dist_map1, this->dist_map2);
	}
}

image_acquisition::RTSPImageCapture::~RTSPImageCapture()
{
}

