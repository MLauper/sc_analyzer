#pragma once
#include <string>
#include "../image_tracking/ObjectTracker.h"

namespace dto {
	namespace Configuration {
		
		const bool LOAD_AF_IMAGE = true;
		
		const bool CREATE_DISTORTED_IMAGE = true;
		const bool SHOW_DISTORTED_IMAGE = true;
		const bool SAVE_DISTORTED_IMAGE = true;
		const std::string DISTORTED_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_ORIGINAL_IMAGES = true;
		const bool SAVE_ORIGINAL_IMAGES = true;
		const std::string ORIGINAL_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_FG_IMAGES = false;
		const bool SAVE_FG_IMAGES = true;
		const std::string FG_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_FG_MASKS = false;
		const bool SAVE_FG_MASK = true;
		const std::string FG_MASKS_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_ALL_CONTOURS = true;
		const bool SAVE_ALL_CONTOURS = true;
		const std::string ALL_CONTOURS_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_CONTOUR_IMAGES = true;
		const bool SAVE_CONTOUR_IMAGES = true;
		const std::string CONTOUR_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_TRACK_IMAGES = false;
		const bool SAVE_TRACK_IMAGES = true;
		const std::string TRACK_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const int MAX_NUMBER_OF_MISSING_FRAMES_IN_TRACK = 8;

		//// Yolo VOC dataset
		//const std::string yoloConfig = "C:\\code\\sc_analyzer\\x64\\Debug\\yolo-voc.cfg.txt";
		//const std::string yoloWeights = "C:\\code\\sc_analyzer\\x64\\Debug\\yolo-voc.weights";
		//const int yoloPersonObjectId = 8;

		// Yolo Coco dataset
		const std::string yoloConfig = "C:\\code\\sc_analyzer\\x64\\Debug\\yolo.cfg.txt";
		const std::string yoloWeights = "C:\\code\\sc_analyzer\\x64\\Debug\\yolo.weights";
		const int yoloPersonObjectId = 0;

		const bool PRINT_YOLO_PERSONS = false;
		const bool SHOW_YOLO_PERSONS_IMAGES = true;
		const bool SAVE_YOLO_PERSONS_IMAGES = false;
		const std::string YOLO_PERSONS_IMAGES_DIRECTORY = "C:\\Temp\\output\\";
		const int YOLO_GPU_ID = 0;
		const bool SAVE_YOLO_TRACK_BB = true;

		const bool USE_YOLO_FOR_TRACKING = true;

		const std::string PERSON_COUNTER_0_PUBLISH_URL = "https://sc-analyzer-reporter.firebaseio.com/person_counter_0.json";
	}
}
