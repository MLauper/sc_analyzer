#pragma once
#include <vector>
#include "Region.h"
#include "Image.h"
#include "Camera.h"

/*! \file Track.h
*	\brief Contains the track dto..
*
* This file contains the tarc dto with all required properties.
*/

/*! \brief The dto namespace contains all DTOs*/
namespace dto
{
	/*! \brief The track dto struct cotnains all track properties.*/
	struct Track
	{
		/*! \brief This enum lists all possible walking directions.*/
		enum WalkingDirection
		{
			/*! \brief Person walks in the room.*/
			out_in,
			/*! \brief Person walks out of the room*/
			in_out,
			/*! \brief Person walks in the room and leaves it again.*/
			out_out,
			/*! \brief Person moves inside the room.*/
			in_in
		};

		/*! \brief This struct contains OpenCV representation of the body parts.*/
		struct Cv_optimalPersonBodyParts
		{
			/*! \brief OpenCV representation of the head.*/
			cv::Mat head;
			/*! \brief OpenCV representation of the upper body.*/
			cv::Mat upperBody;
			/*! \brief OpenCV representation of the lower body.*/
			cv::Mat lowerBody;
		};

		/*! \brief This struct contains the ids of the extracted colors.*/
		struct primaryColorIds
		{
			/*! \brief The recognized upper body color id.*/
			int upperBody;
			/*! \brief The recognized lower body color id.*/
			int lowerBody;
		};

		/*! \brief This struct contains the recognized person sizes.*/
		struct personSize
		{
			/*! \brief The recognized height in cm.*/
			float height;
			/*! \brief The recognized width in cm.*/
			float width;
		};

		/*! \brief This struct contains a suggestion to assign this track to another one.*/
		struct suggestion
		{
			/*! \brief A pointer to the suggested track.*/
			Track* track;
			/*! \brief A likelihood between 0 and 1 how likely the tracks match.*/
			float likelihood;
		};

		/*! \brief Unique track id per camera.*/
		int trackId;

		/*! \brief List of detected regions of one person. */
		std::vector<Region> regions;
		/*! \brief List of detected bounding boxes of one person. */
		std::vector<bbox_t> persons;
		// List of images, corresponding with list of bounding boxes
		std::vector<Image> images;

		// Walking direction of the person
		WalkingDirection walkingDirection;

		// Number of input images which do not allow the track to continue
		int numImagesWithoutRegion;

		// Best picture in the whole track
		int optimalPersonId;

		// Cut image of the best picture of the person (background removed)
		cv::Mat cv_optimalPersonCut;
		// Cut image of the best pictrue of the person (background not removed)
		cv::Mat cv_optimalPersonCut_Full;
		// Split body parts of the person
		Cv_optimalPersonBodyParts cv_optimalPersonBodyParts;

		// Primary color on the body part images
		primaryColorIds primary_color_ids;
		// Estimated person size
		personSize estimatedPersonSize;

		// Surf keypoints and descriptors on the optimal image
		std::vector<cv::KeyPoint> surf_keyPoints;
		cv::Mat surf_descriptors;
		// Sift keypoints and descriptors on the optimal image
		std::vector<cv::KeyPoint> sift_keyPoints;
		cv::Mat sift_descriptors;

		// DB id of track
		int track_db_id;

		// Originating camera of this track
		Camera camera;

		// Identification - Likelihood for the same person
		std::vector<suggestion> surf_keypoint_suggestion;
		std::vector<suggestion> sift_keypoint_suggestion;
		std::vector<suggestion> size_width_suggestion;
		std::vector<suggestion> size_height_suggestion;
		std::vector<suggestion> upper_body_color_suggestion;
		std::vector<suggestion> lower_body_color_suggestion;
		std::vector<suggestion> overall_suggestion;

		int assignedPersonId;
	};
}
