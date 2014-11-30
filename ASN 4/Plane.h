/*
  Plane.h

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

#ifndef _PLANE_H
#define _PLANE_H

#include "GenericObject.h"

class Plane : public GenericObject{
private:

public:
  /*
  Plane()

  The default constructor for Plane.
  Creates a gray plane.
  */
  Plane();

  /*
  Plane(...)

  An extra constructor that allows planes of different colours to be created easily.

  args:
  cv::Mat trans:
  Transform to apply to the plane.
  cv::Scalar rho_a, rho_d, rho_s:
  The ambient, diffuse, and specular lighting factors of the new plane.
  */
  Plane(cv::Mat trans, cv::Scalar rho_a, cv::Scalar rho_d, cv::Scalar rho_s);

  /*
  ~Plane()

  The default Plane destructor.
  There is nothing to do on deletion, so the
  destructor has an empty body.
  */
  ~Plane();

  /*
    intersect(...)

    Calculates the shortest intersection of the ray defined by e and d that has a 
    distance greater than 0.

    args:
      cv::Mat e:
        The starting point of the ray.
      cv::Mat d:
        The direction vector of the ray.
      float &dist:
        The distance to the intersection.
        Actually used as a return value, the initial value is overwritten if an 
        intersection is found.

    return:
      bool:
        True if an intersection was found.
        False otherwise.
  */
  bool intersect(cv::Mat e, cv::Mat d, float &dist);

  /*
    normal(...)

    Finds and returns the surface normal to the sphere at the provided point.

    Note 1: This function DOES NOT verify that the point is actually on the sphere.
    Note 2: Though the point on the plane is not actually needed to find the normal
            vector, it must be included to conform with the super class
            GenericObject's definition. This is done so that all objects can be
            treated in the same manner.

    args:
      cv::Mat point
        The point to find the normal at.

    return:
      cv::Mat:
          The normal vector to the plane.
  */
  cv::Mat normal(cv::Mat point);
};

#endif