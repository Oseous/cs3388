/*
  Camera.cpp

  CS3388 Assignment 2

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  The class that I use to represent a camera for my assignments.
  All members are made public to make manipulating the camera easy.
*/

#include "Camera.h"
#include "opencv2\imgproc\imgproc.hpp"

void Camera::rotate(cv::Mat axis, float angle){
  // Represent rotation axis
  cv::Mat Jv = cv::Mat::zeros(cv::Size(3, 3), CV_32FC1);
  //                 col, row (weird...)
  Jv.at<float>(cvPoint(1, 0)) = -axis.at<float>(2);
  Jv.at<float>(cvPoint(2, 0)) = axis.at<float>(1);
  Jv.at<float>(cvPoint(0, 1)) = axis.at<float>(2);
  Jv.at<float>(cvPoint(2, 1)) = -axis.at<float>(0);
  Jv.at<float>(cvPoint(0, 2)) = -axis.at<float>(1);
  Jv.at<float>(cvPoint(1, 2)) = axis.at<float>(0);
  //Build rotation matrix
  cv::Mat R = cv::Mat::eye(cv::Size(3, 3), CV_32FC1) + sin(angle)*Jv + (1 - cos(angle))*Jv*Jv;
  //Put into homogeneous coordinates by padding with zeros
  cv::copyMakeBorder(R, R, 0, 1, 0, 1, cv::BORDER_CONSTANT, 0);
  //Add one in bottom right
  R.at<float>(cv::Point(3, 3)) = 1;

  //Rotate everything except gaze
  e = e - g;
  e = R*e;
  e = e + g;
  u = R*u;
  v = R*v;
  n = R*n;

  return;
}