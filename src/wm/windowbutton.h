#ifndef __WINDOWBUTTON_H__
#define __WINDOWBUTTON_H__

#include "windowobject.h"
#include <string>
using namespace std;

class cWindowButton : public cWindowObject
{
protected:
	string text;
public:
	cWindowButton()
	{
		w = 100;
		h = 25;
		x = 10;
		y = 10;
		alignment = ALIGN_CENTER;
		text = "Default";
		cursortype = 1;
		selectable = true;
		type = OBJECT_BUTTON;
	}
	virtual ~cWindowButton() {}
	virtual void draw();
	virtual void click() = 0;
	virtual bool onkeyup(int);


};




#endif
