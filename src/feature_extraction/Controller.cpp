#include "Controller.h"

feature_extraction::Controller::Controller()
{

}

void feature_extraction::Controller::processTrack(dto::Track& track, dto::Camera& camera)
{
	de.extractDirection(track, camera);

}

