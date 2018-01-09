#pragma once
#include "../../external/nanodbc/nanodbc.h"
#include "Camera.h"
#include "Track.h"
#include "Person.h"

namespace dto
{
	class SQLHelper
	{
	public:

		SQLHelper();
		~SQLHelper();
		void testSQLConnection();
		void persist_camera(const Camera& camera);
		void retrieve_camera(Camera& camera, const char* directory, const char* prefix, const char* scene);
		void persist_track(const Track& track, const Camera& camera);
		void persist_persons(std::vector<Person>& persons);
		void retrieve_camera(Camera& camera, int camera_id);
		std::vector<Track> retrieve_all_tracks();
		void backup_database(char* destination);
	private:
		nanodbc::connection* conn;
	};
}
