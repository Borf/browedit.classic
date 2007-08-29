#ifndef __SPRITEWINDOW_H__
#define __SPRITEWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"
#include "objectwindow.h"

class cSpriteWindow : public cWindow
{
public:
	cSpriteWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		cWindowObject* o;
		wtype = WT_SPRITE;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = GetMsg("wm/sprite/TITLE");
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);
	}


};

#endif
