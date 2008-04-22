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

class cWindowCloseButton : public cWindowObject
{
public:
	cWindowCloseButton(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent, skin.FirstChildElement("skin")->FirstChildElement("closebutton"))
	{
		resizeto(atoi(skin.FirstChildElement("skin")->FirstChildElement("closebutton")->FirstChildElement("width")->FirstChild()->Value()), atoi(skin.FirstChildElement("skin")->FirstChildElement("closebutton")->FirstChildElement("height")->FirstChild()->Value()));
		moveto(atoi(skin.FirstChildElement("skin")->FirstChildElement("closebutton")->FirstChildElement("offsetx")->FirstChild()->Value()), atoi(skin.FirstChildElement("skin")->FirstChildElement("closebutton")->FirstChildElement("offsety")->FirstChild()->Value()));
		alignment = ALIGN_TOPRIGHT;
	}
	void click()
	{
		parent->close();
	}
};

class cWindowRollupButton : public cWindowObject
{
public:
	cWindowRollupButton(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent, skin.FirstChildElement("skin")->FirstChildElement("collapsebutton"))
	{
		resizeto(atoi(skin.FirstChildElement("skin")->FirstChildElement("collapsebutton")->FirstChildElement("width")->FirstChild()->Value()), atoi(skin.FirstChildElement("skin")->FirstChildElement("collapsebutton")->FirstChildElement("height")->FirstChild()->Value()));
		moveto(atoi(skin.FirstChildElement("skin")->FirstChildElement("collapsebutton")->FirstChildElement("offsetx")->FirstChild()->Value()), atoi(skin.FirstChildElement("skin")->FirstChildElement("collapsebutton")->FirstChildElement("offsety")->FirstChild()->Value()));
		alignment = ALIGN_TOPRIGHT;
	}
	void click()
	{
		parent->toggleroll();
	}
};



#endif
