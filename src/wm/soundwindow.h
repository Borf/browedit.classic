#ifndef __SOUNDWINDOW_H__
#define __SOUNDWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"
#include "objectwindow.h"
#include "soundselectwindow.h"
#include "../undo.h"
extern cUndoStack undostack;

class cSoundWindow : public cWindow
{
public:
	class cWindowSelectFileButton : public cWindowButton
	{
	public:
		cSound*	selectedSound;
		cWindowSelectFileButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
		{
			selectedSound = NULL;
			text = "Select";
			resizeto(80,20);
			moveto(230,20);
			alignment = ALIGN_TOPLEFT;
		}

		void click()
		{
			if(selectedSound == NULL)
				return;
			cWindow* w = Graphics.WM.getwindow(WT_SOUNDSELECT);
			if(w)
			{
				Graphics.WM.togglewindow(WT_SOUNDSELECT);
				((cSoundSelectWindow*)w)->selectedSound = selectedSound;
			}
			else
			{
				cSoundSelectWindow* w = new cSoundSelectWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin, cVector3());
				w->selectedSound = selectedSound;
				Graphics.WM.addwindow(w);
			}



		}


	};


	cUndoItem* undo;

	cSoundWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		undo = NULL;
		cWindowObject* o;
		wtype = WT_LIGHT;
		resizable = false;
		visible = true;

		h = 280+skinOffTop+skinOffBottom;
		w = 310+skinOffLeft+skinOffRight;
		title = GetMsg("wm/sound/TITLE");
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this,skin);
		objects["close"] = new cWindowCloseButton(this,skin);


		addlabel("lblName", 0,0,GetMsg("wm/sound/NAME"));
		addlabel("lblFile", 0,20,GetMsg("wm/sound/FILE"));
		addlabel("lblPos",	0,40, GetMsg("wm/sound/POSITION"));
		addlabel("lblScale",0,60, GetMsg("wm/sound/SCALE"));
		addlabel("lblRot",	0,80,GetMsg("wm/sound/ROTATION"));
		addlabel("lblRepeatDelay",	0,100, GetMsg("wm/sound/REPEATDELAY"));



		o = new cWindowStringInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,0);
		o->resizeto(210,20);
		o->SetText(0,"");
		objects["objectname"] = o;

		o = new cWindowStringInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,20);
		o->resizeto(120,20);
		o->SetText(0,"");
		objects["objectfile"] = o;

		objects["filebutton"] = new cWindowSelectFileButton(this, skin);
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,40);
		o->resizeto(70,20);
		objects["posx"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,40);
		o->resizeto(70,20);
		objects["posy"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,40);
		o->resizeto(70,20);
		objects["posz"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,60);
		o->resizeto(70,20);
		objects["scalex"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,60);
		o->resizeto(70,20);
		objects["scaley"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,60);
		o->resizeto(70,20);
		objects["scalez"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,80);
		o->resizeto(70,20);
		objects["rotx"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,80);
		o->resizeto(70,20);
		objects["roty"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,80);
		o->resizeto(70,20);
		objects["rotz"] = o;
////////////////
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,100);
		o->resizeto(210,20);
		objects["repeatdelay"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,120);
		o->resizeto(210,20);
		objects["unknown2"] = o;

		o = new cWindowLongInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,140);
		o->resizeto(210,20);
		objects["unknown3"] = o;

		o = new cWindowLongInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,160);
		o->resizeto(210,20);
		objects["unknown4"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,180);
		o->resizeto(210,20);
		objects["unknown5"] = o;

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,200);
		o->resizeto(210,20);
		objects["unknown6"] = o;

		o = new cWindowFloatInputBox(this, skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,220);
		o->resizeto(210,20);
		objects["unknown7"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,240);
		o->resizeto(210,20);
		objects["unknown8"] = o;



		objects["OkButton"] = new cWindowOkButton(this,skin);
		objects["CancelButton"] = new cWindowCancelButton(this,skin);
	}	

	void* userfunc(void* param)
	{
		intptr_t p = (intptr_t)param;
		if(p == 0) // cancel
		{
			if(undo != NULL)
			{
				undo->undo();
				delete undo;
			}
		}
		else
		{

			for(map<string, cWindowObject*, less<string> >::iterator i = objects.begin(); i !=  objects.end(); i++)
			{
				if(i->second->type == OBJECT_FLOATINPUTBOX)
					i->second->onkeydown(SDLK_RETURN, false);
			}
			cWindow* w = Graphics.WM.getwindow(WT_SOUNDOVERVIEW);
			if(w != NULL)
				w->userfunc(NULL);
			undostack.push(undo);
		}

		return NULL;
	}
};

#endif
