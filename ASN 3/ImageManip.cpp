/*
  ImageManip.cpp

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  A couple of functions for basic image manipulation to make rendering images
  easier.

  Created for assignment 1.

  Note that I have removed the drawLine function -  it's pretty useless for
  ray tracing.
*/

#include "ImageManip.h"

void ImageManip::whiten(cv::Mat image){

	int rows = image.rows, cols = image.cols, chls = image.channels();
	cols = cols * chls;

	if (image.isContinuous()){
		cols = rows*cols;
		rows = 1;
	}

	int i, j;
	uchar*p;
	for (i = 0; i < rows; i++){
		p = image.ptr<uchar>(i);
		for (j = 0; j < cols; j++){
			p[j] = 255;
		}
	}
}

void ImageManip::plot(cv::Mat image, int x, int y, cv::Scalar colour, bool newImage){
	if ((x < 0) || (y < 0) ||(x > image.cols - 1) || (y > image.rows - 1)){
		return;
	}
	
	//If new image, must get new pointer
	static uchar* ptr = NULL;
	if (newImage||(ptr==NULL)){
		ptr = image.ptr<uchar>(0);
	}
	int idx = image.cols*y + x;
	idx *= 3;

  // Check for overflow
  for (int i = 0; i < 3; i++)
    if (colour[i] > 255)
      colour[i] = 255;

  // Write
	ptr[idx++] = colour[0];
	ptr[idx++] = colour[1];
	ptr[idx] = colour[2];

	return;
}