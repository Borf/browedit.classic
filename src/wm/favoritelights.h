#ifndef __FAVORITELIGHTSWINDOW_H__
#define __FAVORITELIGHTSWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"
#include "objectwindow.h"
#include "windowcheckbox.h"
extern TiXmlDocument favoritelights;

class cFavoriteLightsWindow : public cWindow
{
public:
	cFavoriteLightsWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_FAVLIGHTS;
		resizable = false;
		visible = true;

		h = 260;
		w = 500;
		title = GetMsg("wm/favlight/TITLE");
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		addlabel("lblPos", 20, 60, GetMsg("wm/light/POSITION"));
		addlabel("lblColor", 20, 80, GetMsg("wm/light/COLOR"));
		addlabel("lblIntensity", 20, 100, GetMsg("wm/light/INTENSITY"));
		addlabel("lblRange", 20, 120, GetMsg("wm/light/RANGE"));
		addlabel("lblMaxlightincrement", 20, 140, GetMsg("wm/light/MAXLIGHTINCREMENT"));
		addlabel("lblLightfalloff", 20, 160, GetMsg("wm/light/LIGHTFALLOFF"));
		addlabel("lblCastsShadow", 20, 180, GetMsg("wm/light/CASTSSHADOW"));
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,60);
		o->resizeto(70,20);
		objects["posx"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,60);
		o->resizeto(70,20);
		objects["posy"] = o;
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,60);
		o->resizeto(70,20);
		objects["posz"] = o;
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,80);
		o->resizeto(70,20);
		objects["colorr"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,80);
		o->resizeto(70,20);
		objects["colorg"] = o;
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,80);
		o->resizeto(70,20);
		objects["colorb"] = o;
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,100);
		o->resizeto(140,20);
		objects["intensity"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,120);
		o->resizeto(140,20);
		objects["range"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,140);
		o->resizeto(140,20);
		objects["maxlightincrement"] = o;

		o = new cWindowInputBox(this);
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
};

#endif
