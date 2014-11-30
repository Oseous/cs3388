/*
  GenericLight.h

  CS3388 Assignment 4

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  Superclass for all light sources (point and sun) used for this assignment.

  All member variables are left public to make the lights easy to use.
*/

#ifndef _GENERICLIGHT_H
#define _GENERICLIGHT_H

#include <opencv2\core\core.hpp>

class GenericLight
{
private:

public:
  // The center of the light
  cv::Mat centre;

  // The direction of the light
  cv::Mat direction;

  // Three colour intensity of the light
  cv::Scalar intensity;

  // The name of the light - just for the UI
  std::string name;

  /*
    toLight(...)

    Returns a vector to the light source.
    Must be implemented by all subclasses.
    
    args:
      cv::Mat startPoint:
        The point from which the vector to the light starts.

    return:
      cv::Mat:
        The vector to the light from the starting point.
  */
  virtual cv::Mat toLight(cv::Mat startPoint) = 0;
};

#endif