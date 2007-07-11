#ifndef __MESSAGEWINDOW_H__
#define __MESSAGEWINDOW_H__

#include "../wm/window.h"

#include "../wm/windowobject.h"
#include "../wm/windowroundbutton.h"
#include "../wm/windowbutton.h"
#include "../wm/windowlabel.h"



class cMessageWindow : public cWindow
{
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* parent) : cWindowButton(parent)
		{
			alignment = ALIGN_BOTTOM;
			moveto(0, 20);
			resizeto(100, 20);
			text = "Ok";
		}
		virtual ~cWindowOkButton() {}
		void click()
		{
			parent->close();
		}
	};

public:
	cMessageWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_MESSAGE;
		resizable = false;
		visible = true;
		modal = true;

		h = 200;
		w = 350;
		title = "Message...";
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		addlabel("text", 15,40, "UserId")->resizeto(320, 100);;
		objects["OkButton"] = new cWindowOkButton(this);
	}	
};

#endif
