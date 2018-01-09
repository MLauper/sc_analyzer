#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"
#include "../dto/SQLHelper.h"

namespace feature_extraction
{
	class TrackPersistor
	{
	public:
		TrackPersistor();
		~TrackPersistor();

		void persistTrack(dto::Track& track, dto::Camera& camera);

	private:
		dto::SQLHelper sql_helper;
	};
}
