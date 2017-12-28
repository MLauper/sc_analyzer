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

		int trackId;
		std::vector<dto::Region> regions;
		std::vector<bbox_t> persons;
		std::vector<dto::Image> images;

		WalkingDirection walkingDirection;

		int numImagesWithoutRegion;

		int optimalPersonId;

		personSize extimatedPersonSize;
		
		cv::Mat cv_optimalPersonCut;
		cv::Mat cv_optimalPersonCut_Full;
		Cv_optimalPersonBodyParts cv_optimalPersonBodyParts;

		primaryColorIds primary_color_ids;

		std::vector<cv::KeyPoint> surf_keyPoints;
		std::vector<cv::KeyPoint> sift_keyPoints;
	};
}
