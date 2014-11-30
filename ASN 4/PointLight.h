/*
  PointLight.h

  CS3388 Assignment 4

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  Class for the point lights used in this assignment.
*/

#ifndef _POINTLIGHT_H
#define _POINTLIGHT_H

#include <opencv2\core\core.hpp>

#include "GenericLight.h"

class PointLight : public GenericLight
{
private:

public:
  /*
    toLight(...)

    Specific instance of the function required by the superclass.
    Returns a vector to the light source.
    
    args:
      cv::Mat startPoint:
        The point from which the vector to the light starts.

    return:
      cv::Mat:
        The vector to the light from the starting point.
  */
  cv::Mat toLight(cv::Mat startPoint);
};

#endif