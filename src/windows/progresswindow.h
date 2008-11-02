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
		cCancelButton(cWindow* parent, bool* pCancel, TiXmlDocument* skin = NULL);
		void click();
	};
public:
	cProgressWindow(bool* cancel);
};

#endif
