#pragma once

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "Point.h"

class ImageManip{
public:
	static void whiten(cv::Mat image);
	static void drawLine(cv::Mat image, my::Point a, my::Point b, cv::Scalar colour);
	static void plot(cv::Mat image, int x, int y, cv::Scalar colour, bool newImage);
};