#ifndef __WATERWINDOW_H__
#define __WATERWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"


class cWaterWindowOkButton : public cWindowButton
{
public:
	cWaterWindowOkButton(cWindow* parent) : cWindowButton(parent)
	{
		alignment = ALIGN_BOTTOM;
		moveto(0, 20);
		resizeto(100, 20);
		text = GetMsg("wm/OK");
	}
	virtual ~cWaterWindowOkButton() {}
	void click()
	{
		Graphics.world.water.amplitude = atof(parent->objects["amplitude"]->GetText(0).c_str());
		Graphics.world.water.height = atof(parent->objects["height"]->GetText(0).c_str());
		Graphics.world.water.phase = atof(parent->objects["phase"]->GetText(0).c_str());
		Graphics.world.water.surfacecurve = atof(parent->objects["surfacecurve"]->GetText(0).c_str());
		Graphics.world.water.type = atoi(parent->objects["type"]->GetText(0).c_str());
		parent->close();
	}
};


class cWaterWindow : public cWindow
{
public:
	cWaterWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_WATER;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = GetMsg("wm/water/TITLE");
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		
		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		addlabel("lblAmplitude", 15,20,GetMsg("wm/water/AMPLITUDE"));
		addlabel("lblHeight", 15,40,GetMsg("wm/water/HEIGHT"));
		addlabel("lblPhase", 15,60,GetMsg("wm/water/PHASE"));
		addlabel("lblSurfaceCycle", 15,80,GetMsg("wm/water/SURFACECYCLE"));
		addlabel("lblTexCycle", 15,100,GetMsg("wm/water/TEXTURECYCLE"));
		addlabel("lblType", 15,120,GetMsg("wm/water/TYPE"));


		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,20);
		o->resizeto(70,20);
		objects["amplitude"] = o;
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,40);
		o->resizeto(70,20);
		objects["height"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,60);
		o->resizeto(70,20);
		objects["phase"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,80);
		o->resizeto(70,20);
		objects["surfacecurve"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,100);
		o->resizeto(70,20);
		objects["texcycle"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,120);
		o->resizeto(70,20);
		objects["type"] = o;
		
		objects["OkButton"] = new cWaterWindowOkButton(this);
	}	
};

#endif
