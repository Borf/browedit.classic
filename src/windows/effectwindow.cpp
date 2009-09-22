#include "effectwindow.h"
#include <undo.h>


cEffectWindow::cEffectWindow() : cWindow()
{
	cWindowObject* o;
	windowType = WT_EFFECT;
	resizable = false;
	visible = true;
	
	h = 200;
	w = 350;
	title = GetMsg("wm/effect/TITLE");
	center();
	
	defaultObject = "OkButton";
	
	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);
	
	addLabel("lblPos", 15,60,GetMsg("wm/effect/POS"));
	addLabel("lblScale", 15,80, GetMsg("wm/effect/SCALE"));
	addLabel("lblRot", 15,100,GetMsg("wm/effect/ROT"));
	addLabel("lblLoopTime", 15, 120, GetMsg("wm/effect/LOOPTIME"));
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,20);
	o->resizeTo(210,20);
	o->setText(0,"");
	o->setInt(0,0);
	objects["objectmenu"] = o;
	
	o = new cWindowLabel(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,40);
	o->resizeTo(210,20);
	o->setText(0,"");
	o->setInt(0,0);
	objects["objectname"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,60);
	o->resizeTo(70,20);
	objects["posx"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,60);
	o->resizeTo(70,20);
	objects["posy"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,60);
	o->resizeTo(70,20);
	objects["posz"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,80);
	o->resizeTo(70,20);
	objects["scalex"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,80);
	o->resizeTo(70,20);
	objects["scaley"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,80);
	o->resizeTo(70,20);
	objects["scalez"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,100);
	o->resizeTo(70,20);
	objects["rotx"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,100);
	o->resizeTo(70,20);
	objects["roty"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,100);
	o->resizeTo(70,20);
	objects["rotz"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,120);
	o->resizeTo(210,20);
	objects["looptime"] = o;
	
	objects["OkButton"] = new cWindowOkButton(this);
	objects["CancelButton"] = new cWindowCancelButton(this);
}

void* cEffectWindow::userfunc( void* param )
{
	int p = (int)param;
	if(p == 0) // cancel
	{
		if(undo)
		{
			undo->undo();
			delete undo;
		}
	}
	else
	{
		for(objectlist::iterator i = objects.begin(); i !=  objects.end(); i++)
		{
			if(i->second->type == OBJECT_FLOATINPUTBOX)
				i->second->onKeyDown(SDLK_RETURN, false);
		}
		cGraphics::worldContainer->undoStack->push(undo);
	}
	
	return NULL;
}
