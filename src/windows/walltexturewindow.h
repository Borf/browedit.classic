#ifndef __WALLTEXTUREWINDOW_H__
#define __WALLTEXTUREWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
#include <graphics.h>
#include <undo.h>

class cWallTextureWindow : public cWindow
{
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow*);
		void onClick();
	};
	class cWindowCancelButton : public cWindowButton
	{
	public:
		cWindowCancelButton(cWindow*);
		void onClick();
	};

	class cWindowMyCloseButton : public cWindowCloseButton
	{
	public:
		cWindowMyCloseButton(cWindow*);
		void onClick();
	};

	class cWindowHorizontalFlipButton : public cWindowButton
	{
	public:
		cWindowHorizontalFlipButton(cWindow*);
		void onClick();
	};
	class cWindowVerticalFlipButton : public cWindowButton
	{
	public:
		cWindowVerticalFlipButton(cWindow*);
		void onClick();
	};

	
	class cWindowWallTextureBox : public cWindowObject
	{
		int wallHeight;
		cTile* applyTile;
	public:
		cTile	tile;

		cWindowWallTextureBox(int,int,bool,cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		void draw(int,int,int,int);
		void drag();
	};
public:
	cWallTextureWindow(int,int,bool);	
	cUndoItem* undo;
};

#endif
