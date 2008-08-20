#ifndef __CONFIRMWINDOW_H__
#define __CONFIRMWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"



class cConfirmWindow : public cWindow
{

	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent, skin)
		{
			alignment = ALIGN_BOTTOMLEFT;
			moveTo(20, 20);
			resizeTo(100, 20);
			text = "Ok";
		}
		virtual ~cWindowOkButton() {}
		void click()
		{
			parent->userfunc((void*)1);
		}
	};

	class cWindowCancelButton : public cWindowButton
	{
	public:
		cWindowCancelButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent, skin)
		{
			alignment = ALIGN_BOTTOMRIGHT;
			moveTo(20, 20);
			resizeTo(100, 20);
			text = "Cancel";
		}
		virtual ~cWindowCancelButton() {}
		void click()
		{
			parent->userfunc((void*)0);
		}
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

	
	cConfirmWindow(cConfirmWindowCaller* c, cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t, f,skin)
	{
		caller = c;

		windowType = WT_CONFIRM;
		resizable = false;
		visible = true;
		modal = true;

		h = 200;
		w = 350;
		title = "Please confirm";
		center();

		defaultObject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this,skin);
		objects["close"] = new cWindowCloseButton(this,skin);
		addLabel("text", 15,40,"UserId")->resizeTo(w-30, h-100);
		objects["OkButton"] = new cWindowOkButton(this, skin);
		objects["CancelButton"] = new cWindowCancelButton(this, skin);
	
	}	

	void* userfunc(void* param)
	{
		if ((int)param == 1)
			caller->Ok();
		else
			caller->Cancel();

		delete caller;
		close();
		return NULL;
	}
};

#endif
