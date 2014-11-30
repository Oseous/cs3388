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
  // Transform matrix
  cv::Mat _M;
  // Inverse of the transform matrix
  // Should be updated whenever the transform matrix 
  // is to reduce load
  // Used to transform rays
  cv::Mat _Mi;
public:
  /*
    GenericObject()  
    
    Default Constructor. Creates unit object at origin.
    Light gray and mostly diffuse.
  */
  GenericObject();

  /*
    GenericObject(...)

    Specific Constructor. Creates a transformed object
    with specified lighting factors.

    Args:
      cv::Mat trans:
        The transform to apply to the cylinder.
      cv::Scalar rho_a, rho_d, rho_d:
        The ambient, diffuse, and specular lighting
        factors of the new cylinder.
  */
  GenericObject(cv::Mat trans, cv::Scalar rho_a, cv::Scalar rho_d, cv::Scalar rho_s);

  /*
    ~GenericObject()

    Destructor. Subclasses should try not to have
    any dynamic storage types so that they do not
    need to define their own destructors.
  */
  ~GenericObject();

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

    Should use the Mi matrix to transform the rays.

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
    getTransform()
    getInvTransform()

    Getters for the transform and inverse transform matrices.

    args:
      None

    return:
      cv::Mat:
        Transform or inverse transform matrix.
  */
  cv::Mat getTransform();
  cv::Mat getInvTransform();

  /*
    transformBy(...)

    Applies a transformation matrix to the object's
    transformation matrix.

    args:
      cv::Mat trans:
        The matrix to transform the object's 
        transformation matrix by.

    return:
      void
  */
  void transformBy(cv::Mat trans);

  /*
    transformSet(...)

    Copies the given transform matrix over the object's
    transformation matrix.

    args:
      cv::Mat trans:
        The new transformation matrix to use.

    return:
      void
  */
  void transformSet(cv::Mat trans);
};

#endif