#pragma once
#include <arrayfire.h>

namespace image_segmentation
{
	class Controller
	{
	public:
		void ProcessImage(af::array image);
	};
}
