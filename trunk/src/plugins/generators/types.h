#ifndef __MYTYPES_H__
#define __MYTYPES_H__

#include <vector>

class cPoint
{
public:
	cPoint(int xx,int yy)
	{
		x = xx;
		y = yy;
	}
	cPoint operator+(cPoint b)
	{
		return cPoint(x+b.x, y+b.y);
	}
	cPoint operator*(int b)
	{
		return cPoint(x*b, y*b);
	}
	
	int x,y;
};


class cIntQuad
{
public:
	int x;
	int y;
	int w;
	int h;
	std::vector<int> connections;
	cIntQuad(int xx, int yy, int ww, int hh)
	{
		x = xx;
		y = yy;
		w = ww;
		h = hh;
	}
};

#endif
