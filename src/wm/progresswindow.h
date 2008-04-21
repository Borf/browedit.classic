#ifndef __PROGRESSWINDOW_H__
#define __PROGRESSWINDOW_H__

#include "window.h"
#include <texture.h>
#include <font.h>


#include "windowobject.h"
#include "windowprogressbar.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowlistbox.h"


class cProgressWindow : public cWindow
{
	class cCancelButton : public cWindowButton
	{
	private:
		bool* cancel;
	public:
		cCancelButton(cWindow* parent, bool* pCancel, TiXmlDocument &skin) : cWindowButton(parent,skin)
		{
			cancel = pCancel;
			text = "Cancel";
			alignment = ALIGN_TOPLEFT;
			moveto(5,70);

		}

		void click()
		{
			*cancel = false;
		}
		
	};

public:
	cProgressWindow(cTexture* t, cFont* f, bool* cancel, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		wtype = WT_PROGRESS;
		resizable = false;
		visible = true;

		h = 100;
		w = 350;
		title = GetMsg("wm/progress/TITLE");
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		
		o = new cWindowProgressBar(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(5,40);
		o->resizeto(w-10, 10);
		objects["progress"] = o;

		objects["cancel"] = new cCancelButton(this, cancel,skin);


		//objects["ClButton"] = new cOkButton(this, pCallback);
	}
};

#endif
