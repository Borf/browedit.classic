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
		cWindowOkButton()
		{
			alignment = ALIGN_BOTTOMLEFT;
			moveto(20, 20);
			resizeto(100, 20);
			text = "            Ok";
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
		cWindowCancelButton()
		{
			alignment = ALIGN_BOTTOMRIGHT;
			moveto(20, 20);
			resizeto(100, 20);
			text = "            Cancel";
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

	
	cConfirmWindow(cConfirmWindowCaller* c) : cWindow()
	{
		caller = c;

		wtype = WT_CONFIRM;
		resizable = false;
		visible = true;
		modal = true;

		h = 200;
		w = 350;
		title = "Please confirm...";
		center();

		defaultobject = "OkButton";

		cWindowObject* o = new cWindowRollupButton();
		o->parent = this;
		objects["rollup"] = o;

		o = new cWindowCloseButton();
		o->parent = this;
		objects["close"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 40);
		o->resizeto(320, 100);
		o->SetText(0, "UserId");
		objects["text"] = o;

		o = new cWindowOkButton();
		o->parent = this;
		objects["OkButton"] = o;

		o = new cWindowCancelButton();
		o->parent = this;
		objects["CancelButton"] = o;
	
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
