#ifndef __INPUTWINDOW_H__
#define __INPUTWINDOW_H__

#include "wm/window.h"

#include <wm/windowbutton.h>
#include <wm/windowinputbox.h>



class cInputWindow : public cWindow
{
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* parent, TiXmlDocument* skin = NULL);
		virtual ~cWindowOkButton() {}
		void click();
	};

	class cWindowCancelButton : public cWindowButton
	{
	public:
		cWindowCancelButton(cWindow* parent, TiXmlDocument* skin = NULL);
		virtual ~cWindowCancelButton() {}
		void click();
	};

public:
	class cInputWindowCaller
	{
	public:
		virtual ~cInputWindowCaller() {};
		std::string data;

		cInputWindowCaller() {};
		virtual void Ok() = 0;
		virtual void Cancel() {};
	};

	cInputWindowCaller* caller;
	cInputWindow(cInputWindowCaller* c);	
	void* userfunc(void* param);
};

#endif
