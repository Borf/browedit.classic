#include "ambientlightwindow.h"
#include <graphics.h>

#include <wm/windowlabel.h>
#include <wm/windowinputbox.h>


extern cGraphics Graphics;

cAmbientLightWindow::cAmbientLightWindowOkButton::cAmbientLightWindowOkButton( cWindow* parent) : cWindowButton(parent)
{
	alignment = ALIGN_BOTTOM;
	moveTo(0, 0);
	resizeTo(100, 20);
	text = "Ok";
}

void cAmbientLightWindow::cAmbientLightWindowOkButton::onClick()
{
	cGraphics::world->ambientLight.ambientr = atoi(parent->objects["ambientr"]->getText(0).c_str());
	cGraphics::world->ambientLight.ambientg = atoi(parent->objects["ambientg"]->getText(0).c_str());
	cGraphics::world->ambientLight.ambientb = atoi(parent->objects["ambientb"]->getText(0).c_str());
	
	cGraphics::world->ambientLight.diffuse.x = atof(parent->objects["diffuser"]->getText(0).c_str());
	cGraphics::world->ambientLight.diffuse.y = atof(parent->objects["diffuseg"]->getText(0).c_str());
	cGraphics::world->ambientLight.diffuse.z = atof(parent->objects["diffuseb"]->getText(0).c_str());
	
	cGraphics::world->ambientLight.shadow.x = atof(parent->objects["shadowr"]->getText(0).c_str());
	cGraphics::world->ambientLight.shadow.y = atof(parent->objects["shadowg"]->getText(0).c_str());
	cGraphics::world->ambientLight.shadow.z = atof(parent->objects["shadowb"]->getText(0).c_str());
	
	cGraphics::world->ambientLight.alpha = atof(parent->objects["alpha"]->getText(0).c_str());
	
	parent->close();
}

cAmbientLightWindow::cAmbientLightWindow( ) : cWindow()
{
	windowType = WT_AMBIENTLIGHT;
	resizable = false;
	visible = true;
	
	h = 150;
	w = 310+skinOffLeft+skinOffRight;
	title = GetMsg("wm/ambient/TITLE");
	center();
	
	defaultObject = "OkButton";
	
	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);
	
	cWindowObject* o;
	
	addLabel("lblAmbient", 0,0,GetMsg("wm/ambient/AMBIENT"));
	addLabel("lblDiffuse", 0,20,GetMsg("wm/ambient/DIFFUSE"));
	addLabel("lblShadow",0,40,GetMsg("wm/ambient/SHADOW"));
	addLabel("lblAlpha", 0,60,GetMsg("wm/ambient/ALPHA"));
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,0);
	o->resizeTo(70,20);
	objects["ambientr"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,0);
	o->resizeTo(70,20);
	objects["ambientg"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,0);
	o->resizeTo(70,20);
	objects["ambientb"] = o;
	///////////////////////////////////////////////
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,20);
	o->resizeTo(70,20);
	objects["diffuser"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,20);
	o->resizeTo(70,20);
	objects["diffuseg"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,20);
	o->resizeTo(70,20);
	objects["diffuseb"] = o;
	///////////////////////////////////////////////
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,40);
	o->resizeTo(70,20);
	objects["shadowr"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,40);
	o->resizeTo(70,20);
	objects["shadowg"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,40);
	o->resizeTo(70,20);
	objects["shadowb"] = o;
	/////////////////////////////////////////////////
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,60);
	o->resizeTo(210,20);
	objects["alpha"] = o;
	
	objects["OkButton"] = new cAmbientLightWindowOkButton(this);
}