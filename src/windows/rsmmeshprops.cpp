#include <common.h>
#include <windows/rsmmeshprops.h>

#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowlabel.h>
#include <wm/windowinputbox.h>
#include "objectwindow.h"
#include <undo.h>
#include <bengine/util.h>


cRsmMeshPropsWindow::cRsmMeshPropsWindow(cRsmModelBase::cMesh* m ) : cWindow()
{
	mesh = m;
	cWindowObject* o;
	windowType = WT_RSMMESHPROPS;
	resizable = false;
	visible = true;
	alwaysOnTop = true;
	
	h = 320+skinOffTop+skinOffBottom;
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
	addLabel("lblAnimation",	0,160, "Animation Frames");
	
	
	
	o = new cWindowStringInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(100,0);
	o->resizeTo(210,20);
	((cWindowStringInputBox*)o)->stringetje = &m->name;
	objects["objectname"] = o;
	

	addLabel("lblMesh", 100, 20, "Vertices: " + bEngine::util::intToString(m->nVertices) + ", Faces: " + bEngine::util::intToString(m->nFaces));

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
	

	addLabel("lblFrameCount", 120, 160, bEngine::util::intToString(m->nAnimationFrames));
	objects["btnAddFrame"] = new cButtonAddFrame(this);
	objects["btnRemoveFrame"] = new cButtonRemoveFrame(this);

	o = new cAnimationFrameList(this);
	for(i = 0; i < m->nAnimationFrames; i++)
		((cAnimationFrameList*)o)->values.push_back(bEngine::util::intToString(i));
	objects["lstFrames"] = o;


	o = new cRsmFloatInputBox(this);
	o->moveTo(300, 190);
	if(m->nAnimationFrames > 0)
		((cWindowFloatInputBox*)o)->floatje = &m->animationFrames[0].quat.x;	
	objects["animx"] = o;

	
	o = new cRsmFloatInputBox(this);
	o->moveTo(300, 210);
	if(m->nAnimationFrames > 0)
		((cWindowFloatInputBox*)o)->floatje = &m->animationFrames[0].quat.y;	
	objects["animy"] = o;

	
	o = new cRsmFloatInputBox(this);
	o->moveTo(300, 230);
	if(m->nAnimationFrames > 0)
		((cWindowFloatInputBox*)o)->floatje = &m->animationFrames[0].quat.z;	
	objects["animz"] = o;

	
	o = new cRsmFloatInputBox(this);
	o->moveTo(300, 260);
	if(m->nAnimationFrames > 0)
		((cWindowFloatInputBox*)o)->floatje = &m->animationFrames[0].quat.w;	
	objects["animw"] = o;


	o = new cWindowIntInputBox(this);
	o->moveTo(300, 260);
	if(m->nAnimationFrames > 0)
		((cWindowIntInputBox*)o)->intje = &m->animationFrames[0].time;	
	objects["animtime"] = o;


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

cRsmMeshPropsWindow::cAnimationFrameList::cAnimationFrameList( cWindow* parent, TiXmlDocument* skin) : cWindowListBox(parent, skin)
{
	moveTo(100, 190);
	alignment = ALIGN_TOPLEFT;
	resizeTo(200,100);
}

void cRsmMeshPropsWindow::cAnimationFrameList::onChange( int oldValue )
{
	cRsmModelBase::cMesh* mesh = ((cRsmMeshPropsWindow*)parent)->mesh;
	((cRsmFloatInputBox*)parent->objects["animx"])->floatje = &mesh->animationFrames[selected].quat.x;
	((cRsmFloatInputBox*)parent->objects["animy"])->floatje = &mesh->animationFrames[selected].quat.y;
	((cRsmFloatInputBox*)parent->objects["animz"])->floatje = &mesh->animationFrames[selected].quat.z;
	((cRsmFloatInputBox*)parent->objects["animw"])->floatje = &mesh->animationFrames[selected].quat.w;
}



cRsmMeshPropsWindow::cButtonAddFrame::cButtonAddFrame( cWindow* parent, TiXmlDocument* skin) : cWindowButton(parent, skin)
{
	alignment = ALIGN_TOPLEFT;
	moveTo(170, 162);
	text = "Add";
}

void cRsmMeshPropsWindow::cButtonAddFrame::onClick()
{
	cRsmModelBase::cMesh* mesh = ((cRsmMeshPropsWindow*)parent)->mesh;

	cRsmModelBase::cMesh::cFrame* newAnimationFrames = new cRsmModelBase::cMesh::cFrame[mesh->nAnimationFrames+1];
	for(int i = 0; i < mesh->nAnimationFrames; i++)
	{
		newAnimationFrames[i].time = mesh->animationFrames[i].time;
		newAnimationFrames[i].quat = mesh->animationFrames[i].quat;
	}
	newAnimationFrames[mesh->nAnimationFrames].time = newAnimationFrames[mesh->nAnimationFrames-1].time;
	newAnimationFrames[mesh->nAnimationFrames].quat = newAnimationFrames[mesh->nAnimationFrames-1].quat;

	mesh->nAnimationFrames++;

	delete[] mesh->animationFrames;
	mesh->animationFrames = newAnimationFrames;

	parent->objects["lblFrameCount"]->setText(0, bEngine::util::intToString(mesh->nAnimationFrames));
	((cWindowListBox*)parent->objects["lstFrames"])->values.push_back(bEngine::util::intToString(mesh->nAnimationFrames));
	((cWindowListBox*)parent->objects["lstFrames"])->onChange(0);
}

cRsmMeshPropsWindow::cButtonRemoveFrame::cButtonRemoveFrame( cWindow* parent, TiXmlDocument* skin) : cWindowButton(parent, skin)
{
	alignment = ALIGN_TOPLEFT;
	moveTo(280, 162);
	text = "Remove";
}

void cRsmMeshPropsWindow::cButtonRemoveFrame::onClick()
{
	cRsmModelBase::cMesh* mesh = ((cRsmMeshPropsWindow*)parent)->mesh;
	if(mesh->nAnimationFrames > 0)
	{
		mesh->nAnimationFrames--;
		cRsmModelBase::cMesh::cFrame* newAnimationFrames = new cRsmModelBase::cMesh::cFrame[mesh->nAnimationFrames];
		
		int ii = 0;
		for(int i = 0; i < mesh->nAnimationFrames; i++)
		{
			if(i != ((cWindowListBox*)parent->objects["lstFrames"])->getInt(-1))
			{
				newAnimationFrames[ii].time = mesh->animationFrames[i].time;
				newAnimationFrames[ii].quat = mesh->animationFrames[i].quat;
				ii++;
			}
		}
		
		delete[] mesh->animationFrames;
		mesh->animationFrames = newAnimationFrames;
		
		parent->objects["lblFrameCount"]->setText(0, bEngine::util::intToString(mesh->nAnimationFrames));
		((cWindowListBox*)parent->objects["lstFrames"])->values.pop_back();
		((cWindowListBox*)parent->objects["lstFrames"])->onChange(0);
	}
}