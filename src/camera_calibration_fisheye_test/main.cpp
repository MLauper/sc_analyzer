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
	auto fx = 7.4120290822874836e+02;
	auto cx = 1.3250517972817070e+03;
	auto fy = 7.3861519036843026e+02;
	auto cy = 9.6675701856178614e+02;
	Mat cameraMatrix = (Mat1d(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);

	auto k1 = -2.3735894034208730e-02;
	auto k2 = -2.7198599864624484e-02;
	auto p1 = 1.8642215800088423e-02;
	auto p2 = -5.8100785090142732e-03;
	Mat distCoeffs = (Mat1d(1, 4) << k1, k2, p1, p2);	 

	Size imageSize(2592, 1944);

	Mat map1, map2;

	Mat newCamMat;
	fisheye::estimateNewCameraMatrixForUndistortRectify(cameraMatrix, distCoeffs, imageSize,
		Matx33d::eye(), newCamMat, 0.99f);
	fisheye::initUndistortRectifyMap(cameraMatrix, distCoeffs, Matx33d::eye(), newCamMat, imageSize,
		CV_16SC2, map1, map2);

	//initUndistortRectifyMap(
	//	cameraMatrix, distCoeffs, Mat(),
	//	getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0), imageSize,
	//	CV_16SC2, map1, map2);

	Mat input, output;

	input = imread("C:\\Temp\\out_CAMERA_pan_overview_0000000151.mkv_snapshot_13.20.jpg", IMREAD_COLOR);

	remap(input, output, map1, map2, INTER_LINEAR);
	
	imwrite("c:\\Temp\\out_test.jpg", output);
	imshow("Image View", output);
	waitKey();

	return 0;
}
