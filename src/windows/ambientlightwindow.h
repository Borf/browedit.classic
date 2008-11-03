#ifndef __AMBIENTLIGHTWINDOW_H__
#define __AMBIENTLIGHTWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>

class cAmbientLightWindow : public cWindow
{
	class cAmbientLightWindowOkButton : public cWindowButton
	{
	public:
		cAmbientLightWindowOkButton(cWindow* parent);
		virtual ~cAmbientLightWindowOkButton() {}
		void onClick();
	};

public:
	cAmbientLightWindow();	
};

#endif
