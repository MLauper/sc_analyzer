#include "DirectionExtractor.h"
#include "../dto/Camera.h"

feature_extraction::DirectionExtractor::DirectionExtractor()
{
}

void feature_extraction::DirectionExtractor::extractDirection(dto::Track& track, dto::Camera& camera)
{
	std::cout << "WE ARE USING CAMERA " << camera.directory << " with prefix " << camera.prefix << std::endl;
	
	bool isFirstRegionOnEntranceSide = false;
	bool isLastRegionOnEntranceSide = false;

	float borderPercentage = 0.03f;

	// Analyse Region Positions
	if (camera.entrance_side == dto::Camera::side::bottom){
		if (track.regions.at(0).maxY >= (camera.height - (int)(camera.height * borderPercentage)))
		{
			isFirstRegionOnEntranceSide = true;
		}
		if (track.regions.at(track.regions.size() - 1).maxY >= (camera.height - (int)(camera.height * borderPercentage)))
		{
			isLastRegionOnEntranceSide = true;
		}
	}
	else if (camera.entrance_side == dto::Camera::side::top)
	{
		if (track.regions.at(0).maxY <= (int)(camera.height * borderPercentage))
		{
			isFirstRegionOnEntranceSide = true;
		}
		if (track.regions.at(track.regions.size() - 1).maxY <= (int)(camera.height * borderPercentage))
		{
			isLastRegionOnEntranceSide = true;
		}
	}
	else if (camera.entrance_side == dto::Camera::side::right)
	{
		if (track.regions.at(0).maxX >= (camera.width - (int)(camera.width * borderPercentage)))
		{
			isFirstRegionOnEntranceSide = true;
		}
		if (track.regions.at(track.regions.size() - 1).maxX >= (camera.width - (int)(camera.width * borderPercentage)))
		{
			isLastRegionOnEntranceSide = true;
		}
	}
	else if (camera.entrance_side == dto::Camera::side::left)
	{
		if (track.regions.at(0).maxX <= (camera.width * borderPercentage))
		{
			isFirstRegionOnEntranceSide = true;
		}
		if (track.regions.at(track.regions.size() - 1).maxX <= (int)(camera.width * borderPercentage))
		{
			isLastRegionOnEntranceSide = true;
		}
	}


	// Get walking direction
	if (isFirstRegionOnEntranceSide == true && isLastRegionOnEntranceSide == true) track.walkingDirection = dto::Track::WalkingDirection::out_out;
	if (isFirstRegionOnEntranceSide == true && isLastRegionOnEntranceSide == false) track.walkingDirection = dto::Track::WalkingDirection::out_in;
	if (isFirstRegionOnEntranceSide == false && isLastRegionOnEntranceSide == true) track.walkingDirection = dto::Track::WalkingDirection::in_out;
	if (isFirstRegionOnEntranceSide == false && isLastRegionOnEntranceSide == false) track.walkingDirection = dto::Track::WalkingDirection::in_in;

}
