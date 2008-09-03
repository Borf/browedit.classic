#ifndef __TEXTUREBRUSHWINDOW_H__
#define __TEXTUREBRUSHWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>

class cTextureBrushWindow : public cWindow
{
	class cWindowBrushButton : public cWindowObject
	{
		std::vector<std::vector<bool> > brush;
	public:
		cWindowBrushButton(cWindow* parent, TiXmlDocument &skin,int,int,std::string);
		void draw(int,int,int,int);

	};


	public:
		cTextureBrushWindow(cTexture* t, cFont* f, TiXmlDocument &skin);
		void resizeTo(int ww, int hh);
		
};

#endif
