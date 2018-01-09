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
		void testSQLConnection() const;
		void persist_camera(const Camera& camera) const;
		void retrieve_camera(Camera& camera, const char* directory, const char* prefix, const char* scene) const;
		void persist_track(const Track& track, const Camera& camera) const;
		void persist_persons(std::vector<Person>& persons) const;
		void retrieve_camera(Camera& camera, int camera_id) const;
		std::vector<Track> retrieve_all_tracks() const;
		void backup_database(char* destination) const;
	private:
		nanodbc::connection* conn;
	};
}
