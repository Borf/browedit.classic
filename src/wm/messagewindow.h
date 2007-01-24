#ifndef __MESSAGEWINDOW_H__
#define __MESSAGEWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"

class cWindowOkButton : public cWindowButton
{
public:
	cWindowOkButton()
	{
		alignment = ALIGN_BOTTOM;
		moveto(0, 20);
		resizeto(100, 20);
		text = "            Ok";
	}
	virtual ~cWindowOkButton() {}
	void click()
	{
		parent->close();
	}
};


class cMessageWindow : public cWindow
{
public:
	cMessageWindow()
	{
		wtype = WT_MESSAGE;
		resizable = false;
		visible = true;
		modal = true;

		h = 200;
		w = 350;
		title = "Message";
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
	}	
};

#endif
