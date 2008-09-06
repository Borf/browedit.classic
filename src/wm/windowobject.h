#ifndef __WINDOWOBJECT_H__
#define __WINDOWOBJECT_H__

#include <string>
#include <tinyxml/tinyxml.h>
//using namespace std;


#define focussed parent->selectedObject == this

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
	std::string popup;
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
	cWindowObject();	
	cWindowObject(cWindow* p, TiXmlElement* skin = NULL);

	virtual ~cWindowObject() {}
	OBJECT_BASE type;


	OBJECT_ALIGNMENT				alignment;
	int  getX()						{ return x; }
	int  getY()						{ return y; }
	int  getWidth()					{ return w; }
	int  getHeight()				{ return h; }
	virtual std::string getPopup()	{ return popup; }
	void setPopup(std::string s)	{ popup = s; }
	bool isEnabled()				{ return enabled; }
	void enable()					{ enabled = true; };
	void disable()					{ enabled = false; };

	bool selectable;
	
	virtual void draw(int = 0, int = 0, int = 0, int = 0);
	virtual void click();
	virtual void drag() {}

	int cursorType;
	virtual cWindowObject* inObject();
	cWindowObject* moveTo(int xx, int yy)		{ x = xx; y = yy; return this;}
	cWindowObject* resizeTo(int ww, int hh)		{ w = ww; h = hh; return this;}

	int realX();
	int realY();
	int realy2();
	
	virtual bool onKeyUp(int,bool) { return false; }
	virtual bool onKeyDown(int,bool) { return false; }
	virtual bool onChar(char,bool) { return false; }
	virtual void doubleClick() {}
	virtual void rightClick() {}

	virtual void setText(int, std::string) {}
	virtual void setInt(int, int) {}
	virtual std::string getText(int) { return "NULL"; }
	virtual int getInt(int) { return -1; }
	virtual void holdDragOver() {}
	virtual void dragOver() {}

	virtual void scrollUp() {}
	virtual void scrollDown() {}

	cWindow* parent;
};

extern long mouseX, mouseY;

#endif
