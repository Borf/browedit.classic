#ifndef __LIGHTWINDOW_H__
#define __LIGHTWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"
#include "objectwindow.h"
#include "windowcheckbox.h"

class cLightWindow : public cWindow
{
public:
	cLightWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		wtype = WT_LIGHT;
		resizable = false;
		visible = true;

		h = 180+skinOffTop+skinOffBottom;
		w = 310+skinOffLeft+skinOffRight;
		title = GetMsg("wm/light/TITLE");
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		objects["rollup"] = new cWindowRollupButton(this,skin);
		objects["close"] = new cWindowCloseButton(this,skin);

		addlabel("lblName",				0, 0, GetMsg("wm/light/NAME"));
		addlabel("lblPos",				0, 20, GetMsg("wm/light/POSITION"));
		addlabel("lblColor",			0, 40, GetMsg("wm/light/COLOR"));
		addlabel("lblIntensity",		0, 60, GetMsg("wm/light/INTENSITY"));
		addlabel("lblRange",			0, 80, GetMsg("wm/light/RANGE"));
		addlabel("lblMaxlightincrement",0, 100, GetMsg("wm/light/MAXLIGHTINCREMENT"));
		addlabel("lblLightfalloff",		0, 120, GetMsg("wm/light/LIGHTFALLOFF"));
		addlabel("lblCastsShadow",		0, 140, GetMsg("wm/light/CASTSSHADOW"));
		
		o = new cWindowStringInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,0);
		o->resizeto(140,20);
		objects["name"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,20);
		o->resizeto(70,20);
		objects["posx"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,20);
		o->resizeto(70,20);
		objects["posy"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,20);
		o->resizeto(70,20);
		objects["posz"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,40);
		o->resizeto(70,20);
		objects["colorr"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,40);
		o->resizeto(70,20);
		objects["colorg"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,40);
		o->resizeto(70,20);
		objects["colorb"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,60);
		o->resizeto(140,20);
		objects["intensity"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,80);
		o->resizeto(140,20);
		objects["range"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,100);
		o->resizeto(140,20);
		objects["maxlightincrement"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,120);
		o->resizeto(140,20);
		objects["lightfalloff"] = o;

		o = new cWindowBoolCheckBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,142);
		objects["castshadow"] = o;


		objects["OkButton"] = new cWindowOkButton(this,skin);
		objects["CancelButton"] = new cWindowCancelButton(this,skin);
	}	

	void* userfunc(void* param)
	{
		intptr_t p = (intptr_t)param;
		if(p == 0) // cancel
		{
		}
		else
		{
			for(map<string, cWindowObject*, less<string> >::iterator i = objects.begin(); i !=  objects.end(); i++)
			{
				if(i->second->type == OBJECT_FLOATINPUTBOX)
					i->second->onkeydown(SDLK_RETURN, false);
			}
			cWindow* w = Graphics.WM.getwindow(WT_LIGHTOVERVIEW);
			if(w != NULL)
				w->userfunc(NULL);
		}
		return NULL;
	}
};

#endif
