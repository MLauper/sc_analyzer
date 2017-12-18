#include "Controller.h"
#include "../dto/Configuration.h"

feature_extraction::Controller::Controller()
{
	this->personsInTheRoom = 0;
}

void feature_extraction::Controller::processTrack(dto::Track& track, dto::Camera& camera)
{
	//de.extractDirection(track, camera);
	//rs.SelectRegion(track, camera);

	if (dto::Configuration::SAVE_YOLO_TRACK_BB)
	{
		for (auto& i : track.images)
		{
			 
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

