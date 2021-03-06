#include "PersonDetector.h"
#include "yolo/yolo_v2_class.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>
#include "../dto/Image.h"
#include "../dto/Region.h"
#include "../dto/Configuration.h"


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

	std::stringstream yoloConfigPath;
	yoloConfigPath << getenv("YOLO_PATH") << dto::Configuration::yoloConfig;

	std::stringstream yoloWeightsPath;
	yoloWeightsPath << getenv("YOLO_PATH") << dto::Configuration::yoloWeights;

	this->yoloDetector = new Detector(yoloConfigPath.str().c_str(), yoloWeightsPath.str().c_str(), 0);
}

void image_segmentation::PersonDetector::extractPersonContours(
	dto::Image& image, dto::Camera& camera) const
{
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	try
	{
		findContours(image.cv_fgmask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	}
	catch (std::exception e)
	{
		std::cerr << e.what();
	}

	for (auto i = 0; i < contours.size(); i++)
	{
		auto minX = INT_MAX, maxX = 0, minY = INT_MAX, maxY = 0;
		for (const auto p : contours.at(i))
		{
			if (p.x < minX) minX = p.x;
			if (p.x > maxX) maxX = p.x;
			if (p.y < minY) minY = p.y;
			if (p.y > maxY) maxY = p.y;
		}
		const auto height = maxY - minY;
		const auto width = maxX - minX;
		const auto ratio = static_cast<float>(width) / static_cast<float>(height);

		//std::cout << "CONTOUR: " << width << "x" << height << ", ration: " << ratio << "\n";

		if (width < this->minRegionWidth) continue;
		if (width > this->maxRegionWidth) continue;
		if (minX < this->minRegionX) continue;
		if (maxX > this->maxRegionX) continue;
		if (minY < this->minRegionY) continue;
		if (maxY > this->maxRegionY) continue;

		if (minY != 0 && maxY != imageHeight)
		{
			if (height < this->minRegionHeight) continue;
			if (height > this->maxRegionHeight) continue;
			if (ratio < this->minRatio) continue;
			if (ratio > this->maxRatio) continue;
		}
		else
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

	if (dto::Configuration::SAVE_ALL_CONTOURS)
	{
		// New Image
		// cv::Mat drawingAll = cv::Mat::zeros(image.cv_fgmask.size(), CV_8UC3);
		// Overlay
		cv::Mat drawingAll = image.cv_image_original.clone();

		cv::RNG rng(12345);
		for (size_t i = 0; i < contours.size(); i++)
		{
			const auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawingAll, contours, static_cast<int>(i), color, 2, 8, hierarchy, 0, cv::Point());
		}

		if (dto::Configuration::SAVE_ALL_CONTOURS)
		{
			std::stringstream image_out_path;
			image_out_path << dto::Configuration::ALL_CONTOURS_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix <<
				"\\" << image.filename << "_all_contours.jpg";
			imwrite(image_out_path.str().c_str(), drawingAll);
		}
	}

	if (dto::Configuration::SAVE_CONTOUR_IMAGES)
	{
		// Blank image
		// cv::Mat drawingAll = cv::Mat::zeros(image.cv_fgmask.size(), CV_8UC3);
		// Overlay image
		cv::Mat drawingAll = image.cv_image_original.clone();
		cv::RNG rng(12345);
		for (size_t i = 0; i < image.regions.size(); i++)
		{
			const auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

			drawContours(drawingAll, std::vector<std::vector<cv::Point>>(1, image.regions.at(i).contour), 0, color, 2, 8, 0, 0,
			             cv::Point());
		}

		if (dto::Configuration::SAVE_CONTOUR_IMAGES)
		{
			std::stringstream image_out_path;
			image_out_path << dto::Configuration::CONTOUR_IMAGES_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix
				<< "\\" << image.filename << "_accepted_contours.jpg";
			imwrite(image_out_path.str().c_str(), drawingAll);
		}
	}

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

void image_segmentation::PersonDetector::detectPersonsYolo(dto::Image& Image, dto::Camera& camera) const
{
	std::vector<bbox_t> yoloObjects = this->yoloDetector->detect(Image.cv_image_original, 0.3f, false);

	for (auto& obj : yoloObjects)
	{
		if (obj.obj_id == dto::Configuration::yoloPersonObjectId)
		{
			Image.yoloPersons.push_back(obj);
		}
	}

	if (dto::Configuration::SHOW_YOLO_PERSONS_IMAGES)
	{
		auto drawingAll = Image.cv_image_original.clone();
		cv::RNG rng(12345);
		for (auto& person : Image.yoloPersons)
		{
			const auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

			rectangle(drawingAll, cv::Point(person.x, person.y), cv::Point(person.x + person.w, person.y + person.h), color, 3);
		}

		if (dto::Configuration::SHOW_YOLO_PERSONS_IMAGES)
		{
			imshow("Yolo Persons", drawingAll);
			cv::waitKey(1);
		}

		if (dto::Configuration::SAVE_YOLO_PERSONS_IMAGES)
		{
			std::stringstream image_out_path;
			image_out_path << dto::Configuration::YOLO_PERSONS_IMAGES_DIRECTORY << "scene-" << camera.scene << "\\" << camera.
				prefix << "\\" << Image.filename << "_yolo_persons.jpg";
			imwrite(image_out_path.str().c_str(), drawingAll);
		}
	}
}
