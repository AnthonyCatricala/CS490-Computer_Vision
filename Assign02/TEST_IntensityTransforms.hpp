#ifndef TEST_INTENSITY_TRANSFORMS_H
#define TEST_INTENSITY_TRANSFORMS_H

#include "IntensityTransforms.hpp"

#define EPSILON		1e-06

string getFilename(string path);
void printHistogram(double hist[], int length);
bool isEquals(double hist1[], double hist2[], int length);

bool TEST_ONE_calculateHistogram(int testIndex, Mat image, int histLen, double groundHist[]);
bool TEST_calculateHistogram(string filepath);

bool TEST_ONE_calculateCumulative(int testIndex, double hist[], int histLen, double groundCDF[]);
bool TEST_calculateCumulative();

bool TEST_ONE_stretchCumulative(int testIndex, double cdfHist[], int histLen, double ground[]);
bool TEST_stretchCumulative();

bool TEST_getEqualizedImage(string filepath);

#endif
