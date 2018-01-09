#include "SizeExtractor.h"
#include "../dto/Configuration.h"


feature_extraction::SizeExtractor::SizeExtractor()
{
}


feature_extraction::SizeExtractor::~SizeExtractor()
{
}

void feature_extraction::SizeExtractor::extractBodySizes(dto::Track& track, dto::Camera camera)
{
	// Calculate values based on calibration value
	track.estimatedPersonSize.height = track.persons.at(track.optimalPersonId).h * camera.pixelToCentimeterRatio;
	track.estimatedPersonSize.width = track.persons.at(track.optimalPersonId).w * camera.pixelToCentimeterRatio;

	if (dto::Configuration::SAVE_TRACK_STATISTICS)
	{
		std::ofstream fileStream;
		std::stringstream filePath;
		filePath << dto::Configuration::STATISTICS_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix << "\\" <<
			"Track-" << track.trackId << "_statistics.txt";
		fileStream.open(filePath.str().c_str(), std::fstream::app);

		fileStream << "Estimated Body Sizes:" << std::endl;
		fileStream << " Height: " << track.estimatedPersonSize.height << " cm" << std::endl;
		fileStream << " Width: " << track.estimatedPersonSize.width << " cm" << std::endl;

		fileStream.close();
	}
}
