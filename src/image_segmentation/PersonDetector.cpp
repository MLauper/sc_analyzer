#include "PersonDetector.h"
#include <iostream>
#include "opencv2/objdetect.hpp"
#include <opencv2/videostab/ring_buffer.hpp>
#include <opencv2/highgui.hpp>
#include "../dto/Image.h"
#include "../dto/Region.h"

image_segmentation::PersonDetector::PersonDetector()
{
	this->imageHeight = 1080;
	this->imageWidth = 1920;

	this->minRegionX = 100;
	this->maxRegionX = 1080;
	this->minRegionY = 100;
	this->maxRegionY = 1820;

	this->minRegionWidth = 80;
	this->minRegionHeight = 300;

	this->maxRegionWidth = 360;
	this->maxRegionHeight = 600;

	// Width / Height
	this->minRatio = 0.3f;
	this->maxRatio = 0.625f;


}

void image_segmentation::PersonDetector::extractPersonContours(
	dto::Image& image)
{
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	try {
		cv::findContours(image.cv_fgmask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	}
	catch (std::exception e)
	{
		std::cerr << e.what();
	}

	for (int i = 0; i < contours.size(); i++)
	{
		int minX = INT_MAX, maxX = 0, minY = INT_MAX , maxY = 0;
		for (cv::Point p : contours.at(i))
		{
			if (p.x < minX) minX = p.x;
			if (p.x > maxX) maxX = p.x;
			if (p.y < minY) minY = p.y;
			if (p.y > maxY) maxY = p.y;
		}
		int height = maxY - minY;
		int width = maxX - minX;
		float ratio = (float)width / (float)height;

		//std::cout << "CONTOUR: " << width << "x" << height << ", ration: " << ratio << "\n";

		if (width < this->minRegionWidth) continue;
		if (width > this->maxRegionWidth) continue;
		if (minX < this->minRegionX) continue;
		if (maxX > this->maxRegionX) continue;
		if (minY < this->minRegionY) continue;
		if (maxY > this->maxRegionY) continue;

		if (minY != 0 && maxY != imageHeight) {
			if (height < this->minRegionHeight) continue;
			if (height > this->maxRegionHeight) continue;
			if (ratio < this->minRatio) continue;
			if (ratio > this->maxRatio) continue;
		} else
		{
			if (height < minRegionHeight / 3) continue;
			if (height > maxRegionHeight) continue;
		}

		dto::Region region;
		region.contour = contours.at(i);
		region.minX = minX;
		region.minY = minY;
		region.maxX = maxX;
		region.maxY = maxY;
		region.ratio = ratio;

		image.regions.push_back(region);
	}

	//cv::Mat drawingAll = cv::Mat::zeros(image.cv_fgmask.size(), CV_8UC3);
	//cv::RNG rng(12345);
	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	cv::drawContours(drawingAll, contours, (int)i, color, 2, 8, hierarchy, 0, cv::Point());
	//}
	//cv::imshow("Contours", drawingAll);
	//std::cout << "Found " << contours.size() << " contours.\n";
	//
	//if (contours.size() > 0) {
	//	std::stringstream image_out_path;
	//	//image_out_path << "c:\\temp\\\extracted_persons\\" << filename << "_contoursAll.jpg";
	//	//cv::imwrite(image_out_path.str().c_str(), drawingAll);
	//}
	//
	//cv::Mat drawing = cv::Mat::zeros(image.cv_fgmask.size(), CV_8UC3);
	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	try {
	//		cv::drawContours(drawing, correctContours, (int)i, color, 2, 8, hierarchy, 0, cv::Point());
	//	}
	//	catch (std::exception e)
	//	{
	//		std::cerr << e.what();
	//	}
	//}
	//cv::imshow("PassedContours", drawing);
	//std::cout << "Found " << correctContours.size() << " passed contours.\n";
	//
	//if (correctContours.size() > 0) {
	//	std::stringstream image_out_path_passed;
	//	image_out_path_passed << "c:\\temp\\\extracted_persons\\" << filename << "_contoursPassed.jpg";
	//	cv::imwrite(image_out_path_passed.str().c_str(), drawing);
	//}

}