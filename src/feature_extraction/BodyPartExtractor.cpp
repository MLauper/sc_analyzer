#include "BodyPartExtractor.h"
#include "../dto/Configuration.h"


feature_extraction::BodyPartExtractor::BodyPartExtractor()
{
}


feature_extraction::BodyPartExtractor::~BodyPartExtractor()
{
}

void feature_extraction::BodyPartExtractor::extractBodyParts(dto::Track& track, dto::Camera& camera)
{
	auto& optimalImage = track.cv_optimalPersonCut;

	auto h = optimalImage.size().height;

	// Extract head
	const auto headStart = 0;
	const auto headEnd = static_cast<int>(h * 0.16);
	track.cv_optimalPersonBodyParts.head = optimalImage(cv::Range(headStart, headEnd), cv::Range::all());
	if (dto::Configuration::SAVE_BODY_PARTS_IMAGES)
	{
		std::stringstream image_out_path;
		image_out_path << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "Track-" << track.trackId << "_optimalImage_head.jpg";
		cv::imwrite(image_out_path.str().c_str(), track.cv_optimalPersonBodyParts.head);
	}

	// Extract upper body
	const auto upperBodyStart = static_cast<int>(h * 0.22);
	const auto upperBodyEnd = static_cast<int>(h * 0.50);
	track.cv_optimalPersonBodyParts.upperBody = optimalImage(cv::Range(upperBodyStart, upperBodyEnd), cv::Range::all());
	if (dto::Configuration::SAVE_BODY_PARTS_IMAGES)
	{
		std::stringstream image_out_path;
		image_out_path << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "Track-" << track.trackId << "_optimalImage_upperBody.jpg";
		cv::imwrite(image_out_path.str().c_str(), track.cv_optimalPersonBodyParts.upperBody);
	}

	// Extract lower body
	const auto lowerBodyStart = static_cast<int>(h * 0.55);
	const auto lowerBodyEnd = static_cast<int>(h * 0.75);
	track.cv_optimalPersonBodyParts.lowerBody = optimalImage(cv::Range(lowerBodyStart, lowerBodyEnd), cv::Range::all());
	if (dto::Configuration::SAVE_BODY_PARTS_IMAGES)
	{
		std::stringstream image_out_path;
		image_out_path << dto::Configuration::OPTIMAL_TRACK_DIRECTORY << "Track-" << track.trackId << "_optimalImage_lowerBody.jpg";
		cv::imwrite(image_out_path.str().c_str(), track.cv_optimalPersonBodyParts.lowerBody);
	}

	if (dto::Configuration::SAVE_TRACK_STATISTICS)
	{
		std::ofstream fileStream;
		std::stringstream filePath;
		filePath << dto::Configuration::STATISTICS_DIRECTORY << "Track-" << track.trackId << "_statistics.txt";
		fileStream.open(filePath.str().c_str(), std::fstream::app);

		fileStream << "Body Parts:" << std::endl;
		fileStream << " Head from " << headStart << " to " << headEnd << std::endl;
		fileStream << " Upper Body from " << upperBodyStart << " to " << upperBodyEnd << std::endl;
		fileStream << " Lower Body from " << lowerBodyStart << " to " << lowerBodyEnd<< std::endl;

		fileStream.close();
	}
}
