#ifndef __OBJECTWINDOW_H__
#define __OBJECTWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"
#include "../undo.h"
extern cUndoStack undostack;

class cWindowOkButton : public cWindowButton
{
public:
	cWindowOkButton(cWindow* parent) : cWindowButton(parent)
	{
		alignment = ALIGN_BOTTOM;
		moveto(100, 20);
		resizeto(100, 20);
		text = GetMsg("wm/OK");
	}
	virtual ~cWindowOkButton() {}
	void click()
	{
		if (parent->windowtype() == WT_OBJECT || parent->windowtype() == WT_LIGHT)
		{
			parent->userfunc((void*)1);
		}
		parent->close();
	}
};


class cWindowCancelButton : public cWindowButton
{
public:
	cWindowCancelButton(cWindow* parent) : cWindowButton(parent)
	{
		alignment = ALIGN_BOTTOM;
		moveto(-100, 20);
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

class cWindowFloatInputBox : public cWindowInputBox
{
	float* floatje;
	float lastvalue;
public:
	cWindowFloatInputBox(cWindow* parent) : cWindowInputBox(parent)
	{
		type = OBJECT_FLOATINPUTBOX;
		alignment = ALIGN_TOPLEFT;
		resizeto(70,20);
	}
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
	{
		if(*floatje != lastvalue)
		{
			char buf[100];
			sprintf(buf, "%f", *floatje);
			while(buf[strlen(buf)-1] == '0')
				buf[strlen(buf)-1] = '\0';
			if(buf[strlen(buf)-1] == '.')
				buf[strlen(buf)-1] = '\0';
			text = buf;
			lastvalue = *floatje;
		}
		cWindowInputBox::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
	}

	void SetInt(int id, int val)
	{
		cWindowInputBox::SetInt(id,val);
		if (id == 3)
			floatje = (float*)val;
	}
	bool onkeydown(int keyid, bool shift)
	{
		bool ret = cWindowInputBox::onkeydown(keyid, shift);
		if (keyid == SDLK_RETURN)
		{
			*floatje = atof(text.c_str());
			ret = true;
		}
		return ret;
	}
};


class cObjectWindow : public cWindow
{
public:
	cUndoItem* undo;

	cObjectWindow(cTexture* t, cFont* f) : cWindow(t,f)
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

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);


		addlabel("text", 15,20,GetMsg("wm/object/OBJECT"));
		addlabel("lblPos", 15, 60, GetMsg("wm/object/POSITION"));
		addlabel("lblScale", 15,80, GetMsg("wm/object/SCALE"));
		addlabel("lblRot", 15,100,GetMsg("wm/object/ROTATION"));

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

		objects["OkButton"] = new cWindowOkButton(this);
		objects["CancelButton"] = new cWindowCancelButton(this);
	}	

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
