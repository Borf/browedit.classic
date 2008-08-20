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
	cWaterWindowOkButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
	{
		alignment = ALIGN_BOTTOM;
		moveto(0, 0);
		resizeto(parent->innerw(), 20);
		text = GetMsg("wm/OK");
	}
	virtual ~cWaterWindowOkButton() {}
	void click()
	{
		Graphics.world.water.amplitude = atof(parent->objects["amplitude"]->GetText(0).c_str());
		Graphics.world.water.height = atof(parent->objects["height"]->GetText(0).c_str());
		Graphics.world.water.phase = atof(parent->objects["phase"]->GetText(0).c_str());
		Graphics.world.water.surfaceCurve = atof(parent->objects["surfacecurve"]->GetText(0).c_str());
		Graphics.world.water.type = atoi(parent->objects["type"]->GetText(0).c_str());
		parent->close();
	}
};


class cWaterWindow : public cWindow
{
public:
	cWaterWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		wtype = WT_WATER;
		resizable = false;
		visible = true;

		h = 140+skinOffTop+skinOffBottom;
		w = 200+skinOffLeft+skinOffRight;
		title = GetMsg("wm/water/TITLE");
		center();
		initprops("water");

		defaultobject = "OkButton";

		cWindowObject* o;
		
		objects["rollup"] = new cWindowRollupButton(this,skin);
		objects["close"] = new cWindowCloseButton(this,skin);

		addlabel("lblAmplitude",	0,0,GetMsg("wm/water/AMPLITUDE"));
		addlabel("lblHeight",		0,20,GetMsg("wm/water/HEIGHT"));
		addlabel("lblPhase",		0,40,GetMsg("wm/water/PHASE"));
		addlabel("lblSurfaceCycle", 0,60,GetMsg("wm/water/SURFACECYCLE"));
		addlabel("lblTexCycle",		0,80,GetMsg("wm/water/TEXTURECYCLE"));
		addlabel("lblType",			0,100,GetMsg("wm/water/TYPE"));


		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,0);
		o->resizeto(100,20);
		objects["amplitude"] = o;
		
		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,20);
		o->resizeto(100,20);
		objects["height"] = o;

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,40);
		o->resizeto(100,20);
		objects["phase"] = o;

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,60);
		o->resizeto(100,20);
		objects["surfacecurve"] = o;

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,80);
		o->resizeto(100,20);
		objects["texcycle"] = o;

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,100);
		o->resizeto(100,20);
		objects["type"] = o;
		
		objects["OkButton"] = new cWaterWindowOkButton(this,skin);
	}	
};

#endif
