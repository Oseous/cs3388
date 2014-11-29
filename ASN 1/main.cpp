
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "ImageManip.h"
#include "Polygon.h"
#include <iostream>

#define NUM_SQUARES 30

using namespace cv;

int main(int argc, char** argv){
	char c;
	Mat image(512, 512, CV_8UC3, Scalar(0, 0, 0));
	RNG rng(0xFFFFFFFF);

	//#lazy
	if (!image.isContinuous()){
		return 1;
	}

	//Create objects
	//Make Squares
	//std::vector<Polygon> squares(NUM_SQUARES);
	Polygon* squares = new Polygon[NUM_SQUARES];
	for (int i = 0; i < NUM_SQUARES; i++){
		//Make corner points and put into a vector
		Polygon square(rng);
		square.translate(rng.uniform(0,500), rng.uniform(0,500));
		squares[i] = square;
	}
	//Rectangles

	//Circles

	long frameCnt = 0;
	double t = (double)getTickCount();
	while (true){

		//Over write image in white
		ImageManip::whiten(image);

		//Apply transforms to objects
		for (int i = 0; i < NUM_SQUARES; i++){
			//squares[i].translate(rng.uniform(-2, 2), rng.uniform(-2, 2));
			squares[i].doVelocity();
			//squares[i].rotate(rng.uniform(-0.1,0.1));
			squares[i].validatePosition();
		}

		//Draw objects
		for (int i = 0; i < NUM_SQUARES; i++){
			squares[i].draw(image);
		}

		//For testing drawing angled lines.
		ImageManip::drawLine(image, my::Point(0, 256), my::Point(255, 356), cv::Scalar(0, 0, 0));
		ImageManip::drawLine(image, my::Point(0, 255), my::Point(255, 155), cv::Scalar(0, 0, 0));
		ImageManip::drawLine(image, my::Point(256, 0), my::Point(356, 255), cv::Scalar(0, 0, 0));
		ImageManip::drawLine(image, my::Point(255, 0), my::Point(155, 255), cv::Scalar(0, 0, 0));
		ImageManip::drawLine(image, my::Point(256, 512), my::Point(356, 256), cv::Scalar(0, 0, 0));
		ImageManip::drawLine(image, my::Point(255, 512), my::Point(155, 256), cv::Scalar(0, 0, 0));
		ImageManip::drawLine(image, my::Point(512, 255), my::Point(256, 155), cv::Scalar(0, 0, 0));
		ImageManip::drawLine(image, my::Point(512, 256), my::Point(256, 356), cv::Scalar(0, 0, 0));

		imshow("My Image", image);

		c = cvWaitKey(10); // wait 10 ms or for key stroke
		if ((c == 27)||(c == 'q'))
			break; // if ESC, break and quit

		++frameCnt;

		if (frameCnt % 100 == 0){
			std::cout << frameCnt / (((double)getTickCount()-t)/getTickFrequency()) << " fps\n" ;
		}
	}
}