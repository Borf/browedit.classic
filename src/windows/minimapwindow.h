#ifndef __MINIMAPWINDOW_H__
#define __MINIMAPWINDOW_H__

#include <wm/window.h>
#include <wm/windowobject.h>

class cMiniMapWindow : public cWindow
{
public:

	class cMiniMap : public cWindowObject
	{
	public:
		cMiniMap(cWindow* parent);
		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
		void click();
		void drag();
	};
	cMiniMapWindow(cTexture* t, cFont* f, TiXmlDocument &skin);	
};

#endif
