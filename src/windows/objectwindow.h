#ifndef __OBJECTWINDOW_H__
#define __OBJECTWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
class cUndoItem;

class cWindowOkButton : public cWindowButton
{
public:
	cWindowOkButton(cWindow* parent, TiXmlDocument* = NULL);
	virtual ~cWindowOkButton();
	void click();
};


class cWindowCancelButton : public cWindowButton
{
public:
	cWindowCancelButton(cWindow* parent, TiXmlDocument* = NULL);
	virtual ~cWindowCancelButton();
	void click();
};

class cObjectWindow : public cWindow
{
public:
	cUndoItem* undo;
	cObjectWindow();
	void* userfunc(void* param);
};

#endif
