#ifndef LIB_IMAGE_ACQUISITION
#define LIB_IMAGE_ACQUISITION
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

/*! \file lib_image_acquisition.h
 *	\brief Provides functions for the image acquisition stage
 *	
 *	This file contains all functions for the image acquisition
 *	stage. This includes connecting to cameras, extracting
 *	images and provide them in an useable format.
 */

 __global__ void dummyKernel();

 void printSomething();

#endif // LIB_IMAGE_ACQUISITION