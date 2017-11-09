#include "image_acquisition/image_acquisition.h"
#include "image_segmentation/image_segmentation.h"

int main()
{
	//WatchDirectory("D:\\data\\image");

	image_segmentation::Controller* controller = new image_segmentation::Controller();

	image_acquisition::FileLoader file_loader("D:\\data\\image", "out_CAMERA_door_", controller);

	file_loader.WatchDirectory();

	return 0;
}
