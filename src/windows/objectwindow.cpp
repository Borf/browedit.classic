#include "objectwindow.h"
#include <wm/windowinputbox.h>
#include <undo.h>


cWindowOkButton::cWindowOkButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent, skin)
{
	alignment = ALIGN_BOTTOM;
	moveTo(100, 0);
	resizeTo(100, 20);
	text = GetMsg("wm/OK");
}

void cWindowOkButton::onClick()
{
	if (parent->windowtype() == WT_OBJECT || parent->windowtype() == WT_LIGHT || parent->windowtype() == WT_SOUND)
	{
		parent->userfunc((void*)1);
	}
	parent->close();
}

cWindowOkButton::~cWindowOkButton()
{
	
}

cWindowCancelButton::cWindowCancelButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent, skin)
{
	alignment = ALIGN_BOTTOM;
	moveTo(-100, 0);
	resizeTo(100, 20);
	text = GetMsg("wm/CANCEL");
}

cWindowCancelButton::~cWindowCancelButton()
{
	
}

void cWindowCancelButton::onClick()
{
	if (parent->windowtype() == WT_OBJECT || parent->windowtype() == WT_EFFECT || parent->windowtype() == WT_LIGHT)
	{
		parent->userfunc((void*)0);
	}
	parent->close();
}

cObjectWindow::cObjectWindow( ) : cWindow()
{
	cWindowObject* o;
	windowType = WT_OBJECT;
	resizable = false;
	visible = true;
	
	h = 200;
	w = 350;
	title = GetMsg("wm/object/TITLE");
	center();
	
	defaultObject = "OkButton";
	
	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);
	
	
	addLabel("text", 15,20,GetMsg("wm/object/OBJECT"));
	addLabel("lblPos", 15, 60, GetMsg("wm/object/POSITION"));
	addLabel("lblScale", 15,80, GetMsg("wm/object/SCALE"));
	addLabel("lblRot", 15,100,GetMsg("wm/object/ROTATION"));
	addLabel("lblShadow", 15, 120, GetMsg("wm/object/LIGHTOPACITY"));
	
	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,20);
	o->resizeTo(210,20);
	o->setText(0,"");
	o->setInt(0,0);
	objects["objectmenu"] = o;
	
	o = new cWindowStringInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,40);
	o->resizeTo(210,20);
	o->setText(0,"");
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
	objects["lightopacity"] = o;
	
	objects["OkButton"] = new cWindowOkButton(this);
	objects["CancelButton"] = new cWindowCancelButton(this);
}

void* cObjectWindow::userfunc( void* param )
{
	int p = (int)param;
	if(p == 0) // cancel
	{
		undo->undo();
		delete undo;
	}
	else
	{
		
		for(objectlist::iterator i = objects.begin(); i !=  objects.end(); i++)
		{
			if(i->second->type == OBJECT_FLOATINPUTBOX)
				i->second->onKeyDown(SDLK_RETURN, false);
		}
		cWindow* w = cWM::getWindow(WT_MODELOVERVIEW);
		if(w != NULL)
			w->userfunc(NULL);
		cGraphics::worldContainer->undoStack->push(undo);
	}
	
	return NULL;
}