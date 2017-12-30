#define NOMINMAX
#include <ostream>
#include <iostream>

#include "../dto/SQLHelper.h"
#include "FeaturePointMatcher.h"
#include "SizeMatcher.h"
#include "ColorMatcher.h"
#include "LikelihoodCalculator.h"

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

	identification::FeaturePointMatcher feature_point_matcher;
	feature_point_matcher.matchAllFeaturePoints(tracks);

	identification::SizeMatcher size_matcher;
	size_matcher.matchAllSizes(tracks);

	identification::ColorMatcher color_matcher;
	color_matcher.matchAllColors(tracks);

	identification::LikelihoodCalculator likelihood_calculator;
	likelihood_calculator.calculateAllLikelihoods(tracks);

	std::cout << "Finished Identification" << std::endl;
}
