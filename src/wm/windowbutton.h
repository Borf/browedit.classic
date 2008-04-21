#ifndef __WINDOWBUTTON_H__
#define __WINDOWBUTTON_H__

#include "windowobject.h"
#include <string>
#include <tinyxml/tinyxml.h>
using namespace std;

class cWindowButton : public cWindowObject
{
protected:
	string text;


public:
	cWindowButton(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent, skin.FirstChildElement("skin")->FirstChildElement("button"))
	{
		w = 100;
		h = 25;
		x = 15;
		y = 10;
		alignment = ALIGN_CENTER;
		text = "Default";
		cursortype = 1;
		selectable = true;
		type = OBJECT_BUTTON;
		enabled = true;

	
	
	}
	virtual ~cWindowButton() {}
	virtual void draw(int,int,int,int);
	virtual void click() = 0;
	virtual bool onkeydown(int, bool);

	void SetText(int id, string txt)
	{
		text = txt;
	}


};


#include "window.h"

class cWindowCloseButton : public cWindowButton
{
public:
	cWindowCloseButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
	{
		resizeto(16, 14);
		moveto(4, 4);
		alignment = ALIGN_TOPRIGHT;
		text = "x";
	}
	void click()
	{
		parent->close();
	}
};

class cWindowRollupButton : public cWindowButton
{
public:
	cWindowRollupButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
	{
		resizeto(16, 14);
		moveto(20, 4);
		alignment = ALIGN_TOPRIGHT;
		text = "_";
	}
	void click()
	{
		parent->toggleroll();
	}
};



#endif
