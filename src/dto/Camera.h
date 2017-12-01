#pragma once
#include <string>

namespace dto
{
	struct Camera
	{
		enum side
		{
			left,
			right,
			top,
			bottom,
			none
		};
		enum gateMode
		{
			minLeft,
			minRight,
			minTop,
			minBottom
		};
	public:
		std::string directory;
		std::string prefix;
		side entrance_side;
		int width;
		int height;

		gateMode gateMode;
		int gateValue;
	};
}