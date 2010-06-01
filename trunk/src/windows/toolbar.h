#ifndef __TOOLBARWINDOW_H__
#define __TOOLBARWINDOW_H__

#include <wm/wm.h>
#include <wm/window.h>
#include <wm/windowpicturebox.h>
#include <wm/windowbutton.h>

#include <graphics.h>

class cToolBarWindow : public cWindow
{
	class cModeButton : public cWindowToggleButton
	{
		int mode;
	public:
		cModeButton(cWindow* parent, int mode);
		void onClick();
		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
	};




	public:
		cToolBarWindow();
		void resizeTo(int ww, int hh);
		void draw();
};

#endif
