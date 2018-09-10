#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "IntensityTransforms.hpp"
#include "TEST_IntensityTransforms.hpp"

void calculateHistogram(Mat image, double hist[], int length)
{
  int totalPixels = 0;

  //Zero out all elements of hist[].
  for (int i = 0; i < length; i++)
  {
    hist[i] = 0;
  }

  //Calculate a NORMALIZED histogram with (length) possible values.

  //un-normalized histogram
  for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
      totalPixels++;
			int pixel = (int)image.at<uchar>(i,j);
			hist[pixel] ++;
		}
	}
  //normalize it!
  for (int i = 0; i < length; i++)
  {
    hist[i] /= totalPixels;
  }
}


void calculateCumulative(double hist[], double cdfHist[], int length)
{
  cdfHist[0] = hist[0];

  for (int i = 1; i < length; i++)
  {
    cdfHist[i] = hist[i] + cdfHist[i-1];
  }
}

void stretchCumulative(double cdfHist[], double stretchCDFHist[], int length)
{
  //Get the value of CDF[0] and subtract it from every value
  double cdfZero = cdfHist[0];
  for (int i = 0; i < length; i++)
	{
    stretchCDFHist[i] = cdfHist[i] - cdfZero;
  }

  //Get value of CDF[255] and divide all values by CDF[255]
  double cdfMax = stretchCDFHist[length -1];
  for (int i = 0; i < length; i++)
	{
    stretchCDFHist[i] = stretchCDFHist[i] / cdfMax;
  }

  //Then, perform remapping
  //???


}

void getEqualizedImage(Mat origImage, Mat &equalizedImage)
{
  const int length = 256;
  double hist[length];
  double cdfHist[length];
  double stretchCDFHist[length];

  //initializes the output matrix.
  equalizedImage.create(origImage.rows, origImage.cols, origImage.type());

  //runs the 3 other functions.
  calculateHistogram(origImage, hist, length);

  calculateCumulative(hist, cdfHist, length);

  stretchCumulative(cdfHist, stretchCDFHist, length);

  //Does the histogram equalization.
  for (int i = 0; i < origImage.rows; i++)
	{
		for (int j = 0; j < origImage.cols; j++)
		{
			int pixel = (int)origImage.at<uchar>(i,j);

      //s = T(r) = 255*CDF(r), but it's the stretchCDFHist from earlier.
      double value = 255*stretchCDFHist[pixel];

      //writes to the output image.
			equalizedImage.at<uchar>(i,j) = saturate_cast<uchar>(value);
		}
	}

}
