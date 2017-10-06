#include "lib_image_acquisition.h"
#include <cstdio>

/*! \brief Dummy CUDA kernel with no practical use
 *
 * This kernel does not run anything on the GPU, but can be used
 * to test if it is possible to properly lunch a kernel on the GPU.
 */
//__global__ void dummyKernel()
//{
//	return;
//}

void printSomething()
{
	printf("something");
}

__global__ void dummyKernel()
{
	return;
}