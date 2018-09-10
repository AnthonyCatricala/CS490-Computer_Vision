#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "IntensityTransforms.hpp"
#include "TEST_IntensityTransforms.hpp"

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
		
		// Process image
		Mat equalizedImage;
		getEqualizedImage(image, equalizedImage);
	
		// Get path to output
		string outputDir = string(argv[2]);

		// Save output image
		string outputFile = outputDir + "/" + filename;
		imwrite(outputFile, equalizedImage);
		cout << "Saved image to: " << outputFile << endl;

		// Show our image (with the filename as the window title)
		imshow(filename, image);

		// Show equalized image
		imshow("Histogram Equalized Image", equalizedImage);

		// Wait for a keystroke to close the window
		waitKey(-1);

		// Cleanup all windows
		destroyAllWindows();
	}
	else {
		// TESTING MODE

		bool allGood = true;
		
		// Testing calculateHistogram
		if (!TEST_calculateHistogram(filepath)) {
			cout << "TEST_calculateHistogram: FAILED" << endl;
			allGood = false;
		}
		else {
			cout << "TEST_calculateHistogram: SUCCESS" << endl;
		}

		// Testing calculateCumulative
		if (!TEST_calculateCumulative()) {
			cout << "TEST_calculateCumulative: FAILED" << endl;
			allGood = false;
		}
		else {
			cout << "TEST_calculateCumulative: SUCCESS" << endl;
		}

		// Testing stretchCumulative
		if (!TEST_stretchCumulative()) {
			cout << "TEST_stretchCumulative: FAILED" << endl;
			allGood = false;
		}
		else {
			cout << "TEST_stretchCumulative: SUCCESS" << endl;
		}

		// Testing getEqualizedImage
		if (!TEST_getEqualizedImage(filepath)) {
			cout << "TEST_getEqualizedImage: FAILED" << endl;
			allGood = false;
		}
		else {
			cout << "TEST_getEqualizedImage: SUCCESS" << endl;
		}

		// All tests good?
		if (allGood) {
			cout << "ALL TESTS SUCCEED!" << endl;
		}
	}

	return 0;
}

