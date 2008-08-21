#include "filewindow.h"
#include <graphics.h>
#include <filesystem.h>
#include <wm/windowlistbox.h>

extern cFileSystem fs;
extern cGraphics Graphics;
extern std::string rodir;

cFileWindow::cOkButton::cOkButton( cWindow* parent, void (*pCallback)(std::string), TiXmlDocument &skin ) : cWindowButton(parent,skin)
{
	callback = pCallback;
	alignment = ALIGN_BOTTOMRIGHT;
	resizeTo(100,20);
	moveTo(0,0);
	text = "Load";
}

void cFileWindow::cOkButton::click()
{
	std::string filename = parent->objects["filebox"]->getText(-1);
	filename = filename.substr(0, filename.length()-4);
	parent->close();
	callback(rodir + filename);
}

cFileWindow::cWindowFilterBox::cWindowFilterBox( cWindow* parent, TiXmlDocument &skin ) : cWindowInputBox(parent,skin)
{
	alignment = ALIGN_BOTTOMLEFT;
	moveTo(0,0);
	resizeTo(parent->innerWidth()-100,20);
	text = "";
}

void cFileWindow::cWindowFilterBox::onChange()
{
	cWindowListBox* o = (cWindowListBox*)parent->objects["filebox"];
	o->values.clear();
	for(unsigned int i = 0; i < ((cFileWindow*)parent)->mapnames.size(); i++)
	{
		if(((cFileWindow*)parent)->mapnames[i].find(text) != std::string::npos)
			o->setText(-1, ((cFileWindow*)parent)->mapnames[i]);
	}
}

cFileWindow::cFileWindow( cTexture* t, cFont* f, void (*pCallback)(std::string), TiXmlDocument &skin ) : cWindow(t,f,skin)
{
	windowType = WT_FILE;
	resizable = true;
	visible = true;
	
	h = Graphics.h()-20;
	w = 350;
	title = GetMsg("wm/file/TITLE");
	center();
	
	defaultObject = "OkButton";
	
	cWindowObject* o;
	
	objects["close"] = new cWindowCloseButton(this,skin);
	
	objects["filter"] = new cWindowFilterBox(this,skin);
	//		addlabel("lblLookIn", 15,20,GetMsg("wm/file/SELECTMAP"));
	
	
	o = new cWindowListBox(this,skin);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(0,0);
	o->resizeTo(innerWidth(), innerHeight()-20);
	objects["filebox"] = o;
	
	mapnames.clear();
	unsigned int i;
	for(i = 0; i < fs.locations.size(); i++)
	{
		for(std::map<std::string, cFile*, std::less<std::string> >::iterator it = fs.locations[i]->files.begin(); it != fs.locations[i]->files.end(); it++)
		{
			if(it->first.find(".rsw") != std::string::npos)
				mapnames.push_back(it->first.substr(rodir.length()));
			
		}
	}
	
	mergesort<std::string>(mapnames, compare<std::string>);
	
	for(i = 0; i < mapnames.size(); i++)
		o->setText(-1, mapnames[i]);
	
	objects["OkButton"] = new cOkButton(this, pCallback,skin);
	selectedObject = objects["filter"];
}

void cFileWindow::resizeTo( int ww, int hh )
{
	cWindow::resizeTo(ww,hh);
	objects["filebox"]->resizeTo(innerWidth(), innerHeight()-20);
}