#define NOMINMAX
#include <ostream>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


int main()
{
	std::string sourceFile = "C:\\data\\test-scenes-cut\\FeatureMatching\\Image_Frontal_Room_cut.jpg";
	std::string testFile = "C:\\data\\test-scenes-cut\\FeatureMatching\\Image_Frontal_Door_cut.jpg";
	//std::string testFile = "C:\\data\\test-scenes-cut\\FeatureMatching\\Other_Person_Frontal_Door_cut.jpg";

	const cv::Mat source = cv::imread(sourceFile.c_str());
	const cv::Mat test = cv::imread(testFile.c_str());

	const int minHessian = 400;
	cv::Ptr<cv::xfeatures2d::SURF> surf_detector_ = cv::xfeatures2d::SURF::create(minHessian);
	cv::Ptr<cv::xfeatures2d::SIFT> sift_detector_ = cv::xfeatures2d::SIFT::create();

	std::vector<cv::KeyPoint> sourceSurfKeypoints;
	std::vector<cv::KeyPoint> testSurfKeypoints;
	cv::Mat sourceSurfDescriptors;
	cv::Mat testSurfDescriptors;
	surf_detector_->detectAndCompute(source, cv::Mat(), sourceSurfKeypoints, sourceSurfDescriptors);
	surf_detector_->detectAndCompute(test, cv::Mat(), testSurfKeypoints, testSurfDescriptors);

	//	std::vector<cv::KeyPoint> sourceSiftKeypoints;
	//	std::vector<cv::KeyPoint> testSiftKeypoints;
	//	sift_detector_->detect(source, sourceSiftKeypoints);
	//	sift_detector_->detect(test, testSiftKeypoints);

	cv::BFMatcher bf_matcher_;
	std::vector<cv::DMatch> surfMatches;
	//	std::vector<cv::DMatch> siftMatches;
	bf_matcher_.match(sourceSurfDescriptors, testSurfDescriptors, surfMatches);
	//	bf_matcher_->match(testSiftKeypoints, sourceSiftKeypoints, siftMatches);

	std::cout << "Found " << surfMatches.size() << " matches with surf" << std::endl;
	//	std::cout << "Found " << siftMatches.size() << " matches with sift" << std::endl;

	double max_dist = 0;
	double min_dist = 100;

	for (int i = 0; i < sourceSurfDescriptors.rows; i++)
	{
		const double dist = surfMatches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	std::vector<cv::DMatch> good_matches;
	for (int i = 0; i < sourceSurfDescriptors.rows; i++)
	{
		if (surfMatches[i].distance <= 0.25)
		{
			good_matches.push_back(surfMatches[i]);
		}
	}

	std::cout << "Found " << good_matches.size() << " good matches with surf" << std::endl;

	cv::Mat img_matches;
	drawMatches(source, sourceSurfKeypoints, test, testSurfKeypoints,
	            good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
	            std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("Good Matches", img_matches);

	imwrite("c:\\temp\\matches.jpg", img_matches);

	cv::waitKey(0);

	std::string temp;
	std::cin >> temp;
}
