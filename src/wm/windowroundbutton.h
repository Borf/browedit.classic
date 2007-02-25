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
	bool inobject();
};


class cWindowCloseButton : public cWindowRoundButton
{
public:
	cWindowCloseButton(cWindow* parent) : cWindowRoundButton(parent)
	{
		resizeto(11, 11);
		moveto(4, 4);
		alignment = ALIGN_TOPRIGHT;
	}
	void click()
	{
		parent->close();
	}
};

class cWindowRollupButton : public cWindowRoundButton
{
public:
	cWindowRollupButton(cWindow* parent) : cWindowRoundButton(parent)
	{
		resizeto(11, 11);
		moveto(16, 4);
		alignment = ALIGN_TOPRIGHT;
	}
	void click()
	{
		parent->toggleroll();
	}
};

#endif
