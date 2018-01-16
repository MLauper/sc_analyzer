#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <cctype>
#include <stdio.h>
//#include <string.h>
#include <time.h>

using namespace cv;
using namespace std;

const char* usage =
	" \nexample command line for calibration from a live feed.\n"
	"   calibration  -w=4 -h=5 -s=0.025 -o=camera.yml -op -oe\n"
	" \n"
	" example command line for calibration from a list of stored images:\n"
	"   imagelist_creator image_list.xml *.png\n"
	"   calibration -w=4 -h=5 -s=0.025 -o=camera.yml -op -oe image_list.xml\n"
	" where image_list.xml is the standard OpenCV XML/YAML\n"
	" use imagelist_creator to create the xml or yaml list\n"
	" file consisting of the list of strings, e.g.:\n"
	" \n"
	"<?xml version=\"1.0\"?>\n"
	"<opencv_storage>\n"
	"<images>\n"
	"view000.png\n"
	"view001.png\n"
	"<!-- view002.png -->\n"
	"view003.png\n"
	"view010.png\n"
	"one_extra_view.jpg\n"
	"</images>\n"
	"</opencv_storage>\n";


const char* live_capture_help =
	"When the live video from camera is used as input, the following hot-keys may be used:\n"
	"  <ESC>, 'q' - quit the program\n"
	"  'g' - start capturing images\n"
	"  'u' - switch undistortion on/off\n";

static void help()
{
	printf("This is a camera calibration sample.\n"
		"Usage: calibration\n"
		"     -w=<board_width>         # the number of inner corners per one of board dimension\n"
		"     -h=<board_height>        # the number of inner corners per another board dimension\n"
		"     [-pt=<pattern>]          # the type of pattern: chessboard or circles' grid\n"
		"     [-n=<number_of_frames>]  # the number of frames to use for calibration\n"
		"                              # (if not specified, it will be set to the number\n"
		"                              #  of board views actually available)\n"
		"     [-d=<delay>]             # a minimum delay in ms between subsequent attempts to capture a next view\n"
		"                              # (used only for video capturing)\n"
		"     [-s=<squareSize>]       # square size in some user-defined units (1 by default)\n"
		"     [-o=<out_camera_params>] # the output filename for intrinsic [and extrinsic] parameters\n"
		"     [-op]                    # write detected feature points\n"
		"     [-oe]                    # write extrinsic parameters\n"
		"     [-zt]                    # assume zero tangential distortion\n"
		"     [-a=<aspectRatio>]      # fix aspect ratio (fx/fy)\n"
		"     [-p]                     # fix the principal point at the center\n"
		"     [-v]                     # flip the captured images around the horizontal axis\n"
		"     [-V]                     # use a video file, and not an image list, uses\n"
		"                              # [input_data] string for the video file name\n"
		"     [-su]                    # show undistorted images after calibration\n"
		"     [input_data]             # input data, one of the following:\n"
		"                              #  - text file with a list of the images of the board\n"
		"                              #    the text file can be generated with imagelist_creator\n"
		"                              #  - name of video file with a video of the board\n"
		"                              # if input_data not specified, a live view from the camera is used\n"
		"\n");
	printf("\n%s", usage);
	printf("\n%s", live_capture_help);
}

enum { detection = 0, capturing = 1, calibrated = 2 };

enum pattern { chessboard, circles_grid, asymmetric_circles_grid };

static double computeReprojectionErrors(
	const vector<vector<Point3f>>& object_points,
	const vector<vector<Point2f>>& image_points,
	const vector<Mat>& rvecs, const vector<Mat>& tvecs,
	const Mat& camera_matrix, const Mat& dist_coeffs,
	vector<float>& per_view_errors)
{
	vector<Point2f> image_points2;
	auto total_points = 0;
	double total_err = 0;
	per_view_errors.resize(object_points.size());

	for (auto i = 0; i < static_cast<int>(object_points.size()); i++)
	{
		projectPoints(Mat(object_points[i]), rvecs[i], tvecs[i],
		              camera_matrix, dist_coeffs, image_points2);
		const auto err = norm(Mat(image_points[i]), Mat(image_points2), NORM_L2);
		const auto n = static_cast<int>(object_points[i].size());
		per_view_errors[i] = static_cast<float>(std::sqrt(err * err / n));
		total_err += err * err;
		total_points += n;
	}

	return std::sqrt(total_err / total_points);
}

static void calc_chessboard_corners(const Size board_size, const float square_size, vector<Point3f>& corners,
                                    const pattern pattern_type = chessboard)
{
	corners.resize(0);

	switch (pattern_type)
	{
	case chessboard:
	case circles_grid:
		for (auto i = 0; i < board_size.height; i++)
			for (auto j = 0; j < board_size.width; j++)
				corners.push_back(Point3f(float(j * square_size),
				                          float(i * square_size), 0));
		break;

	case asymmetric_circles_grid:
		for (auto i = 0; i < board_size.height; i++)
			for (auto j = 0; j < board_size.width; j++)
				corners.push_back(Point3f(float((2 * j + i % 2) * square_size),
				                          float(i * square_size), 0));
		break;

	default:
		CV_Error(Error::StsBadArg, "Unknown pattern type\n");
	}
}

static bool runCalibration(vector<vector<Point2f>> image_points,
                           const Size image_size, const Size board_size, const pattern pattern_type,
                           const float square_size, const float aspect_ratio,
                           const int flags, Mat& camera_matrix, Mat& dist_coeffs,
                           vector<Mat>& rvecs, vector<Mat>& tvecs,
                           vector<float>& reproj_errs,
                           double& total_avg_err)
{
	camera_matrix = Mat::eye(3, 3, CV_64F);
	if (flags & CALIB_FIX_ASPECT_RATIO)
		camera_matrix.at<double>(0, 0) = aspect_ratio;

	dist_coeffs = Mat::zeros(8, 1, CV_64F);

	vector<vector<Point3f>> object_points(1);
	calc_chessboard_corners(board_size, square_size, object_points[0], pattern_type);

	object_points.resize(image_points.size(), object_points[0]);

	const auto rms = calibrateCamera(object_points, image_points, image_size, camera_matrix,
	                                 dist_coeffs, rvecs, tvecs, flags | CALIB_FIX_K4 | CALIB_FIX_K5);
	///*|CALIB_FIX_K3*/|CALIB_FIX_K4|CALIB_FIX_K5);
	printf("RMS error reported by calibrateCamera: %g\n", rms);

	const auto ok = checkRange(camera_matrix) && checkRange(dist_coeffs);

	total_avg_err = computeReprojectionErrors(object_points, image_points,
	                                          rvecs, tvecs, camera_matrix, dist_coeffs, reproj_errs);

	return ok;
}


static void saveCameraParams(const string& filename,
                             const Size image_size, const Size board_size,
                             const float square_size, const float aspect_ratio, const int flags,
                             const Mat& camera_matrix, const Mat& dist_coeffs,
                             const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                             const vector<float>& reproj_errs,
                             const vector<vector<Point2f>>& image_points,
                             const double total_avg_err)
{
	FileStorage fs(filename, FileStorage::WRITE);

	time_t tt;
	time(&tt);
	const auto t2 = localtime(&tt);
	char buf[1024];
	strftime(buf, sizeof buf - 1, "%c", t2);

	fs << "calibration_time" << buf;

	if (!rvecs.empty() || !reproj_errs.empty())
		fs << "nframes" << static_cast<int>(std::max(rvecs.size(), reproj_errs.size()));
	fs << "image_width" << image_size.width;
	fs << "image_height" << image_size.height;
	fs << "board_width" << board_size.width;
	fs << "board_height" << board_size.height;
	fs << "square_size" << square_size;

	if (flags & CALIB_FIX_ASPECT_RATIO)
		fs << "aspectRatio" << aspect_ratio;

	if (flags != 0)
	{
		sprintf(buf, "flags: %s%s%s%s",
		        flags & CALIB_USE_INTRINSIC_GUESS ? "+use_intrinsic_guess" : "",
		        flags & CALIB_FIX_ASPECT_RATIO ? "+fix_aspectRatio" : "",
		        flags & CALIB_FIX_PRINCIPAL_POINT ? "+fix_principal_point" : "",
		        flags & CALIB_ZERO_TANGENT_DIST ? "+zero_tangent_dist" : "");
		//cvWriteComment( *fs, buf, 0 );
	}

	fs << "flags" << flags;

	fs << "camera_matrix" << camera_matrix;
	fs << "distortion_coefficients" << dist_coeffs;

	fs << "avg_reprojection_error" << total_avg_err;
	if (!reproj_errs.empty())
		fs << "per_view_reprojection_errors" << Mat(reproj_errs);

	if (!rvecs.empty() && !tvecs.empty())
	{
		CV_Assert(rvecs[0].type() == tvecs[0].type())		;
		const Mat bigmat(static_cast<int>(rvecs.size()), 6, rvecs[0].type());
		for (auto i = 0; i < static_cast<int>(rvecs.size()); i++)
		{
			auto r = bigmat(Range(i, i + 1), Range(0, 3));
			auto t = bigmat(Range(i, i + 1), Range(3, 6));

			CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1)			;
			CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1)			;
			//*.t() is MatExpr (not Mat) so we can use assignment operator
			r = rvecs[i].t();
			t = tvecs[i].t();
		}
		//cvWriteComment( *fs, "a set of 6-tuples (rotation vector + translation vector) for each view", 0 );
		fs << "extrinsic_parameters" << bigmat;
	}

	if (!image_points.empty())
	{
		Mat image_pt_mat(static_cast<int>(image_points.size()), static_cast<int>(image_points[0].size()), CV_32FC2);
		for (auto i = 0; i < static_cast<int>(image_points.size()); i++)
		{
			auto r = image_pt_mat.row(i).reshape(2, image_pt_mat.cols);
			Mat imgpti(image_points[i]);
			imgpti.copyTo(r);
		}
		fs << "image_points" << image_pt_mat;
	}
}

static bool read_string_list(const string& filename, vector<string>& l)
{
	l.resize(0);
	FileStorage fs(filename, FileStorage::READ);
	if (!fs.isOpened())
		return false;
	auto n = fs.getFirstTopLevelNode();
	if (n.type() != FileNode::SEQ)
		return false;
	auto it = n.begin();
	const auto it_end = n.end();
	for (; it != it_end; ++it)
		l.push_back(static_cast<string>(*it));
	return true;
}


static bool run_and_save(const string& outputFilename,
                         const vector<vector<Point2f>>& imagePoints,
                         const Size imageSize, const Size boardSize, const pattern patternType, const float squareSize,
                         const float aspectRatio, const int flags, Mat& cameraMatrix,
                         Mat& distCoeffs, const bool writeExtrinsics, const bool writePoints)
{
	vector<Mat> rvecs, tvecs;
	vector<float> reprojErrs;
	double totalAvgErr = 0;

	const auto ok = runCalibration(imagePoints, imageSize, boardSize, patternType, squareSize,
	                               aspectRatio, flags, cameraMatrix, distCoeffs,
	                               rvecs, tvecs, reprojErrs, totalAvgErr);
	printf("%s. avg reprojection error = %.2f\n",
	       ok ? "Calibration succeeded" : "Calibration failed",
	       totalAvgErr);

	if (ok)
		saveCameraParams(outputFilename, imageSize,
		                 boardSize, squareSize, aspectRatio,
		                 flags, cameraMatrix, distCoeffs,
		                 writeExtrinsics ? rvecs : vector<Mat>(),
		                 writeExtrinsics ? tvecs : vector<Mat>(),
		                 writeExtrinsics ? reprojErrs : vector<float>(),
		                 writePoints ? imagePoints : vector<vector<Point2f>>(),
		                 totalAvgErr);
	return ok;
}


int main(const int argc, char** argv)
{
	Size boardSize, imageSize;
	Mat cameraMatrix, distCoeffs;
	string outputFilename;
	string inputFilename = "";

	int i;
	auto undistortImage = false;
	auto flags = 0;
	VideoCapture capture;
	clock_t prevTimestamp = 0;
	int mode = detection;
	auto cameraId = 0;
	vector<vector<Point2f>> imagePoints;
	vector<string> imageList;
	pattern pattern = chessboard;

	CommandLineParser parser(argc, argv,
	                         "{help ||}{w||}{h||}{pt|chessboard|}{n|10|}{d|1000|}{s|1|}{o|out_camera_data.yml|}"
	                         "{op||}{oe||}{zt||}{a|1|}{p||}{v||}{V||}{su||}"
	                         "{@input_data|0|}");
	if (parser.has("help"))
	{
		help();
		return 0;
	}
	boardSize.width = parser.get<int>("w");
	boardSize.height = parser.get<int>("h");
	if (parser.has("pt"))
	{
		const auto val = parser.get<string>("pt");
		if (val == "circles")
			pattern = circles_grid;
		else if (val == "acircles")
			pattern = asymmetric_circles_grid;
		else if (val == "chessboard")
			pattern = chessboard;
		else
			return fprintf(stderr, "Invalid pattern type: must be chessboard or circles\n"), -1;
	}
	const auto squareSize = parser.get<float>("s");
	auto nframes = parser.get<int>("n");
	const auto aspectRatio = parser.get<float>("a");
	const auto delay = parser.get<int>("d");
	const auto writePoints = parser.has("op");
	const auto writeExtrinsics = parser.has("oe");
	if (parser.has("a"))
		flags |= CALIB_FIX_ASPECT_RATIO;
	if (parser.has("zt"))
		flags |= CALIB_ZERO_TANGENT_DIST;
	if (parser.has("p"))
		flags |= CALIB_FIX_PRINCIPAL_POINT;
	const auto flipVertical = parser.has("v");
	const auto videofile = parser.has("V");
	if (parser.has("o"))
		outputFilename = parser.get<string>("o");
	const auto showUndistorted = parser.has("su");
	if (isdigit(parser.get<string>("@input_data")[0]))
		cameraId = parser.get<int>("@input_data");
	else
		inputFilename = parser.get<string>("@input_data");
	if (!parser.check())
	{
		help();
		parser.printErrors();
		return -1;
	}
	if (squareSize <= 0)
		return fprintf(stderr, "Invalid board square width\n"), -1;
	if (nframes <= 3)
		return printf("Invalid number of images\n"), -1;
	if (aspectRatio <= 0)
		return printf("Invalid aspect ratio\n"), -1;
	if (delay <= 0)
		return printf("Invalid delay\n"), -1;
	if (boardSize.width <= 0)
		return fprintf(stderr, "Invalid board width\n"), -1;
	if (boardSize.height <= 0)
		return fprintf(stderr, "Invalid board height\n"), -1;

	if (!inputFilename.empty())
	{
		if (!videofile && read_string_list(inputFilename, imageList))
			mode = capturing;
		else
			capture.open(inputFilename);
	}
	else
		capture.open(cameraId);

	if (!capture.isOpened() && imageList.empty())
		return fprintf(stderr, "Could not initialize video (%d) capture\n", cameraId), -2;

	if (!imageList.empty())
		nframes = static_cast<int>(imageList.size());

	if (capture.isOpened())
		printf("%s", live_capture_help);

	namedWindow("Image View", 1);

	for (i = 0;; i++)
	{
		Mat view, viewGray;
		auto blink = false;

		if (capture.isOpened())
		{
			Mat view0;
			capture >> view0;
			view0.copyTo(view);
		}
		else if (i < static_cast<int>(imageList.size()))
			view = imread(imageList[i], 1);

		if (view.empty())
		{
			if (imagePoints.size() > 0)
				run_and_save(outputFilename, imagePoints, imageSize,
				             boardSize, pattern, squareSize, aspectRatio,
				             flags, cameraMatrix, distCoeffs,
				             writeExtrinsics, writePoints);
			break;
		}

		imageSize = view.size();

		if (flipVertical)
			flip(view, view, 0);

		vector<Point2f> pointbuf;
		cvtColor(view, viewGray, COLOR_BGR2GRAY);

		bool found;
		switch (pattern)
		{
		case chessboard:
			found = findChessboardCorners(view, boardSize, pointbuf,
			                              CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
			break;
		case circles_grid:
			found = findCirclesGrid(view, boardSize, pointbuf);
			break;
		case asymmetric_circles_grid:
			found = findCirclesGrid(view, boardSize, pointbuf, CALIB_CB_ASYMMETRIC_GRID);
			break;
		default:
			return fprintf(stderr, "Unknown pattern type\n"), -1;
		}

		// improve the found corners' coordinate accuracy
		if (pattern == chessboard && found)
			cornerSubPix(viewGray, pointbuf, Size(11, 11),
			             Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));

		if (mode == capturing && found &&
			(!capture.isOpened() || clock() - prevTimestamp > delay * 1e-3 * CLOCKS_PER_SEC))
		{
			imagePoints.push_back(pointbuf);
			prevTimestamp = clock();
			blink = capture.isOpened();
		}

		if (found)
			drawChessboardCorners(view, boardSize, Mat(pointbuf), found);

		string msg = mode == capturing ? "100/100" : mode == calibrated ? "Calibrated" : "Press 'g' to start";
		auto baseLine = 0;
		const auto textSize = getTextSize(msg, 1, 1, 1, &baseLine);
		const Point textOrigin(view.cols - 2 * textSize.width - 10, view.rows - 2 * baseLine - 10);

		if (mode == capturing)
		{
			if (undistortImage)
				msg = format("%d/%d Undist", static_cast<int>(imagePoints.size()), nframes);
			else
				msg = format("%d/%d", static_cast<int>(imagePoints.size()), nframes);
		}

		putText(view, msg, textOrigin, 1, 1,
		        mode != calibrated ? Scalar(0, 0, 255) : Scalar(0, 255, 0));

		if (blink)
			bitwise_not(view, view);

		if (mode == calibrated && undistortImage)
		{
			const auto temp = view.clone();
			undistort(temp, view, cameraMatrix, distCoeffs);
		}

		imshow("Image View", view);
		const auto key = static_cast<char>(waitKey(capture.isOpened() ? 50 : 500));

		if (key == 27)
			break;

		if (key == 'u' && mode == calibrated)
			undistortImage = !undistortImage;

		if (capture.isOpened() && key == 'g')
		{
			mode = capturing;
			imagePoints.clear();
		}

		if (mode == capturing && imagePoints.size() >= static_cast<unsigned>(nframes))
		{
			if (run_and_save(outputFilename, imagePoints, imageSize,
			                 boardSize, pattern, squareSize, aspectRatio,
			                 flags, cameraMatrix, distCoeffs,
			                 writeExtrinsics, writePoints))
				mode = calibrated;
			else
				mode = detection;
			if (!capture.isOpened())
				break;
		}
	}

	if (!capture.isOpened() && showUndistorted)
	{
		Mat rview, map1, map2;
		initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
		                        getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, nullptr),
		                        imageSize, CV_16SC2, map1, map2);

		for (i = 0; i < static_cast<int>(imageList.size()); i++)
		{
			auto view = imread(imageList[i], 1);
			if (view.empty())
				continue;
			//undistort( view, rview, cameraMatrix, distCoeffs, cameraMatrix );
			remap(view, rview, map1, map2, INTER_LINEAR);
			imshow("Image View", rview);
			const auto c = static_cast<char>(waitKey());
			if (c == 27 || c == 'q' || c == 'Q')
				break;
		}
	}

	return 0;
}
