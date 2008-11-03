#ifndef __RSMEDITWINDOW_H__
#define __RSMEDITWINDOW_H__

#include <wm/window.h>

#include <wm/windowbutton.h>
#include <wm/windowrgbpicker.h>
#include <wm/windowpicturebox.h>
#include <wm/windowscrollpanel.h>


class cRSMModel;

class cRSMEditWindow : public cWindow
{
	class cWindowOpenButton : public cWindowButton
	{
	public:
		cWindowOpenButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};
	class cWindowSaveButton : public cWindowButton
	{
	public:
		cWindowSaveButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};
	class cWindowSaveAsButton : public cWindowButton
	{
	public:
		cWindowSaveAsButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};

	class cWindowModel : public cWindowObject
	{
	public:
		cRSMModel* model;
		std::string data;
		float roty;
		float oldx;
		float oldy;
		long rotate;

		cVector3 backgroundcolor;

		cWindowModel(cWindow* parent);
		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
		void drag();
		void setText(int i, std::string s);
		std::string getText(int i);
		void setInt(int i, int id);
		~cWindowModel();
		void onScrollUp();
		void onScrollDown();
	};

	class cWindowModelTexture : public cWindowPictureBox
	{
	public:
		int i;
		cWindowModelTexture(cWindow* parent, int ii);
		void onClick();

	};


	class cRGBPicker : public cWindowRGBPicker
	{
	public:
		cRGBPicker(cWindow* p);
		void onClick();	
	};

public:

	char filename[255];
	int selected;

	cRSMEditWindow();	
	void resizeTo(int ww, int hh);
	void open();
	void onStopDrag();
	void changetexture(std::string newtexture);
};

#endif
