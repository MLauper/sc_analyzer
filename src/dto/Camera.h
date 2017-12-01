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
	public:
		std::string directory;
		std::string prefix;
		side entrance_side;
		int width;
		int height;
	};
}