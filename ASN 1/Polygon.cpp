#include "Polygon.h"
#include "opencv\cv.h"

Polygon::Polygon(){}

Polygon::Polygon(cv::RNG rng){
	numPoints = 4;
	_centre.x = 0;
	_centre.y = 0;
	int halfW = rng.uniform(15, 512 / 4);
	_points[0] = my::Point(halfW, halfW);
	_points[1] = my::Point(halfW, -halfW);
	_points[2] = my::Point(-halfW, -halfW);
	_points[3] = my::Point(-halfW, halfW);
	_colour[0] = rng.uniform(10, 255);
	_colour[1] = rng.uniform(10, 255);
	_colour[2] = rng.uniform(10, 255);
	_velocity.x = rng.uniform(-3, 3);
	_velocity.y = rng.uniform(-3, 3);
	_rotVelocity = rng.uniform(-0.05, 0.05);
}

my::Point& Polygon::getCentre(){
	return _centre;
}

void Polygon::draw(cv::Mat image){
	if (numPoints > 10){
		//Wat
		int x = 0;
	}
	for (int i = 0; i < numPoints - 1; i++){
		if ((_points[i].x < 0) || (_points[i].x>512) || (_points[i].y < 0) || (_points[i].y>512)||
			(_points[i+1].x < 0) || (_points[i+1].x>512) || (_points[i+1].y < 0) || (_points[i+1].y>512)){
			return;
		}
		ImageManip::drawLine(image, _points[i], _points[i + 1], _colour);
	}
	ImageManip::drawLine(image, _points[0], _points[numPoints - 1], _colour);
}

void Polygon::translate(int x, int y){
	for (int i = 0; i < numPoints; ++i){
		_points[i].x += x;
		_points[i].y += y;
	}
	_centre.x += x;
	_centre.y += y;
}

void Polygon::doVelocity(){
	this->translate(_velocity.x, _velocity.y);
	this->rotate(_rotVelocity);
}

void Polygon::rotate(float radsCCW){
	//Move to origin to rotate about self
	my::Point oldPos = this->getCentre();
	this->translate(-oldPos.x, -oldPos.y);
	float cosT = cos(radsCCW);  //These two could be stored to save computation time, but oh well.
	float sinT = sin(radsCCW);
	float oldX;
	for (int i = 0; i < numPoints; ++i){
		//Do each rotation
		//This is the matrix math from class, but without the matricies
		oldX = _points[i].x;
		_points[i].x = _points[i].x*cosT - _points[i].y*sinT;
		_points[i].y = oldX*sinT + _points[i].y*cosT;
	}
	this->translate(oldPos.x, oldPos.y);
	return;
}

void Polygon::validatePosition(){
	//Check if out of bounds
	float maxXintr = 0;  //Needs to be a float since x,y are floats
	float maxYintr = 0;  //Otherwise, they can't be contained properly and this freaks out
	for (int i = 0; i < numPoints; ++i){
		if (_points[i].x < 0){
			if (maxXintr > _points[i].x){
				maxXintr = _points[i].x;
			}
		}
		else if (_points[i].x > 512){
			if ((_points[i].x - 512) > maxXintr){
				maxXintr = _points[i].x - 512;
			}
		}
		if (_points[i].y < 0){
			if (maxYintr > _points[i].y){
				maxYintr = _points[i].y;
			}
		}
		else if (_points[i].y > 512){
			if ((_points[i].y - 512) > maxYintr){
				maxYintr = _points[i].y - 512;
			}
		}
	}
	if ((maxXintr != 0) || (maxYintr != 0)){
		if (maxXintr != 0){
			_velocity.x = -_velocity.x;
			if (maxXintr > 0){
				maxXintr++; //Move in by at least one pixel
			}
			else{
				maxXintr--;
			}
		}
		else{
			_velocity.y = -_velocity.y;
			if (maxYintr > 0){
				maxYintr++;
			}
			else{
				maxYintr--;
			}
		}

		this->translate(-(maxXintr), -(maxYintr)); //Move back inside by one pixel
	}
}