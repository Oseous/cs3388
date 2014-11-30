/*
  Cylinder.h

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

#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "GenericObject.h"

class Cylinder : public GenericObject{
private:

public:
  /*
    Cylinder()

    Default constructor for cylinder.
    Creates a gray Cylinder with radius 1 centred at
    the origin.
  */
  Cylinder();

  /*
    Cylinder(...)

    An extra constructor for cylinder that allows any
    specific cylinder to be initialized easily.

    Args:
      cv::Mat trans:
        The transform to apply to the cylinder.
      cv::Scalar rho_a, rho_d, rho_d:
        The ambient, diffuse, and specular lighting
        factors of the new cylinder.
  */
  Cylinder(cv::Mat trans, cv::Scalar rho_a, cv::Scalar rho_d, cv::Scalar rho_s);

  /*
    ~Cylinder()

    The default Cylinder destructor.
    There is nothing to do on deletion, so the
    destructor has an empty body.
  */
  ~Cylinder();

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
    cylinder at the provided point.
    Note that the function DOES NOT verify that the
    point is actually on the cylinder.

    args:
      cv::Mat point
        The point to find the normal at.

    return:
      cv::Mat:
        The normal vector to the cylinder at the
        point.
  */
  cv::Mat normal(cv::Mat point);
};

#endif