#include "ColorExtractor.h"
#include "../dto/Configuration.h"


feature_extraction::ColorExtractor::ColorExtractor()
{
}


feature_extraction::ColorExtractor::~ColorExtractor()
{
}


void feature_extraction::ColorExtractor::extractMaxHue(dto::Track& track, dto::Camera& camera, cv::Mat& hsv_image, int& maxBucketId, bool isUpperBody)
{
	cv::Mat generatedImage;

	if (dto::Configuration::SAVE_HUE_IMAGE) {
		generatedImage = hsv_image.clone();
	}
	const int numOfBuckets = 16;
	std::vector<int> hueBuckets(numOfBuckets);

	// Count Pixels
	for (int i = 0; i < hsv_image.rows; i++) {
		for (int j = 0; j < hsv_image.cols; j++) {
			if (hsv_image.at<cv::Vec3b>(i, j)[2] > 1) {
				hueBuckets.at(static_cast<int>(hsv_image.at<cv::Vec3b>(i, j)[0] / numOfBuckets)) += 1;
				if (dto::Configuration::SAVE_HUE_IMAGE) {
					generatedImage.at<cv::Vec3b>(i, j)[0] = static_cast<int>(hsv_image.at<cv::Vec3b>(i, j)[0] / numOfBuckets) * 16;
					generatedImage.at<cv::Vec3b>(i, j)[1] = 180;
					generatedImage.at<cv::Vec3b>(i, j)[2] = 255;
				}
			} else
			{
				if (dto::Configuration::SAVE_HUE_IMAGE) {
					generatedImage.at<cv::Vec3b>(i, j)[0] = 0;
					generatedImage.at<cv::Vec3b>(i, j)[1] = 0;
					generatedImage.at<cv::Vec3b>(i, j)[2] = 0;
				}
			}
		}
	}
	
	if (dto::Configuration::SAVE_HUE_IMAGE) {
		cv::Mat generatedRGB;
		cvtColor(generatedImage, generatedRGB, CV_HSV2BGR);

		std::stringstream image_out_path;
		if (isUpperBody) {
			image_out_path << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix << "\\" << "Track-" << track.trackId << "_hue_upperBody.jpg";
		} else
		{
			image_out_path << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix << "\\" << "Track-" << track.trackId << "_hue_lowerBody.jpg";
		}
		cv::imwrite(image_out_path.str().c_str(), generatedRGB);
	}

	if (dto::Configuration::PRINT_HSV_VALUES) {
		std::cout << "HUE: " << std::endl;
		for (int i = 0; i < hsv_image.rows; i++) {
			for (int j = 0; j < hsv_image.cols; j++) {
				std::cout << static_cast<int>(hsv_image.at<cv::Vec3b>(i, j)[0]) << "; ";
			}
		}
		std::cout << std::endl;
		std::cout << "VALUE: " << std::endl;
		for (int i = 0; i < hsv_image.rows; i++) {
			for (int j = 0; j < hsv_image.cols; j++) {
				std::cout << static_cast<int>(hsv_image.at<cv::Vec3b>(i, j)[2]) << "; ";
			}
		}
		std::cout << std::endl;
	}

	maxBucketId = 0;
	int maxBucketValue = 0;
	for (int i = 0; i < hueBuckets.size(); i++)
	{
		if (hueBuckets.at(i) > maxBucketValue)
		{
			maxBucketId = i;
			maxBucketValue = hueBuckets.at(i);
		}
	}
}

void feature_extraction::ColorExtractor::extractPrimaryColors(dto::Track& track, dto::Camera& camera)
{
	// Convert image to HSV
	cv::Mat hsv_upperBody;
	cvtColor(track.cv_optimalPersonBodyParts.upperBody, hsv_upperBody, CV_BGR2HSV);
	cv::Mat hsv_lowerBody;
	cvtColor(track.cv_optimalPersonBodyParts.lowerBody, hsv_lowerBody, CV_BGR2HSV);

	
	extractMaxHue(track, camera, hsv_upperBody, track.primary_color_ids.upperBody, true);
	extractMaxHue(track, camera, hsv_lowerBody, track.primary_color_ids.lowerBody, false);


	if (dto::Configuration::SAVE_TRACK_STATISTICS)
	{
		std::ofstream fileStream;
		std::stringstream filePath;
		filePath << dto::Configuration::STATISTICS_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix << "\\" << "Track-" << track.trackId << "_statistics.txt";
		fileStream.open(filePath.str().c_str(), std::fstream::app);

		fileStream << "Primary Colors:" << std::endl;
		fileStream << " Upper Body: " << track.primary_color_ids.upperBody << std::endl;
		fileStream << " Lower Body: " << track.primary_color_ids.lowerBody << std::endl;
		
		fileStream.close();
	}
}
