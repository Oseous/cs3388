#pragma once

#include "point.h"
#include "opencv/cv.h"
#include "ImageManip.h"
#include <iostream>

class Polygon{
protected:
	cv::Scalar _colour;
	my::Point  _centre;
	my::Point  _velocity;
	float _rotVelocity;
	my::Point _points[4];
	int numPoints;
public:
	Polygon();
	Polygon(cv::RNG rng);

	my::Point& getCentre();
	void draw(cv::Mat image);
	void translate(int x, int y);
	void doVelocity();
	void rotate(float radsCCW);
	void validatePosition();
};