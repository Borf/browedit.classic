#include <common.h>
#include <windows/rsmmeshprops.h>

#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowlabel.h>
#include <wm/windowinputbox.h>
#include "objectwindow.h"
#include <undo.h>


cRsmMeshPropsWindow::cRsmMeshPropsWindow(cRsmModelBase::cMesh* m ) : cWindow()
{
	mesh = m;
	cWindowObject* o;
	windowType = WT_RSMMESHPROPS;
	resizable = false;
	visible = true;
	alwaysOnTop = true;
	
	h = 280+skinOffTop+skinOffBottom;
	w = 410+skinOffLeft+skinOffRight;
	title = "Rsm Mesh Properties";
	center();
	
	defaultObject = "OkButton";
	
	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);
	
	
	addLabel("lblName", 0,0,GetMsg("wm/sound/NAME"));
	addLabel("lblFile", 0,20,GetMsg("wm/sound/FILE"));
	addLabel("lblPos",	0,40, GetMsg("wm/sound/POSITION"));
	addLabel("lblScale",0,120, GetMsg("wm/sound/SCALE"));
	addLabel("lblRot",	0,140,GetMsg("wm/sound/ROTATION"));
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

	for(int i = 0; i < 16; i++)
	{
		o = new cRsmFloatInputBox(this);
		o->moveTo(100 + 70 * (i/4),40 + 20*(i % 4));
		o->resizeTo(70,20);
		((cWindowFloatInputBox*)o)->floatje = &m->offset.values[i];
		char buf[6];
		sprintf(buf, "off%i", i);
		objects[buf] = o;
	}
	
	o = new cRsmFloatInputBox(this);
	o->moveTo(100,120);
	o->resizeTo(70,20);
	((cWindowFloatInputBox*)o)->floatje = &m->scale.x;
	objects["scalex"] = o;
	
	o = new cRsmFloatInputBox(this);
	o->moveTo(170,120);
	o->resizeTo(70,20);
	((cWindowFloatInputBox*)o)->floatje = &m->scale.y;
	objects["scaley"] = o;
	
	o = new cRsmFloatInputBox(this);
	o->moveTo(240,120);
	o->resizeTo(70,20);
	((cWindowFloatInputBox*)o)->floatje = &m->scale.z;
	objects["scalez"] = o;
	
	o = new cRsmFloatInputBox(this);
	o->moveTo(100,140);
	o->resizeTo(70,20);
	((cWindowFloatInputBox*)o)->floatje = &m->rotaxis.x;
	objects["rotx"] = o;
	
	o = new cRsmFloatInputBox(this);
	o->moveTo(170,140);
	o->resizeTo(70,20);
	((cWindowFloatInputBox*)o)->floatje = &m->rotaxis.y;
	objects["roty"] = o;
	
	o = new cRsmFloatInputBox(this);
	o->moveTo(240,140);
	o->resizeTo(70,20);
	((cWindowFloatInputBox*)o)->floatje = &m->rotaxis.z;
	objects["rotz"] = o;

	o = new cRsmFloatInputBox(this);
	o->moveTo(310,140);
	o->resizeTo(70,20);
	((cWindowFloatInputBox*)o)->floatje = &m->rotangle;
	objects["rota"] = o;
	
	
	
	
	objects["OkButton"] = new cWindowOkButton(this);
	objects["CancelButton"] = new cWindowCancelButton(this);
	onClick();
}

void* cRsmMeshPropsWindow::userfunc( void* param )
{
	int p = (int)param;
	if(p == 0) // cancel
	{
	}
	else
	{
		
		for(objectlist::iterator i = objects.begin(); i !=  objects.end(); i++)
		{
			if(i->second->type == OBJECT_FLOATINPUTBOX)
				i->second->onKeyDown(SDLK_RETURN, false);
		}

	}
	
	return NULL;
}


void cRsmMeshPropsWindow::onClick()
{
	cWindow::onClick();
	cRsmModel::cMesh* root = mesh;
	while(root->parent != NULL)
		root = root->parent;
	
	root->setSelection(mesh);
}

bool cRsmMeshPropsWindow::cRsmFloatInputBox::onKeyDown( int keyid, bool shift )
{
	bool b = cWindowFloatInputBox::onKeyDown(keyid, shift);
	if (keyid == SDLK_RETURN && floatje != NULL)
	{
		((cRsmMeshPropsWindow*)parent)->mesh->cache1 = false;
		((cRsmMeshPropsWindow*)parent)->mesh->cache2 = false;
	}
	return b;
}