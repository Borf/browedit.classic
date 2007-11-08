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
	cLightWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_LIGHT;
		resizable = false;
		visible = true;

		h = 260;
		w = 350;
		title = GetMsg("wm/light/TITLE");
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		addlabel("lblName", 20, 40, GetMsg("wm/light/NAME"));
		addlabel("lblPos", 20, 60, GetMsg("wm/light/POSITION"));
		addlabel("lblColor", 20, 80, GetMsg("wm/light/COLOR"));
		addlabel("lblIntensity", 20, 100, GetMsg("wm/light/INTENSITY"));
		addlabel("lblRange", 20, 120, GetMsg("wm/light/RANGE"));
		addlabel("lblMaxlightincrement", 20, 140, GetMsg("wm/light/MAXLIGHTINCREMENT"));
		addlabel("lblLightfalloff", 20, 160, GetMsg("wm/light/LIGHTFALLOFF"));
		addlabel("lblCastsShadow", 20, 180, GetMsg("wm/light/CASTSSHADOW"));
		
		o = new cWindowStringInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,40);
		o->resizeto(140,20);
		objects["name"] = o;
		
		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,60);
		o->resizeto(70,20);
		objects["posx"] = o;

		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,60);
		o->resizeto(70,20);
		objects["posy"] = o;
		
		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,60);
		o->resizeto(70,20);
		objects["posz"] = o;
		
		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,80);
		o->resizeto(70,20);
		objects["colorr"] = o;

		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,80);
		o->resizeto(70,20);
		objects["colorg"] = o;
		
		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,80);
		o->resizeto(70,20);
		objects["colorb"] = o;
		
		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,100);
		o->resizeto(140,20);
		objects["intensity"] = o;

		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,120);
		o->resizeto(140,20);
		objects["range"] = o;

		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,140);
		o->resizeto(140,20);
		objects["maxlightincrement"] = o;

		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,160);
		o->resizeto(140,20);
		objects["lightfalloff"] = o;

		o = new cWindowBoolCheckBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,182);
		objects["castshadow"] = o;


		objects["OkButton"] = new cWindowOkButton(this);
		objects["CancelButton"] = new cWindowCancelButton(this);
	}	

	void* userfunc(void* param)
	{
		int p = (int)param;
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
