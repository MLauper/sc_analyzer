#define NOMINMAX
#include <ostream>
#include <iostream>

#include "../dto/SQLHelper.h"

int main()
{

	std::cout << "Start Identification" << std::endl << std::endl;

	dto::SQLHelper sql_helper;

	dto::Camera camera;
	camera.directory = "fefe";
	camera.prefix = "prefixfee";
	camera.width = 109;
	camera.height = 2;
	camera.fps = 8;
	camera.pixelToCentimeterRatio = 1.13;

	sql_helper.persist_camera(camera);

	dto::Camera db_camera;
	sql_helper.retrieve_camera(db_camera, "fefe", "prefixfee");

	std::cout << "Camera: " << db_camera.directory  << ", " << db_camera.pixelToCentimeterRatio << std::endl;

	std::cout << "Finished Identification" << std::endl;
}
