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
	cLightWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_LIGHT;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = msgtable[WINDOW_LIGHT_TITLE];
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		addlabel("lblPos", 20, 60, msgtable[WINDOW_LIGHT_POSITION]);
		addlabel("lblColor", 20, 80, msgtable[WINDOW_LIGHT_COLOR]);
		addlabel("lblIntensity", 20, 100, msgtable[WINDOW_LIGHT_INTENSITY]);
		
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
		o->moveto(100,100);
		o->resizeto(210,20);
		objects["intensity"] = o;

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
		}
		return NULL;
	}
};

#endif
