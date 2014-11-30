/*
  PointLight.cpp

  CS3388 Assignment 4

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  Class for the point lights used in this assignment.
*/

#include "PointLight.h"

cv::Mat PointLight::toLight(cv::Mat startPoint){
  cv::Mat ret = centre - startPoint; // Very simple for a point light
  ret.at<float>(3) = 0; // Vector!
  return ret;
}