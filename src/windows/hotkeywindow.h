#ifndef __HOTKEYWINDOW_H__
#define __HOTKEYWINDOW_H__

#include <wm/window.h>

#include <wm/windowpicturebox.h>

class cHotkeyWindow : public cWindow
{
public:
	class cHotkeyButton : public cWindowPictureBox
	{
	public:
		GLuint tid;
		char* im;
			
		cVector2 camerapointer;
		float camerarot;
		float cameraheight;
		float cameraangle;
		bool topcamera;
		bool sidecamera;

		cHotkeyButton(cWindow* parent);
		~cHotkeyButton();
		void onRightClick();
		void onClick();
		cWindowObject* inObject();
		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
		void* userfunc(void* param);
	};



	cHotkeyWindow();	

};

#endif
