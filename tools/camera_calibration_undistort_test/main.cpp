#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <cctype>
#include <stdio.h>
#include <string.h>
#include <time.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	auto fx = 1.5429064838570325e+03;
	auto cx = 9.5369579797955782e+02;
	auto fy = 1.5429064838570325e+03;
	auto cy = 5.7690440309632538e+02;
	Mat cameraMatrix = (Mat1d(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);

	auto k1 = -6.2252844915595262e-01;
	auto k2 = 2.9966919648514401e-01;
	auto p1 = -8.8257543325885261e-03;
	auto p2 = -4.3121497449825881e-03;
	auto k3 = 3.7852955947755049e-02;
	Mat distCoeffs = (Mat1d(1, 5) << k1, k2, p1, p2, k3);

	Size imageSize(1920, 1080);

	Mat map1, map2;

	initUndistortRectifyMap(
		cameraMatrix, distCoeffs, Mat(),
		getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0), imageSize,
		CV_16SC2, map1, map2);

	Mat input, output;

	input = imread("C:\\data\\test-scenes\\calibration_videos\\images_door\\out_CAMERA_door_0000000098.mkv_snapshot_40.07_[2017.12.15_12.00.11].jpg", IMREAD_COLOR);

	remap(input, output, map1, map2, INTER_LINEAR);
	imshow("Image View", output);

	imwrite("c:\\temp\\out_calib_test.jpg", output);

	waitKey();

	return 0;
}
