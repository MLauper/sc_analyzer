#include "RegionSelector.h"
#include "../dto/Track.h"
#include "../dto/Camera.h"

feature_extraction::RegionSelector::RegionSelector()
{
}

void feature_extraction::RegionSelector::SelectRegion(dto::Track& track, const dto::Camera& camera)
{
	int optimalRegionCandidate = 0;
	int candidateDistance = INT_MAX;

	if (camera.gateMode == dto::Camera::gateMode::minBottom)
	{
		for (int i = 0; i < track.regions.size(); i++){
			int distance = abs(camera.height - camera.gateValue - track.regions.at(i).maxY);
			if (distance < candidateDistance) {
				candidateDistance = distance;
				optimalRegionCandidate = i;
			}
		}
	} else if (camera.gateMode == dto::Camera::gateMode::minTop)
	{
		for (int i = 0; i < track.regions.size(); i++) {
			int distance = abs(camera.gateValue - track.regions.at(i).minY);
			if (distance < candidateDistance) {
				candidateDistance = distance;
				optimalRegionCandidate = i;
			}
		}
	} else if (camera.gateMode == dto::Camera::gateMode::minLeft)
	{
		for (int i = 0; i < track.regions.size(); i++)
		{
			int distance = abs(camera.gateValue - track.regions.at(i).minX);
			if (distance < candidateDistance) {
				candidateDistance = distance;
				optimalRegionCandidate = i;
			}
		}
	}
	else if (camera.gateMode == dto::Camera::gateMode::minRight)
	{
		for (int i = 0; i < track.regions.size(); i++)
		{
			int distance = abs(camera.width - camera.gateValue - track.regions.at(i).maxX);
			if (distance < candidateDistance) {
				candidateDistance = distance;
				optimalRegionCandidate = i;
			}
		}
	}

	track.optimalRegionId = optimalRegionCandidate;

	std::cout << "Selected Region Id " << track.optimalRegionId << "\n";
}
