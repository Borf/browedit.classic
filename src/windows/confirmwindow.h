#ifndef __CONFIRMWINDOW_H__
#define __CONFIRMWINDOW_H__

#include <wm/window.h>

#include <wm/windowbutton.h>
#include <wm/windowlabel.h>



class cConfirmWindow : public cWindow
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
	class cConfirmWindowCaller
	{
	public:
		virtual ~cConfirmWindowCaller() { };
		cConfirmWindowCaller() {};
		virtual void Ok() = 0;
		virtual void Cancel() {};
	};

	
	cConfirmWindowCaller* caller;
	cConfirmWindow(cConfirmWindowCaller* c);	
	void* userfunc(void* param);
};

#endif
