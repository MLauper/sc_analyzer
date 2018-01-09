#include "SQLHelper.h"

#include "../../external/nanodbc/nanodbc.h"
#include <iostream>
#include "../dto/Configuration.h"
#include "../dto/Track.h"
#include "../dto/Person.h"

dto::SQLHelper::SQLHelper()
{
	this->conn = new nanodbc::connection(Configuration::DATABASE_ODBC_NAME, Configuration::DATABASE_USER,
	                                     Configuration::DATABASE_PASSWORD, 5);
}


dto::SQLHelper::~SQLHelper()
{
}

void dto::SQLHelper::testSQLConnection()
{
	try
	{
		std::cout << "Start testSQLConnection" << std::endl;

		std::cout << "Connecting to DB..." << std::endl;


		std::cout << "DB Information:" << std::endl;
		std::cout << conn->driver_name() << std::endl;
		std::cout << conn->dbms_name() << std::endl;

		std::cout << "Retrieving data from DB: " << std::endl;

		nanodbc::result row = execute(*conn, NANODBC_TEXT("SELECT id, directory, prefix FROM cameras"));

		for (int i = 1; row.next(); ++i)
		{
			std::cout << i << ": " << row.get<std::string>("directory") << std::endl;
		}
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "ERROR" << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

void dto::SQLHelper::persist_camera(const Camera& camera)
{
	std::string select_query;
	std::string insert_query;
	nanodbc::result result;

	try
	{
		// Check if camera is already in database
		std::ostringstream select_query_stream;
		select_query_stream << "SELECT id FROM cameras"
			<< " WHERE directory='" << camera.directory << "'"
			<< " AND prefix='" << camera.prefix << "'"
			<< " AND scene='" << camera.scene << "'";

		select_query = select_query_stream.str();

		result = execute(*conn, select_query);
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << select_query << std::endl;
		std::cerr << e.what() << std::endl;
		return;
	}

	try
	{
		// Insert camera if not already in database
		if (!result.next())
		{
			std::ostringstream instert_query_stream;
			instert_query_stream << "INSERT INTO cameras"
				<< "(scene, directory, prefix, width, height, fps, pixelToCentimeterRatio) "
				<< "VALUES ('" << camera.scene << "'"
				<< ",'" << camera.directory << "'"
				<< ",'" << camera.prefix << "'"
				<< ",'" << camera.width << "'"
				<< ",'" << camera.height << "'"
				<< ",'" << camera.fps << "'"
				<< ",'" << camera.pixelToCentimeterRatio << "'"
				<< ")";

			insert_query = instert_query_stream.str();

			execute(*conn, insert_query);
		}
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << insert_query << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

void dto::SQLHelper::retrieve_camera(Camera& camera, const char* directory, const char* prefix, const char* scene)
{
	std::string select_query;
	nanodbc::result result;

	try
	{
		// Retrieve camera
		std::ostringstream select_query_stream;
		select_query_stream << "SELECT * FROM cameras"
			<< " WHERE directory='" << directory << "'"
			<< " AND prefix='" << prefix << "'"
			<< " AND scene='" << scene << "'";

		select_query = select_query_stream.str();

		result = execute(*conn, select_query);

		if (result.rowset_size() != 1)
		{
			std::cerr << "Camera could not be retrieved. Query: " << select_query << std::endl;
		}
		else
		{
			result.next();
			camera.scene = result.get<int>("scene");
			camera.directory = result.get<std::string>("directory");
			camera.prefix = result.get<std::string>("prefix");
			camera.width = result.get<int>("width");
			camera.height = result.get<int>("height");
			camera.fps = result.get<int>("fps");
			camera.pixelToCentimeterRatio = result.get<float>("pixelToCentimeterRatio");
		}
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << select_query << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

void dto::SQLHelper::persist_track(const Track& track, const Camera& camera)
{
	// Ensure that camera is persisted
	this->persist_camera(camera);

	std::string select_camera_query;
	std::string insert_track_query;
	std::string select_track_query;
	nanodbc::result result;

	int camera_id, track_id;

	try
	{
		// Retrieve camera id
		std::ostringstream select_query_stream;
		select_query_stream << "SELECT id FROM cameras"
			<< " WHERE directory='" << camera.directory << "'"
			<< " AND prefix='" << camera.prefix << "'"
			<< " AND scene='" << camera.scene << "'";

		select_camera_query = select_query_stream.str();

		result = execute(*conn, select_camera_query);

		if (result.next())
		{
			camera_id = result.get<int>("id");
		}
		else
		{
			std::cerr << "Camera could not retrieved from DB. Query: " << select_camera_query << std::endl;
		}
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << select_camera_query << std::endl;
		std::cerr << e.what() << std::endl;
		return;
	}

	// Insert Track
	try
	{
		std::ostringstream instert_query_stream;

		std::string walkingDirection;
		if (track.walkingDirection == Track::WalkingDirection::in_in) walkingDirection = "in_in";
		else if (track.walkingDirection == Track::WalkingDirection::in_out) walkingDirection = "in_out";
		else if (track.walkingDirection == Track::WalkingDirection::out_in) walkingDirection = "out_in";
		else if (track.walkingDirection == Track::WalkingDirection::out_out) walkingDirection = "out_out";
		else walkingDirection = "";

		instert_query_stream << "INSERT INTO tracks"
			<< "(camera_id, trackId, walkingDirection, primaryColor_UpperBody, primaryColor_LowerBody, personSize_height, personSize_width) "
			<< "VALUES ('" << camera_id << "'"
			<< ",'" << track.trackId << "'"
			<< ",'" << walkingDirection << "'"
			<< ",'" << track.primary_color_ids.upperBody << "'"
			<< ",'" << track.primary_color_ids.lowerBody << "'"
			<< ",'" << track.estimatedPersonSize.height << "'"
			<< ",'" << track.estimatedPersonSize.width << "'"
			<< ")";

		insert_track_query = instert_query_stream.str();

		execute(*conn, insert_track_query);
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << insert_track_query << std::endl;
		std::cerr << e.what() << std::endl;
	}

	// Retrieve track id
	try
	{
		std::ostringstream select_query_stream;
		select_query_stream << "SELECT id FROM tracks"
			<< " WHERE trackId='" << track.trackId << "'"
			<< " AND camera_id='" << camera_id << "'";

		select_track_query = select_query_stream.str();

		result = execute(*conn, select_track_query);

		if (result.next())
		{
			track_id = result.get<int>("id");
		}
		else
		{
			std::cerr << "Track ID could not retrieved from DB. Query: " << select_track_query << std::endl;
		}
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << select_track_query << std::endl;
		std::cerr << e.what() << std::endl;
		return;
	}

	// Insert SIFT points
	for (auto& fp : track.sift_keyPoints)
	{
		try
		{
			std::ostringstream instert_query_stream;
			instert_query_stream << "INSERT INTO siftKeypoints"
				<< "(track_id, x, y, size, angle, response, octave, class_id) "
				<< "VALUES ('" << track_id << "'"
				<< ",'" << fp.pt.x << "'"
				<< ",'" << fp.pt.y << "'"
				<< ",'" << fp.size << "'"
				<< ",'" << fp.angle << "'"
				<< ",'" << fp.response << "'"
				<< ",'" << fp.octave << "'"
				<< ",'" << fp.class_id << "'"
				<< ")";

			insert_track_query = instert_query_stream.str();

			execute(*conn, insert_track_query);
		}
		catch (std::runtime_error const& e)
		{
			std::cerr << "Error in SQL Query: " << insert_track_query << std::endl;
			std::cerr << e.what() << std::endl;
		}
	}

	// Insert SURF points
	for (auto& fp : track.surf_keyPoints)
	{
		try
		{
			std::ostringstream instert_query_stream;
			instert_query_stream << "INSERT INTO surfKeypoints"
				<< "(track_id, x, y, size, angle, response, octave, class_id) "
				<< "VALUES ('" << track_id << "'"
				<< ",'" << fp.pt.x << "'"
				<< ",'" << fp.pt.y << "'"
				<< ",'" << fp.size << "'"
				<< ",'" << fp.angle << "'"
				<< ",'" << fp.response << "'"
				<< ",'" << fp.octave << "'"
				<< ",'" << fp.class_id << "'"
				<< ")";

			insert_track_query = instert_query_stream.str();

			execute(*conn, insert_track_query);
		}
		catch (std::runtime_error const& e)
		{
			std::cerr << "Error in SQL Query: " << insert_track_query << std::endl;
			std::cerr << e.what() << std::endl;
		}
	}
}

void dto::SQLHelper::persist_persons(std::vector<Person>& persons)
{
	for (auto& p : persons)
	{
		std::string insert_person_query;
		std::string select_person_query;

		int person_db_id;

		nanodbc::result result;

		// Insert Person
		try
		{
			std::ostringstream insert_query_stream;

			insert_query_stream << "INSERT INTO persons"
				<< "(person_id) "
				<< "VALUES ('" << p.person_id << "'"
				<< ")";

			insert_person_query = insert_query_stream.str();

			execute(*conn, insert_person_query);
		}
		catch (std::runtime_error const& e)
		{
			std::cerr << "Error in SQL Query: " << insert_person_query << std::endl;
			std::cerr << e.what() << std::endl;
		}

		// Retrieve person id
		try
		{
			std::ostringstream select_query_stream;
			select_query_stream << "SELECT id FROM persons"
				<< " WHERE person_id=" << p.person_id << "";

			select_person_query = select_query_stream.str();

			result = execute(*conn, select_person_query);

			if (result.next())
			{
				person_db_id = result.get<int>("id");
			}
			else
			{
				std::cerr << "Person ID could not retrieved from DB. Query: " << select_person_query << std::endl;
			}
		}
		catch (std::runtime_error const& e)
		{
			std::cerr << "Error in SQL Query: " << select_person_query << std::endl;
			std::cerr << e.what() << std::endl;
			return;
		}

		// Update Track
		for (auto& t : p.tracks)
		{
			std::string update_track_query;

			try
			{
				std::ostringstream update_query_stream;
				update_query_stream << "UPDATE tracks "
					<< "SET person_id = '" << person_db_id << "' "
					<< "WHERE id = '" << t.track_db_id << "'";

				update_track_query = update_query_stream.str();

				execute(*conn, update_track_query);
			}
			catch (std::runtime_error const& e)
			{
				std::cerr << "Error in SQL Query: " << update_track_query << std::endl;
				std::cerr << e.what() << std::endl;
			}
		}
	}
}

void dto::SQLHelper::retrieve_camera(Camera& camera, int camera_id)
{
	std::string select_query;
	nanodbc::result result;

	try
	{
		// Retrieve camera
		std::ostringstream select_query_stream;
		select_query_stream << "SELECT * FROM cameras"
			<< " WHERE id='" << camera_id << "'";

		select_query = select_query_stream.str();

		result = execute(*conn, select_query);

		if (result.rowset_size() != 1)
		{
			std::cerr << "Camera could not be retrieved. Query: " << select_query << std::endl;
		}
		else
		{
			result.next();
			camera.scene = result.get<int>("scene");
			camera.directory = result.get<std::string>("directory");
			camera.prefix = result.get<std::string>("prefix");
			camera.width = result.get<int>("width");
			camera.height = result.get<int>("height");
			camera.fps = result.get<int>("fps");
			camera.pixelToCentimeterRatio = result.get<float>("pixelToCentimeterRatio");
		}
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << select_query << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

std::vector<dto::Track> dto::SQLHelper::retrieve_all_tracks()
{
	std::string select_query;
	nanodbc::result result;

	std::vector<Track> tracks;

	try
	{
		// Retrieve tracks
		std::ostringstream select_query_stream;
		select_query_stream << "SELECT "
			<< "tracks.*, "
			<< "cameras.id AS camera_id, "
			<< "cameras.scene AS camera_scene, "
			<< "cameras.directory AS camera_directory, "
			<< "cameras.prefix AS camera_prefix, "
			<< "cameras.fps AS camera_fps, "
			<< "cameras.height AS camera_height, "
			<< "cameras.width AS camera_width, "
			<< "cameras.pixelToCentimeterRatio AS camera_pixelToCentimeterRatio "
			<< "FROM tracks "
			<< "LEFT JOIN cameras ON tracks.camera_id = cameras.id ";

		select_query = select_query_stream.str();

		result = execute(*conn, select_query);

		while (result.next())
		{
			Track t;
			Camera c;

			t.track_db_id = result.get<int>("id");
			t.trackId = result.get<int>("trackId");
			t.primary_color_ids.upperBody = result.get<int>("primaryColor_UpperBody");
			t.primary_color_ids.lowerBody = result.get<int>("primaryColor_LowerBody");
			t.estimatedPersonSize.height = result.get<float>("personSize_height");
			t.estimatedPersonSize.width = result.get<float>("personSize_width");

			std::string walkingDirection = result.get<std::string>("walkingDirection");
			if (walkingDirection == "in_in") t.walkingDirection = Track::WalkingDirection::in_in;
			else if (walkingDirection == "in_out") t.walkingDirection = Track::WalkingDirection::in_out;
			else if (walkingDirection == "out_in") t.walkingDirection = Track::WalkingDirection::out_in;
			else if (walkingDirection == "out_out") t.walkingDirection = Track::WalkingDirection::out_out;

			c.scene = result.get<int>("camera_scene");
			c.directory = result.get<std::string>("camera_directory");
			c.prefix = result.get<std::string>("camera_prefix");
			c.fps = result.get<int>("camera_fps");
			c.height = result.get<int>("camera_height");
			c.width = result.get<int>("camera_width");
			c.pixelToCentimeterRatio = result.get<float>("camera_pixelToCentimeterRatio");

			t.camera = c;

			tracks.push_back(t);
		}
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << select_query << std::endl;
		std::cerr << e.what() << std::endl;
		std::vector<Track> t;
		return t;
	}

	/*
	try
	{
		for (auto& t : tracks)
		{
			// Retrieve sift points
			std::ostringstream select_query_stream;
			select_query_stream << "SELECT * FROM siftKeypoints"
				<< " WHERE track_id='" << t.track_db_id << "'";

			select_query = select_query_stream.str();

			result = execute(*conn, select_query);

			while (result.next())
			{
				cv::KeyPoint p = cv::KeyPoint(result.get<float>("x"), result.get<float>("y"), result.get<float>("size"),
				                              result.get<float>("angle"), result.get<float>("response"), result.get<int>("octave"),
				                              result.get<int>("class_id"));

				t.sift_keyPoints.push_back(p);
			}
		}
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << select_query << std::endl;
		std::cerr << e.what() << std::endl;
		std::vector<Track> t;
		return t;
	}

	try
	{
		for (auto& t : tracks)
		{
			// Retrieve surf points
			std::ostringstream select_query_stream;
			select_query_stream << "SELECT * FROM surfKeypoints"
				<< " WHERE track_id='" << t.track_db_id << "'";

			select_query = select_query_stream.str();

			result = execute(*conn, select_query);

			while (result.next())
			{
				cv::KeyPoint p = cv::KeyPoint(result.get<int>("x"), result.get<int>("y"), result.get<float>("size"),
				                              result.get<float>("angle"), result.get<float>("response"), result.get<int>("octave"),
				                              result.get<int>("class_id"));

				t.surf_keyPoints.push_back(p);
			}
		}
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << select_query << std::endl;
		std::cerr << e.what() << std::endl;
		std::vector<Track> t;
		return t;
	}
	*/

	return tracks;
}

void dto::SQLHelper::backup_database(char* destination)
{
	std::string backup_query;
	try
	{
		std::ostringstream backup_query_stream;
		backup_query_stream << "BACKUP DATABASE sc_analyzer "
			<< "TO DISK='" << destination << "' "
			<< "WITH FORMAT ";

		backup_query = backup_query_stream.str();

		std::cout << "Execute SQL Query: " << backup_query << std::endl;
		execute(*conn, backup_query);
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Error in SQL Query: " << backup_query << std::endl;
		std::cerr << e.what() << std::endl;
	}
}
