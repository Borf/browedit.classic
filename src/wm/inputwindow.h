#ifndef __INPUTWINDOW_H__
#define __INPUTWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"



class cInputWindow : public cWindow
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
	class cInputWindowCaller
	{
	public:
		string data;

		cInputWindowCaller() {};
		virtual void Ok() = 0;
		virtual void Cancel() {};
	};
	cInputWindowCaller* caller;

	
	cInputWindow(cInputWindowCaller* c) : cWindow()
	{
		caller = c;

		wtype = WT_INPUT;
		resizable = false;
		visible = true;
		modal = true;

		h = 200;
		w = 350;
		title = "Please Input...";
		center();

		defaultobject = "OkButton";

		addlabel("text", 15,40,"UserId")->resizeto(w-30, 12);

		cWindowObject* o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15,60);
		o->resizeto(w-30, 20);
		objects["input"] = o;

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);;
		objects["OkButton"] = new cWindowOkButton(this);
		objects["CancelButton"] = new cWindowCancelButton(this);
	
	}	

	void* userfunc(void* param)
	{
		caller->data = objects["input"]->GetText(0);
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
