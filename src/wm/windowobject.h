#ifndef __WINDOWOBJECT_H__
#define __WINDOWOBJECT_H__

#include <common.h>
#include <string>
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
};


class cWindow;

class cWindowObject
{
protected:
	int	x, y;
	int h, w;
	string popup;
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
	}	
	cWindowObject(cWindow* p)
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

	bool selectable;
	
	virtual void draw(int = 0, int = 0, int = 0, int = 0) = 0;
	virtual void click();
	virtual void drag() {}

	int cursortype;
	virtual cWindowObject* inobject();
	cWindowObject* moveto(int xx, int yy)		{ x = xx; y = yy; return this;}
	cWindowObject* resizeto(int ww, int hh)	{ w = ww; h = hh; return this;}

	int realx();
	int realy();
	int realy2();
	
	virtual bool onkeyup(int) { return false; }
	virtual bool onkeydown(int) { return false; }
	virtual bool onchar(char) { return false; }
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

extern float mousex, mousey;

#endif
