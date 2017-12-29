#include "SQLHelper.h"

#include "../../external/nanodbc/nanodbc.h"
#include <iostream>
#include "../dto/Configuration.h"

dto::SQLHelper::SQLHelper()
{
	this->conn = new nanodbc::connection(dto::Configuration::DATABASE_ODBC_NAME, dto::Configuration::DATABASE_USER,
	                                     dto::Configuration::DATABASE_PASSWORD, 5);

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
			<< " AND prefix='" << camera.prefix << "'";

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
				<< "(directory, prefix, width, height, fps, pixelToCentimeterRatio) "
				<< "VALUES ('" << camera.directory << "'"
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

void dto::SQLHelper::retrieve_camera(dto::Camera& camera, const char* directory, const char* prefix)
{
	std::string select_query;
	nanodbc::result result;

	try
	{
		// Retrieve camera
		std::ostringstream select_query_stream;
		select_query_stream << "SELECT * FROM cameras"
			<< " WHERE directory='" << directory << "'"
			<< " AND prefix='" << prefix << "'";

		select_query = select_query_stream.str();

		result = execute(*conn, select_query);

		if (result.rowset_size() != 1)
		{
			std::cerr << "Camera could not be retrieved. Query: " << select_query << std::endl;
		} else
		{
			result.next();
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
		return;
	}
}

void dto::SQLHelper::persist_track(const dto::Track& track, const dto::Camera& camera)
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
			<< " AND prefix='" << camera.prefix << "'";

		select_camera_query = select_query_stream.str();

		result = execute(*conn, select_camera_query);

		if (result.next())
		{
			camera_id = result.get<int>("id");
		} else
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
			instert_query_stream << "INSERT INTO tracks"
				<< "(camera_id, trackId, walkingDirection, primaryColor_UpperBody, primaryColor_LowerBody, personSize_height, personSize_width) "
				<< "VALUES ('" << camera_id << "'"
				<< ",'" << track.trackId << "'"
				<< ",'" << track.walkingDirection << "'"
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
		// Retrieve camera id
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
	for (auto& fp : track.sift_keyPoints) {
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
	for (auto& fp : track.surf_keyPoints) {
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