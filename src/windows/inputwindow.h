#ifndef __INPUTWINDOW_H__
#define __INPUTWINDOW_H__

#include "wm/window.h"

#include <wm/windowbutton.h>
#include <wm/windowinputbox.h>


class cInputWindowCaller
{
public:
	virtual ~cInputWindowCaller() {};
	std::string data;
	
	cInputWindowCaller() {};
	virtual void Ok() = 0;
	virtual void Cancel() {};
};


class cInputWindow : public cWindow
{
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		virtual ~cWindowOkButton() {}
		void onClick();
	};

	class cWindowCancelButton : public cWindowButton
	{
	public:
		cWindowCancelButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		virtual ~cWindowCancelButton() {}
		void onClick();
	};

public:

	cInputWindowCaller* caller;
	cInputWindow(cInputWindowCaller* c);	
	void* userfunc(void* param);
};

#endif
