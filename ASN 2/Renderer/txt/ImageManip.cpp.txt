
#include "ImageManip.h"

void ImageManip::whiten(cv::Mat image){

	//Make sure image is compatible
	//CV_ASSERT(image.depth() != sizeof(uchar));

	int rows = image.rows, cols = image.cols, chls = image.channels();
	cols = cols * chls;

	if (image.isContinuous()){
		cols = rows*cols;
		rows = 1;
	}

	int i, j;
	uchar*p;
	for (i = 0; i < rows; i++){
		p = image.ptr<uchar>(i);
		for (j = 0; j < cols; j++){
			p[j] = 255;
		}
	}
}

void ImageManip::drawLine(cv::Mat image, my::Point a, my::Point b, cv::Scalar colour){
	//If a is more to the right than b, swap them.
	//Always iterate left to right
	//Iterate x if abs(dx) >= abs(dy), otherwise iterate y

	//Implement line drawing algorithm from class
	int p;
	int dx, dy;
	int x, y;
	bool itrX;
	bool neg;

	//Decide whether to iterate x or y first
	if (abs(a.x - b.x) >= abs(a.y - b.y)){
		itrX = true;
	}
	else{
		itrX = false;
	}

	//Check which point to start at
	if (itrX){  //Want to iterate right
		if (a.x <= b.x){
			dx = b.x - a.x;
			dy = b.y - a.y;
			x = a.x;
			y = a.y;
		}
		else{
			dx = a.x - b.x;
			dy = a.y - b.y;
			x = b.x;
			y = b.y;
		}
	}
	else{  //Want to iterate up
		if (a.y <= b.y){
			dx = b.x - a.x;
			dy = b.y - a.y;
			x = a.x;
			y = a.y;
		}
		else{
			dx = a.x - b.x;
			dy = a.y - b.y;
			x = b.x;
			y = b.y;
		}
	}

  //Draw first point on line
  plot(image, x, y, colour, false);

	//Check whether to iterate x or y
	if (itrX){
		if (dy < 0){
			neg = true;
			dy = -dy;
		}
		else{
			neg = false;
		}
		//iterate x
		for (int i = 1; i <= dx; i++){
			if (i == 1){
				p = 2 * dy - dx;
			}
			else{
				if (p < 0){
					p = p + 2 * dy;
				}
				else{
					p = p + 2 * dy - 2 * dx;
					if (!neg){
						y++;
					}
					else{
						y--;
					}
				}
			}
			x++;
			plot(image, x, y, colour, false);
		}
	}
	else{
		if (dx < 0){
			neg = true;
			dx = -dx;
		}
		else{
			neg = false;
		}
		//iterate y
		for (int i = 1; i <= dy; i++){
			if (i == 1){
				p = 2 * dx - dy;
			}
			else{
				if (p < 0){
					p = p + 2 * dx;
				}
				else{
					p = p + 2 * dx - 2 * dy;
					if (!neg){
						x++;
					}
					else{
						x--;
					}
				}
			}
			y++;
			plot(image, x, y, colour, false);
		}
	}

	plot(image, x, y, colour, false);  //I only ever work with one image

}

void ImageManip::plot(cv::Mat image, int x, int y, cv::Scalar colour, bool newImage){
	if ((x < 0) || (y < 0) ||(x > image.cols - 1) || (y > image.rows - 1)){
		return;
	}
	
	//If new image, must get new pointer
	static uchar* ptr = NULL;
	if (newImage||(ptr==NULL)){
		ptr = image.ptr<uchar>(0);
	}
	int idx = image.cols*y + x;
	idx *= 3;
	ptr[idx++] = colour[0];
	ptr[idx++] = colour[1];
	ptr[idx] = colour[2];

	return;
}