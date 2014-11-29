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
#include "Light.h"

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
    modifyLighting(...)

    Allows the user to modify the scene's light.

    args:
      Light &light:
        A reference to the scene's light.

    return:
      bool:
        True if the changes were made.
        Otherwise, false.
  */
  static bool modifyLighting(Light &light);

  /*
    setupNewObject(...)

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

private:

};

#endif