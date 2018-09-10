#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "Filtering.hpp"
#include "TEST_Filtering.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv) {

	// If no command line arguments entered, exit
	if (argc < 4) {
		cout << "ERROR: Insufficient command line arguments!" << endl;
		cout << "[path to image] [path to directory for output] [testing? (0 or 1)]" << endl;
		return -1;
	}

	// Testing mode?
	int testingMode = atoi(argv[3]);

	// Get path to file
	string filepath = string(argv[1]);

	if (!testingMode) {
		// REGULAR APPLICATION

		// Get filename
		string filename = getFilename(filepath);

		// Load image
		cout << "Loading image: " << filepath << endl;
		Mat image = imread(filepath, IMREAD_GRAYSCALE);

		// Check if data is invalid
		if (!image.data) {
			cout << "ERROR: Could not open or find the image!" << endl;
			return -1;
		}
				
		// Box filtered image
		Mat boxFilteredImage;
		applyBoxFilter(image, 11, 11, boxFilteredImage);

		// Gaussian filtered image
		Mat gaussianFilteredImage;
		applyGaussian3x3(image, gaussianFilteredImage);
		
		// Sobel filters
		Mat sobelVert, sobelHoriz;
		applySobel3x3(image, true, sobelVert);
		applySobel3x3(image, false, sobelHoriz);
		
		// Get path to output
		string outputDir = string(argv[2]);

		// Convert to UCHAR and scale for display
		boxFilteredImage.convertTo(boxFilteredImage, CV_8UC1);
		gaussianFilteredImage.convertTo(gaussianFilteredImage, CV_8UC1);
		sobelVert += 255.0;
		sobelHoriz += 255.0;
		sobelVert /= 2.0;
		sobelHoriz /= 2.0;
		sobelHoriz.convertTo(sobelHoriz, CV_8UC1);
		sobelVert.convertTo(sobelVert, CV_8UC1);
		
		// Save output images	
		imwrite(outputDir + "/BOX_" + filename, boxFilteredImage);
		imwrite(outputDir + "/GAUSS_" + filename, gaussianFilteredImage);
		imwrite(outputDir + "/SOBEL_VERT_" + filename, sobelVert);
		imwrite(outputDir + "/SOBEL_HORIZ_" + filename, sobelHoriz);
		cout << "Images saved." << endl;

		// Show our image (with the filename as the window title)
		imshow(filename, image);
				
		// Show filtered images
		imshow("Box filtered Image", boxFilteredImage);
		imshow("Gaussian filtered Image", gaussianFilteredImage);
		imshow("Sobel (vertical) filtered Image", sobelVert);
		imshow("Sobel (horizontal) filtered Image", sobelHoriz);

		// Wait for a keystroke to close the window
		waitKey(-1);

		// Cleanup all windows
		destroyAllWindows();
	}
	else {
		// TESTING MODE

		bool allGood = true;

		// Testing applyLinearFilter
		if (!TEST_applyLinearFilter(filepath)) {
			cout << "TEST_applyLinearFilter: FAILED" << endl;
			allGood = false;
		}
		else {
			cout << "TEST_applyLinearFilter: SUCCESS" << endl;
		}

		// Testing applyBoxFilter
		if (!TEST_applyBoxFilter(filepath)) {
			cout << "TEST_applyBoxFilter: FAILED" << endl;
			allGood = false;
		}
		else {
			cout << "TEST_applyBoxFilter: SUCCESS" << endl;
		}

		// Testing applyGaussian3x3
		if (!TEST_applyGaussian3x3(filepath)) {
			cout << "TEST_applyGaussian3x3: FAILED" << endl;
			allGood = false;
		}
		else {
			cout << "TEST_applyGaussian3x3: SUCCESS" << endl;
		}

		// Testing applySobel3x3
		if (!TEST_applySobel3x3(filepath)) {
			cout << "TEST_applySobel3x3: FAILED" << endl;
			allGood = false;
		}
		else {
			cout << "TEST_applySobel3x3: SUCCESS" << endl;
		}

		// All tests good?
		if (allGood) {
			cout << "ALL TESTS SUCCEED!" << endl;
		}
	}

	return 0;
}

