#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <common.h>

#include <map>
using namespace std;

#include <texture.h>
#include <font.h>
#include "windowobject.h"

typedef map<string, cWindowObject*, less<string> > objectlist;

extern cWindowObject* draggingobject;

enum WINDOW_TYPE
{
	WT_MESSAGE,
	WT_CONFIRM,
	WT_OBJECT,
	WT_EFFECT,
	WT_WATER,
	WT_AMBIENTLIGHT,
	WT_LIGHT,
	WT_HOTKEY,
	WT_TEXTURE,
	WT_MODELS,
	WT_INPUT,
	WT_KEYBIND,
	WT_TOOLBOX,
	WT_RSMEDIT,
	WT_AREACOPY,
	WT_SPRITE,
	WT_FAVLIGHTS,
	WT_MODELOVERVIEW,
	WT_LIGHTOVERVIEW,
	WT_MINIMAP,
	WT_FILE,
	WT_PROGRESS,
	WT_SOUND,
	WT_SOUNDSELECT,
	WT_SOUNDOVERVIEW,


	WT_LOGIN,
	WT_SESSIONLIST,
	WT_SESSIONCREATE,
	WT_SESSIONOVERVIEW,

};
enum CLOSE_TYPE
{
	CLOSE,
	HIDE
};
class cWindow
{
protected:
	bool modal;
	bool visible;
	bool rolledup;
	bool topwindow;
	int  x, y;
	int  h, w;
	bool resizable;
	bool movable;
	bool resizingx;
	bool resizingxy;
	bool resizingy;
	bool resizingyx;
	bool checkborders;
	int minh, minw;
	WINDOW_TYPE wtype;
	string title;
	CLOSE_TYPE closetype;
	bool enabled;
	string saveprops;
	bool alwaysontop;
	bool notransparency;

//skinning
	float titlecolor[3];
	int	skinTopHeight;
	int skinTop;
	int skinBottomHeight;
	int skinBottom;

	int skinLeft;
	int skinLeftWidth;
	int skinRight;
	int skinRightWidth;




	int titlexoff,titleyoff;





public:
	int skinOffLeft;
	int skinOffRight;
	int skinOffTop;
	int skinOffBottom;

	
	float fontcolor[3];
	float currentColor[4];
	bool savewindow;
	string defaultobject;
	cWindow(cTexture* t, cFont* f, TiXmlDocument& skin);
	virtual ~cWindow()
	{
		for(map<string, cWindowObject*, less<string> >::iterator i = objects.begin(); i != objects.end(); i++)
		{
			if(draggingobject == i->second)
				draggingobject = NULL;
			delete i->second;
		}

		objects.clear();
		// delete objects
	}
	int  px()						{ return x; }
	int  py()						{ return rolledup ? y+h-16 : y; }
	int  py2()						{ return y; }
	int  pw()						{ return w; }
	int  ph()						{ return rolledup ? 16 : h; }
	int  ph2()						{ return h; }
	int  innerw()					{ return w-skinOffLeft-skinOffRight; }
	int  innerh()					{ return h-skinOffTop-skinOffBottom; }
	bool modality()					{ return modal; }
	void istopwindow(bool b)		{ topwindow = b; }
	bool istopwindow()				{ return topwindow; }

	void init(cTexture* t, cFont* f){ texture = t; font = f; }
		
	WINDOW_TYPE windowtype()		{ return wtype; }
	bool isvisible()				{ return visible || currentColor[3] != 0; }
	void hide()						{ visible = false; }
	void show()						{ visible = true; }
	void togglevis()				{ visible = !visible; }
	void rollup()					{ rolledup  = true; }
	void rollback()					{ rolledup  = false; }
	void toggleroll()				{ rolledup  = !rolledup; }
	void stopresizing()				{ resizingxy = (resizingyx = (resizingy = (resizingx = false))); }
	void startresisingx()			{ resizingx = true; }
	void startresisingxy()			{ resizingxy = true; }
	void startresizingy()			{ resizingy = true; }
	void startresizingyx()			{ resizingyx = true; }
	void disable()					{ enabled = false; }
	void enable()					{ enabled = true; }
	bool isenabled()				{ return enabled || currentColor[3] != 0; }
	bool isalwaysontop()			{ return alwaysontop; }
	bool resizing()					{ return resizingxy | resizingyx | resizingx | resizingy; }
	bool canbetransparent()			{ return !notransparency; }
	bool onborder();
	void moveto(int xx, int yy)		{ x = xx; y = yy; }
	virtual void resizeto(int ww, int hh)	{ w = ww; h = hh; }
	void initprops(string);

	void SetTitle(string t)			{ title = t; }
	string gettitle()				{ return title; }

	int getcursor();
	virtual void close(bool force = false);

	virtual bool inwindow();
	cWindowObject* inobject();
	bool drag();
	virtual void click();
	virtual void draw();
	virtual void save();
	virtual void doubleclick();
	virtual void rightclick();
	virtual void stopdrag() {}
	virtual void dragover();
	virtual void holddragover();
	virtual void scrollup();
	virtual void scrolldown();

	virtual bool onkeyup(int,bool);
	virtual bool onkeydown(int,bool);
	virtual bool onchar(char,bool);

	cWindowObject* addlabel(string, int,int,string);
	cWindowObject* addinputbox(string, int,int,int,string, TiXmlDocument &skin);
	cWindowObject* addcheckbox(string, int,int,bool, TiXmlDocument &skin);

	void center();

	virtual void* userfunc(void*) { return NULL; };

	cTexture* texture;
	cFont* font;
	objectlist objects;
	cWindowObject*	selectedobject;
};

extern long mousestartx, mousestarty, dragoffsetx, dragoffsety;

#endif
