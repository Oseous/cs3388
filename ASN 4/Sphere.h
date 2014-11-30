/*
  Sphere.h

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

#ifndef _SPHERE_H
#define _SPHERE_H

#include "GenericObject.h"

class Sphere : public GenericObject{
private:

public:
  /*
    Sphere()

    Default constructor for Sphere.
    Creates a gray sphere with radius 1 centred at
    the origin.
  */
  Sphere();

  /*
    Sphere(...)

    An extra constructor for sphere that allows any
    specific sphere to be initialized easily.

    Args:
      cv::Mat trans:
        The transform to apply to the sphere.
      cv::Scalar rho_a, rho_d, rho_d:
        The ambient, diffuse, and specular lighting
        factors of the new sphere.
  */
  Sphere(cv::Mat trans, cv::Scalar rho_a, cv::Scalar rho_d, cv::Scalar rho_s);

  /*
    ~Sphere()

    The default sphere destructor.
    There is nothing to do on deletion, so the
    destructor has an empty body.
  */
  ~Sphere();

  /*
    intersect(...)

    Calculates the shortest intersection of the
    ray defined by e and d that has a distance 
    greater than 0.

    args:
      cv::Mat e:
        The starting point of the ray.
      cv::Mat d:
        The direction vector of the ray.
      float &dist:
        The distance to the intersection.
        Actually used as a return value, the initial 
        value is overwritten if an intersection is
        found.

    return:
      bool:
        True if an intersection was found.
        False otherwise.
  */
  bool intersect(cv::Mat e, cv::Mat d, float &dist);

  /*
    normal(...)

    Finds and returns the surface normal to the
    sphere at the provided point.
    Note that the function DOES NOT verify that the 
    point is actually on the sphere.

    args:
      cv::Mat point
        The point to find the normal at.

    return:
      cv::Mat:
        The normal vector to the sphere at the
        point.
  */
  cv::Mat normal(cv::Mat point);
};

#endif