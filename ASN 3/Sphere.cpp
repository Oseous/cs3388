/*
  Sphere.cpp

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  Sphere is an implementation of the GenericObject super class.
  Obviously, it's a sphere.
  This class implements the specific intersect and normal functions
  for a sphere, and also adds its own constructor to allow spheres
  to be created easily.
*/

#include "Sphere.h"
#include "RenderUtils.h"

Sphere::Sphere()
{
  // Default radius: 1
  // Default location: origin
  transformSet(cv::Mat::eye(4, 4, CV_32FC1));

  //Default colour: light gray
  rho_a = cv::Scalar(0, 0, 0);
  rho_d = cv::Scalar(1, 1, 1);
  rho_s = cv::Scalar(0, 0, 0);
}

Sphere::Sphere(cv::Mat trans, cv::Scalar rho_a, cv::Scalar rho_d, cv::Scalar rho_s)
{
  transformSet(trans);  // Handles setting the inverse matrix too
  this->rho_a = rho_a;
  this->rho_d = rho_d;
  this->rho_s = rho_s;
}

Sphere::~Sphere()
{
  //Nothing to let go of
}

bool Sphere::intersect(cv::Mat e, cv::Mat d, float &dist)
{
  // Act as if the sphere is at the origin, and transform e & d
  cv::Mat ep = _Mi*e;
  cv::Mat dp = _Mi*d;

  // Check if there is an intersection
  float a = RenderUtils::homoMagSq(dp);
  float b = 2 * RenderUtils::homoDot(dp, ep);
  float c = RenderUtils::homoMagSq(ep) - 1;

  // Calculate determinant
  float det = pow(b, 2) - 4 * a * c;

  // Check if there even is an intersection
  if (det < 0)
  {
    // Return false - no intersection
    return false;
  }

  // Try shorter distance first
  float sqrtDet = sqrt(det);
  float t = (-b - sqrtDet) / (2 * a);

  // Check if the lesser intersection is at
  // a positive t (if it is, it's closer)
  if (t > 0)
  {
    dist = t;
    return true;
  }

  // See if the other intersection works
  // Try the other intersection, it might be positive
  t = (-b + sqrtDet) / (2 * a);
  if (t > 0)
  {
    // Intersection found!
    dist = t;
    return true;
  }

  // It's a ray, not a line, an intersection behind the
  // starting point doesn't count.  Rays propagate in
  // one direction
  return false;
}

cv::Mat Sphere::normal(cv::Mat point)
{
  // Inverse transform the point to be on our unit sphere
  cv::Mat pointp = _Mi*point;

  // Normal is just the vector to the point
  cv::Mat norm;
  pointp.copyTo(norm);

  // Vector - do not translate!
  norm.at<float>(3) = 0;
  // Transform the normal into the world space
  norm = _M*norm;
  RenderUtils::homoNormalize(norm);

  return norm;
}