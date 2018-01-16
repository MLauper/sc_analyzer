#define NOMINMAX
#include <ostream>
#include <iostream>

#include "../dto/SQLHelper.h"
#include "../dto/Configuration.h"
#include "FeaturePointMatcher.h"
#include "SizeMatcher.h"
#include "ColorMatcher.h"
#include "LikelihoodCalculator.h"
#include "PersonAssigner.h"

int main()
{
	std::cout << "Start Identification" << std::endl << std::endl;

	dto::SQLHelper sql_helper;

	//dto::Camera camera;
	//camera.directory = "fefe";
	//camera.prefix = "prefixfee";
	//camera.width = 109;
	//camera.height = 2;
	//camera.fps = 8;
	//camera.pixelToCentimeterRatio = 1.13;
	//
	//sql_helper.persist_camera(camera);
	//
	//dto::Camera db_camera;
	//sql_helper.retrieve_camera(db_camera, "fefe", "prefixfee");
	//
	//std::cout << "Camera: " << db_camera.directory  << ", " << db_camera.pixel << std::endl;

	auto tracks = sql_helper.retrieve_all_tracks();
	std::vector<dto::Person> persons;

	identification::FeaturePointMatcher feature_point_matcher;
	feature_point_matcher.matchAllFeaturePoints(tracks);

	identification::SizeMatcher size_matcher;
	size_matcher.matchAllSizes(tracks);

	identification::ColorMatcher color_matcher;
	color_matcher.matchAllColors(tracks);

	identification::LikelihoodCalculator likelihood_calculator;
	likelihood_calculator.calculateAllLikelihoods(tracks);

	identification::PersonAssigner person_assigner;
	person_assigner.assignTracksToPerson(tracks, persons);

	sql_helper.persist_persons(persons);

	if (dto::Configuration::SAVE_PERSON_IDENTIFICATION)
	{
		std::ofstream fileStream;
		std::stringstream filePath;
		filePath << dto::Configuration::STATISTICS_DIRECTORY << "scene-1" "\\" << "person_statistics.txt";
		fileStream.open(filePath.str().c_str(), std::fstream::app);

		fileStream << "Person Identification Statistics" << std::endl;
		for (auto i = 0; i < persons.size(); i++)
		{
			fileStream << " Person " << i << ": " << std::endl;
			for (auto j = 0; j < persons.at(i).tracks.size(); j++)
			{
				fileStream << "  Scene: " << persons.at(i).tracks.at(j).camera.scene << ", Track: " << persons.at(i).tracks.at(j).
				                                                                                               trackId << std::endl;
			}
		}

		fileStream << std::endl;

		fileStream.close();
	}

	std::cout << "Finished Identification" << std::endl;
}
