#pragma once
#include <string>
#include "../image_tracking/ObjectTracker.h"

/*! \file Configuration.h
*	\brief Contains compile time program options.
*
* This file contains all global configurable options. These options
* are only availabe at compile time.
*/

/*! \brief The dto namespace contains all DTOs*/
namespace dto
{
	/*! \brief This namepsace contains all configuration options.*/
	namespace Configuration
	{
		/*! \brief Configure if ArrayFire image should be loaded*/
		const bool LOAD_AF_IMAGE = false;

		/*! \brief Configure if distorted images should be calculated.*/
		const bool CREATE_DISTORTED_IMAGE = false;
		/*! \brief Configure if distorted images should be displayed.*/
		const bool SHOW_DISTORTED_IMAGE = false;
		/*! \brief Configure if distorted images should be saved.*/
		const bool SAVE_DISTORTED_IMAGE = false;
		/*! \brief Save location for distorted images.*/
		const std::string DISTORTED_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		/*! \brief Configure if original images should be displayed.*/
		const bool SHOW_ORIGINAL_IMAGES = false;
		/*! \brief Configure if original images should be saved.*/
		const bool SAVE_ORIGINAL_IMAGES = true;
		/*! \brief Original images save location.*/
		const std::string ORIGINAL_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		/*! \brief Configure if foreground images should be displayed.*/
		const bool SHOW_FG_IMAGES = true;
		/*! \brief Configure if foreground images should be saved.*/
		const bool SAVE_FG_IMAGES = true;
		/*! \brief Original images save location.*/
		const std::string FG_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		/*! \brief Configure if foreground mask should be displayed.*/
		const bool SHOW_FG_MASKS = false;
		/*! \brief Configure if foreground mask should be saved.*/
		const bool SAVE_FG_MASK = false;
		/*! \brief Foreground mask save location.*/
		const std::string FG_MASKS_DIRECTORY = "C:\\Temp\\output\\";

		/*! \brief Configure if all countours should be displayed.*/
		const bool SHOW_ALL_CONTOURS = false;
		/*! \brief Configure if all contours should be saved.*/
		const bool SAVE_ALL_CONTOURS = true;
		/*! \brief All contours save directory.*/
		const std::string ALL_CONTOURS_DIRECTORY = "C:\\Temp\\output\\";

		/*! \brief Configure if contour images should be displayed.*/
		const bool SHOW_CONTOUR_IMAGES = false;
		/*! \brief Configure if contour images should be saved.*/
		const bool SAVE_CONTOUR_IMAGES = true;
		/*! \brief Contour images save directory.*/
		const std::string CONTOUR_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		/*! \brief Configure if track images should be displayed.*/
		const bool SHOW_TRACK_IMAGES = false;
		/*! \brief Configure if track images should be saved.*/
		const bool SAVE_TRACK_IMAGES = true;
		/*! \brief Track images save directory.*/
		const std::string TRACK_IMAGES_DIRECTORY = "C:\\Temp\\output\\";

		/*! \brief Configure the maximum number of missing frames in a track.*/
		const int MAX_NUMBER_OF_MISSING_FRAMES_IN_TRACK = 8;
		/*! \brief Configure the minimum number of frames in a track*/
		const int MIN_NUMBERS_OF_FRAMES_IN_TRACK = 8;
		
		// Yolo Coco dataset
		/*! \brief Name of the yolo config file.*/
		const std::string yoloConfig = "yolo.cfg.txt";
		/*! \brief Name of the yolo weights file.*/
		const std::string yoloWeights = "yolo.weights";
		/*! \brief Yolo person object id*/
		const int yoloPersonObjectId = 0;

		/*! \brief Configure if Yolo BBs should be printed on console.*/
		const bool PRINT_YOLO_PERSONS = false;
		/*! \brief Configure if Yolo BBs should be displayed.*/
		const bool SHOW_YOLO_PERSONS_IMAGES = true;
		/*! \brief Configure if Yolo BBs should be saved.*/
		const bool SAVE_YOLO_PERSONS_IMAGES = true;
		/*! \brief Configure if local contrast correction should be applied on input image.*/
		const bool USE_HIGH_CONTRAST_IMAGE_FOR_YOLO = false;
		/*! \brief Yolo BB Images save directory.*/
		const std::string YOLO_PERSONS_IMAGES_DIRECTORY = "C:\\Temp\\output\\";
		/*! \brief GPU ID for Yolo inferencing.*/
		const int YOLO_GPU_ID = 0;

		/*! \brief Configure if Yolo images should be used for tracking.*/
		const bool USE_YOLO_FOR_TRACKING = true;
		/*! \brief Configure if Yolo images should be used for frame selection.*/
		const bool USE_YOLO_FOR_FRAMESELECTION = true;
		/*! \brief Configure if FG images should be used as input for yolo.*/
		const bool USE_FG_IMAGE_FOR_YOLO = false;

		/*! \brief Configure Publish URL for person counter.*/
		const std::string PERSON_COUNTER_0_PUBLISH_URL = "https://sc-analyzer-reporter.firebaseio.com/person_counter_0.json";

		/*! \brief Configure percentage of border. This is used for entry side detection.*/
		const float IMAGE_BORDER_PERCENTATGE = 0.03f;
		/*! \brief Configure if statistics file for a track should be saved.*/
		const bool SAVE_TRACK_STATISTICS = true;
		/*! \brief Statistics save directory.*/
		const std::string STATISTICS_DIRECTORY = "C:\\Temp\\output\\";
		/*! \brief Configure maximum number of pixels a bounding box can move between two frames.*/
		const double MAX_TRACKING_DISTANCE = 50;

		/*! \brief Configure if optimal track position should use a point.*/
		const bool USE_POINT_FOR_OPTIMAL_TRACK = true;
		/*! \brief Configure max distance from optimal point to bounding box.*/
		const long MAX_OPTIMAL_DISTANCE = 50;
		/*! \brief Configure if frame selection steps should be printed to console.*/
		const bool PRINT_FRAME_SELECTION_STEPS = false;
		/*! \brief Configure if optimal track image should be saved.*/
		const bool SAVE_OPTIMAL_TRACK_IMAGE = true;
		/*! \brief Configure if cut optimal track image should be saved.*/
		const bool SAVE_OPTIMAL_TRACK_IMAGE_CUT = true;
		/*! \brief Optimal track image save directory.*/
		const std::string OPTIMAL_TRACK_DIRECTORY = "C:\\Temp\\output\\";
		/*! \brief Configure if body part images should be saved.*/
		const bool SAVE_BODY_PARTS_IMAGES = true;
		/*! \brief Configure if generated image with hue only should be saved.*/
		const bool SAVE_HUE_IMAGE = true;
		/*! \brief Configure if HSV values should be printed on console.*/
		const bool PRINT_HSV_VALUES = false;

		/*! \brief Configure if feature point images should be saved.*/
		const bool SAVE_FEATURE_POINT_IMAGES = true;
		/*! \brief Configure if foreground image should be used for feature point extraction.*/
		const bool USE_FG_IMAGE_FOR_FEATURE_POINTS = true;
		/*! \brief Configure if tracks should be persisted in the MSSQL database.*/
		const bool STORE_TRACK_RESULTS_IN_DB = false;

		/*! \brief ODBC connection name. Must be configured on the system.*/
		const nanodbc::string DATABASE_ODBC_NAME = "sc_analyzer";
		/*! \brief ODBC user name to connect to the database.*/
		const nanodbc::string DATABASE_USER = "db_user";
		/*! \brief ODBC user password to connect to the database.*/
		const nanodbc::string DATABASE_PASSWORD = "Rtchir3ORJe2";

		/*! \brief Weight of SURF keypoints.*/
		const float surf_keypoint_suggestion_weight = 0.4f;
		/*! \brief Weigth of sift keypoints.*/
		const float sift_keypoint_suggestion_weight = 0.0f;
		/*! \brief Weight of the estimtated width.*/
		const float size_width_suggestion_weight = 0.15f;
		/*! \brief Weight of the estimated height.*/
		const float size_height_suggestion_weight = 0.15f;
		/*! \brief Weight of the upper body color.*/
		const float upper_body_color_suggestion_weight = 0.15f;
		/*! \brief Weight of the lower body color.*/
		const float lower_body_color_suggestion_weight = 0.15f;

		/*! \brief Configure if person identification results should be saved.*/
		const bool SAVE_PERSON_IDENTIFICATION = true;
		/*! \brief Configure minimum likelihood between two persons.*/
		const float ALWAYS_MATCH_LIKELIHOOD = 0.5f;
		/*! \brief Configure if person selection steps should be printed on console.*/
		const bool PRINT_PERSON_SELECTOR_STEPS = false;
	}
}
