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
		cOkButton(cWindow* parent, void (*pCallback)(string)) : cWindowButton(parent)
		{
			callback = pCallback;
			alignment = ALIGN_BOTTOMRIGHT;
			resizeto(100,20);
			moveto(10,10);
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
	cFileWindow(cTexture* t, cFont* f, void (*pCallback)(string)) : cWindow(t,f)
	{
		wtype = WT_FILE;
		resizable = false;
		visible = true;

		h = 300;
		w = 350;
		title = GetMsg("wm/file/TITLE");
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		
		objects["close"] = new cWindowCloseButton(this);

		addlabel("lblLookIn", 15,20,GetMsg("wm/file/SELECTMAP"));
		

		o = new cWindowListBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(5,40);
		o->resizeto(w-10, ((h-80)/12)*12-8);
		objects["filebox"] = o;

		
		for(unsigned int i = 0; i < fs.locations.size(); i++)
		{
			for(map<string, cFile*, less<string> >::iterator it = fs.locations[i]->files.begin(); it != fs.locations[i]->files.end(); it++)
			{
				if(it->first.find(".rsw") != string::npos)
					o->SetText(-1,it->first.substr(rodir.length()));

			}
		}

		mergesort<string>(((cWindowListBox*)o)->values, compare<string>);

		objects["OkButton"] = new cOkButton(this, pCallback);
	}
};

#endif
