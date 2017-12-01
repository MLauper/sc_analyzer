#include "PersonDetectorHog.h"
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/videostab/ring_buffer.hpp>
#include <iostream>
#include <opencv2/videostab/global_motion.hpp>

image_segmentation::PersonDetectorHog::PersonDetectorHog()
{
	cv::namedWindow("PersonDetector");
}

void image_segmentation::PersonDetectorHog::detectPerson(cv::cuda::GpuMat d_image)
{
	int win_width = 48;
	int win_stride_width = 8;
	int win_stride_height = 8;
	int block_width = 16;
	int block_stride_width = 8;
	int block_stride_height = 8;
	int cell_width = 8;
	int nbins = 9;

	cv::Size win_size(win_width, win_width * 2);
	cv::Size block_size(block_width, block_width);
	cv::Size block_stride(block_stride_width, block_stride_height);
	cv::Size cell_size(cell_width, cell_width);

	cv::Ptr<cv::cuda::HOG> gpu_hog = cv::cuda::HOG::create(win_size, block_size, block_stride, cell_size, nbins);
	cv::Mat detector = gpu_hog->getDefaultPeopleDetector();
	gpu_hog->setSVMDetector(detector);

	std::vector<cv::Rect> found;

	double scale = 1.05;
	int nlevels = 13;
	int gr_threshold = 8;
	double hit_threshold = 1.4;

	cv::Size win_stride(win_stride_width, win_stride_height);

	gpu_hog->setNumLevels(nlevels);
	gpu_hog->setHitThreshold(hit_threshold);
	gpu_hog->setWinStride(win_stride);
	gpu_hog->setScaleFactor(scale);
	gpu_hog->setGroupThreshold(gr_threshold);

	int width = 640;
	int height = 480;

	cv::Mat temp;
	d_image.download(temp);
	resize(temp, temp, cv::Size(width, height));
	d_image.upload(temp);

	cv::cuda::cvtColor(d_image, d_image, cv::COLOR_BGR2BGRA);

	try
	{
		gpu_hog->detectMultiScale(d_image, found);
	}
	catch (std::exception ex)
	{
		std::cerr << ex.what();
	}

	std::cout << "Found: " << found.size() << " Persons...\n";
	cv::Mat img_to_show;
	d_image.download(img_to_show);

	// Draw positive classified windows
	for (size_t i = 0; i < found.size(); i++)
	{
		cv::Rect r = found[i];
		rectangle(img_to_show, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);
	}

	putText(img_to_show, "Mode: GPU", cv::Point(5, 25), cv::FONT_HERSHEY_SIMPLEX, 1., cv::Scalar(255, 100, 0), 2);

	imshow("PersonDetector", img_to_show);
}
