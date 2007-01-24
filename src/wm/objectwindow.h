#ifndef __OBJECTWINDOW_H__
#define __OBJECTWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"

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


class cObjectWindow : public cWindow
{
public:
	cObjectWindow()
	{
		wtype = WT_OBJECT;
		resizable = false;
		visible = true;
		modal = true;

		h = 200;
		w = 350;
		title = "Object";
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
		o->moveto(15, 20);
		o->resizeto(320, 100);
		o->SetText(0, "Object:");
		objects["text"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 40);
		o->resizeto(320, 100);
		o->SetText(0, "Object:");
		objects["text"] = o;


		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 60);
		o->resizeto(320, 100);
		o->SetText(0, "Position:");
		objects["lblPos"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 80);
		o->resizeto(320, 100);
		o->SetText(0, "Scale:");
		objects["lblScale"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 100);
		o->resizeto(320, 100);
		o->SetText(0, "Rotation:");
		objects["lblRot"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,20);
		o->resizeto(210,20);
		o->SetText(0,"");
		o->SetInt(0,0);
		objects["objectmenu"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,40);
		o->resizeto(210,20);
		o->SetText(0,"");
		o->SetInt(0,0);
		objects["objectname"] = o;
		
		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,60);
		o->resizeto(70,20);
		o->SetText(0,"10");
		objects["posx"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,60);
		o->resizeto(70,20);
		o->SetText(0,"10");
		objects["posy"] = o;
		
		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,60);
		o->resizeto(70,20);
		o->SetText(0,"10");
		objects["posz"] = o;
		
		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,80);
		o->resizeto(70,20);
		o->SetText(0,"1.000000");
		objects["scalex"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,80);
		o->resizeto(70,20);
		o->SetText(0,"1.000000");
		objects["scaley"] = o;
		
		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,80);
		o->resizeto(70,20);
		o->SetText(0,"1.000000");
		objects["scalez"] = o;
		
		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,100);
		o->resizeto(70,20);
		o->SetText(0,"10");
		objects["rotx"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,100);
		o->resizeto(70,20);
		o->SetText(0,"10");
		objects["roty"] = o;
		
		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,100);
		o->resizeto(70,20);
		o->SetText(0,"10");
		objects["rotz"] = o;


		o = new cWindowOkButton();
		o->parent = this;
		objects["OkButton"] = o;
	}	
};

#endif
