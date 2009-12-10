#ifndef __RSMMESHPROPSWINDOW_H__
#define __RSMMESHPROPSWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
#include <wm/windowinputbox.h>
#include <RSMModel.h>


class cRsmMeshPropsWindow : public cWindow
{
public:
	class cRsmFloatInputBox : public cWindowFloatInputBox
	{
	public:
		cRsmFloatInputBox(cWindow* parent, TiXmlDocument* skin = &cWM::skin) : cWindowFloatInputBox(parent, skin)
		{
			alignment = ALIGN_TOPLEFT;
		}
		bool onKeyDown(int keyid, bool shift);
	};


	cRsmModelBase::cMesh* mesh;
	cRsmMeshPropsWindow(cRsmModelBase::cMesh* m);	
	void* userfunc(void* param);

	void onClick();
};

#endif
