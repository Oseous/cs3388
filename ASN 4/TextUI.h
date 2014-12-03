/*
  TextUI.h

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  A basic UI class used by the main program to allow users to
  interact with the ray tracer.
*/

#ifndef _TEXTUI_H
#define _TEXTUI_H

#include <vector>

#include "GenericObject.h"
#include "GenericLight.h"
#include "PointLight.h"

class TextUI{
private:

public:

  /*
    displayHeader()

    Prints a nice little header on the command line for the user.

    args:
      void

    return:
      void
  */
	static void displayHeader();

  /*
  displatMenu()

  Prints out the menu for the ray tracer.
  Does not actually handle input.

  args:
  void

  return:
  void
  */
  static void displayMenu();

  /*
    displayProcessingMessage()

    Prints out a message to let the user know the program is busy.

    args:
      void

    return:
      void
  */
  static void displayProcessingMessage();

  /*
    displayStartupMessage()

    Prints out a message to let the user know the program has just started.

    args:
      void

    return:
      void
  */
  static void displayStartupMessage();

  /*
    addLight(...)

    Adds a new light to the scene.
    Handles user input and initializing the new light.

    Does not deal with invalid input from the user (intensity values < 0, > 1).

    args:
      std::vector<Light> &lights:
        A reference to the scene's list of lights.

    return:
      bool:
        True if the changes were made.
        Otherwise, false.
  */
  static bool addLight(std::vector<GenericLight*> &lights);

  /*
    removeLight(...)

    Removes a light from the scene.
    Handles user input and deleting the light.

    args:
      std::vector<Light> &lights:
        A reference to the scene's list of lights.

    return:
      bool:
        True if the changes were made.
        Otherwise, false.
  */
  static bool removeLight(std::vector<GenericLight*> &lights);

  /*
    adjustAmbient(...)

    A routine that allows the user to adjust the ambient light.

    args:
      PointLight &ambientLight:
        A reference to the scene's ambient light.

    return:
      bool:
        Whether a result was parsed or not.
  */
  static bool adjustAmbient(PointLight &ambientLight);

  /*
    addObject(...)

    Adds a new object to the scene.
    Handles user input and initializing the new object.

    Does not deal with invalid input from the user (rho values < 0, > 1).

    args:
      std::vector<GenericObject*> &objects:
        A reference to the vector of objects. The new object will be added to
        this.

    return:
      bool:
        Whether insertion of a new object was successful.
  */
	static bool addObject(std::vector<GenericObject*> &objects);

  /*
   removeObject(...)

    Removes an object from the scene.
    Handles user input and deleting the object.

    args:
      std::vector<GenericObject*> &objects:
        A reference to the vector of objects. The new object will be added to
        this.

    return:
      bool:
        Whether insertion of a new object was successful.
  */
  static bool removeObject(std::vector<GenericObject*> &objects);

  /*
    transformLight(...)

    Allows the user to transform an object.
    DOES NOT verify that the transform is legal.
    Note that this really only includes rotation and translation.
    Can SET the transform, or APPLY a transform.

    args:
      std::vector<GenericObject*> &objects:
        A reference to the list of pointers of objects in the scene.

    return:
      bool:
        Whether a transform was parsed.
  */
  static bool transformObject(std::vector<GenericObject*> &objects);

  /*
  snowManPrompt()

  Asks the user if they want to build a snowman.

  args:
  void

  return:
  bool:
  True if the user wants a snowman.
  */
  static bool snowManPrompt();

private:

};

#endif