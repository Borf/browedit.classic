#ifndef __OBJECTWINDOW_H__
#define __OBJECTWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
class cUndoItem;

class cWindowOkButton : public cWindowButton
{
public:
	cWindowOkButton(cWindow* parent, TiXmlDocument &skin);
	virtual ~cWindowOkButton();
	void click();
};


class cWindowCancelButton : public cWindowButton
{
public:
	cWindowCancelButton(cWindow* parent, TiXmlDocument &skin);
	virtual ~cWindowCancelButton();
	void click();
};

class cObjectWindow : public cWindow
{
public:
	cUndoItem* undo;
	cObjectWindow(cTexture* t, cFont* f, TiXmlDocument &skin);	
	void* userfunc(void* param);
};

#endif
