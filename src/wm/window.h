#ifndef __WINDOW_H__
#define __WINDOW_H__

#ifdef WIN32
#ifndef __MINGW32__
#pragma warning( disable : 4786 )
#endif
#endif
#include <map>
#include <string>

class cTexture;
class cFont;
class cWindowObject;

#include <tinyxml/tinyxml.h>

typedef std::map<std::string, cWindowObject*, std::less<std::string> > objectlist;

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
			int				skinOffLeft;
			int				skinOffRight;
			int				skinOffTop;
			int				skinOffBottom;

			
			float			fontColor[3];
			float			currentColor[4];
			bool			saveWindow;
			std::string		defaultObject;

							cWindow(cTexture* t = NULL, cFont* f = NULL, TiXmlDocument* skin = NULL);
			virtual			~cWindow();
			int				getX()						{ return x; }
			int				getY()						{ return rolledUp ? y+h-16 : y; }
			int				py2()						{ return y; }
			int				getWidth()					{ return w; }
			int				getHeight()				{ return rolledUp ? 16 : h; }
			int				ph2()						{ return h; }
			int				innerWidth()				{ return w-skinOffLeft-skinOffRight; }
			int				innerHeight()				{ return h-skinOffTop-skinOffBottom; }
			bool			modality()					{ return modal; }
			void			istopwindow(bool b)		{ topWindow = b; }
			bool			istopwindow()				{ return topWindow; }

			void			init(cTexture* t, cFont* f)	{ texture = t; font = f; }
				
			WINDOW_TYPE		windowtype()				{ return windowType; }
			bool 			isVisible()					{ return visible || currentColor[3] != 0; }
			void 			hide()						{ visible = false; }
			void 			show()						{ visible = true; }
			void 			togglevis()					{ visible = !visible; }
			void 			rollup()					{ rolledUp  = true; }
			void 			rollback()					{ rolledUp  = false; }
			void 			toggleroll()				{ rolledUp  = !rolledUp; }
			void 			stopresizing()				{ resizingXY = (resizingYX = (resizingY = (resizingX = false))); }
			void 			startresisingx()			{ resizingX = true; }
			void 			startresisingxy()			{ resizingXY = true; }
			void 			startresizingy()			{ resizingY = true; }
			void 			startresizingyx()			{ resizingYX = true; }
			void 			disable()					{ enabled = false; }
			void 			enable()					{ enabled = true; }
			bool 			isEnabled()					{ return enabled || currentColor[3] != 0; }
			bool 			isAlwaysOnTop()				{ return alwaysOnTop; }
			bool 			isResizing()				{ return resizingXY | resizingYX | resizingX | resizingY; }
			bool			canBeResized()				{ return !noTransparency; }
			bool			onBorder();
			void			moveTo(int xx, int yy)		{ x = xx; y = yy; }
	virtual void			resizeTo(int ww, int hh)	{ w = ww; h = hh; }
			void			initProps(std::string);

			void			SetTitle(std::string t)	{ title = t; }
			std::string		getTitle()			{ return title; }

			int				getCursor();
	virtual void			close(bool force = false);

	virtual	bool			inWindow();
			cWindowObject*	inObject();
			bool			drag();
	virtual void			draw();
	virtual void			save();


	
	virtual void			onClick();
	virtual void			onDoubleClick();
	virtual void			onRightClick();
	virtual void			onStopDrag() {}
	virtual void			onDragOver();
	virtual void			onHoldDragOver();
	virtual void 			onScrollUp();
	virtual void 			onScrollDown();
	virtual bool 			onKeyUp(int,bool);
	virtual bool 			onKeyDown(int,bool);
	virtual bool 			onChar(char,bool);

			cWindowObject*	addLabel(std::string, int,int,std::string);
			cWindowObject*	addInputBox(std::string, int,int,int,std::string, TiXmlDocument* = NULL);
			cWindowObject*	addCheckBox(std::string, int,int,bool, TiXmlDocument* = NULL);

			void			center();

	virtual void*			userfunc(void*) { return NULL; };

	cTexture*				texture;
	cFont*					font;
	objectlist				objects;
	cWindowObject*			selectedObject;
};

#endif
