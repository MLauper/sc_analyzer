#include "JPGFileLoader.h"
#include <tchar.h>
#include <filesystem>
#include <arrayfire.h>
#include <strsafe.h>
#include "dto/Image.h"
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

	while (true)
	{
		delete this->cv_videoCapture;
		this->cv_videoCapture = new cv::VideoCapture(this->camera.rtspConnectionString);

		if (!this->cv_videoCapture->read(image.cv_image_original))
		{
			std::cerr << "RTSP stream is not open. Retry...\n";
			continue;
		}

		imshow("Original", image.cv_image_original);

		personDetector.detectPersonsYolo(image, this->camera);

		cv::waitKey(1);
	}
}

image_acquisition::RTSPImageCapture::RTSPImageCapture(dto::Camera& camera,
                                                      image_segmentation::Controller* segmentation_controller)
{
	this->camera = camera;
	this->frameNumber = 0;

	this->cv_videoCapture = new cv::VideoCapture(this->camera.rtspConnectionString);

	this->segmentation_controller = segmentation_controller;
}

image_acquisition::RTSPImageCapture::~RTSPImageCapture()
{
}
