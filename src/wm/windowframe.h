#ifndef __WINDOWFRAME_H__
#define __WINDOWFRAME_H__

#include "windowobject.h"
#include <string>
using namespace std;

class cWindowFrame : public cWindowObject
{
public:
	cWindowFrame(cWindow* parent) : cWindowObject(parent)
	{
		w = 100;
		h = 25;
		x = 10;
		y = 10;
		alignment = ALIGN_CENTER;
		type = OBJECT_FRAME;
	}
	virtual void draw(int,int,int,int);
};




#endif
