#include "BackgroundRemover.h"
#include <opencv2/cudalegacy.hpp>

image_segmentation::BackgroundRemover::BackgroundRemover()
{
	//create Background Subtractor objects
	this->gpuMOG2 = cv::cuda::createBackgroundSubtractorMOG2(20, 16, false);

	cv::namedWindow("Frame");
	cv::namedWindow("FG Mask MOG 2");
}

void image_segmentation::BackgroundRemover::removeBackground(std::string image_path)
{
	//read the first file of the sequence
	this->frame = cv::imread(image_path);

	this->d_frame.upload(this->frame);
	this->gpuMOG2->apply(d_frame, d_fgmask);

	this->keyboard = 0;
	//update the background model
	//this->pMOG2->apply(this->frame, this->fgMaskMOG2);
	//get the frame number and write it on the current frame
	//size_t index = fn.find_last_of("\\");
	//size_t index2 = fn.find_last_of(".");
	//std::string prefix = fn.substr(0, index + 1);
	//std::string suffix = fn.substr(index2);
	//std::string frameNumberString = fn.substr(index + 1, index2 - index - 1);
	//std::istringstream iss(frameNumberString);
	//int frameNumber = 0;
	//iss >> frameNumber;
	//rectangle(this->frame, cv::Point(10, 2), cv::Point(100, 20),
	//          cv::Scalar(255, 255, 255), -1);
	//putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
	//        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
	//show the current frame and the fg masks
	
	this->d_fgmask.download(this->fgmask);
	imshow("Frame", this->frame);
	imshow("FG Mask MOG 2", this->fgmask);

	//get the input from the keyboard
	this->keyboard = (char)cv::waitKey(100);
	//search for the next image in the sequence
	std::ostringstream oss;
	//oss << (frameNumber + 1);
	//std::string nextFrameNumberString = oss.str();
	//std::string nextFrameFilename = prefix + nextFrameNumberString + suffix;
	//read the next frame
	//this->frame = cv::imread("D:\\data\\room_walk_with_different_lighting\\image\\out_CAMERA_door_0000000002.jpg");
	//if (this->frame.empty())
	//{
	//	//error in opening the next image in the sequence
	//	std::cerr << "Unable to open image frame: " << "fefe" << std::endl;
	//	exit(EXIT_FAILURE);
	//}
	//update the path of the current frame
	//fn.assign(nextFrameFilename);
}
