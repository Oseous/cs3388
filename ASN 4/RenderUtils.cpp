/*
  RenderUtils.cpp

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  A set of functions to help with rendering assignments for CS3388.
  Originally written for assignment 2.
  Consists of vector and point operations that work on homogeneous vectors/points.
  All member functions are static so that they may be called without instantiating the class.
*/

#include "RenderUtils.h"

//Calculates the cross product using only the first 3 entries
cv::Mat RenderUtils::homoCross(cv::Mat a, cv::Mat b)
{
  cv::Mat ret = cv::Mat::zeros(4, 1, CV_32FC1);
  ret.at<float>(0) = a.at<float>(1)*b.at<float>(2) - a.at<float>(2)*b.at<float>(1);
  ret.at<float>(1) = a.at<float>(2)*b.at<float>(0) - a.at<float>(0)*b.at<float>(2);
  ret.at<float>(2) = a.at<float>(0)*b.at<float>(1) - a.at<float>(1)*b.at<float>(0);
  //Vector, not a point
  ret.at<float>(3) = 0;
  //Note that zero magnitude normals mean no area!
  return ret;
}

//Normalize the first 3 entries
cv::Mat RenderUtils::homoNormalize(cv::Mat a)
{
  float mag;
  mag = sqrt(a.at<float>(0)*a.at<float>(0) + a.at<float>(1)*a.at<float>(1) + a.at<float>(2)*a.at<float>(2));
  if (mag != 0){
    a.at<float>(0) = a.at<float>(0) / mag;
    a.at<float>(1) = a.at<float>(1) / mag;
    a.at<float>(2) = a.at<float>(2) / mag;
  }
  return a;
}

//Calculates the dot product using the first three entries
float RenderUtils::homoDot(cv::Mat a, cv::Mat b)
{
  float sum = 0;
  for (int i = 0; i < 3; i++){
    sum += a.at<float>(i)*b.at<float>(i);
  }
  return sum;
}

float RenderUtils::homoMagSq(cv::Mat a){
  return pow(a.at<float>(0), 2) + pow(a.at<float>(1), 2) + pow(a.at<float>(2), 2);
}