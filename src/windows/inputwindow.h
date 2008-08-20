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

	
	cInputWindow(cInputWindowCaller* c, cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		caller = c;

		windowType = WT_INPUT;
		resizable = false;
		visible = true;
		modal = true;

		h = 200;
		w = 350;
		title = "Please input...";
		center();

		defaultObject = "OkButton";

		addLabel("text", 15,40,"UserId")->resizeTo(w-30, 12);

		cWindowObject* o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveTo(15,60);
		o->resizeTo(w-30, 20);
		objects["input"] = o;
		selectedObject = o;

		objects["rollup"] = new cWindowRollupButton(this,skin);
		objects["close"] = new cWindowCloseButton(this,skin);;
		objects["OkButton"] = new cWindowOkButton(this,skin);
		objects["CancelButton"] = new cWindowCancelButton(this,skin);
	
	}	

	void* userfunc(void* param)
	{
		caller->data = objects["input"]->getText(0);
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
