#ifndef __WINDOWROUNDBUTTON_H__
#define __WINDOWROUNDBUTTON_H__

#include "windowobject.h"
#include "window.h"

class cWindowRoundButton : public cWindowObject
{
public:
	cWindowRoundButton()
	{
		cursortype = 1;
		type = OBJECT_ROUNDBUTTON;
	}
	void draw();
	void click();
	bool inobject();
};


class cWindowCloseButton : public cWindowRoundButton
{
public:
	cWindowCloseButton()
	{
		resizeto(13, 13);
		moveto(2, 2);
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
	cWindowRollupButton()
	{
		resizeto(13, 13);
		moveto(16, 2);
		alignment = ALIGN_TOPRIGHT;
	}
	void click()
	{
		parent->toggleroll();
	}
};

#endif
