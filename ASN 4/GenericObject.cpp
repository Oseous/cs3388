/*
  GenericObject.cpp

  CS3388 Assignment 3

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  A superclass for all objects to be rendered by the ray tracer.

  This class cannot be instantiated. It simply defines an interface that must
  be provided by all rendering objects.
*/

#include "GenericObject.h"

GenericObject::GenericObject(){
  // Default: unit size at origin
  transformSet(cv::Mat::eye(4, 4, CV_32FC1));

  //Default colour: light gray, mostly diffuse
  rho_a = cv::Scalar(0.8, 0.8, 0.8);
  rho_d = cv::Scalar(0.8, 0.8, 0.8);
  rho_s = cv::Scalar(0.2, 0.2, 0.2);
  
  return;
}

GenericObject::GenericObject(cv::Mat trans, cv::Scalar rho_a, cv::Scalar rho_d, cv::Scalar rho_s)
{
  transformSet(trans);  // Handles setting the inverse matrix too
  this->rho_a = rho_a;
  this->rho_d = rho_d;
  this->rho_s = rho_s;
}

GenericObject::~GenericObject(){
  // Nothing to let go of
  return;
}

cv::Mat GenericObject::getTransform(){ 
  return _M;
}

cv::Mat GenericObject::getInvTransform(){ 
  return _Mi; 
}

void GenericObject::transformBy(cv::Mat trans){
  _M = trans*_M;
  _Mi = _M.inv();
  return;
}

void GenericObject::transformSet(cv::Mat trans){
  trans.copyTo(_M);
  _Mi = _M.inv();
  return;
}