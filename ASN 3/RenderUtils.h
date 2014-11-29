/*
  RenderUtils.h

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  A set of functions to help with rendering assignments for CS3388.
  Originally written for assignment 2.
  Consists of vector and point operations that work on homogeneous vectors/points.
  All member functions are static so that they may be called without instantiating the class.
*/

#ifndef _RENDERUTILS_H
#define _RENDERUTILS_H

#include <string>
#include <opencv2\core\core.hpp>

class RenderUtils{
private:

public:
  /*
    homoCross(...)
    
    Calculates the cross product of homogeneous vectors a and b.

    args:
      cv::Mat a:
        The first vector for the cross product.
      cv::Mat b:
        The second vector for the cross product.

    return:
      cv::Mat
        The vector result of the cross product.
        The last entry of the mat (entry 3) is 0 to mark it as a vector.
  */
  static cv::Mat homoCross(cv::Mat a, cv::Mat b);

  /*
    homoNormalize(...)

    Normalizes the passed homogeneous vector/point.
    Note that the matrix passed is modified as well - not just the return.
    This is done so that a matrix can be normalized in place, but also so the 
    normalization can be a part of a formula.

    args:
      cv::Mat a:
        The vector to be normalized.
        Entry 3 (the point/vector flag) is NOT modified.

    return:
      cv::Mat:
        Normalized vector.
        This is the same matrix as the one passed, but allows use in formulas.
  */
  static cv::Mat homoNormalize(cv::Mat a);

  /*
    homoDot(...)

    Calculates the dot product of two homogeneous vectors/points.

    args:
      cv::Mat a, b:
        The two vectors to take the dot product of.

    return:
      float:
        The result of the dot product of a and b.
  */
  static float homoDot(cv::Mat a, cv::Mat b);

  /*
    homoMagSq(...)

    Finds the square of the magnitude of a vector or point.
    The square root is not found to get the magnitude to save processing power
    where the actual length is not needed.

    args:
      cv::Mat a:
        The homogeneous vector/point to find the squared magnitude of.

    return:
      float:
        The squared magnitude of a.
  */
  static float homoMagSq(cv::Mat a);
};

#endif