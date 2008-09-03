#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <common.h>

#include <map>
//using namespace std;

#include <texture.h>
#include <font.h>
#include "windowobject.h"

typedef std::map<std::string, cWindowObject*, std::less<std::string> > objectlist;

extern cWindowObject* draggingObject;

enum WINDOW_TYPE
{
	WT_GENERIC,
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
	WT_MAPDATABASE,
	WT_MAPDETAIL,
	WT_XML,
	WT_TEXTURETOOLS,
	WT_TEXTUREBRUSH,


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
	bool rolledUp;
	bool topWindow;
	int  x, y;
	int  h, w;
	bool resizable;
	bool movable;
	bool resizingX;
	bool resizingXY;
	bool resizingY;
	bool resizingYX;
	bool checkBorders;
	int minHeight, minWidth;
	WINDOW_TYPE windowType;
	std::string title;
	CLOSE_TYPE closeType;
	bool enabled;
	std::string saveProperties;
	bool alwaysOnTop;
	bool noTransparency;

//skinning
	float titleColor[3];
	int	skinTopHeight;
	int skinTop;
	int skinBottomHeight;
	int skinBottom;

	int skinLeft;
	int skinLeftWidth;
	int skinRight;
	int skinRightWidth;




	int titleOffX,titleOffY;





public:
	int skinOffLeft;
	int skinOffRight;
	int skinOffTop;
	int skinOffBottom;

	
	float							fontColor[3];
	float							currentColor[4];
	bool							saveWindow;
	std::string						defaultObject;

	cWindow(cTexture* t, cFont* f, TiXmlDocument& skin);
	virtual ~cWindow();
	int  getX()						{ return x; }
	int  getY()						{ return rolledUp ? y+h-16 : y; }
	int  py2()						{ return y; }
	int  getWidth()					{ return w; }
	int  getHeight()				{ return rolledUp ? 16 : h; }
	int  ph2()						{ return h; }
	int  innerWidth()				{ return w-skinOffLeft-skinOffRight; }
	int  innerHeight()				{ return h-skinOffTop-skinOffBottom; }
	bool modality()					{ return modal; }
	void istopwindow(bool b)		{ topWindow = b; }
	bool istopwindow()				{ return topWindow; }

	void init(cTexture* t, cFont* f){ texture = t; font = f; }
		
	WINDOW_TYPE windowtype()		{ return windowType; }
	bool isVisible()				{ return visible || currentColor[3] != 0; }
	void hide()						{ visible = false; }
	void show()						{ visible = true; }
	void togglevis()				{ visible = !visible; }
	void rollup()					{ rolledUp  = true; }
	void rollback()					{ rolledUp  = false; }
	void toggleroll()				{ rolledUp  = !rolledUp; }
	void stopresizing()				{ resizingXY = (resizingYX = (resizingY = (resizingX = false))); }
	void startresisingx()			{ resizingX = true; }
	void startresisingxy()			{ resizingXY = true; }
	void startresizingy()			{ resizingY = true; }
	void startresizingyx()			{ resizingYX = true; }
	void disable()					{ enabled = false; }
	void enable()					{ enabled = true; }
	bool isenabled()				{ return enabled || currentColor[3] != 0; }
	bool isalwaysontop()			{ return alwaysOnTop; }
	bool resizing()					{ return resizingXY | resizingYX | resizingX | resizingY; }
	bool canbetransparent()			{ return !noTransparency; }
	bool onborder();
	void moveto(int xx, int yy)		{ x = xx; y = yy; }
	virtual void resizeTo(int ww, int hh)	{ w = ww; h = hh; }
	void initprops(std::string);

	void SetTitle(std::string t)			{ title = t; }
	std::string gettitle()				{ return title; }

	int getcursor();
	virtual void close(bool force = false);

	virtual bool inwindow();
	cWindowObject* inObject();
	bool drag();
	virtual void click();
	virtual void draw();
	virtual void save();
	virtual void doubleclick();
	virtual void rightclick();
	virtual void stopdrag() {}
	virtual void dragover();
	virtual void holddragover();
	virtual void scrollUp();
	virtual void scrollDown();

	virtual bool onKeyUp(int,bool);
	virtual bool onKeyDown(int,bool);
	virtual bool onChar(char,bool);

	cWindowObject* addLabel(std::string, int,int,std::string);
	cWindowObject* addInputBox(std::string, int,int,int,std::string, TiXmlDocument &skin);
	cWindowObject* addCheckBox(std::string, int,int,bool, TiXmlDocument &skin);

	void center();

	virtual void* userfunc(void*) { return NULL; };

	cTexture* texture;
	cFont* font;
	objectlist objects;
	cWindowObject*	selectedObject;
};

extern long mousestartx, mousestarty, dragoffsetx, dragoffsety;

#endif
