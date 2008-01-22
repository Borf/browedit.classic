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
		cWindowOkButton(cWindow* parent) : cWindowButton(parent)
		{
			alignment = ALIGN_BOTTOMLEFT;
			moveto(20, 20);
			resizeto(100, 20);
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
		cWindowCancelButton(cWindow* parent) : cWindowButton(parent)
		{
			alignment = ALIGN_BOTTOMRIGHT;
			moveto(20, 20);
			resizeto(100, 20);
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
		cConfirmWindowCaller() {};
		virtual void Ok() = 0;
		virtual void Cancel() {};
	};
	cConfirmWindowCaller* caller;

	
	cConfirmWindow(cConfirmWindowCaller* c, cTexture* t, cFont* f) : cWindow(t, f)
	{
		caller = c;

		wtype = WT_CONFIRM;
		resizable = false;
		visible = true;
		modal = true;

		h = 200;
		w = 350;
		title = "Please confirm";
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);;
		addlabel("text", 15,40,"UserId")->resizeto(w-30, h-100);
		objects["OkButton"] = new cWindowOkButton(this);
		objects["CancelButton"] = new cWindowCancelButton(this);
	
	}	

	void* userfunc(void* param)
	{
		if ((uintptr_t)param == 1)
			caller->Ok();
		else
			caller->Cancel();

		delete caller;
		close();
		return NULL;
	}
};

#endif
