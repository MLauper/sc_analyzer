#pragma once
#include <vector>
#include "Region.h"
#include "Image.h"

namespace dto
{
	struct Track
	{
		enum WalkingDirection
		{
			out_in,  // Person walks in the room
			in_out,  // Person walks out of the room
			out_out, // Person walsk in the room and leaves it again
			in_in    // Person moves inside the room
		};

		struct Cv_optimalPersonBodyParts
		{
			cv::Mat head;
			cv::Mat upperBody;
			cv::Mat lowerBody;
		};
		struct primaryColorIds
		{
			int upperBody;
			int lowerBody;
		};
		struct personSize
		{
			float height;
			float width;
		};

		// This id should be unique per camera (increase with every track)
		int trackId;

		// List of detected regions of one person
		std::vector<dto::Region> regions;
		// List of detected bounding boxes of one person
		std::vector<bbox_t> persons;
		// List of images, corresponding with list of bounding boxes
		std::vector<dto::Image> images;

		// Walking directo of the person
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
		personSize extimatedPersonSize;

		// Surf keypoints on the optimal image
		std::vector<cv::KeyPoint> surf_keyPoints;
		// Sift keypoints on the optimal image
		std::vector<cv::KeyPoint> sift_keyPoints;
	};
}
