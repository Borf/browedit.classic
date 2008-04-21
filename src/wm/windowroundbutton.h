#ifndef __WINDOWROUNDBUTTON_H__
#define __WINDOWROUNDBUTTON_H__

#include "windowobject.h"
#include "window.h"

class cWindowRoundButton : public cWindowObject
{
public:
	cWindowRoundButton(cWindow* parent) : cWindowObject(parent)
	{
		cursortype = 1;
		type = OBJECT_ROUNDBUTTON;
	}
	void draw(int,int,int,int);
	void click();
	cWindowObject* inobject();
};

#endif
