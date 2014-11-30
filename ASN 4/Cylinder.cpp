/*
  Cylinder.cpp

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  Cylinder is an implementation of the GenericObject super class.
  Obviously, it's a cylinder.
  This class implements the specific intersect and normal functions
  for a cylinder, and also adds its own constructor to allow cylinders
  to be created easily.
*/

#include "Cylinder.h"
#include "RenderUtils.h"

Cylinder::Cylinder()
  :GenericObject(){
  // Nothing special to do...
}

Cylinder::Cylinder(cv::Mat trans, cv::Scalar rho_a, cv::Scalar rho_d, cv::Scalar rho_s)
  : GenericObject(trans, rho_a, rho_d, rho_s){
  // Nothing special to do...
}

Cylinder::~Cylinder(){
  // Nothing special to do...
}

bool Cylinder::intersect(cv::Mat e, cv::Mat d, float &dist)
{
  // Inverse transform e & d
  cv::Mat ep = _Mi * e;
  cv::Mat dp = _Mi * d;

  // Intersect with body
  // Calculate quadratic coefficients
  float a = pow(dp.at<float>(0), 2) + pow(dp.at<float>(1), 2);
  float b = ep.at<float>(0)*dp.at<float>(0) + ep.at<float>(1)*dp.at<float>(1);
  float c = pow(ep.at<float>(0),2) + pow(ep.at<float>(1), 2) - 1;

  // Calculate determinant
  float det = pow(b, 2) - a * c;

  // Check if there even is an intersection
  // The ray won't hit the caps either if it
  // doesn't pierce the infinite cylinder.
  if (det < 0)
  {
    // Return false - no intersection
    return false;
  }

  // Try shorter distance first
  float sqrtDet = sqrt(det);
  float t = (-b - sqrtDet) / a;

  // Check if the lesser intersection is at
  // a positive t (if it is, it's closer)
  if (t > 0)
  {
    // Check if the intersection is within the length
    // of the cylinder.
    cv::Mat intr = ep + dp*t;
    float z = intr.at<float>(2);
    if (-1 <= z && 1 >= z)
    {
      // Hits on the main body
      dist = t;
      return true; // Not possible to hit a cap first
    }  
  }

  // Try the caps
  // Top cap
  bool hits = false;
  t = (1 - ep.at<float>(2)) / dp.at<float>(2);
  if (t > 0)
  {
    // Check if within circle
    cv::Mat intr = ep + dp*t;
    if ((pow(intr.at<float>(0), 2) + pow(intr.at<float>(1), 2)) < 1)
    {
      dist = t;
      hits = true;
    }
  }
  // Bottom cap
  t = (-1 - ep.at<float>(2)) / dp.at<float>(2);
  if (t > 0 && ((t < dist) || !hits)) // Distance must be > 0, less than the previous hit,
  {                                   // if there was a previous hit.
    // Check if within circle
    cv::Mat intr = ep + dp*t;
    if ((pow(intr.at<float>(0), 2) + pow(intr.at<float>(1), 2)) < 1)
    {
      dist = t;
      hits = true;
    }
  }

  // Not possible to hit the far side before a cap or the near side.
  // If we've already found a hit, it's closer.
  if (hits)
  {
    return true;
  }

  // See if the other body intersection works
  t = (-b + sqrtDet) / a;
  if (t > 0)
  {
    // Check if z is valid
    cv::Mat intr = ep + dp*t;
    float z = intr.at<float>(2);
    if (-1 <= z && 1 >= z)
    {
      // Hits on the main body
      dist = t;
      return true; // Not possible to hit a cap first
    }
  }

  return false;
}

cv::Mat Cylinder::normal(cv::Mat point)
{
  // Inverse transform the point
  cv::Mat pp = _Mi * point;
  cv::Mat norm= cv::Mat::zeros(4, 1, CV_32FC1);
  // Top cap
  if (pp.at<float>(2) >= 1.0f)
  {
    // Straight up
    norm.at<float>(2) = 1;
    norm.at<float>(3) = 1;
  }
  // Bottom cap
  else if (pp.at<float>(2) <= -1.0f)
  {
    // Straight down
    norm.at<float>(2) = -1;
    norm.at<float>(3) = 1;
  }
  // Body
  else
  {
    // Straight out
    norm.at<float>(0) = pp.at<float>(0);
    norm.at<float>(1) = pp.at<float>(1);
  }

  // Put normal back into the world's space
  norm = _M * norm;

  // Normalize the normal
  RenderUtils::homoNormalize(norm);

  // Return the normal
  return norm;
}