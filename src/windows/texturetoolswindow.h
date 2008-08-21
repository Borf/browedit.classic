#ifndef __TEXTURETOOLSWINDOW_H__
#define __TEXTURETOOLSWINDOW_H__

#include <wm/window.h>
#include <wm/windowpicturebox.h>
#include <wm/windowbutton.h>
enum eTool;

class cTextureToolsWindow : public cWindow
{
	class cWindowToolbarButton : public cWindowPictureBox
	{
	public:
		bool activated;
		eTool tool;

		cWindowToolbarButton(cWindow* parent, TiXmlDocument &totalskin, std::string image, eTool t);
		virtual void draw(int a,int b,int c,int d);
		virtual void click();
	};

	class cWindowBrushShape : public cWindowButton
	{
	public:
		cWindowBrushShape(cWindow* parent, TiXmlDocument &totalskin);
		void draw(int a,int b,int c,int d);
		void click();
	};


	class cWindowSelectArea : public cWindowToolbarButton
	{
	public:
		cWindowSelectArea(cWindow* parent, TiXmlDocument &totalskin);
	};
	class cWindowSelectBrush : public cWindowToolbarButton
	{
	public:
		cWindowSelectBrush(cWindow* parent, TiXmlDocument &totalskin);
	};
	class cWindowSelectWand : public cWindowToolbarButton
	{
	public:
		cWindowSelectWand(cWindow* parent, TiXmlDocument &totalskin);
	};
	class cWindowBrush : public cWindowToolbarButton
	{
	public:
		cWindowBrush(cWindow* parent, TiXmlDocument &totalskin);
	};

public:
	cTextureToolsWindow(cTexture* t, cFont* f, TiXmlDocument &skin);
	void reOrder();
	void resizeTo(int ww, int hh);

};

#endif
