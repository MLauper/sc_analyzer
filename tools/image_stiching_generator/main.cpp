#define NOMINMAX

#include <iostream>
#include "opencv2/core/core.hpp"
#include <opencv2/stitching.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

int main()
{
	std::cout << "Starting Image Stiching Generator" << std::endl;

	std::vector<cv::Mat> inputImages;
	auto inputImagesPaths = {
		"C:\\Temp\\stiching\\out_CAMERA_door_0000000000.mkv_snapshot_09.56.jpg",
		"C:\\Temp\\stiching\\out_CAMERA_center_0000000000.mkv_snapshot_10.06.jpg",
		"C:\\Temp\\stiching\\out_CAMERA_room_0000000000.mkv_snapshot_10.12.jpg",
	};

	for (const auto& path : inputImagesPaths)
	{
		inputImages.push_back(cv::imread(path));
	}

	bool try_use_gpu = false;
	bool divide_images = false;
	cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;
	std::string result_name = "c:\\temp\\stiching\\result.jpg";

	cv::Mat pano;
	cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode, try_use_gpu);

	cv::Stitcher::Status status = stitcher->stitch(inputImages, pano);

	if (status != cv::Stitcher::OK)
	{
		std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
		return -1;
	}
	imwrite(result_name, pano);
	std::cout << "stitching completed successfully\n" << result_name << " saved!";
}
