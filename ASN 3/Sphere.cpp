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
  _radius = 1.0f;
  // Default location: origin
  _location = cv::Mat::zeros(4, 1, CV_32FC1);
  _location.at<float>(3) = 1.0f;
  //Default colour: light gray
  rho_a = cv::Scalar(0, 0, 0);
  rho_d = cv::Scalar(1, 1, 1);
  rho_s = cv::Scalar(0, 0, 0);
}

Sphere::Sphere(float x, float y, float z, float radius, cv::Scalar rho_a, cv::Scalar rho_d, cv::Scalar rho_s)
{
  _radius = radius;
  _location = cv::Mat::zeros(4, 1, CV_32FC1);
  _location.at<float>(0) = x;
  _location.at<float>(1) = y;
  _location.at<float>(2) = z;
  _location.at<float>(3) = 1.0f;
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
  // Check if there is an intersection
  // TODO: Don't assume the sphere is at the origin!!!
  float a = RenderUtils::homoMagSq(d);
  float b = 2 * RenderUtils::homoDot(d, (e - _location));
  float c = (RenderUtils::homoMagSq(e - _location) - pow(_radius, 2));

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
  dist = (-b + sqrtDet) / (2 * a);
  if (dist > 0)
  {
    // Intersection found!
    return true;
  }

  // It's a ray, not a line, an intersection behind the
  // starting point doesn't count.  Rays propagate in
  // one direction
  return false;
}

cv::Mat Sphere::normal(cv::Mat point)
{
  cv::Mat norm = RenderUtils::homoNormalize(point - _location);
  norm.at<float>(3) = 0; // Vector

  return norm;
}