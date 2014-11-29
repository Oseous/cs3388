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
  return;
}

GenericObject::~GenericObject(){
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