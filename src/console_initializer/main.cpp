#define NOMINMAX

#include "image_acquisition/image_acquisition.h"
#include "image_segmentation/image_segmentation.h"

int main()
{
	//WatchDirectory("D:\\data\\image");

	dto::Camera camera;

	// Room Walk with Different Lighting
	//camera.directory = "D:\\data\\room_walk_with_different_lighting\\image\\";
	//camera.prefix = "out_CAMERA_room_";
	camera.fps = 25;
	camera.backgroundThreshold = 8;
	camera.entry_side = dto::Camera::entrySide::entry_bottom;
	camera.personCountMode = dto::Camera::personCountUpWhen::in_to_entry;
	camera.width = 1920;
	camera.height = 1080;
	camera.gateMode = dto::Camera::gateMode::minBottom;
	camera.gateValue = 100;
	camera.secondGateMode = dto::Camera::gateMode::minLeft;
	camera.secondGateValue = 630;
	camera.optimalPersonLocation = cv::Point(770, 950);

	camera.pixelToCentimeterRation = (180.0f / 540.0f);

	// Door Camera distortion
	const double cam_fx = 1.5429064838570325e+03;
	const double cam_cx = 9.5369579797955782e+02;
	const double cam_fy = 1.5429064838570325e+03;
	const double cam_cy = 5.7690440309632538e+02;
	camera.cameraMatrix = (cv::Mat1d(3, 3) << cam_fx, 0, cam_cx, 0, cam_fy, cam_cy, 0, 0, 1);

	const double k1 = -6.2252844915595262e-01;
	const double k2 = 2.9966919648514401e-01;
	const double p1 = -8.8257543325885261e-03;
	const double p2 = -4.3121497449825881e-03;
	const double k3 = 3.7852955947755049e-02;
	camera.distCoeffs = (cv::Mat1d(1, 5) << k1, k2, p1, p2, k3);

	// New Setup Room walk
	//camera.directory = "C:\\data\\test-scenes-cut\\new-setup-walks_door\\";
	//camera.directory = "C:\\data\\test-scenes-cut\\MultiplePeopleDoor\\";
	camera.directory = "C:\\data\\test-scenes-cut\\room_walk_Room\\";
	camera.prefix = "out_CAMERA_room_";
	// Single Person
	//camera.directory = "C:\\data\\test-scenes-cut\\SinglePersonRoom\\";

	// Two Persons Leave
	//camera.directory = "C:\\data\\test-scenes-cut\\TwoPersonsLeave\\";

	// Single Person Door
	//camera.directory = "D:\\data\\room_walk_with_different_lighting-Copy\\image\\";
	//// Multiple People Door
	//camera.directory = "C:\\data\\test-scenes-cut\\MultiplePeopleDoor\\";
	//camera.prefix = "out_CAMERA_door_";
	//camera.fps = 4;
	//camera.backgroundThreshold = 8;
	//camera.entry_side = dto::Camera::entrySide::entry_bottom;
	//camera.width = 1920;
	//camera.height = 1080;
	//camera.gateMode = dto::Camera::gateMode::minBottom;
	//camera.gateValue = 150;

	//// Multiple People Door
	//camera.directory = "C:\\data\\test-scenes-cut\\MultiplePeopleRoom_2\\";
	//camera.prefix = "out_CAMERA_room_";
	//camera.fps = 4;
	//camera.backgroundThreshold = 16;
	//camera.entry_side = dto::Camera::entrySide::entry_bottom;
	//camera.width = 1920;
	//camera.height = 1080;
	//camera.gateMode = dto::Camera::gateMode::minBottom;
	//camera.gateValue = 150;

	image_segmentation::Controller* controller = new image_segmentation::Controller(camera);

	image_acquisition::FileLoader file_loader(camera, controller);
	//image_acquisition::FileLoader file_loader("D:\\data\\room_walk_with_differnt_lighting_8fps\\image\\", "out_CAMERA_room_", controller);
	//image_acquisition::FileLoader file_loader("D:\\data\\room_walk_with_different_lighting\\image\\", "out_CAMERA_room_", controller);
	//image_acquisition::FileLoader file_loader("D:\\data\\room_walk_with_different_lighting\\image\\",
	//                                          "out_CAMERA_room_", controller);

	file_loader.ProcessFiles();
	file_loader.WatchDirectory();

	return 0;
}
