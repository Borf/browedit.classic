#ifndef __WINDOWMAINBUTTON_H__
#define __WINDOWMAINBUTTON_H__

#include "windowobject.h"
#include "windowbutton.h"
#include "window.h"

class cWindowMainButton : public cWindowButton
{
protected:
	string text;
public:
	cWindowMainButton(cWindow* parent) : cWindowButton(parent)
	{
		cursortype = 1;
		type = OBJECT_MAINBUTTON;
	}
	void draw();
	void click();
	void SetText(int, string);
	string GetText(int);
};


#endif
