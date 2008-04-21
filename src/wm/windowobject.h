#ifndef __WINDOWOBJECT_H__
#define __WINDOWOBJECT_H__

#include <common.h>
#include <string>
#include <tinyxml/tinyxml.h>
using namespace std;


#define focussed parent->selectedobject == this

enum OBJECT_ALIGNMENT
{
	ALIGN_TOPLEFT = 1|8,
	ALIGN_TOP = 1|16,
	ALIGN_TOPRIGHT = 1|32,
	ALIGN_LEFT = 2|8,
	ALIGN_CENTER = 2|16,
	ALIGN_RIGHT = 2|32,
	ALIGN_BOTTOMLEFT = 4|8,
	ALIGN_BOTTOM = 4|16,
	ALIGN_BOTTOMRIGHT = 4|32,

	ALIGN_VTOP = 1,
	ALIGN_VCENTER = 2,
	ALIGN_VBOTTOM = 4,
	ALIGN_HLEFT = 8,
	ALIGN_HCENTER = 16,
	ALIGN_HRIGHT = 32
};

enum OBJECT_BASE
{
	OBJECT_UNDEFINED,
	OBJECT_BUTTON,
	OBJECT_CHATBOX,
	OBJECT_CHECKBOX,
	OBJECT_INPUTBOX,
	OBJECT_LABEL,
	OBJECT_LISTBOX,
	OBJECT_PICTUREBOX,
	OBJECT_ROUNDBUTTON,
	OBJECT_MAINBUTTON,
	OBJECT_LINE,
	OBJECT_PROGRESSBAR,
	OBJECT_SCROLLPANEL,
	OBJECT_TABPANEL,
	OBJECT_FRAME,
	OBJECT_TREE,
	OBJECT_MODEL,
	OBJECT_BMLBOX,
	OBJECT_FLOATINPUTBOX,
	OBJECT_RGBPICKER,
};


class cWindow;

class cWindowObject
{
protected:
	int	x, y;
	int h, w;
	string popup;
	bool	enabled;

//skinning
	float fontcolor[3];
	int	skinTopHeight;
	int skinTop;
	int skinBottomHeight;
	int skinBottom;

	int skinLeft;
	int skinLeftWidth;
	int skinRight;
	int skinRightWidth;


public:
	cWindowObject()
	{
		parent = NULL;
		x = 10;
		y = 10;
		w = 100;
		h = 100;
		cursortype = 0;
		alignment = ALIGN_CENTER;
		selectable = true;
		type = OBJECT_UNDEFINED;
		enabled = true;
	}	
	cWindowObject(cWindow* p, TiXmlElement* skin = NULL)
	{
		parent = p;
		x = 10;
		y = 10;
		w = 100;
		h = 100;
		cursortype = 0;
		alignment = ALIGN_CENTER;
		selectable = true;
		type = OBJECT_UNDEFINED;

		if(skin != NULL)
		{
			string color = skin->FirstChildElement("fontcolor")->FirstChild()->Value();
			fontcolor[0] = hex2dec(color.substr(0,2)) / 256.0f;
			fontcolor[1] = hex2dec(color.substr(2,2)) / 256.0f;
			fontcolor[2] = hex2dec(color.substr(4,2)) / 256.0f;

			skinTopHeight = atoi(skin->FirstChildElement("top")->Attribute("height"));
			skinTop =		512 - atoi(skin->FirstChildElement("top")->FirstChild()->Value());
			skinBottomHeight = atoi(skin->FirstChildElement("bottom")->Attribute("height"));
			skinBottom =		512 - atoi(skin->FirstChildElement("bottom")->FirstChild()->Value());
			
			skinLeftWidth = atoi(skin->FirstChildElement("left")->Attribute("width"));
			skinLeft =		atoi(skin->FirstChildElement("left")->FirstChild()->Value());
			skinRightWidth = atoi(skin->FirstChildElement("right")->Attribute("width"));
			skinRight =		atoi(skin->FirstChildElement("right")->FirstChild()->Value());
		}
	}

	virtual ~cWindowObject() {}
	OBJECT_BASE type;


	OBJECT_ALIGNMENT				alignment;
	int  px()						{ return x; }
	int  py()						{ return y; }
	int  pw()						{ return w; }
	int  ph()						{ return h; }
	virtual string ppopup()					{ return popup; }
	void setpopup(string s)			{ popup = s; }
	bool isEnabled()				{ return enabled; }
	void enable()					{ enabled = true; };
	void disable()					{ enabled = false; };

	bool selectable;
	
	virtual void draw(int = 0, int = 0, int = 0, int = 0);
	virtual void click();
	virtual void drag() {}

	int cursortype;
	virtual cWindowObject* inobject();
	cWindowObject* moveto(int xx, int yy)		{ x = xx; y = yy; return this;}
	cWindowObject* resizeto(int ww, int hh)	{ w = ww; h = hh; return this;}

	int realx();
	int realy();
	int realy2();
	
	virtual bool onkeyup(int,bool) { return false; }
	virtual bool onkeydown(int,bool) { return false; }
	virtual bool onchar(char,bool) { return false; }
	virtual void doubleclick() {}
	virtual void rightclick() {}

	virtual void SetText(int, string) {}
	virtual void SetInt(int, int) {}
	virtual string GetText(int) { return "NULL"; }
	virtual int GetInt(int) { return -1; }
	virtual void holddragover() {}
	virtual void dragover() {}

	virtual void scrollup() {}
	virtual void scrolldown() {}

	cWindow* parent;
};

extern long mousex, mousey;

#endif
