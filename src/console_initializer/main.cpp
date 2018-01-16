#define NOMINMAX

//#include "image_acquisition/image_acquisition.h"
#include "image_acquisition/MKVFileLoader.h"

int main(const int argc, char* argv[])
{
	// Read commandline arguments
	if (argc != 5)
	{
		std::cerr << "Commandline arguments not provided. Please provide: scene, prefix, fps, videoFilePath" << std::endl;
		std::cerr << "Found the following arguments: " << std::endl;
		for (auto i = 1; i < argc; i++)
		{
			std::cerr << " " << i << ": " << argv[i] << std::endl;
		}
		return -1;
	}

	dto::Camera camera;

	// Configure camera based on arguments
	camera.scene = atoi(argv[1]); // For Example: 1;
	camera.prefix = argv[2];// For Example: "out_CAMERA_door_";
	camera.fps = atoi(argv[3]);// For Example: 4;
	camera.videoFilePath = argv[4];// For Example: "C:\\data\\test-scenes-cut\\out_CAMERA_door_0000000216.0.mkv";

	// Only used when using FileLoader
	//camera.directory = "D:\\data\\room_walk_with_different_lighting\\image\\";

	// Static values
	camera.backgroundThreshold = 16;

	// Static values based on camera
	if (camera.prefix == "out_CAMERA_room_")
	{
		camera.entry_side = dto::Camera::entrySide::entry_bottom;
		camera.personCountMode = dto::Camera::personCountUpWhen::in_to_entry;
		camera.width = 1920;
		camera.height = 1080;
		camera.gateMode = dto::Camera::gateMode::minBottom;
		camera.gateValue = 100;
		camera.secondGateMode = dto::Camera::gateMode::minLeft;
		camera.secondGateValue = 770;
		camera.optimalPersonLocation = cv::Point(770, 950);
		camera.pixelToCentimeterRatio = 180.0f / 515.0f;

		// Camera distortion
		const auto cam_fx = 1.5429064838570325e+03;
		const auto cam_cx = 9.5369579797955782e+02;
		const auto cam_fy = 1.5429064838570325e+03;
		const auto cam_cy = 5.7690440309632538e+02;
		camera.cameraMatrix = (cv::Mat1d(3, 3) << cam_fx, 0, cam_cx, 0, cam_fy, cam_cy, 0, 0, 1);

		const auto k1 = -6.2252844915595262e-01;
		const auto k2 = 2.9966919648514401e-01;
		const auto p1 = -8.8257543325885261e-03;
		const auto p2 = -4.3121497449825881e-03;
		const auto k3 = 3.7852955947755049e-02;
		camera.distCoeffs = (cv::Mat1d(1, 5) << k1, k2, p1, p2, k3);
	}
	else if (camera.prefix == "out_CAMERA_door_")
	{
		camera.entry_side = dto::Camera::entrySide::entry_bottom;
		camera.personCountMode = dto::Camera::personCountUpWhen::entry_to_in;
		camera.width = 1920;
		camera.height = 1080;
		camera.gateMode = dto::Camera::gateMode::minBottom;
		camera.gateValue = 100;
		camera.secondGateMode = dto::Camera::gateMode::minLeft;
		camera.secondGateValue = 922;
		camera.optimalPersonLocation = cv::Point(922, 950);
		camera.pixelToCentimeterRatio = 180.0f / 540.0f;

		// Camera distortion
		const auto cam_fx = 1.5429064838570325e+03;
		const auto cam_cx = 9.5369579797955782e+02;
		const auto cam_fy = 1.5429064838570325e+03;
		const auto cam_cy = 5.7690440309632538e+02;
		camera.cameraMatrix = (cv::Mat1d(3, 3) << cam_fx, 0, cam_cx, 0, cam_fy, cam_cy, 0, 0, 1);

		const auto k1 = -6.2252844915595262e-01;
		const auto k2 = 2.9966919648514401e-01;
		const auto p1 = -8.8257543325885261e-03;
		const auto p2 = -4.3121497449825881e-03;
		const auto k3 = 3.7852955947755049e-02;
		camera.distCoeffs = (cv::Mat1d(1, 5) << k1, k2, p1, p2, k3);
	}
	else
	{
		std::cerr << "Unknown camera. Required information not available..." << std::endl;
		return -1;
	}

	auto controller = new image_segmentation::Controller(camera);

	// To use the FileLoader instead of mkv loader, use the following lines
	//image_acquisition::FileLoader file_loader(camera, controller);
	//file_loader.ProcessFiles();
	//file_loader.WatchDirectory();

	image_acquisition::MKVFileLoader mkv_file_loader(camera, controller);
	mkv_file_loader.process_file();

	std::cout << "Finished processing input file: " << camera.videoFilePath << std::endl;
	return 0;
}
