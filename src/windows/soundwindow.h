#ifndef __SOUNDWINDOW_H__
#define __SOUNDWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
class cUndoItem;
class cSound;

class cSoundWindow : public cWindow
{
public:
	class cWindowSelectFileButton : public cWindowButton
	{
	public:
		cSound*	selectedSound;
		cWindowSelectFileButton(cWindow* parent, TiXmlDocument &skin);
		void click();
	};
	cUndoItem* undo;
	cSoundWindow(cTexture* t, cFont* f, TiXmlDocument &skin);	
	void* userfunc(void* param);
};

#endif
