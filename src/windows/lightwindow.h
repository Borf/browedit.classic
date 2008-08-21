#ifndef __LIGHTWINDOW_H__
#define __LIGHTWINDOW_H__

#include <wm/window.h>


class cLightWindow : public cWindow
{
public:
	cLightWindow(cTexture* t, cFont* f, TiXmlDocument &skin);	
	void* userfunc(void* param);
};

#endif
