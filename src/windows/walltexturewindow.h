#ifndef __WALLTEXTUREWINDOW_H__
#define __WALLTEXTUREWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
#include <graphics.h>

class cWallTextureWindow : public cWindow
{
	class cWindowWallTextureBox : public cWindowObject
	{
		cTile	tile;
		int wallHeight;
		cTile* applyTile;
	public:
		cWindowWallTextureBox(int,int,bool,cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		void draw(int,int,int,int);
		void drag();
	};
public:
	cWallTextureWindow(int,int,bool);	
};

#endif
