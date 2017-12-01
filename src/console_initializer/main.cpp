#define NOMINMAX

#include "image_acquisition/image_acquisition.h"
#include "image_segmentation/image_segmentation.h"

int main()
{
	//WatchDirectory("D:\\data\\image");

	dto::Camera camera;
	camera.directory = "D:\\data\\room_walk_with_different_lighting\\image\\";
	camera.prefix = "out_CAMERA_room_";
	camera.entrance_side = dto::Camera::side::bottom;
	camera.width = 1920;
	camera.height = 1080;
	camera.gateMode = dto::Camera::gateMode::minBottom;
	camera.gateValue = 150;

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
