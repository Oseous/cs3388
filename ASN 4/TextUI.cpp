/*
  TextUI.cpp

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  A basic UI class used by the main program to allow users to
  interact with the ray tracer.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

#include <opencv2\core\core.hpp>

#include "TextUI.h"
#include "InputParser.h"


#include "Plane.h"
#include "Sphere.h"

void TextUI::displayHeader(){
	std::cout << "\n" << "******************************************************\n" << std::endl
		                << "      Andrew Simpson's Ray Tracer - CS3388 Asn 3\n" << std::endl
		                << "******************************************************\n" << std::endl;
	return;
}

void TextUI::displayMenu(){
  std::cout <<
    "============================\n" <<
    "            Menu\n" <<      
    "   (render window on top)\n"
    "============================\n" <<
    " a / d - Yaw left / right\n" <<
    " w / s - Pitch down / up\n" <<
    " z / c - Roll left / right\n" <<
    " r / f - Zoom in / out\n" <<
    "   k   - Insert new light\n" <<
    "   l   - Delete light\n" <<
    "   n   - Insert new object\n" <<
    "   m   - Delete object\n" <<
    " SPACE - Render next frame\n" <<
    " q/ESC - Quit\n" <<
    "============================\n";

  return;
}

void TextUI::displayProcessingMessage(){
  std::cout <<
    "Processing next frame...\n" <<
    "Please wait about ten seconds...\n";
  return;
}

void TextUI::displayStartupMessage(){
  std::cout <<
    "Rendering default scene...\n" <<
    "Please wait about ten seconds...\n";
  return;
}

bool TextUI::addLight(std::vector<Light> &lights){
  // Make a light
  std::cout << "What do you want to call the light?\n";
  std::string name;
  if (!InputParser::getString(name))
    return false;
  // Get point location
  std::cout << "Please enter the new point light coordinates (x,y,z) one at a time:\n";
  float co[6];
  for (int i = 0; i < 3; i++)
  {
    if (!InputParser::getFloat(co[i]))
      return false;
  }
  std::cout << "Please enter the new point light intensity (B,G,R) coefficients one at a time:\n";
  // Get point intensity
  for (int i = 3; i < 6; i++)
  {
    if (!InputParser::getFloat(co[i]))
      return false;
  }
  //std::cout << "Please enter the new ambient light intensity (B,G,R) coefficients one at a time:\n";
  //// Get ambient intensity
  //for (int i = 6; i < 9; i++)
  //{
  //  if (!InputParser::getFloat(co[i]))
  //    return false;
  //}
  Light light;
  light.name = name;
  light.centre.at<float>(0) = co[0];
  light.centre.at<float>(1) = co[1];
  light.centre.at<float>(2) = co[2];
  cv::Scalar intensity = cv::Scalar(co[3], co[4], co[5]);
  light.intensity = intensity;

  lights.emplace_back(light);

  return true;
}

bool TextUI::removeLight(std::vector<Light> &lights){
  // List all lights:
  std::cout
    << "******************************\n"
    << " Lights:\n"
    << "------------------------------\n";
  for (int i = 0; i < lights.size(); i++)
  {
    std::cout << "   " << i << " - " << lights[i].name << std::endl;
  }
  std::cout << "******************************\n"
    << "Which object would you like to delete?\n";
  int idx;
  if (!InputParser::getInt(idx))
    return false;
  if (0 > idx || idx >= lights.size())
    return false;

  // Delete the pointer
  lights.erase(lights.begin() + idx);

  return true;
}

bool TextUI::addObject(std::vector<GenericObject*> &objects){
  std::cout << "Would you like to create a plane (p/P) or sphere (s/S)?\n";
  char c;
  if (!InputParser::getChar(c))
    return false;
  if (c == 'p' || c == 'P')
  {
    // Make a plane
    std::cout << "What do you want to call the plane?\n";
    std::string name;
    if (!InputParser::getString(name))
      return false;
    std::cout << "Please enter the rho_a, rho_d, and rho_s (B,G,R) coefficients one at a time:\n";
    float co[9];
    for (int i = 0; i < 9; i++)
    {
      if (!InputParser::getFloat(co[i]))
        return false;
    }
    // Make the plane
    cv::Mat trans = cv::Mat::eye(4, 4, CV_32FC1); // Just make a base plane...
    cv::Scalar rho_a(co[0], co[1], co[2]), rho_d(co[3], co[4], co[5]), rho_s(co[6], co[7], co[8]);
    Plane *newPlane = new Plane(trans, rho_a, rho_d, rho_s);
    newPlane->name = name;

    // Add to the list of objects
    objects.push_back(newPlane);

    // Return success
    return true;
  }
  else if (c == 's' || c == 'S')
  {
    // Make a sphere
    std::cout << "What do you want to call the sphere?\n";
    std::string name;
    if (!InputParser::getString(name))
      return false;
    std::cout << "Please enter the coordinates (x,y,z) of the centre of the sphere one at a time:\n";
    float coord[3];
    if (!(InputParser::getFloat(coord[0]) && InputParser::getFloat(coord[1]) &&
        InputParser::getFloat(coord[2])))
      return false;
    std::cout << "Please enter the radius of the sphere:\n";
    float rad;
    if (!InputParser::getFloat(rad))
      return false;
    std::cout << "Please enter the rho_a, rho_d, and rho_s (B,G,R) coefficients one at a time:\n";
    float co[9];
    for (int i = 0; i < 9; i++)
    {
      if (!InputParser::getFloat(co[i]))
        return false;
    }
    // Make the plane
    cv::Mat trans = cv::Mat::eye(4, 4, CV_32FC1);
    trans.at<float>(0, 0) = rad;
    trans.at<float>(1, 1) = rad;
    trans.at<float>(2, 2) = rad;
    trans.at<float>(0, 3) = coord[0];
    trans.at<float>(1, 3) = coord[1];
    trans.at<float>(2, 3) = coord[2];
    cv::Scalar rho_a(co[0], co[1], co[2]), rho_d(co[3], co[4], co[5]), rho_s(co[6], co[7], co[8]);
    Sphere *newSphere = new Sphere(trans, rho_a, rho_d, rho_s);
    newSphere->name = name;

    // Add to the list of objects
    objects.push_back(newSphere);

    // Return success
    return true;
  }
  
  // Char must not have matched an option
  return false;
}

bool TextUI::removeObject(std::vector<GenericObject*> &objects)
{
  // List all objects:
  std::cout
    << "******************************\n"
    << " Objects:\n"
    << "------------------------------\n";
  for (int i = 0; i < objects.size(); i++)
  {
    std::cout << "   " << i << " - " << objects[i]->name << std::endl;
  }
  std::cout << "******************************\n"
    << "Which object would you like to delete?\n";
  int idx;
  if (!InputParser::getInt(idx))
    return false;
  if (0 > idx || idx >= objects.size())
    return false;
  // Free the actual generic object
  delete objects[idx];
  // Delete the pointer
  objects.erase(objects.begin() + idx);

  return true;
}