/*
  RayTracer.h

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  Class that implements the ray tracing algorithm discussed in class.
  User just calls traceImage().
*/

#ifndef _RAYTRACER_H
#define _RAYTRACER_H

#include <vector>
#include <opencv2\core\core.hpp>

#include "GenericObject.h"
#include "Camera.h"
#include "Light.h"

class RayTracer{
public:
  /*
    traceImage()

    Ray traces the provided image.

    args:
      cv::Mat renderImage:
        A reference to the image container to save the rendering to.
      float W:
        The half-width of the near plane.
      float H:
        The half-height of the near plane.
      float N:
        The distance of the near plane from the camera's origin.
      std::vector<GenericObject*> &objects:
        A reference to the list of pointers to all of the objects in the scene.
      Camera &cam:
        A reference to the camera to use for rendering the scene.
      cv::Scalar &backColour:
        A reference to the scalar holding the background colour of the scene.
        If a ray has no intersections, this is the colour that will be used
        for that point in the final image.
      std::vector<Light> &lights:
        A reference to the list of lights to use for rendering the scene.
      Light &ambientLight:
        A reference point to the special light that is the ambient light.
        Location is ignored.

    return:
      void
  */
  static void traceImage(cv::Mat &renderImage, float W, float H, float N, 
    std::vector<GenericObject*> &objects, Camera &cam, 
    cv::Scalar &backColour, std::vector<Light> &lights,
    Light &ambientLight);

private:
  /*
    traceRay(...)

    Traces and individual ray.

    args:
      float u:
        The u coordinate of the vector's intersection 
        with the near plane.
      float v:
        The v coordinate of the vector's intersection
        with the near plane
      float N:
        The distance from the centre of the camera to
        the near plane.
      std::vector<GenericObject*> &objects:
        A reference to the vector of pointers to all
        GenericObjects in the scene.
      Camera &cam:
        A reference to the camera to trace the ray 
        from.
      cv::Scalar &backColour:
        The background colour to use if a ray does
        not intersect any objects.
      vector<Light> &lights:
        A reference to the scene's list of point lights.
      Light &ambientLight:
        A reference point to the special light that is the ambient light.
        Location is ignored.

      return:
        cv::Scalar:
          The colour of the point of intersection of the
          ray. This can simply be assigned to the ray's
          coordinates in the image.
  */
  static cv::Scalar traceRay(float u, float v, float N, std::vector<GenericObject*> &objects, 
    Camera &cam, cv::Scalar &backColour, std::vector<Light> &light, Light &ambientLight);

  /*
    intersectRay(...)

    Intersects an individual ray with ALL objects, looking for the 
    shortest intersection that is greater than 0.

    args:
      cv::Mat e:
        The starting point of the ray.
      cv::Mat d:
        The direction vector of the ray.
      float &t:
        The distance to the intersection.
        Actually used as a return value, the initial 
        value is overwritten if an intersection is
        found.
      std::vector<GenericObject*> &objects:
        A vector of pointers to all of the GenericObejects
        in the scene to check for intersection with.
        Function will attempt to intersect the ray with
        each of these.
    
    return:
      GenericObject*:
        A pointer to the object that had the closest
        intersection that was at positive time.
  */
  static GenericObject* intersectRay(cv::Mat e, cv::Mat d, float &t, 
        std::vector<GenericObject*> &objects);

  /*
    shadowRay(...)

    Casts a shadow ray from the specified point to the origin of a light.

    args:
      cv::Mat start:
        The point to start the shadow ray cast at.
      cv::Mat toEnd:
        Vector to the end point from the start.
        IE. A vector to the light source from the point.
        Note that this is NOT a unit vector - it must go all the way 
        to the light. This is because this function restricts that 
        the distance of an intersection must be between 0 and 1. 
        This way, if an object is on the other side of the light, it 
        will not shadow our object.
      std::vector<GenericObject*> &objects:
        A reference to the array of pointers to all of the objects
        in the scene. This function will check if the ray from start
        to start + toEnd intersects any objects.

      return:
        bool:
          True if the ray is a shadow ray, IE. there is an intersection.
          Returns false otherwise.
  */
  static bool shadowRay(cv::Mat lightLoc, cv::Mat toPoint, std::vector<GenericObject*> &objects);
};

#endif