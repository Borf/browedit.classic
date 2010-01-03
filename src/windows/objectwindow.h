#ifndef __OBJECTWINDOW_H__
#define __OBJECTWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
class cUndoItem;

class cWindowOkButton : public cWindowButton
{
public:
	cWindowOkButton(cWindow* parent, Json::Value &skin = cWM::skin);
	virtual ~cWindowOkButton();
	void onClick();
};


class cWindowCancelButton : public cWindowButton
{
public:
	cWindowCancelButton(cWindow* parent, Json::Value &skin = cWM::skin);
	virtual ~cWindowCancelButton();
	void onClick();
};

class cObjectWindow : public cWindow
{
public:
	cUndoItem* undo;
	cObjectWindow();
	void* userfunc(void* param);
};

#endif
