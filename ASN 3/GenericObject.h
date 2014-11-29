/*
  GenericObject.h

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  A superclass for all objects to be rendered by the ray tracer.

  This class cannot be instantiated. It simply defines an interface that must 
  be provided by all rendering objects.

  Note that the lighting factors are left public to make them easier to
  manage for the program.
*/

#ifndef _GENERICOBJECT_H
#define _GENERICOBJECT_H

#include <opencv2\core\core.hpp>
#include <string>

class GenericObject
{
protected:

public:
  // Constructor
  GenericObject(){}
  // Destructor
  ~GenericObject(){}

  // Name of the object - just for the UI
  std::string name;

  // Ambient, diffuse and specular lighting coefficients
  cv::Scalar rho_a; // Ambient
  cv::Scalar rho_d; // Diffuse
  cv::Scalar rho_s; // Specular

  /*
    intersect(...)

    Calculates the shortest intersection of the ray defined by e and d that has
    a distance greater than 0.

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
  virtual bool intersect(cv::Mat e, cv::Mat d, float &dist) = 0;

  /*
  normal(...)

  Finds and returns the surface normal to the object at the provided point.
  Note that the function DOES NOT verify that the point is actually on the object.

  args:
    cv::Mat point
      The point to find the normal at.

  return:
    cv::Mat:
      The normal vector to the object at the point.
  */
  virtual cv::Mat normal(cv::Mat point) = 0;  

  /*
  These functions are not necessary for this assignment, and will be
  implemented at a later time.
  */
  // TODO: Implement
  //// Moves the object to a point
  //virtual void moveTo(float x, float y, float z);
  //// Moves the object by a vector
  //virtual void moveBy(float x, float y, float z);
};

#endif