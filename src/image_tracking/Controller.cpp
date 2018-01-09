#include "Controller.h"
#include "../dto/Image.h"
#include "../dto/Configuration.h"


image_tracking::Controller::Controller()
{
}

void image_tracking::Controller::ProcessImage(dto::Image& image, dto::Camera& camera)
{
	if (dto::Configuration::USE_YOLO_FOR_TRACKING)
	{
		ot_yolo.apply(image);
		ot_yolo.SendFinishedTracksTo(this->feature_extraction_controller, camera);
	}
}
