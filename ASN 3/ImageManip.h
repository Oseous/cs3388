/*
  ImageManip.h

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

#ifndef _IMAGEMANIP_H
#define _IMAGEMANIP_H

#include "opencv/cv.h"
#include "opencv/highgui.h"
//#include "Point.h"

class ImageManip
{
public:
  /*
    whiten(...)

    Turns the passed image entirely white to 'refresh' for the next cycle when
    drawing sparse features.

    Needed when not every pixel is drawn to.

    args:
      cv::Mat image:
        The image to turn white.

    return:
      void
  */
	static void whiten(cv::Mat image);

  /*
    plot(...)

    Plots a pixel on to an image.

    Note that this function is a bit of a hack.
    I keep a static pointer to the image's data member (the image array itself)
    that this function is very fast.
    If the image is not continuous this function will fail - make sure to check
    this yourself.

    To start using a new image, set the newImage boolean to true.

    args:
      cv::Mat image:
        The image to draw a pixel to.
        Only used on the first call or if newImage is true.
      int x, y:
        The coordinates of the pixel to draw to.
      cv::Scalar colour:
        The colour of the pixel to draw.
      bool newImage:
        A boolean flag for if this is not the same image as the last time.
        Updates the static pointer to point to the new image if this is true.

      return:
        void
  */
	static void plot(cv::Mat image, int x, int y, cv::Scalar colour, bool newImage);
};

#endif