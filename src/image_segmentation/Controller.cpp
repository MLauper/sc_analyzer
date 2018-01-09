#include "Controller.h"
#include <fstream>
#include "BackgroundRemover.h"
#undef min
#undef max

image_segmentation::Controller::Controller(dto::Camera& camera): current_image_time(nullptr)
{
	this->camera = camera;
	this->br = new BackgroundRemover(camera);
	this->imageWidth = camera.width;
	this->imageHeight = camera.height;
	this->numImagePixels = imageHeight * imageWidth;

	this->current_image = new af::array{};
	this->current_image_grayscale = new af::array{};
	this->background = new af::array{};
	this->mask = new af::array{};

	//this->minPixelChangedPercentageThreshold = 0.005f;
	//this->maxPixelChangedPercentageThreshold = 0.10f;
	//this->minPixelDeltaValue = 45;
	this->minPixelChangedPercentageThreshold = 0.0f;
	this->maxPixelChangedPercentageThreshold = 1.0f;
	this->minPixelDeltaValue = 45;

	this->morphMask = af::constant(1, 3, 3);
	this->morphMask2 = af::constant(1, 19, 19);

	this->stat_out = std::ofstream("c:\\temp\\\extracted_persons\\_statistics.txt");
}

void image_segmentation::Controller::ProcessImage(dto::Image& image)
{
	//std::cout << "Controller is processing data\n";

	br->removeBackground(image, camera);
	pd.extractPersonContours(image, camera);
	pd.detectPersonsYolo(image, camera);

	//std::vector<std::vector<cv::Point>> contours = pd.extractPersonContours(temp_image, temp_mask, image.filename);
	this->image_tracking_controller.ProcessImage(image, camera);
}

void image_segmentation::Controller::ProcessImage(SYSTEMTIME* time, af::array& image, std::string path,
                                                  std::string filename)
{
	std::cout << "___________________------------______________\n";

	//	br.removeBackground(path, filename);
	//	cv::cuda::GpuMat d_img = br.getFGImage();
	//	cv::cuda::GpuMat d_fgmask = br.getFGMask();
	//
	//	cv::Mat temp_image;
	//	d_img.download(temp_image);
	//
	//	cv::Mat temp_mask;
	//	d_fgmask.download(temp_mask);
	//
	//	std::vector<std::vector<cv::Point>> contours = pd.extractPersonContours(temp_image, temp_mask, filename);
	//	this->image_tracking_controller.ProcessImage(d_img, d_fgmask, contours);
	//pd.detectPerson(d_img);


	//	this->current_image = image;
	//	this->current_image_time = time;
	//	
	//	// create grayscale and hsv image
	//	this->current_image_grayscale = af::colorSpace(this->current_image, AF_GRAY, AF_RGB);
	//	// HSV is between 0 and 1, Grayscale is between 0 and 255, multiply by 255
	//	//this->current_image_hsv = af::colorSpace(this->current_image, AF_HSV, AF_RGB)*255;
	//	//this->current_image_grayscale = this->current_image_hsv(af::span, af::span, 0);
	//	
	//	//af::saveImage("c:\\temp\\out_hue_only.jpg", this->current_image_hsv(af::span, af::span, 0));
	//
	//	// promote first image to background
	//	if (this->background.dims(0) == 0)
	//	{
	//		this->background = this->current_image_grayscale;
	//	}
	//
	//	//af::saveImage("c:\\temp\\out_background.jpg", this->background);
	//
	//	// remove background and do binary threshold to generate mask
	//	this->mask = af::abs(current_image_grayscale - background);
	//
	//	//stat_out << filename << ";";
	//	//float * h_mask = this->mask.host<float>();
	//	//int count[256] = {0};
	//	//for (int i = 0; i < this->imageWidth*this->imageHeight; i++)
	//	//{
	//	//	int index = (int)h_mask[i];
	//	//	count[index]++;
	//	//}
	//	//for (int i = 0; i < 256; i++)
	//	//{
	//	//	stat_out << count[i] << ";";
	//	//}
	//	//stat_out << std::endl;
	//	////stat_out.close();
	//
	//	this->mask = this->binThreshold(this->mask, this->minPixelDeltaValue);
	//	//std::stringstream image_out_path;
	//	//image_out_path << "c:\\temp\\\extracted_persons\\" << filename << "_diff.jpg";
	//	//af::saveImage(image_out_path.str().c_str(), this->mask);
	//	// erode and dilate to remove noise
	//	this->mask = af::erode(this->mask, this->morphMask);
	//	this->mask = af::dilate(this->mask, this->morphMask);
	//	// dilate and erode to close holes
	//	// Workaround: Dilate and Erode in CUDA only support maks of up to 19x19
	//	// Running them twice will increase their size
	//	this->mask = af::dilate(this->mask, this->morphMask2);
	//	this->mask = af::dilate(this->mask, this->morphMask2);
	//	this->mask = af::erode(this->mask, this->morphMask2);
	//	this->mask = af::erode(this->mask, this->morphMask2);
	//	
	//	this->mask = af::dilate(this->mask, this->morphMask2);
	//
	//	// Retrieve number of changed pixels
	//	af::array pixelChanged = af::sum(af::sum(this->mask));
	//	float * h_pixelChanged = pixelChanged.host<float>();
	//	//printf("Number of Pixels changed: %f\n", *h_pixelChanged);
	//	//printf("Number of Pixels: %f\n", (float)this->numImagePixels);
	//	//printf("Min Changed Pixels: %f\n", (float)this->numImagePixels * this->minPixelChangedPercentageThreshold);
	//	//printf("Max Changed Pixels: %f\n", (float)this->numImagePixels * this->maxPixelChangedPercentageThreshold);
	//
	//	if (h_pixelChanged[0] > this->numImagePixels * minPixelChangedPercentageThreshold && h_pixelChanged[0] < this->numImagePixels * maxPixelChangedPercentageThreshold) {
	//		printf("DETECTED PERSON\n");
	//		//af_print(this->mask);
	//		af::saveImage("c:\\temp\\out_mask.jpg", this->mask);
	//		af::array extractedImage_gray = this->mask * this->current_image_grayscale;
	//		af::array extractedImage = af::gray2rgb(this->mask) * this->current_image;
	//		af::saveImage("c:\\temp\\extractedPerson_gray.jpg", extractedImage_gray);
	//		af::saveImage("c:\\temp\\extractedPerson.jpg", extractedImage);
	//
	//		//af::array rgb_max = af::max(af::max(extractedImage));
	//		//af_print(rgb_max);
	//
	//		// Copy max of mask to cpu to find start and end points
	//		af::array max_0 = af::max(this->mask, 0); // Y-Axis
	//		float* h_max_0 = max_0.host<float>(); // Y-Axis
	//		af::array max_1 = af::max(this->mask, 1); // X-Axis
	//		float* h_max_1 = max_1.host<float>(); // X-Axis
	//
	//		//// Print mask max values
	//		//for (int i = 0; i < this->imageWidth; i++) std::cout << h_max_0[i]; // Y-Axis
	//		//std::cout << "\n\n";
	//		//for (int i = 0; i < this->imageHeight; i++)	std::cout << h_max_1[i]; // X-Axis
	//		
	//		// Find min Y-Value on mask
	//		int min_y_index = 0;
	//		for (int i = 0; i < this->imageWidth; i++)
	//		{
	//			if (h_max_0[i] > 0)
	//			{
	//				min_y_index = i;
	//				break;
	//			}
	//		}
	//
	//		// Find max Y-Value on mask
	//		int max_y_index = this->imageWidth-1;
	//		for (int i = min_y_index; i < this->imageWidth; i++)
	//		{
	//			if (h_max_0[i] < 1.0f)
	//			{
	//				max_y_index = i;
	//				break;
	//			}
	//		}
	//
	//		// Find min X-Value on mask
	//		int min_x_index = 0;
	//		for (int i = 0; i < this->imageHeight; i++)
	//		{
	//			if (h_max_1[i] > 0)
	//			{
	//				min_x_index = i;
	//				break;
	//			}
	//		}
	//
	//		// Find max X-Value on mask
	//		int max_x_index = this->imageHeight-1;
	//		for (int i = min_x_index; i < this->imageHeight; i++)
	//		{
	//			if (h_max_1[i] < 1.0f)
	//			{
	//				max_x_index = i;
	//				break;
	//			}
	//		}
	//
	//		// Print bb mask
	//		printf("mask start bb is %ix%i\n", min_y_index, min_x_index);
	//		printf("mask end bb is %ix%i\n", max_y_index, max_x_index);
	//
	//		// Draw bonding box of extracted person
	//		// Array Adressing is (X-Axis, Y-Axis, R/G/B)
	//		af::array extractedImage_modified = af::gray2rgb(this->mask) * this->current_image;
	//
	//		std::stringstream image_out_unmodified_path;
	//		image_out_unmodified_path << "c:\\temp\\\extracted_persons\\" << filename << "_extractedPerson.jpg";
	//		af::saveImage(image_out_unmodified_path.str().c_str(), extractedImage_modified);
	//
	//		// Top Line
	//		extractedImage_modified(min_x_index, af::seq(min_y_index, max_y_index, 1.f), 0) = 255.0f;
	//		extractedImage_modified(min_x_index, af::seq(min_y_index, max_y_index, 1.f), 1) = 255.0f;
	//		extractedImage_modified(min_x_index, af::seq(min_y_index, max_y_index, 1.f), 2) = 255.0f;
	//		// Bottom Line
	//		extractedImage_modified(max_x_index, af::seq(min_y_index, max_y_index, 1.f), 0) = 255.0f;
	//		extractedImage_modified(max_x_index, af::seq(min_y_index, max_y_index, 1.f), 1) = 255.0f;
	//		extractedImage_modified(max_x_index, af::seq(min_y_index, max_y_index, 1.f), 2) = 255.0f;
	//		// Left Line
	//		extractedImage_modified(af::seq(min_x_index, max_x_index, 1.f), min_y_index, 0) = 255.0f;
	//		extractedImage_modified(af::seq(min_x_index, max_x_index, 1.f), min_y_index, 1) = 255.0f;
	//		extractedImage_modified(af::seq(min_x_index, max_x_index, 1.f), min_y_index, 2) = 255.0f;
	//		// Right Line
	//		extractedImage_modified(af::seq(min_x_index, max_x_index, 1.f), max_y_index, 0) = 255.0f;
	//		extractedImage_modified(af::seq(min_x_index, max_x_index, 1.f), max_y_index, 1) = 255.0f;
	//		extractedImage_modified(af::seq(min_x_index, max_x_index, 1.f), max_y_index, 2) = 255.0f;
	//
	//		std::stringstream image_out_path;
	//		image_out_path << "c:\\temp\\\extracted_persons\\" << filename << "_extractedPerson_modified.jpg";
	//		af::saveImage(image_out_path.str().c_str(), extractedImage_modified);
	//		
	//		//std::stringstream metadata_out_path;
	//		//metadata_out_path << "c:\\temp\\\extracted_persons\\" << filename << "_metadata.txt";
	//		//std::ofstream metadata_out(metadata_out_path.str().c_str());
	//		//metadata_out << "Filename: " << filename << std::endl;
	//		//metadata_out << "Timestamp: " << this->current_image_time << std::endl;
	//		//metadata_out << "Number of Pixels changed : " << *h_pixelChanged << std::endl;
	//		//metadata_out << "Number of Pixels: " << (float)this->numImagePixels << std::endl;
	//		//metadata_out << "Min Changed Pixels: " << (float)this->numImagePixels * this->minPixelChangedPercentageThreshold << std::endl;
	//		//metadata_out << "Max Changed Pixels: " << (float)this->numImagePixels * this->maxPixelChangedPercentageThreshold << std::endl;
	//		//metadata_out.close();
	//	} else
	//	{
	//		this->background = this->current_image_grayscale;
	//
	//		//std::stringstream image_out_path;
	//		//image_out_path << "c:\\temp\\\extracted_persons\\" << filename << "_new_mask.jpg";
	//		//af::saveImage(image_out_path.str().c_str(), this->background);
	//
	//		//std::stringstream metadata_out_path;
	//		//metadata_out_path << "c:\\temp\\\extracted_persons\\" << filename << "_metadata.txt";
	//		//std::ofstream metadata_out(metadata_out_path.str().c_str());
	//		//metadata_out << "Filename: " << filename << std::endl;
	//		//metadata_out << "Timestamp: " << this->current_image_time << std::endl;
	//		//metadata_out << "Number of Pixels changed : " << *h_pixelChanged << std::endl;
	//		//metadata_out << "Number of Pixels: " << (float)this->numImagePixels << std::endl;
	//		//metadata_out << "Min Changed Pixels: " << (float)this->numImagePixels * this->minPixelChangedPercentageThreshold << std::endl;
	//		//metadata_out << "Max Changed Pixels: " << (float)this->numImagePixels * this->maxPixelChangedPercentageThreshold << std::endl;
	//		//metadata_out.close();
	//	}
	//	
	//	// Cleanup memory
	//	//delete[] host_sum;
	//	//free(*this->current_image_time);
	//	//af::free(this->current_image);
	//	//af::free(this->current_image_grayscale);
}

af::array image_segmentation::Controller::binThreshold(const af::array& in, const float thresholdValue)
{
	af::array ret_val = in.copy();
	ret_val = (ret_val < thresholdValue) * 0.0f + 1.0f * (ret_val > thresholdValue);
	return ret_val;
}
