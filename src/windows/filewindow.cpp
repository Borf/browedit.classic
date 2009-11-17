#include "filewindow.h"
#include <graphics.h>
#include <filesystem.h>
#include <wm/windowlistbox.h>
#include "settings.h"
#include <algorithm>

cFileWindow::cOkButton::cOkButton( cWindow* parent, void (*pCallback)(std::string), TiXmlDocument* skin ) : cWindowButton(parent,skin)
{
	callback = pCallback;
	alignment = ALIGN_BOTTOMRIGHT;
	resizeTo(100,20);
	moveTo(0,0);
	text = "Load";
}

void cFileWindow::cOkButton::onClick()
{
	std::string filename = parent->objects["filebox"]->getText(-1);
	parent->close();
	if(filename == "")
		return;
	filename = filename.substr(0, filename.length()-4);
	callback(cSettings::roDir + filename);
}

cFileWindow::cWindowFilterBox::cWindowFilterBox( cWindow* parent, TiXmlDocument* skin ) : cWindowInputBox(parent,skin)
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
	for(unsigned int i = 0; i < ((cFileWindow*)parent)->mapNames.size(); i++)
	{
		if(((cFileWindow*)parent)->mapNames[i].find(text) != std::string::npos)
			o->setText(-1, ((cFileWindow*)parent)->mapNames[i]);
	}
}

cFileWindow::cFileWindow(void (*pCallback)(std::string)) : cWindow()
{
	windowType = WT_FILE;
	resizable = true;
	visible = true;
	
	h = cGraphics::h()-20;
	w = 350;
	title = GetMsg("wm/file/TITLE");
	center();
	
	defaultObject = "OkButton";
	
	cWindowObject* o;
	
	objects["close"] = new cWindowCloseButton(this);
	
	objects["filter"] = new cWindowFilterBox(this);
	//		addlabel("lblLookIn", 15,20,GetMsg("wm/file/SELECTMAP"));
	
	
	o = new cWindowListBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(0,0);
	o->resizeTo(innerWidth(), innerHeight()-20);
	objects["filebox"] = o;
	
	mapNames.clear();
	unsigned int i;
	for(i = 0; i < cFileSystem::locations.size(); i++)
	{
		for(std::map<std::string, cFile*, std::less<std::string> >::iterator it = cFileSystem::locations[i]->files.begin(); it != cFileSystem::locations[i]->files.end(); it++)
		{
			if(it->first.find(".rsw") != std::string::npos)
				mapNames.push_back(it->first.substr(cSettings::roDir.length()));
			
		}
	}
	
	std::sort(mapNames.begin(), mapNames.end());
	
	for(i = 1; i < mapNames.size(); i++)
	{
		if(mapNames[i] == mapNames[i-1])
		{
			mapNames.erase(mapNames.begin() + i);
			i--;
		}
	}
	
	for(i = 0; i < mapNames.size(); i++)
		o->setText(-1, mapNames[i]);
	
	objects["OkButton"] = new cOkButton(this, pCallback);
	selectedObject = objects["filter"];
}

void cFileWindow::resizeTo( int ww, int hh )
{
	cWindow::resizeTo(ww,hh);
	objects["filebox"]->resizeTo(innerWidth(), innerHeight()-20);
}
