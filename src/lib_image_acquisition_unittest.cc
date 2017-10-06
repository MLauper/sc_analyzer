#include "lib_image_acquisition_unittest.h"


TEST(executeKernel, dummy)
{
	dummyKernel << < 1, 1 >> >();
	EXPECT_EQ(cudaSuccess, cudaGetLastError());
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}