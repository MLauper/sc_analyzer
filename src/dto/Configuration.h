#pragma once
#include <string>

namespace dto {
	namespace Configuration {
		const bool SHOW_ORIGINAL_IMAGES = true;
		const bool SAVE_ORIGINAL_IMAGES = true;
		const std::string ORIGINAL_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_FG_IMAGES = true;
		const bool SAVE_FG_IMAGES = true;
		const std::string FG_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_FG_MASKS = true;
		const bool SAVE_FG_MASK = true;
		const std::string FG_MASKS_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_ALL_CONTOURS = true;
		const bool SAVE_ALL_CONTOURS = true;
		const std::string ALL_CONTOURS_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_CONTOUR_IMAGES = true;
		const bool SAVE_CONTOUR_IMAGES = true;
		const std::string CONTOUR_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_TRACK_IMAGES = true;
		const bool SAVE_TRACK_IMAGES = true;
		const std::string TRACK_IMAGES_DIRECTORY = "C:\\Temp\\output\\";
	}
}