#include "BackgroundRemover.h"
#include <opencv2/cudalegacy.hpp>

image_segmentation::BackgroundRemover::BackgroundRemover()
{
	//create Background Subtractor objects
	this->gpuMOG2 = cv::cuda::createBackgroundSubtractorMOG2(20, 16, true);

	cv::namedWindow("Frame");
	cv::namedWindow("FG Mask");
	cv::namedWindow("FG Image");
	namedWindow("Contours", cv::WINDOW_AUTOSIZE);
}

cv::cuda::GpuMat image_segmentation::BackgroundRemover::removeBackground(std::string image_path, std::string filename)
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
	//imshow("FG Mask MOG 2", this->fgmask);

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

	d_fgmask.download(fgmask);

	// Blur the foreground mask to reduce the effect of noise and false positives
	cv::blur(fgmask, fgmask, cv::Size(15, 15), cv::Point(-1, -1));

	// Remove the shadow parts and the noise
	cv::threshold(fgmask, fgmask, 128, 255, cv::THRESH_BINARY);

	int erosion_type = cv::MORPH_ELLIPSE;
	int erosion_size = 3;
	cv::Mat element = getStructuringElement(erosion_type,
	                                        cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
	                                        cv::Point(erosion_size, erosion_size));
	cv::dilate(fgmask, fgmask, element);

	d_fgmask.upload(fgmask);

	d_fgimg.create(d_frame.size(), d_frame.type());
	d_fgimg.setTo(cv::Scalar::all(0));
	d_frame.copyTo(d_fgimg, d_fgmask);

	d_fgimg.download(fgimg);

	std::stringstream image_out_path;
	image_out_path << "c:\\temp\\\extracted_persons\\" << filename << "_diff.jpg";
	cv::imwrite(image_out_path.str().c_str(), fgimg);

	imshow("FG Image", this->fgimg);
	imshow("FG Mask", this->fgmask);

	cv::Mat canny_output;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	int thresh = 100;
	cv::Canny(this->fgmask, canny_output, thresh, thresh * 2, 3);
	cv::findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	cv::RNG rng(12345);
	for (size_t i = 0; i< contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		cv::drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, cv::Point());
	}
	cv::imshow("Contours", drawing);
	std::cout << "Found " << contours.size() << " contours.\n";

	return this->d_frame;
}
