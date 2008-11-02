#include "waterwindow.h"
#include <graphics.h>
extern cGraphics Graphics;


cWaterWindowOkButton::cWaterWindowOkButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent,skin)
{
	alignment = ALIGN_BOTTOM;
	moveTo(0, 0);
	resizeTo(parent->innerWidth(), 20);
	text = GetMsg("wm/OK");
}

cWaterWindowOkButton::~cWaterWindowOkButton()
{
	
}

void cWaterWindowOkButton::click()
{
	Graphics.world.water.amplitude = atof(parent->objects["amplitude"]->getText(0).c_str());
	Graphics.world.water.height = atof(parent->objects["height"]->getText(0).c_str());
	Graphics.world.water.phase = atof(parent->objects["phase"]->getText(0).c_str());
	Graphics.world.water.surfaceCurve = atof(parent->objects["surfacecurve"]->getText(0).c_str());
	Graphics.world.water.type = atoi(parent->objects["type"]->getText(0).c_str());
	parent->close();
}

cWaterWindow::cWaterWindow() : cWindow()
{
	windowType = WT_WATER;
	resizable = false;
	visible = true;
	
	h = 140+skinOffTop+skinOffBottom;
	w = 200+skinOffLeft+skinOffRight;
	title = GetMsg("wm/water/TITLE");
	center();
	initprops("water");
	
	defaultObject = "OkButton";
	
	cWindowObject* o;
	
	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);
	
	addLabel("lblAmplitude",	0,0,GetMsg("wm/water/AMPLITUDE"));
	addLabel("lblHeight",		0,20,GetMsg("wm/water/HEIGHT"));
	addLabel("lblPhase",		0,40,GetMsg("wm/water/PHASE"));
	addLabel("lblSurfaceCycle", 0,60,GetMsg("wm/water/SURFACECYCLE"));
	addLabel("lblTexCycle",		0,80,GetMsg("wm/water/TEXTURECYCLE"));
	addLabel("lblType",			0,100,GetMsg("wm/water/TYPE"));
	
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,0);
	o->resizeTo(100,20);
	objects["amplitude"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,20);
	o->resizeTo(100,20);
	objects["height"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,40);
	o->resizeTo(100,20);
	objects["phase"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,60);
	o->resizeTo(100,20);
	objects["surfacecurve"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,80);
	o->resizeTo(100,20);
	objects["texcycle"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,100);
	o->resizeTo(100,20);
	objects["type"] = o;
	
	objects["OkButton"] = new cWaterWindowOkButton(this);
}