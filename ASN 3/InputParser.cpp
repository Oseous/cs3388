/*
  InputParser.cpp

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  A class used to get user input without failing on bad input.
  All functions are static so that the class does not need to be instantiated.
  Created for CS3307 assignment 1.
  I have removed most of its functions - they would be useless for my ray tracer.
*/

#include <iostream>
#include <string>

#include "InputParser.h"

bool InputParser::getFloat(float& io){
	std::string strIn;
	std::cin >> strIn;
	if (strIn.length() > 0){  //Check that string is not 0 length.
		try{
			io = std::stof(strIn);  //Attempt to parse to a float.
			return true;
		}
		catch (std::invalid_argument e){
			return false;  //If an exception is thrown, parsing probably failed.
		}
	}
	else{  //I don't think cin will allow this to happen, but just in case.
		return false;
	}
}

bool InputParser::getInt(int& io){
  std::string strIn;
  std::cin >> strIn;
  if (strIn.length() > 0){  //Check that string is not 0 length.
    try{
      io = std::stoi(strIn);  //Attempt to parse to a float.
      return true;
    }
    catch (std::invalid_argument e){
      return false;  //If an exception is thrown, parsing probably failed.
    }
  }
  else{  //I don't think cin will allow this to happen, but just in case.
    return false;
  }
}

bool InputParser::getString(std::string& io){
  std::cin.ignore(); // Clear old newlines
  std::getline(std::cin, io); 
  if (io.length() == 0)
    return false;
  std::cout << "\n";  //Print an extra line to look nice
  return true;
}

bool InputParser::getChar(char& io){
  std::string strIn;
  std::cin >> strIn;
  if (strIn.length() > 0){
    io = strIn.at(0);
    std::cout << "\n";  //Print an extra line to look nice
  }
  else
    return false;
  return true;
}