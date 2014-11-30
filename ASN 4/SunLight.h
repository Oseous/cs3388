/*
  SunLight.h

  CS3388 Assignment 4

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  Class for the sun lights used in this assignment.
*/

#ifndef _SUNLIGHT_H
#define _SUNLIGHT_H

#include <opencv2\core\core.hpp>

#include "GenericLight.h"

class SunLight : public GenericLight
{
private:

public:
  /*
  toLight(...)

  Specific instance of the function required by the superclass.
  Returns a vector to the light source.

  args:
  cv::Mat startSun:
  The sun from which the vector to the light starts.

  return:
  cv::Mat:
  The vector to the light from the starting sun.
  */
  cv::Mat toLight(cv::Mat startPoint);
};

#endif