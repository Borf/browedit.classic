#ifndef __OBJECTWINDOW_H__
#define __OBJECTWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"
#include "windowcheckbox.h"
#include "../undo.h"
extern cUndoStack undostack;

class cWindowOkButton : public cWindowButton
{
public:
	cWindowOkButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent, skin)
	{
		alignment = ALIGN_BOTTOM;
		moveto(100, 0);
		resizeto(100, 20);
		text = GetMsg("wm/OK");
	}
	virtual ~cWindowOkButton() {}
	void click()
	{
		if (parent->windowtype() == WT_OBJECT || parent->windowtype() == WT_LIGHT || parent->windowtype() == WT_SOUND)
		{
			parent->userfunc((void*)1);
		}
		parent->close();
	}
};


class cWindowCancelButton : public cWindowButton
{
public:
	cWindowCancelButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
	{
		alignment = ALIGN_BOTTOM;
		moveto(-100, 0);
		resizeto(100, 20);
		text = GetMsg("wm/CANCEL");
	}
	virtual ~cWindowCancelButton() {}
	void click()
	{
		if (parent->windowtype() == WT_OBJECT || parent->windowtype() == WT_EFFECT || parent->windowtype() == WT_LIGHT)
		{
			parent->userfunc((void*)0);
		}
		parent->close();
	}
};



class cObjectWindow : public cWindow
{
public:
	cUndoItem* undo;

	cObjectWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		cWindowObject* o;
		wtype = WT_OBJECT;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = GetMsg("wm/object/TITLE");
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this,skin);
		objects["close"] = new cWindowCloseButton(this,skin);


		addlabel("text", 15,20,GetMsg("wm/object/OBJECT"));
		addlabel("lblPos", 15, 60, GetMsg("wm/object/POSITION"));
		addlabel("lblScale", 15,80, GetMsg("wm/object/SCALE"));
		addlabel("lblRot", 15,100,GetMsg("wm/object/ROTATION"));
		addlabel("lblShadow", 15, 120, GetMsg("wm/object/CASTSHADOW"));

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,20);
		o->resizeto(210,20);
		o->SetText(0,"");
		o->SetInt(0,0);
		objects["objectmenu"] = o;

		o = new cWindowStringInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,40);
		o->resizeto(210,20);
		o->SetText(0,"");
		objects["objectname"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,60);
		o->resizeto(70,20);
		objects["posx"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,60);
		o->resizeto(70,20);
		objects["posy"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,60);
		o->resizeto(70,20);
		objects["posz"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,80);
		o->resizeto(70,20);
		objects["scalex"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,80);
		o->resizeto(70,20);
		objects["scaley"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,80);
		o->resizeto(70,20);
		objects["scalez"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,100);
		o->resizeto(70,20);
		objects["rotx"] = o;

		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,100);
		o->resizeto(70,20);
		objects["roty"] = o;
		
		o = new cWindowFloatInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,100);
		o->resizeto(70,20);
		objects["rotz"] = o;

		o = new cWindowBoolCheckBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,120);
		objects["castshadow"] = o;

		objects["OkButton"] = new cWindowOkButton(this,skin);
		objects["CancelButton"] = new cWindowCancelButton(this,skin);
	}	

	void* userfunc(void* param)
	{
		intptr_t p = (intptr_t)param;
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
			cWindow* w = Graphics.WM.getwindow(WT_MODELOVERVIEW);
			if(w != NULL)
				w->userfunc(NULL);
			undostack.push(undo);
		}

		return NULL;
	}
};

#endif
