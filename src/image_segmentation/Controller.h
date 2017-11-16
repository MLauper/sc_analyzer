#pragma once
#include <arrayfire.h>
#include <fstream>
#define NOMINMAX

namespace image_segmentation
{
	class Controller
	{
	public:
		Controller(int imageHeight, int imageWidth);
		void ProcessImage(SYSTEMTIME* time, af::array& image, std::string filename);
	private:
		af::array current_image;
		SYSTEMTIME* current_image_time;
		int imageHeight;
		int imageWidth;
		float minPixelChangedPercentageThreshold;
		float maxPixelChangedPercentageThreshold;
		int minPixelDeltaValue;
		af::array current_image_grayscale;
		af::array background;
		af::array mask;
		long int numImagePixels;
		af::array morphMask;
		af::array morphMask2;
		af::array current_image_hsv;
		std::ofstream stat_out;
		static af::array binThreshold(const af::array& in, float thresholdValue);
		static void drawRectangle(af::array& out, unsigned x, unsigned y, unsigned dim0, unsigned dim1);
	};
}
