#ifndef __EFFECTWINDOW_H__
#define __EFFECTWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"


class cEffectWindow : public cWindow
{
public:
	cEffectWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		cWindowObject* o;
		wtype = WT_EFFECT;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = GetMsg("wm/effect/TITLE");
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		addlabel("lblPos", 15,60,GetMsg("wm/effect/POS"));
		addlabel("lblScale", 15,80, GetMsg("wm/effect/SCALE"));
		addlabel("lblRot", 15,100,GetMsg("wm/effect/ROT"));

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,20);
		o->resizeto(210,20);
		o->SetText(0,"");
		o->SetInt(0,0);
		objects["objectmenu"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,40);
		o->resizeto(210,20);
		o->SetText(0,"");
		o->SetInt(0,0);
		objects["objectname"] = o;
		
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
		objects["scalex"] = o;

		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,80);
		o->resizeto(70,20);
		objects["scaley"] = o;
		
		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,80);
		o->resizeto(70,20);
		objects["scalez"] = o;
		
		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,100);
		o->resizeto(70,20);
		objects["rotx"] = o;

		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,100);
		o->resizeto(70,20);
		objects["roty"] = o;
		
		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,100);
		o->resizeto(70,20);
		objects["rotz"] = o;

		o = new cWindowFloatInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,120);
		o->resizeto(210,20);
		objects["looptime"] = o;

		objects["OkButton"] = new cWindowOkButton(this);
		objects["CancelButton"] = new cWindowCancelButton(this);
	}
	cUndoItem* undo;
	void* userfunc(void* param)
	{
		int p = (int)param;
		if(p == 0) // cancel
		{
			undo->undo();
			delete undo;
		}
		else
		{
			for(map<string, cWindowObject*, less<string> >::iterator i = objects.begin(); i !=  objects.end(); i++)
			{
				if(i->second->type == OBJECT_FLOATINPUTBOX)
					i->second->onkeydown(SDLK_RETURN, false);
			}
			undostack.push(undo);
		}

		return NULL;
	}
};

#endif
