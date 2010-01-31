#ifndef __CONFIRMWINDOW_H__
#define __CONFIRMWINDOW_H__

#include <wm/window.h>

#include <wm/windowbutton.h>
#include <wm/windowlabel.h>


class cConfirmWindowCaller
{
public:
	virtual ~cConfirmWindowCaller() { };
	cConfirmWindowCaller() {};
	virtual void Ok() = 0;
	virtual void Cancel() {};
};


class cConfirmWindow : public cWindow
{
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* parent, Json::Value &skin = cWM::skin);
		virtual ~cWindowOkButton() {}
		void onClick();
	};

	class cWindowCancelButton : public cWindowButton
	{
	public:
		cWindowCancelButton(cWindow* parent, Json::Value &skin = cWM::skin);
		virtual ~cWindowCancelButton() {}
		void onClick();
	};

public:

	cConfirmWindowCaller* caller;
	cConfirmWindow(cConfirmWindowCaller* c);	
	void* userfunc(void* param);
};

#endif
