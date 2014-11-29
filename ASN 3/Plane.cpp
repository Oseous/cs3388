/*
  Plane.cpp

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  Plane is an implementation of the GenericObject super class.
  Obviously, it's a plane.
  This class implements the specific intersect and normal functions
  for a plane, and also adds its own constructor to allow different
  coloured planes to be created easily.

  NOTE: As in the class slides, I have only created a plane that is
  on the X-Y plane: Z=0. Other planes were not discussed, so I've
  assumed that we do not need to implement them for this project.
*/

#include "Plane.h"

Plane::Plane()
{
  _z = 0;
  for (int i = 0; i < 3; i++)
  {
    rho_a[i] = rho_s[i] = 0;
    rho_d[i] = 0.9;
  }
}

Plane::Plane(cv::Scalar rho_a, cv::Scalar rho_d, cv::Scalar rho_s)
{
  _z = 0;
  this->rho_a = rho_a;
  this->rho_d = rho_d;
  this->rho_s = rho_s;
}

Plane::~Plane()
{

}

bool Plane::intersect(cv::Mat e, cv::Mat d, float &dist)
{
  if (d.at<float>(2) == 0)
  {
    // Doesn't intersect
    return false;
  }

  // Calculate the distance to the intersection
  dist = -e.at<float>(2) / d.at<float>(2);

  // Intersects!
  return true;
}

cv::Mat Plane::normal(cv::Mat point)
{
  cv::Mat ret = cv::Mat::zeros(4, 1, CV_32FC1);
  ret.at<float>(2) = 1;

  return ret;
}