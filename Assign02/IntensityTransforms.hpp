#ifndef INTENSITY_TRANSFORMS_H
#define INTENSITY_TRANSFORMS_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void calculateHistogram(Mat image, double hist[], int length);
void calculateCumulative(double hist[], double cdfHist[], int length);
void stretchCumulative(double cdfHist[], double stretchCDFHist[], int length);
void getEqualizedImage(Mat origImage, Mat &equalizedImage);

#endif
