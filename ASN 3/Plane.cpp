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
#include "RenderUtils.h"

Plane::Plane()
{
  transformSet(cv::Mat::eye(4, 4, CV_32FC1));
  for (int i = 0; i < 3; i++)
  {
    rho_a[i] = rho_s[i] = 0;
    rho_d[i] = 0.9;
  }
}

Plane::Plane(cv::Mat trans, cv::Scalar rho_a, cv::Scalar rho_d, cv::Scalar rho_s)
{
  transformSet(trans); // Also handles setting up the inverse transform
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
  // Point isn't actually needed to get the normal.
  // Only needed for complying to the super class (other objects need the point).

  // Normal in base plane coordinates
  cv::Mat ret = cv::Mat::zeros(4, 1, CV_32FC1);
  ret.at<float>(2) = 1;

  // Transform into world coordinates
  ret = _M*ret;

  // Normalize
  RenderUtils::homoNormalize(ret);

  return ret;
}