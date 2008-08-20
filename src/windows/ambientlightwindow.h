#ifndef __AMBIENTLIGHTWINDOW_H__
#define __AMBIENTLIGHTWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>

class cAmbientLightWindow : public cWindow
{
	class cAmbientLightWindowOkButton : public cWindowButton
	{
	public:
		cAmbientLightWindowOkButton(cWindow* parent, TiXmlDocument &skin);
		virtual ~cAmbientLightWindowOkButton() {}
		void click();
	};

public:
	cAmbientLightWindow(cTexture* t, cFont* f, TiXmlDocument &skin);	
};

#endif
