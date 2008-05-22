#ifndef __FILEWINDOW_H__
#define __FILEWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowlistbox.h"


class cFileWindow : public cWindow
{

	class cOkButton : public cWindowButton
	{
	public:
		void (*callback)(string);
		cOkButton(cWindow* parent, void (*pCallback)(string), TiXmlDocument &skin) : cWindowButton(parent,skin)
		{
			callback = pCallback;
			alignment = ALIGN_BOTTOMRIGHT;
			resizeto(100,20);
			moveto(0,0);
			text = "Load";
		}

		void click()
		{
			string filename = parent->objects["filebox"]->GetText(-1);
			filename = filename.substr(0, filename.length()-4);
			parent->close();
			callback(rodir + filename);
		}
	};


public:
	vector<string> mapnames;

	class cWindowFilterBox : public cWindowInputBox
	{
	public:
		cWindowFilterBox(cWindow* parent, TiXmlDocument &skin) : cWindowInputBox(parent,skin)
		{
			alignment = ALIGN_BOTTOMLEFT;
			moveto(0,0);
			resizeto(parent->innerw()-100,20);
			text = "";
		}
		void onchange()
		{
			cWindowListBox* o = (cWindowListBox*)parent->objects["filebox"];
			o->values.clear();
			for(unsigned int i = 0; i < ((cFileWindow*)parent)->mapnames.size(); i++)
			{
				if(((cFileWindow*)parent)->mapnames[i].find(text) != string::npos)
					o->SetText(-1, ((cFileWindow*)parent)->mapnames[i]);
			}
			
		}
	};

	cFileWindow(cTexture* t, cFont* f, void (*pCallback)(string), TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		wtype = WT_FILE;
		resizable = true;
		visible = true;

		h = Graphics.h()-20;
		w = 350;
		title = GetMsg("wm/file/TITLE");
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		
		objects["close"] = new cWindowCloseButton(this,skin);

		objects["filter"] = new cWindowFilterBox(this,skin);
//		addlabel("lblLookIn", 15,20,GetMsg("wm/file/SELECTMAP"));
		

		o = new cWindowListBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(0,0);
		o->resizeto(innerw(), innerh()-20);
		objects["filebox"] = o;
		
		mapnames.clear();
		unsigned int i;
		for(i = 0; i < fs.locations.size(); i++)
		{
			for(map<string, cFile*, less<string> >::iterator it = fs.locations[i]->files.begin(); it != fs.locations[i]->files.end(); it++)
			{
				if(it->first.find(".rsw") != string::npos)
					mapnames.push_back(it->first.substr(rodir.length()));

			}
		}

		mergesort<string>(mapnames, compare<string>);

		for(i = 0; i < mapnames.size(); i++)
			o->SetText(-1, mapnames[i]);

		objects["OkButton"] = new cOkButton(this, pCallback,skin);
		selectedobject = objects["filter"];
	}


	void resizeto(int ww, int hh)
	{
		cWindow::resizeto(ww,hh);
		objects["filebox"]->resizeto(innerw(), innerh()-20);

	}
};

#endif
