#pragma once
#include "../dto/Camera.h"
#include "../dto/Track.h"
#include "../dto/SQLHelper.h"

/*! \file TrackPersistor.h
*	\brief Contains TrackPersistor class.
*
* This file contains the TrackPersistor class.
* It is used to persist all relevant properties
* of a track in the central database.
*/

/*! \brief The feature_extraction namespace contains all classes used for feature extraction.*/
namespace feature_extraction
{
	/*! \brief This class is used to persist tracks on the sql database.*/
	class TrackPersistor
	{
	public:
		/*! \brief Default constructor.*/
		TrackPersistor();
		/*! \brief Default destructor.*/
		~TrackPersistor();
	
		/*! \brief Persist a track in the sql database.*/
		void persistTrack(dto::Track& track, dto::Camera& camera) const;

	private:
		dto::SQLHelper sql_helper;
	};
}
