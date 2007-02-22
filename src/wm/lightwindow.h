#ifndef __LIGHTWINDOW_H__
#define __LIGHTWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"


class cLightWindow : public cWindow
{
public:
	cLightWindow()
	{
		wtype = WT_LIGHT;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = "Light";
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
		o->moveto(15, 60);
		o->resizeto(320, 100);
		o->SetText(0, "Position:");
		objects["lblPos"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 80);
		o->resizeto(320, 100);
		o->SetText(0, "Color:");
		objects["lblScale"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 100);
		o->resizeto(320, 100);
		o->SetText(0, "Intensity:");
		objects["lblRot"] = o;
		
		o = new cWindowFloatInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,60);
		o->resizeto(70,20);
		objects["posx"] = o;

		o = new cWindowFloatInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,60);
		o->resizeto(70,20);
		objects["posy"] = o;
		
		o = new cWindowFloatInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,60);
		o->resizeto(70,20);
		objects["posz"] = o;
		
		o = new cWindowFloatInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,80);
		o->resizeto(70,20);
		objects["colorr"] = o;

		o = new cWindowFloatInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,80);
		o->resizeto(70,20);
		objects["colorg"] = o;
		
		o = new cWindowFloatInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,80);
		o->resizeto(70,20);
		objects["colorb"] = o;
		
		o = new cWindowFloatInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,100);
		o->resizeto(210,20);
		objects["intensity"] = o;

		o = new cWindowOkButton();
		o->parent = this;
		objects["OkButton"] = o;

		o = new cWindowCancelButton();
		o->parent = this;
		objects["CancelButton"] = o;
	}	
};

#endif
