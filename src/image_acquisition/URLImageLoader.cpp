#include "FileLoader.h"
#include <string>
#include <bemapiset.h>
#include <tchar.h>
#include <filesystem>
#include <arrayfire.h>
#include <strsafe.h>
#include <set>
#include "dto/Image.h"
#include "dto/Configuration.h"
#include "URLImageLoader.h"
#include "curl/curl.h"

namespace fs = std::experimental::filesystem;

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	std::vector<uchar> *stream = (std::vector<uchar>*)userdata;
	size_t count = size * nmemb;
	stream->insert(stream->end(), ptr, ptr + count);
	return count;
}

//function to retrieve the image as cv::Mat data type
cv::Mat curlImg(const char *img_url, const char *username, const char *password, int timeout = 100)
{
	std::vector<uchar> stream;
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, img_url); //the img url
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // pass the writefunction
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream); // pass the stream ptr to the writefunction
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout); // timeout if curl_easy hangs, 
	curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST); // timeout if curl_easy hangs
	curl_easy_setopt(curl, CURLOPT_USERNAME, username);
	curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

	CURLcode res = curl_easy_perform(curl); // start curl

	curl_easy_cleanup(curl); // cleanup
	return cv::imdecode(stream, -1); // 'keep-as-is'
}


static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t retcode;
	curl_off_t nread;

	memcpy(ptr, stream, sizeof(int));
	retcode = sizeof(int);

	nread = (curl_off_t)retcode;

	std::cout << "Source as int is: " << *(int*)stream << std::endl;
	std::cout << "Destination as int is: " << *(int*)ptr << std::endl;

	fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
		" bytes from data\n", nread);

	return retcode;
}	

void image_acquisition::URLImageLoader::publishResults(int data)
{
	std::stringstream publish_command;
	publish_command << "powershell.exe -NoProfile -NoLogo -Command \"Invoke-WebRequest -Method Put -Uri '" << dto::Configuration::PERSON_COUNTER_0_PUBLISH_URL << "' -Body \"" << data << "\" | Out-Null\"";
	system(publish_command.str().c_str());

	/*
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, dto::Configuration::PERSON_COUNTER_0_PUBLISH_URL.c_str());

	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl, CURLOPT_PUT, 1L);

	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
	curl_easy_setopt(curl, CURLOPT_READDATA, &data);

	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)sizeof(int));

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	CURLcode res = curl_easy_perform(curl);

	std::cout << "Curl returned: " << res << std::endl;

	curl_easy_cleanup(curl);
	*/
}

void image_acquisition::URLImageLoader::startCapturing()
{
	dto::Image image;

	image_segmentation::PersonDetector personDetector;

	image.path = "http:\\";
	image.filename = this->frameNumber++;

	//image.filetime = ;

	cv::namedWindow("Original", CV_WINDOW_AUTOSIZE);

	while (true) {
		image.yoloPersons.clear();

		image.cv_image_original = curlImg(camera.urlConnectionString.c_str(), camera.urlUsername.c_str(), camera.urlPassword.c_str());
		if (image.cv_image_original.empty())
		{
			std::cerr << "Failed to load image, retry...\n";
			continue;
		}

		cv::imshow("Original", image.cv_image_original);

		personDetector.detectPersonsYolo(image);

		std::cout << "Seeing " << image.yoloPersons.size() << " Persons in the current image." << std::endl;

		publishResults(image.yoloPersons.size());

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

image_acquisition::URLImageLoader::URLImageLoader(dto::Camera& camera, image_segmentation::Controller* segmentation_controller)
{
	this->camera = camera;
	this->frameNumber = 0;

	this->segmentation_controller = segmentation_controller;

	if (dto::Configuration::CREATE_DISTORTED_IMAGE)
	{
		initUndistortRectifyMap(
			camera.cameraMatrix, camera.distCoeffs, cv::Mat(),
			cv::getOptimalNewCameraMatrix(camera.cameraMatrix, camera.distCoeffs, cv::Size(camera.width, camera.height), 1, cv::Size(camera.width, camera.height), 0), cv::Size(camera.width, camera.height),
			CV_16SC2, this->dist_map1, this->dist_map2);
	}
}

image_acquisition::URLImageLoader::~URLImageLoader()
{
}

