#pragma once
#include "../../external/nanodbc/nanodbc.h"
#include "Camera.h"
#include "Track.h"

namespace dto {
	class SQLHelper
	{
	public:
		
		SQLHelper();
		~SQLHelper();
		void testSQLConnection();
		void persist_camera(const Camera& camera);
		void retrieve_camera(Camera& camera, const char* directory, const char* prefix);
		void persist_track(const Track& track, const Camera& camera);
		void retrieve_camera(dto::Camera& camera, int camera_id);
		std::vector<dto::Track> retrieve_all_tracks();
	private:
		nanodbc::connection* conn;
	};
}
