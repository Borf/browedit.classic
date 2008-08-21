#ifndef __EFFECTWINDOW_H__
#define __EFFECTWINDOW_H__

#include <wm/window.h>

#include <wm/windowobject.h>
#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowlabel.h>
#include <wm/windowinputbox.h>
#include "objectwindow.h"

class cEffectWindow : public cWindow
{
public:
	cEffectWindow(cTexture* t, cFont* f, TiXmlDocument &skin);
	cUndoItem* undo;
	void* userfunc(void* param);
};

#endif
