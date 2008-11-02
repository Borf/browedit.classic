#ifndef __LIGHTWINDOW_H__
#define __LIGHTWINDOW_H__

#include <wm/window.h>


class cLightWindow : public cWindow
{
public:
	cLightWindow();	
	void* userfunc(void* param);
};

#endif
