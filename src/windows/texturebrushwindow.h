#ifndef __TEXTUREBRUSHWINDOW_H__
#define __TEXTUREBRUSHWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
#include <vector>

class cTextureBrushWindow : public cWindow
{
	class cWindowBrushButton : public cWindowObject
	{
		std::vector<std::vector<bool> > brush;
	public:
		cWindowBrushButton(cWindow* parent, TiXmlDocument &skin,int,int,std::string);
		void draw(int,int,int,int);
		void click();
	};

	class cWindowBrushTile : public cWindowObject
	{
	public:
		bool	on;
		cWindowBrushTile(cWindow* parent, TiXmlDocument &skin);
		void draw(int,int,int,int);
		void click();
	};

	class cWindowBrushChangeButton : public cWindowButton
	{
	public:
		cWindowBrushChangeButton(cWindow* parent, TiXmlDocument &skin);
		void click();
	};

	class cWindowBrushOkButton : public cWindowButton
	{
	public:
		cWindowBrushOkButton(cWindow* parent, TiXmlDocument &skin);
		void click();
	};

	class cWindowBrushSaveButton : public cWindowButton
	{
	public:
		cWindowBrushSaveButton(cWindow* parent, TiXmlDocument &skin);
		void click();
	};


	public:
		int brushWidth;
		int brushHeight;
		std::vector<std::vector<bool> > brush;

		cTextureBrushWindow(cTexture* t, cFont* f, TiXmlDocument &skin);
		void resizeTo(int ww, int hh);
		
};

#endif
