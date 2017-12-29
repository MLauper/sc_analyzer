#include "TrackPersistor.h"
#include "../dto/SQLHelper.h"


feature_extraction::TrackPersistor::TrackPersistor()
{
	
}


feature_extraction::TrackPersistor::~TrackPersistor()
{
}

void feature_extraction::TrackPersistor::persistTrack(dto::Track& track, dto::Camera& camera)
{
	this->sql_helper.persist_track(track, camera);
}
