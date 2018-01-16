#pragma once
#include "../../external/nanodbc/nanodbc.h"
#include "Camera.h"
#include "Track.h"
#include "Person.h"

/*! \file SQLHelper.h
*	\brief Contains a helper class for SQL opoerations.
*
* This file contains helper functions to persist and read objects from the central database.
* It uses ODBC connection to access the database.
*/

/*! \brief The dto namespace contains all DTOs*/
namespace dto
{
	/*! \brief The SQLHelper class contains all helper functions and manages the connection state.*/
	class SQLHelper
	{
	public:
		/*! \brief Constructor of the SQLHelper class.*/
		SQLHelper();
		/*! \brief Destructor of the SQLHelper class.*/
		~SQLHelper();
		/*! \brief Test function to check SQL server connectivity.*/
		void testSQLConnection() const;
		/*! \brief Persists a camera in the database.*/
		void persist_camera(const Camera& camera) const;
		/*! \brief Reads camera from the database.*/
		void retrieve_camera(Camera& camera, const char* directory, const char* prefix, const char* scene) const;
		/*! \brief Persist a track on the database.*/
		void persist_track(const Track& track, const Camera& camera) const;
		/*! \brief Persist a person on the database.*/
		void persist_persons(std::vector<Person>& persons) const;
		/*! \brief Retrieve a camera from the database.*/
		void retrieve_camera(Camera& camera, int camera_id) const;
		/*! \brief Retrieve all tracks from the database.*/
		std::vector<Track> retrieve_all_tracks() const;
		/*! \brief Backup the database on the server.*/
		void backup_database(char* destination) const;
	private:
		/*! \brief The database connection object.*/
		nanodbc::connection* conn;
	};
}
