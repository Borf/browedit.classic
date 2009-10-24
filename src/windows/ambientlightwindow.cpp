#include <common.h>
#include "ambientlightwindow.h"
#include <graphics.h>

#include <wm/windowlabel.h>
#include <wm/windowinputbox.h>


cAmbientLightWindow::cAmbientLightWindowOkButton::cAmbientLightWindowOkButton( cWindow* parent) : cWindowButton(parent)
{
	alignment = ALIGN_BOTTOM;
	moveTo(0, 5);
	resizeTo(100, 20);
	text = "Ok";
}

void cAmbientLightWindow::cAmbientLightWindowOkButton::onClick()
{
	//Corrected global light properties window according to the new file information. by Henko

	cGraphics::world->ambientLight.lightLongitude = atoi(parent->objects["longitude"]->getText(0).c_str());
	cGraphics::world->ambientLight.lightLatitude = atoi(parent->objects["latitude"]->getText(0).c_str());

	cGraphics::world->ambientLight.ambient.x = atof(parent->objects["ambientr"]->getText(0).c_str());
	cGraphics::world->ambientLight.ambient.y = atof(parent->objects["ambientg"]->getText(0).c_str());
	cGraphics::world->ambientLight.ambient.z = atof(parent->objects["ambientb"]->getText(0).c_str());
	
	cGraphics::world->ambientLight.diffuse.x = atof(parent->objects["diffuser"]->getText(0).c_str());
	cGraphics::world->ambientLight.diffuse.y = atof(parent->objects["diffuseg"]->getText(0).c_str());
	cGraphics::world->ambientLight.diffuse.z = atof(parent->objects["diffuseb"]->getText(0).c_str());

	cGraphics::world->ambientLight.ambintensity = atof(parent->objects["ambintensity"]->getText(0).c_str());
	
	parent->close();
}

cAmbientLightWindow::cAmbientLightWindow( ) : cWindow()
{
	windowType = WT_AMBIENTLIGHT;
	resizable = false;
	visible = true;
	
	h = 240;
	w = 330+skinOffLeft+skinOffRight;
	title = GetMsg("wm/globallighting/TITLE");
	center();
	
	defaultObject = "OkButton";
	
	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);
	
	cWindowObject* o;

	//Corrected global light properties window according to the new file information. by Henko
	
	addLabel("lblDiffuseTitle", 3,0,GetMsg("wm/globallighting/DIFFUSETITLE"));
	addLabel("lblDiffuse", 3,20,GetMsg("wm/globallighting/DIFFUSECOLOR"));
	addLabel("lblLongitude", 3,40,GetMsg("wm/globallighting/LONGITUDE"));
	addLabel("lblLatitude", 3,60,GetMsg("wm/globallighting/LATITUDE"));
	
	addLabel("lblAmbientTitle", 3,100,GetMsg("wm/globallighting/AMBIENTTITLE"));
	addLabel("lblAmbient", 3,120,GetMsg("wm/globallighting/AMBIENTCOLOR"));
	addLabel("lblAlpha", 3,140,GetMsg("wm/globallighting/AMBINTENSITY"));
	
	///////////////////////////////////////////////
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPRIGHT;
	o->moveTo(140,20);
	o->resizeTo(70,20);
	objects["diffuser"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPRIGHT;
	o->moveTo(70,20);
	o->resizeTo(70,20);
	objects["diffuseg"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPRIGHT;
	o->moveTo(0,20);
	o->resizeTo(70,20);
	objects["diffuseb"] = o;

	///////////////////////////////////////////////

	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPRIGHT;
	o->moveTo(0,40);
	o->resizeTo(210,20);
	objects["longitude"] = o;

	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPRIGHT;
	o->moveTo(0,60);
	o->resizeTo(210,20);
	objects["latitude"] = o;

	///////////////////////////////////////////////

	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPRIGHT;
	o->moveTo(140,120);
	o->resizeTo(70,20);
	objects["ambientr"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPRIGHT;
	o->moveTo(70,120);
	o->resizeTo(70,20);
	objects["ambientg"] = o;
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPRIGHT;
	o->moveTo(0,120);
	o->resizeTo(70,20);
	objects["ambientb"] = o;

	///////////////////////////////////////////////

	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPRIGHT;
	o->moveTo(0,140);
	o->resizeTo(210,20);
	objects["ambintensity"] = o;

	///////////////////////////////////////////////
	
	objects["OkButton"] = new cAmbientLightWindowOkButton(this);
}
