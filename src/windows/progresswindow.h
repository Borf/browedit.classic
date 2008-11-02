#ifndef __PROGRESSWINDOW_H__
#define __PROGRESSWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>


class cProgressWindow : public cWindow
{
	class cCancelButton : public cWindowButton
	{
	private:
		bool* cancel;
	public:
		cCancelButton(cWindow* parent, bool* pCancel, TiXmlDocument &skin);
		void click();
	};
public:
	cProgressWindow(cTexture* t, cFont* f, bool* cancel, TiXmlDocument &skin);
};

#endif
