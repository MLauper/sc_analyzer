#include "DirectionExtractor.h"
#include "../dto/Camera.h"
#include "../dto/Configuration.h"

feature_extraction::DirectionExtractor::DirectionExtractor()
{
}

void feature_extraction::DirectionExtractor::extractDirection(dto::Track& track, dto::Camera& camera)
{
	if (dto::Configuration::USE_YOLO_FOR_TRACKING)
	{
		const auto& firstPerson = track.persons.at(0);
		const auto& lastPerson = track.persons.back();

		bool isFirstRegionOnEntranceSide = false;
		bool isLastRegionOnEntranceSide = false;

		float borderPercentage = dto::Configuration::IMAGE_BORDER_PERCENTATGE;

		// Analyse Region Positions
		if (camera.entry_side == dto::Camera::entrySide::entry_bottom)
		{
			if (firstPerson.y + firstPerson.h >= (camera.height - (int)(camera.height * borderPercentage)))
			{
				isFirstRegionOnEntranceSide = true;
			}
			if (lastPerson.y + lastPerson.h >= (camera.height - (int)(camera.height * borderPercentage)))
			{
				isLastRegionOnEntranceSide = true;
			}
		}
		else if (camera.entry_side == dto::Camera::entrySide::entry_top)
		{
			if (firstPerson.y <= (int)(camera.height * borderPercentage))
			{
				isFirstRegionOnEntranceSide = true;
			}
			if (lastPerson.y <= (int)(camera.height * borderPercentage))
			{
				isLastRegionOnEntranceSide = true;
			}
		}
		else if (camera.entry_side == dto::Camera::entrySide::entry_right)
		{
			if (firstPerson.x + firstPerson.w >= (camera.width - (int)(camera.width * borderPercentage)))
			{
				isFirstRegionOnEntranceSide = true;
			}
			if (lastPerson.x + lastPerson.w >= (camera.width - (int)(camera.width * borderPercentage)))
			{
				isLastRegionOnEntranceSide = true;
			}
		}
		else if (camera.entry_side == dto::Camera::entrySide::entry_left)
		{
			if (firstPerson.x <= (camera.width * borderPercentage))
			{
				isFirstRegionOnEntranceSide = true;
			}
			if (lastPerson.x <= (int)(camera.width * borderPercentage))
			{
				isLastRegionOnEntranceSide = true;
			}
		}


		// Get walking direction
		if (isFirstRegionOnEntranceSide == true && isLastRegionOnEntranceSide == true)
			track.walkingDirection = dto::Track::
				WalkingDirection::out_out;
		if (isFirstRegionOnEntranceSide == true && isLastRegionOnEntranceSide == false)
			track.walkingDirection = dto::Track::
				WalkingDirection::out_in;
		if (isFirstRegionOnEntranceSide == false && isLastRegionOnEntranceSide == true)
			track.walkingDirection = dto::Track::
				WalkingDirection::in_out;
		if (isFirstRegionOnEntranceSide == false && isLastRegionOnEntranceSide == false)
			track.walkingDirection = dto::Track::
				WalkingDirection::in_in;
	}
	else
	{
		std::cout << "WE ARE USING CAMERA " << camera.directory << " with prefix " << camera.prefix << std::endl;

		bool isFirstRegionOnEntranceSide = false;
		bool isLastRegionOnEntranceSide = false;

		float borderPercentage = 0.03f;

		// Analyse Region Positions
		if (camera.entry_side == dto::Camera::entrySide::entry_bottom)
		{
			if (track.regions.at(0).maxY >= (camera.height - (int)(camera.height * borderPercentage)))
			{
				isFirstRegionOnEntranceSide = true;
			}
			if (track.regions.at(track.regions.size() - 1).maxY >= (camera.height - (int)(camera.height * borderPercentage)))
			{
				isLastRegionOnEntranceSide = true;
			}
		}
		else if (camera.entry_side == dto::Camera::entrySide::entry_top)
		{
			if (track.regions.at(0).minY <= (int)(camera.height * borderPercentage))
			{
				isFirstRegionOnEntranceSide = true;
			}
			if (track.regions.at(track.regions.size() - 1).minY <= (int)(camera.height * borderPercentage))
			{
				isLastRegionOnEntranceSide = true;
			}
		}
		else if (camera.entry_side == dto::Camera::entrySide::entry_right)
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
		else if (camera.entry_side == dto::Camera::entrySide::entry_left)
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
		if (isFirstRegionOnEntranceSide == true && isLastRegionOnEntranceSide == true)
			track.walkingDirection = dto::Track::
				WalkingDirection::out_out;
		if (isFirstRegionOnEntranceSide == true && isLastRegionOnEntranceSide == false)
			track.walkingDirection = dto::Track::
				WalkingDirection::out_in;
		if (isFirstRegionOnEntranceSide == false && isLastRegionOnEntranceSide == true)
			track.walkingDirection = dto::Track::
				WalkingDirection::in_out;
		if (isFirstRegionOnEntranceSide == false && isLastRegionOnEntranceSide == false)
			track.walkingDirection = dto::Track::
				WalkingDirection::in_in;
	}


	if (dto::Configuration::SAVE_TRACK_STATISTICS)
	{
		std::ofstream fileStream;
		std::stringstream filePath;
		filePath << dto::Configuration::STATISTICS_DIRECTORY << "scene-" << camera.scene << "\\" << camera.prefix << "\\" <<
			"Track-" << track.trackId << "_statistics.txt";
		fileStream.open(filePath.str().c_str(), std::fstream::app);
		switch (track.walkingDirection)
		{
		case dto::Track::WalkingDirection::out_out:
			fileStream << "Walking Direction: " << "out_out" << std::endl;
			break;
		case dto::Track::WalkingDirection::out_in:
			fileStream << "Walking Direction: " << "out_in" << std::endl;
			break;
		case dto::Track::WalkingDirection::in_out:
			fileStream << "Walking Direction: " << "in_out" << std::endl;
			break;
		case dto::Track::WalkingDirection::in_in:
			fileStream << "Walking Direction: " << "in_in" << std::endl;
			break;
		}

		fileStream.close();
	}
}
