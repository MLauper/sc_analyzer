#include "Controller.h"
#include <opencv2/core/cuda.hpp>
#include "opencv2/highgui.hpp"
#include "../dto/Image.h"


image_tracking::Controller::Controller()
{

}

void image_tracking::Controller::ProcessImage(dto::Image& image, dto::Camera& camera)
{

	ot.apply(image);
	ot.SendFinishedTracksTo(this->feature_extraction_controller, camera);

}

