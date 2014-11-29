/*
  InputParser.h

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  A class used to get user input without failing on bad input.
  All functions are static so that the class does not need to be instantiated.
  Created for CS3307 assignment 1.
  I have removed most of its functions - they would be useless for my ray tracer.
*/

#ifndef _INPUTPARSER_H
#define _INPUTPARSER_H

#include <iostream>
#include <string>

class InputParser
{
public:
  /*
    getFloat(...)

    Gets a float from the user.
    Returns false if the user doesn't enter a valid float.

    args:
      float &io:
        The retrieved float will be written to this reference if successful.

    return:
      bool:
        Whether a value was successfully retrieved.
  */
  static bool getFloat(float &io);

  /*
    getInt(...)

    Gets a int from the user.
    Returns false if the user doesn't enter a valid int.

    args:
      int &io:
        The retrieved int will be written to this reference if successful.

    return:
      bool:
        Whether a value was successfully retrieved.
  */
  static bool getInt(int &io);

  /*
    getString(...)

    Gets a string from the user.
    Returns false if the string is invalid (0 length in this case).

    args:
      std::string& io:
        The retrieved string will be written to this reference if successful.

    return:
      bool:
        Whether a value was successfully retrieved.
  */
  static bool getString(std::string& io);

  /*
    getString(...)

    Gets a char from the user.
    Returns false if the string is invalid (0 length in this case).

    args:
      char& io:
        The retrieved char will be written to this reference if successful.

    return:
      bool:
        Whether a value was successfully retrieved.
  */
  static bool getChar(char& io);
};

#endif