/*
  Camera.h

  CS3388 Assignment 2

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  The class that I use to represent a camera for my assignments.
  All members are made public to make manipulating the camera easy.
*/

#ifndef _CAMERA_H
#define _CAMERA_H

#include <opencv2\core\core.hpp>

class Camera
{
private:

public:
  // Matracies used to characterize the position and orientation of the camera
  cv::Mat e, g, u, v, n;

  /*
    rotate(...)
    
    Rotates the camera CCW about a given axis by rads radians
  
    args:
      cv::Mat axis:
        The axis to rotate the camera about.
      float rads:
        The number of radians to rotate the camera by.

    return:
      void
  */
  void rotate(cv::Mat axis, float rads);
};

#endif