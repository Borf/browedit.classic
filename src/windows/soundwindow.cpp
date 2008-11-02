#include <common.h>
#include "soundwindow.h"

#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowlabel.h>
#include <wm/windowinputbox.h>
#include "objectwindow.h"
#include "soundselectwindow.h"
#include <undo.h>
extern cUndoStack undostack;


cSoundWindow::cWindowSelectFileButton::cWindowSelectFileButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent,skin)
{
	selectedSound = NULL;
	text = "Select";
	resizeTo(80,20);
	moveTo(230,20);
	alignment = ALIGN_TOPLEFT;
}

void cSoundWindow::cWindowSelectFileButton::click()
{
	if(selectedSound == NULL)
		return;
	cWindow* w = cWM::getwindow(WT_SOUNDSELECT);
	if(w)
	{
		cWM::togglewindow(WT_SOUNDSELECT);
		((cSoundSelectWindow*)w)->selectedSound = selectedSound;
	}
	else
	{
		cSoundSelectWindow* w = new cSoundSelectWindow(cVector3());
		w->selectedSound = selectedSound;
		cWM::addwindow(w);
	}
}

cSoundWindow::cSoundWindow( ) : cWindow()
{
	undo = NULL;
	cWindowObject* o;
	windowType = WT_LIGHT;
	resizable = false;
	visible = true;
	
	h = 280+skinOffTop+skinOffBottom;
	w = 310+skinOffLeft+skinOffRight;
	title = GetMsg("wm/sound/TITLE");
	center();
	
	defaultObject = "OkButton";
	
	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);
	
	
	addLabel("lblName", 0,0,GetMsg("wm/sound/NAME"));
	addLabel("lblFile", 0,20,GetMsg("wm/sound/FILE"));
	addLabel("lblPos",	0,40, GetMsg("wm/sound/POSITION"));
	addLabel("lblScale",0,60, GetMsg("wm/sound/SCALE"));
	addLabel("lblRot",	0,80,GetMsg("wm/sound/ROTATION"));
	addLabel("lblRepeatDelay",	0,100, GetMsg("wm/sound/REPEATDELAY"));
	addLabel("lblValue1",	0,120, "Value 1");
	addLabel("lblValue2",	0,140, "Value 2");
	addLabel("lblValue3",	0,160, "Value 3");
	addLabel("lblValue4",	0,180, "Value 4");
	addLabel("lblValue5",	0,220, "Value 5");
	addLabel("lblValue6",	0,240, "Value 6");
	
	
	
	o = new cWindowStringInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,0);
	o->resizeTo(210,20);
	o->setText(0,"");
	objects["objectname"] = o;
	
	o = new cWindowStringInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,20);
	o->resizeTo(120,20);
	o->setText(0,"");
	objects["objectfile"] = o;
	
	objects["filebutton"] = new cWindowSelectFileButton(this);
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,40);
	o->resizeTo(70,20);
	objects["posx"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,40);
	o->resizeTo(70,20);
	objects["posy"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,40);
	o->resizeTo(70,20);
	objects["posz"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,60);
	o->resizeTo(70,20);
	objects["scalex"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,60);
	o->resizeTo(70,20);
	objects["scaley"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,60);
	o->resizeTo(70,20);
	objects["scalez"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,80);
	o->resizeTo(70,20);
	objects["rotx"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(170,80);
	o->resizeTo(70,20);
	objects["roty"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(240,80);
	o->resizeTo(70,20);
	objects["rotz"] = o;
	////////////////
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,100);
	o->resizeTo(210,20);
	objects["repeatdelay"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,120);
	o->resizeTo(210,20);
	objects["unknown2"] = o;
	
	o = new cWindowLongInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,140);
	o->resizeTo(210,20);
	objects["unknown3"] = o;
	
	o = new cWindowLongInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,160);
	o->resizeTo(210,20);
	objects["unknown4"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,180);
	o->resizeTo(210,20);
	objects["unknown5"] = o;
	
	/*	o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(100,200);
	o->resizeto(210,20);
	objects["unknown6"] = o;*/
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,220);
	o->resizeTo(210,20);
	objects["unknown7"] = o;
	
	o = new cWindowFloatInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,240);
	o->resizeTo(210,20);
	objects["unknown8"] = o;
	
	
	
	objects["OkButton"] = new cWindowOkButton(this);
	objects["CancelButton"] = new cWindowCancelButton(this);
}

void* cSoundWindow::userfunc( void* param )
{
	int p = (int)param;
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
		
		for(objectlist::iterator i = objects.begin(); i !=  objects.end(); i++)
		{
			if(i->second->type == OBJECT_FLOATINPUTBOX)
				i->second->onKeyDown(SDLK_RETURN, false);
		}
		cWindow* w = cWM::getwindow(WT_SOUNDOVERVIEW);
		if(w != NULL)
			w->userfunc(NULL);
		undostack.push(undo);
	}
	
	return NULL;
}