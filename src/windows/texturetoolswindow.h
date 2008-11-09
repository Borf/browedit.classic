#ifndef __TEXTURETOOLSWINDOW_H__
#define __TEXTURETOOLSWINDOW_H__

#include <wm/wm.h>
#include <wm/window.h>
#include <wm/windowpicturebox.h>
#include <wm/windowbutton.h>
#include <wm/windowinputbox.h>
//enum eTool;
#include <graphics.h>

class cTextureToolsWindow : public cWindow
{
	class cWindowToolbarButton : public cWindowPictureBox
	{
	public:
		bool activated;
		eTool tool;

		cWindowToolbarButton(cWindow* parent, std::string image, eTool t, TiXmlDocument* totalskin = &cWM::skin);
		virtual void draw(int a,int b,int c,int d);
		virtual void onClick();
	};

	class cWindowBrushShape : public cWindowButton
	{
	public:
		cWindowBrushShape(cWindow* parent, TiXmlDocument* totalskin = &cWM::skin);
		void draw(int a,int b,int c,int d);
		void onClick();
	};

	class cWindowLimitedFloatInputBox : public cWindowFloatInputBox
	{
	public:
		cWindowLimitedFloatInputBox(cWindow* parent, TiXmlDocument* skin = &cWM::skin) : cWindowFloatInputBox(parent,skin) {}
		bool onChar(char,bool);
	};


	class cWindowSelectArea : public cWindowToolbarButton
	{
	public:
		cWindowSelectArea(cWindow* parent, TiXmlDocument* totalskin = &cWM::skin);
	};
	class cWindowSelectBrush : public cWindowToolbarButton
	{
	public:
		cWindowSelectBrush(cWindow* parent, TiXmlDocument* totalskin = &cWM::skin);
	};
	class cWindowSelectWand : public cWindowToolbarButton
	{
	public:
		cWindowSelectWand(cWindow* parent, TiXmlDocument* totalskin = &cWM::skin);
	};
	class cWindowBrush : public cWindowToolbarButton
	{
	public:
		cWindowBrush(cWindow* parent, TiXmlDocument* totalskin = &cWM::skin);
	};

public:
	cTextureToolsWindow();
	void reOrder();
	void resizeTo(int ww, int hh);

};

#endif
