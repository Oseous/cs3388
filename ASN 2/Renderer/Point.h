#pragma once

namespace my
{
	struct Point{
		float x, y;  //Needs to be float so that rotation works
					//Made int when drawing still
		Point(){}
		Point(int x, int y){ this->x = x; this->y = y; }
	};
}