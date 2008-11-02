#ifndef __TEXTURETOOLSWINDOW_H__
#define __TEXTURETOOLSWINDOW_H__

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

		cWindowToolbarButton(cWindow* parent, std::string image, eTool t, TiXmlDocument* totalskin = NULL);
		virtual void draw(int a,int b,int c,int d);
		virtual void click();
	};

	class cWindowBrushShape : public cWindowButton
	{
	public:
		cWindowBrushShape(cWindow* parent, TiXmlDocument* totalskin = NULL);
		void draw(int a,int b,int c,int d);
		void click();
	};

	class cWindowLimitedFloatInputBox : public cWindowFloatInputBox
	{
	public:
		cWindowLimitedFloatInputBox(cWindow* parent, TiXmlDocument* skin = NULL) : cWindowFloatInputBox(parent,skin) {}
		bool onChar(char,bool);
	};


	class cWindowSelectArea : public cWindowToolbarButton
	{
	public:
		cWindowSelectArea(cWindow* parent, TiXmlDocument* totalskin = NULL);
	};
	class cWindowSelectBrush : public cWindowToolbarButton
	{
	public:
		cWindowSelectBrush(cWindow* parent, TiXmlDocument* totalskin = NULL);
	};
	class cWindowSelectWand : public cWindowToolbarButton
	{
	public:
		cWindowSelectWand(cWindow* parent, TiXmlDocument* totalskin = NULL);
	};
	class cWindowBrush : public cWindowToolbarButton
	{
	public:
		cWindowBrush(cWindow* parent, TiXmlDocument* totalskin = NULL);
	};

public:
	cTextureToolsWindow();
	void reOrder();
	void resizeTo(int ww, int hh);

};

#endif
