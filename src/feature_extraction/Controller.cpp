#include "Controller.h"
#include "../dto/Configuration.h"

feature_extraction::Controller::Controller()
{
	this->personsInTheRoom = 0;
}

void feature_extraction::Controller::processTrack(dto::Track& track, dto::Camera& camera)
{
	if (dto::Configuration::SAVE_TRACK_STATISTICS)
	{
		std::ofstream fileStream;
		std::stringstream filePath;
		filePath << dto::Configuration::STATISTICS_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix << "\\" << "Track-" << track.trackId << "_statistics.txt";
		fileStream.open(filePath.str().c_str(), std::fstream::app);
		
		fileStream << "Yolo Bounding Boxes" << std::endl;
		fileStream << "x: ";
		for (auto& bb : track.persons)
		{
			fileStream << bb.x << "; ";
		}
		fileStream << std::endl << "y: ";
		for (auto& bb : track.persons)
		{
			fileStream << bb.y << "; ";
		}
		fileStream << std::endl << "w: ";
		for (auto& bb : track.persons)
		{
			fileStream << bb.w << "; ";
		}
		fileStream << std::endl << "h: ";
		for (auto& bb : track.persons)
		{
			fileStream << bb.h << "; ";
		}
		fileStream << std::endl;

		fileStream.close();
	}

	direction_extractor_.extractDirection(track, camera);
	frame_selector_.SelectFrame(track, camera);
	if (track.optimalPersonId != -1) {
		frame_selector_.SaveRegion(track, camera);
		body_part_extractor_.extractBodyParts(track, camera);
		color_extractor_.extractPrimaryColors(track, camera);
		size_extractor_.extractBodySizes(track, camera);
		feature_point_extractor_.extractFeaturePoints(track, camera);

		if (dto::Configuration::STORE_TRACK_RESULTS_IN_DB){
			track_persistor_.persistTrack(track, camera);
		}
	}

	//if (track.walkingDirection == dto::Track::out_in)
	//{
	//	std::cout << "=============PERSON ENTERED ROOM============" << "\n" << "PERSON COUNT: " << ++this->personsInTheRoom << "\n";
	//} else if (track.walkingDirection == dto::Track::in_out)
	//{
	//	std::cout << "=============PERSON LEFT ROOM============" << "\n" << "PERSON COUNT: " << --this->personsInTheRoom << "\n";
	//}


}

