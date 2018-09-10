#include "TEST_Filtering.hpp"

string getFilename(string path) {
	// Find right-most / or \\ character
	size_t found = path.rfind("/");
	if (found == string::npos) {
		found = path.rfind("\\");
	}

	// Didn't find anything; start at beginning
	if (found == string::npos) {
		found = 0;
	}

	size_t newLen = path.length() - found;
	string filename = path.substr(found+1, newLen);

	return filename;
}

double computeErrorSum(Mat processedImage, Mat groundImage) {
	// Compare output image to ground truth
	// (Some code taken from: https://docs.opencv.org/2.4/doc/tutorials/highgui/video-input-psnr-ssim/video-input-psnr-ssim.html)
	Mat diffImage;
	absdiff(processedImage, groundImage, diffImage);
	Mat floatDiffImage;
	diffImage.convertTo(floatDiffImage, CV_64F);
	floatDiffImage = floatDiffImage.mul(floatDiffImage);
	Scalar diff = sum(floatDiffImage);
	return diff[0];
}

bool checkForError(int testIndex, Mat computed, Mat ground) {

	// Compute error
	double error = computeErrorSum(computed, ground);

	cout << "\t" << "TEST " << testIndex << " total difference: " << error << endl;

	bool testPassed = true;
	if (error > EPSILON) {
		cout << "\t" << "TEST " << testIndex << ": FAILED" << endl;
		cout << "\t" << "COMPUTED: " << computed << endl;
		cout << "\t" << "GROUND: " << ground << endl;
		exit(1);
		testPassed = false;
	}

	return testPassed;
}

bool TEST_ONE_applyLinearFilter(int testIndex, Mat input, Mat filter, double scaleFactor) {

	// Compute filtered image
	Mat output;
	applyLinearFilter(input, filter, output, scaleFactor);

	// Compute ground image
	Mat groundOutput;
	cv::flip(filter, filter, -1);
	cv::filter2D(input, groundOutput, CV_64F, filter, cv::Point(-1,-1), 0.0, BORDER_CONSTANT);
	groundOutput *= scaleFactor;

	// Did we pass?
	return checkForError(testIndex, output, groundOutput);
}

bool TEST_applyLinearFilter(string filepath) {

	bool allPassed = true;

	// TEST 1 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyLinearFilter(
		1,
		(Mat_<uchar>(1, 6) << 0, 1, 1, 0, 2, 1),
		(Mat_<double>(1, 3) << 1, 2, 3),
		1.0);

	// TEST 2 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyLinearFilter(
		2,
		(Mat_<uchar>(1, 6) << 0, 1, 1, 0, 2, 1),
		(Mat_<double>(1, 3) << 1, 2, 3),
		1.0/6.0);

	// TEST 3 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyLinearFilter(
		3,
		(Mat_<uchar>(3, 6) <<  0, 1, 1, 0, 2, 1,
								1,2, 1, 4, 5, 1,
								7, 3, 2, 4, 2, 1),
		(Mat_<double>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9),
		1);

	// TEST 4 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyLinearFilter(
		4,
		(Mat_<uchar>(3, 6) <<
			0, 1, 1, 0, 2, 1,
			1, 2, 1, 4, 5, 1,
			7, 3, 2, 4, 2, 1),
		(Mat_<double>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9),
		1.0 / 45.0);

	// TEST 5 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyLinearFilter(
		5,
		(Mat_<uchar>(3, 6) <<
			0, 1, 1, 0, 2, 1,
			1, 2, 1, 4, 5, 1,
			7, 3, 2, 4, 2, 1),
		(Mat_<double>(3, 3) << -3, -2, -1, 0, 1, 2, 3, 4, 5),
		1.0);

	// TEST 6 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyLinearFilter(
		6,
		(Mat_<uchar>(3, 6) <<
			0, 1, 1, 0, 2, 1,
			1, 2, 1, 4, 5, 1,
			7, 3, 2, 4, 2, 1),
			(Mat_<double>(3, 3) << -3, -2, -1, 0, 1, 2, 3, 4, 5),
		1.0/21.0);

	// TEST 7 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyLinearFilter(
		7,
		(Mat_<uchar>(3, 6) <<
			0, 1, 1, 0, 2, 1,
			1, 2, 1, 4, 5, 1,
			7, 3, 2, 4, 2, 1),
			(Mat_<double>(3, 1) << 1, 2, 3),
		1.0/6.0);

	// TEST 8 //////////////////////////////////////////
	Mat image = imread(filepath, IMREAD_GRAYSCALE);

	allPassed &= TEST_ONE_applyLinearFilter(
		8,
		image,
		(Mat_<double>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9),
		1.0 / 45.0);

	return allPassed;
}

bool TEST_ONE_applyBoxFilter(int testIndex, Mat input, int filterWidth, int filterHeight) {

	// Compute filtered image
	Mat output;
	applyBoxFilter(input, filterWidth, filterHeight, output);

	// Compute ground image
	Mat groundOutput;
	cv::boxFilter(input, groundOutput, CV_64F, Size(filterWidth, filterHeight), Point(-1, -1), true, BORDER_CONSTANT);

	// Did we pass?
	return checkForError(testIndex, output, groundOutput);
}

bool TEST_applyBoxFilter(string filepath) {

	bool allPassed = true;

	// TEST 1 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyBoxFilter(
		1,
		(Mat_<uchar>(1, 6) << 0, 1, 1, 0, 2, 1),
		3, 1);

	// TEST 2 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyBoxFilter(
		2,
		(Mat_<uchar>(1, 6) << 0, 1, 1, 0, 2, 1),
		1, 3);

	// TEST 3 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyBoxFilter(
		3,
		(Mat_<uchar>(3, 6) << 0, 1, 1, 0, 2, 1,
			1, 2, 1, 4, 5, 1,
			7, 3, 2, 4, 2, 1),
			3,3);

	// TEST 4 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyBoxFilter(
		4,
		(Mat_<uchar>(3, 6) <<
			0, 1, 1, 0, 2, 1,
			1, 2, 1, 4, 5, 1,
			7, 3, 2, 4, 2, 1),
			1, 3);

	// TEST 5 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyBoxFilter(
		5,
		(Mat_<uchar>(3, 6) <<
			0, 1, 1, 0, 2, 1,
			1, 2, 1, 4, 5, 1,
			7, 3, 2, 4, 2, 1),
			3,5);

	// TEST 6 //////////////////////////////////////////
	Mat image = imread(filepath, IMREAD_GRAYSCALE);

	allPassed &= TEST_ONE_applyBoxFilter(
		6,
		image,
		3, 3);

	// TEST 7 //////////////////////////////////////////
	image = imread(filepath, IMREAD_GRAYSCALE);

	allPassed &= TEST_ONE_applyBoxFilter(
		7,
		image,
		11, 11);

	return allPassed;
}

bool TEST_ONE_applyGaussian3x3(int testIndex, Mat input) {
	// Compute filtered image
	Mat output;
	applyGaussian3x3(input, output);

	// Compute ground image
	Mat groundOutput;
	Mat inputFloat;
	input.convertTo(inputFloat, CV_64F);
	cv::GaussianBlur(inputFloat, groundOutput, Size(3, 3), 0, 0, BORDER_CONSTANT);

	// Did we pass?
	return checkForError(testIndex, output, groundOutput);
}

bool TEST_applyGaussian3x3(string filepath) {

	bool allPassed = true;

	// TEST 1 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyGaussian3x3(
		1,
		(Mat_<uchar>(1, 6) << 0, 1, 1, 0, 2, 1));

	// TEST 2 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyGaussian3x3(
		2,
		(Mat_<uchar>(1, 6) << 0, 1, 1, 0, 2, 1));

	// TEST 3 //////////////////////////////////////////

	allPassed &= TEST_ONE_applyGaussian3x3(
		3,
		(Mat_<uchar>(3, 6) <<
			0, 1, 1, 0, 2, 1,
			1, 2, 1, 4, 5, 1,
			7, 3, 2, 4, 2, 1));


	// TEST 4 //////////////////////////////////////////
	Mat image = imread(filepath, IMREAD_GRAYSCALE);

	allPassed &= TEST_ONE_applyGaussian3x3(
		4,
		image);

	return allPassed;
}

bool TEST_ONE_applySobel3x3(int testIndex, Mat input, bool isVertical) {
	// Compute filtered image
	Mat output;
	applySobel3x3(input, isVertical, output);

	// Compute ground image
	Mat groundOutput;
	int dx, dy;
	if (isVertical) {
		dx = 0;
		dy = 1;
	}
	else {
		dx = 1;
		dy = 0;
	}
	cv::Sobel(input, groundOutput, CV_64F, dx, dy, 3, 0.25, 0.0, BORDER_CONSTANT);

	// Did we pass?
	return checkForError(testIndex, output, groundOutput);
}

bool TEST_applySobel3x3(string filepath) {

	bool allPassed = true;

	// TEST 1 //////////////////////////////////////////

	allPassed &= TEST_ONE_applySobel3x3(
		1,
		(Mat_<uchar>(1, 6) << 0, 1, 1, 0, 2, 1), true);

	// TEST 2 //////////////////////////////////////////

	allPassed &= TEST_ONE_applySobel3x3(
		2,
		(Mat_<uchar>(1, 6) << 0, 1, 1, 0, 2, 1), false);

	// TEST 3 //////////////////////////////////////////

	allPassed &= TEST_ONE_applySobel3x3(
		3,
		(Mat_<uchar>(3, 6) <<
			0, 1, 1, 0, 2, 1,
			1, 2, 1, 4, 5, 1,
			7, 3, 2, 4, 2, 1),
		true);

	// TEST 4 //////////////////////////////////////////

	allPassed &= TEST_ONE_applySobel3x3(
		4,
		(Mat_<uchar>(3, 6) <<
			0, 1, 1, 0, 2, 1,
			1, 2, 1, 4, 5, 1,
			7, 3, 2, 4, 2, 1),
		false);


	// TEST 6 //////////////////////////////////////////
	Mat image = imread(filepath, IMREAD_GRAYSCALE);

	allPassed &= TEST_ONE_applySobel3x3(
		6,
		image,
		true);

	// TEST 7 //////////////////////////////////////////
	image = imread(filepath, IMREAD_GRAYSCALE);

	allPassed &= TEST_ONE_applySobel3x3(
		7,
		image,
		false);

	return allPassed;
}
