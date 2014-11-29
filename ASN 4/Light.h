/*
  Light.h

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  Class for the point light object used for this assignment.
  Note that in this assignment there is only ever one light.

  All member variables are left public to make the light easy to use.
  
  Note that the ambient light intensity of the scene is attached to
  this object because there is only ever one light. Attaching the
  ambient light variable to this class meant fewer arguments else
  where in my program, making things a little easier to code/look at.
  This will need to change in the future when we have more light sources.
*/

#ifndef _LIGHT_H
#define _LIGHT_H

#include <opencv2\core\core.hpp>

class Light
{
private:

public:
  // The center of the point light
  cv::Mat centre;

  // Three colour intensity of the light
  cv::Scalar intensity;

  // The name of the light - just for the UI
  std::string name;
};

#endif