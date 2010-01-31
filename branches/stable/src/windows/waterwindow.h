#ifndef __WATERWINDOW_H__
#define __WATERWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>


class cWaterWindowOkButton : public cWindowButton
{
public:
	cWaterWindowOkButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
	virtual ~cWaterWindowOkButton();
	void onClick();
};


class cWaterWindow : public cWindow
{
public:
	cWaterWindow();	
};

#endif
