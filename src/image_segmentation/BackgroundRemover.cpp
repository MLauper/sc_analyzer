#include "BackgroundRemover.h"
#include <opencv2/cudalegacy.hpp>
#include "../dto/Image.h"
#include "../dto/Configuration.h"
#include "../dto/Camera.h"

image_segmentation::BackgroundRemover::BackgroundRemover(dto::Camera& camera)
{
	//create Background Subtractor objects
	this->fps = camera.fps;
	this->gpuMOG2 = cv::cuda::createBackgroundSubtractorMOG2(fps * 20, camera.backgroundThreshold, true);

	//cv::namedWindow("Frame");
	//cv::namedWindow("FG Mask");
	//cv::namedWindow("FG Image");
	//namedWindow("Contours", cv::WINDOW_AUTOSIZE);
}

void image_segmentation::BackgroundRemover::removeBackground(dto::Image& image, dto::Camera& camera)
{
	//read the first file of the sequence
	//image.cv_image_original = cv::imread(image.path);

	if (dto::Configuration::SAVE_ORIGINAL_IMAGES)
	{
		std::stringstream image_out_path;
		image_out_path << dto::Configuration::ORIGINAL_IMAGES_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix
			<< "\\" << image.filename << "_original.jpg";
		imwrite(image_out_path.str().c_str(), image.cv_image_original);
	}

	if (dto::Configuration::SHOW_ORIGINAL_IMAGES)
	{
		imshow("Original", image.cv_image_original);
		cv::waitKey(1);
	}

	image.cv_gpu_image.upload(image.cv_image_original);
	this->gpuMOG2->apply(image.cv_gpu_image, image.cv_gpu_fgmask);


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

	//this->d_fgmask.download(this->fgmask);

	//imshow("FG Mask MOG 2", this->fgmask);

	// Witkey is required, otherwise all imshow will not work

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

	image.cv_gpu_fgmask.download(image.cv_fgmask);

	// Blur the foreground mask to reduce the effect of noise and false positives
	blur(image.cv_fgmask, image.cv_fgmask, cv::Size(15, 15), cv::Point(-1, -1));

	// Remove the shadow parts and the noise
	threshold(image.cv_fgmask, image.cv_fgmask, 128, 255, cv::THRESH_BINARY);

	int erosion_type = cv::MORPH_ELLIPSE;
	int erosion_size = 3;
	cv::Mat element = getStructuringElement(erosion_type,
	                                        cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
	                                        cv::Point(erosion_size, erosion_size));
	dilate(image.cv_fgmask, image.cv_fgmask, element);

	image.cv_gpu_fgmask.upload(image.cv_fgmask);

	image.cv_fgimg.create(image.cv_gpu_image.size(), image.cv_gpu_image.type());
	image.cv_fgimg.setTo(cv::Scalar::all(0));
	image.cv_gpu_image.copyTo(image.cv_gpu_fgimg, image.cv_gpu_fgmask);

	image.cv_gpu_fgimg.download(image.cv_fgimg);

	//	std::stringstream image_out_path;
	//	image_out_path << "c:\\temp\\\extracted_persons\\" << filename << "_diff.jpg";
	//	imwrite(image_out_path.str().c_str(), fgimg);

	if (dto::Configuration::SAVE_FG_IMAGES)
	{
		std::stringstream image_out_path;
		image_out_path << dto::Configuration::FG_IMAGES_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix << "\\"
			<< image.filename << "_FG.jpg";
		imwrite(image_out_path.str().c_str(), image.cv_fgimg);
	}
	if (dto::Configuration::SAVE_FG_MASK)
	{
		std::stringstream image_out_path;
		image_out_path << dto::Configuration::FG_MASKS_DIRECTORY << image.filename << "_FG_MASK.jpg";
		imwrite(image_out_path.str().c_str(), image.cv_fgmask);
	}

	if (dto::Configuration::SHOW_FG_IMAGES)
	{
		imshow("FG Images", image.cv_fgimg);
		cv::waitKey(1);
	}
	if (dto::Configuration::SHOW_FG_MASKS)
	{
		imshow("FG Mask", image.cv_fgmask);
		cv::waitKey(1);
	}
}
