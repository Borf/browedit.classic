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
		cWindowOkButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
		{
			alignment = ALIGN_BOTTOM;
			moveTo(0, 20);
			resizeTo(100, 20);
			text = "Ok";
		}
		virtual ~cWindowOkButton() {}
		void click()
		{
			parent->close();
		}
	};

public:
	cMessageWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		windowType = WT_MESSAGE;
		resizable = false;
		visible = true;
		modal = true;

		h = 200;
		w = 350;
		title = "Message...";
		center();

		defaultObject = "OkButton";

//		objects["rollup"] = new cWindowRollupButton(this);
//		objects["close"] = new cWindowCloseButton(this);

		addLabel("text", 15,40, "UserId")->resizeTo(320, 100);;
		objects["OkButton"] = new cWindowOkButton(this,skin);
	}	
};

#endif
