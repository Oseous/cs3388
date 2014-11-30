/*
  SunLight.cpp

  CS3388 Assignment 4

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  Class for the sun lights used in this assignment.
*/

#include "SunLight.h"
#include "RenderUtils.h"

cv::Mat SunLight::toLight(cv::Mat startPoint){
  float dist = RenderUtils::homoDot(startPoint - centre, direction);
  cv::Mat ret = -direction * dist; // Very simple for a sun light
  ret.at<float>(3) = 0; // Vector!
  return ret;
}