#include <common.h>
#include "lightwindow.h"

#include <wm/windowbutton.h>
#include <wm/windowinputbox.h>
#include <wm/windowcheckbox.h>
#include "objectwindow.h"
#include <graphics.h>

cLightWindow::cLightWindow( ) : cWindow()
{
	windowType = WT_LIGHT;
	resizable = false;
	visible = true;
	
	h = 180+skinOffTop+skinOffBottom;
	w = 310+skinOffLeft+skinOffRight;
	title = GetMsg("wm/light/TITLE");
	center();
	
	defaultObject = "OkButton";
	
	cWindowObject* o;
	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);
	
	addLabel("lblName",				0, 0, GetMsg("wm/light/NAME"));
	addLabel("lblPos",				0, 20, GetMsg("wm/light/POSITION"));
	addLabel("lblColor",			0, 40, GetMsg("wm/light/COLOR"));
	addLabel("lblIntensity",		0, 60, GetMsg("wm/light/INTENSITY"));
	addLabel("lblRange",			0, 80, GetMsg("wm/light/RANGE"));
	addLabel("lblMaxlightincrement",0, 100, GetMsg("wm/light/MAXLIGHTINCREMENT"));
	addLabel("lblLightfalloff",		0, 120, GetMsg("wm/light/LIGHTFALLOFF"));
	addLabel("lblCastsShadow",		0, 140, GetMsg("wm/light/CASTSSHADOW"));
	
	o = new cWindowStringInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,0);
	o->resizeTo(140,20);
	objects["name"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,20);
	o->resizeTo(70,20);
	objects["posx"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,20);
	o->resizeTo(70,20);
	objects["posy"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,20);
	o->resizeTo(70,20);
	objects["posz"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,40);
	o->resizeTo(70,20);
	objects["colorr"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,40);
	o->resizeTo(70,20);
	objects["colorg"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,40);
	o->resizeTo(70,20);
	objects["colorb"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,60);
	o->resizeTo(140,20);
	objects["intensity"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,80);
	o->resizeTo(140,20);
	objects["range"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,100);
	o->resizeTo(140,20);
	objects["maxlightincrement"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,120);
	o->resizeTo(140,20);
	objects["lightfalloff"] = o;
	
	o = new cWindowBoolCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,142);
	objects["castshadow"] = o;
	
	
	objects["OkButton"] = new cWindowOkButton(this);
	objects["CancelButton"] = new cWindowCancelButton(this);
}

void* cLightWindow::userfunc( void* param )
{
	int p = (int)param;
	if(p == 0) // cancel
	{
	}
	else
	{
		for(objectlist::iterator i = objects.begin(); i !=  objects.end(); i++)
		{
			if(i->second->type == OBJECT_FLOATINPUTBOX)
				i->second->onKeyDown(SDLK_RETURN, false);
		}
		cWindow* w = cWM::getWindow(WT_LIGHTOVERVIEW);
		if(w != NULL)
			w->userfunc(NULL);
	}
	return NULL;
}
