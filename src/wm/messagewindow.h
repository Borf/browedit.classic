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
	cWindowOkButton(cWindow* parent) : cWindowButton(parent)
	{
		alignment = ALIGN_BOTTOM;
		moveto(0, 20);
		resizeto(100, 20);
		text = msgtable[WINDOW_OK];
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
		title = msgtable[WINDOW_MESSAGE_TITLE];
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		addlabel("text", 15,40, "UserId")->resizeto(320, 100);;
		objects["OkButton"] = new cWindowOkButton(this);
	}	
};

#endif
