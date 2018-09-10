#ifndef FILTERING_H
#define FILTERING_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void applyLinearFilter(Mat input, Mat filter, Mat &output, double scaleFactor = 1.0);
void applyBoxFilter(Mat input, int filterWidth, int filterHeight, Mat &output);
void applyGaussian3x3(Mat input, Mat &output);
void applySobel3x3(Mat input, bool isVertical, Mat &output);


#endif
