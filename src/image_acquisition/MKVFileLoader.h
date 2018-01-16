#pragma once
#include <string>
#include "../image_segmentation/Controller.h"
#include "../dto/Camera.h"

/*! \file MKVFileLoader.h
*	\brief Contains MKVFileLoader class.
*
* This file contains the MKVFileLoader class.
* It is used to load one local MKV file and 
* stream each frame in the pipeline.
*/

/*! \brief The image_acquisition namespace contains all classes used for image acquisition.*/
namespace image_acquisition
{
	/*! \brief This class is used to streams frames from a local MKV file.*/
	class MKVFileLoader
	{
	public:
		/*! \brief Constructor.*/
		MKVFileLoader(dto::Camera& camera, image_segmentation::Controller* segmentation_controller);
		/*! \brief Default destructor.*/
		~MKVFileLoader();
		/*! \brief Process the configured mkv file.*/
		void process_file() const;
		/*! \brief Extract file name from file path.*/
		static std::string extract_filename(char const* path_c);
	private:
		dto::Camera camera;
		image_segmentation::Controller* segmentation_controller;
	};
}
