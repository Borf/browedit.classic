#ifndef __WINDOWRGBPICKER_H__
#define __WINDOWRGBPICKER_H__

#include "windowobject.h"
#include <string>
using namespace std;

class cWindowRGBPicker : public cWindowObject
{
protected:
	float r,g,b;
public:
	cWindowRGBPicker(cWindow* parent) : cWindowObject(parent)
	{
		w = 100;
		h = 25;
		x = 10;
		y = 10;
		alignment = ALIGN_CENTER;
		type = OBJECT_RGBPICKER;
		r = 0.5f;
		g = 0.5f;
		b = 1;
	}
	virtual ~cWindowRGBPicker() {}
	virtual void draw(int,int,int,int);
	virtual void click();
	virtual void drag();



};




#endif
