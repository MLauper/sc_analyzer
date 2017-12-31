#include "MKVFileLoader.h"
#include <opencv2/highgui.hpp>
#include "dto/Configuration.h"


image_acquisition::MKVFileLoader::MKVFileLoader(dto::Camera& camera, image_segmentation::Controller* segmentation_controller)
{
	this->camera = camera;
	this->segmentation_controller = segmentation_controller;
}


image_acquisition::MKVFileLoader::~MKVFileLoader()
{
}

void image_acquisition::MKVFileLoader::process_file()
{
	cv::VideoCapture capture(this->camera.videoFilePath.c_str());
	cv::Mat frame;

	int numOfSkippedFrames = static_cast<int>((24/camera.fps)) - 1;

	if (!capture.isOpened())
		std::cerr << "Error when reading from File: " << camera.videoFilePath.c_str() << std::endl;
	
	int i = 0;
	for (; ; )
	{
		dto::Image image;
		
		bool isImageEmpty = false;
		for (int j = 0; j < numOfSkippedFrames; j++) {
			capture >> image.cv_image_original;
			if (image.cv_image_original.empty())
				isImageEmpty = true;
		}
		if (isImageEmpty) break;

		std::stringstream image_path_stream;
		image_path_stream << camera.directory << camera.prefix << "frame-" << i++ << ".jpg";
		image.path = image_path_stream.str();
		image.filename = this->extract_filename(image.path.c_str());
		
		this->segmentation_controller->ProcessImage(image);
	}

	for (int j = 0; j < dto::Configuration::MAX_NUMBER_OF_MISSING_FRAMES_IN_TRACK; j++)
	{
		dto::Image image;

		image.cv_image_original = cv::Mat(camera.height, camera.width, CV_8UC3, cv::Scalar(0, 0, 0));

		std::stringstream image_path_stream;
		image_path_stream << camera.directory << camera.prefix << "frame-" << i++ << ".jpg";
		image.path = image_path_stream.str();
		image.filename = this->extract_filename(image.path.c_str());

		this->segmentation_controller->ProcessImage(image);
	}
	
}

std::string image_acquisition::MKVFileLoader::extract_filename(char const* path_c)
{
	const std::set<char> delimiters = { '\\' };

	std::vector<std::string> ret;

	char const* start = path_c;
	for (; *path_c; ++path_c)
	{
		if (delimiters.find(*path_c) != delimiters.end())
		{
			if (start != path_c)
			{
				std::string str(start, path_c);
				ret.push_back(str);
			}
			else
			{
				ret.push_back("");
			}
			start = path_c + 1;
		}
	}
	ret.push_back(start);

	return ret.back();
}