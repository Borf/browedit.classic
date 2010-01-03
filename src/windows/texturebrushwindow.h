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
		cWindowBrushButton(cWindow* parent,int,int,std::string, Json::Value &skin = cWM::skin);
		void draw(int,int,int,int);
		void onClick();
	};

	class cWindowBrushTile : public cWindowObject
	{
	public:
		bool	on;
		cWindowBrushTile(cWindow* parent, Json::Value &skin = cWM::skin);
		void draw(int,int,int,int);
		void onClick();
	};

	class cWindowBrushChangeButton : public cWindowButton
	{
	public:
		cWindowBrushChangeButton(cWindow* parent, Json::Value &skin = cWM::skin);
		void onClick();
	};

	class cWindowBrushOkButton : public cWindowButton
	{
	public:
		cWindowBrushOkButton(cWindow* parent, Json::Value &skin = cWM::skin);
		void onClick();
	};

	class cWindowBrushSaveButton : public cWindowButton
	{
	public:
		cWindowBrushSaveButton(cWindow* parent, Json::Value &skin = cWM::skin);
		void onClick();
	};

	class cWindowBrushRotateButton : public cWindowButton
	{
	public:
		cWindowBrushRotateButton(cWindow* parent, Json::Value &skin = cWM::skin);
		void onClick();
	};


	public:
		int brushWidth;
		int brushHeight;
		std::vector<std::vector<bool> > brush;

		cTextureBrushWindow();
		void resizeTo(int ww, int hh);
		
};

#endif
