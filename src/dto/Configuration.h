#pragma once
#include <string>
#include "../image_tracking/ObjectTracker.h"

namespace dto {
	namespace Configuration {
		
		const bool LOAD_AF_IMAGE = false;
		
		const bool CREATE_DISTORTED_IMAGE = false;
		const bool SHOW_DISTORTED_IMAGE = false;
		const bool SAVE_DISTORTED_IMAGE = false;
		const std::string DISTORTED_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_ORIGINAL_IMAGES = false;
		const bool SAVE_ORIGINAL_IMAGES = false;
		const std::string ORIGINAL_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_FG_IMAGES = true;
		const bool SAVE_FG_IMAGES = false;
		const std::string FG_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_FG_MASKS = false;
		const bool SAVE_FG_MASK = false;
		const std::string FG_MASKS_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_ALL_CONTOURS = false;
		const bool SAVE_ALL_CONTOURS = false;
		const std::string ALL_CONTOURS_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_CONTOUR_IMAGES = false;
		const bool SAVE_CONTOUR_IMAGES = false;
		const std::string CONTOUR_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const bool SHOW_TRACK_IMAGES = false;
		const bool SAVE_TRACK_IMAGES = false;
		const std::string TRACK_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		const int MAX_NUMBER_OF_MISSING_FRAMES_IN_TRACK = 8;
		const int MIN_NUMBERS_OF_FRAMES_IN_TRACK = 8;

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
		const bool USE_HIGH_CONTRAST_IMAGE_FOR_YOLO = false;
		const std::string YOLO_PERSONS_IMAGES_DIRECTORY = "C:\\Temp\\output\\";
		const int YOLO_GPU_ID = 0;

		const bool USE_YOLO_FOR_TRACKING = true;
		const bool USE_YOLO_FOR_FRAMESELECTION = true;
		const bool USE_FG_IMAGE_FOR_YOLO = true;

		const std::string PERSON_COUNTER_0_PUBLISH_URL = "https://sc-analyzer-reporter.firebaseio.com/person_counter_0.json";

		const float IMAGE_BORDER_PERCENTATGE = 0.03f;
		const bool SAVE_TRACK_STATISTICS = true;
		const std::string STATISTICS_DIRECTORY = "C:\\Temp\\output\\";
		const double MAX_TRACKING_DISTANCE = 100;

		const bool USE_POINT_FOR_OPTIMAL_TRACK = true;
		const long MAX_OPTIMAL_DISTANCE = 100;
		const bool PRINT_FRAME_SELECTION_STEPS = true;
		const bool SAVE_OPTIMAL_TRACK_IMAGE = true;
		const bool SAVE_OPTIMAL_TRACK_IMAGE_CUT = true;
		const std::string OPTIMAL_TRACK_DIRECTORY = "C:\\Temp\\output\\";
		const bool SAVE_BODY_PARTS_IMAGES = false;
		const bool SAVE_HUE_IMAGE = false;
		const bool PRINT_HSV_VALUES = false;

		const bool SAVE_FEATURE_POINT_IMAGES = true;
		const bool USE_FG_IMAGE_FOR_FEATURE_POINTS = true;
	}
}
