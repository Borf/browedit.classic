#ifndef __WATERWINDOW_H__
#define __WATERWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>


class cWaterWindowOkButton : public cWindowButton
{
public:
	cWaterWindowOkButton(cWindow* parent, TiXmlDocument &skin);
	virtual ~cWaterWindowOkButton();
	void click();
};


class cWaterWindow : public cWindow
{
public:
	cWaterWindow(cTexture* t, cFont* f, TiXmlDocument &skin);	
};

#endif
