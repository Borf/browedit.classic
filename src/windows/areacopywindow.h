#ifndef __AREACOPYWINDOW_H__
#define __AREACOPYWINDOW_H__

#include <SDL/SDL_syswm.h>
#include <wm/window.h>
#include <wm/windowobject.h>
#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowlabel.h>
#include <wm/windowinputbox.h>
#include <wm/windowcheckbox.h>

#include <clipboard.h>

class cAreaCopyWindow : public cWindow
{
private:
	class cWindowOkButton : public cWindowButton
	{

	public:
		cWindowOkButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin) : cWindowButton(parent,skin)
		{
			alignment = ALIGN_BOTTOM;
			moveTo(0, 10);
			resizeTo(100, 20);
			text = GetMsg("wm/OK");
		}
		virtual ~cWindowOkButton() {}
		void onClick()
		{
			bool textures = 1==parent->objects["textures"]->getInt(0);
			bool height =	1==parent->objects["height"]->getInt(0);
			bool objects =	1==parent->objects["objects"]->getInt(0);
			bool gat =		1==parent->objects["gat"]->getInt(0);
			cClipBoard::setClipBoard(new cClipBoardArea(textures, height, objects, gat));
			cClipBoard::pasting = true;
			parent->close();
		}
	};
public:
	cAreaCopyWindow() : cWindow()
	{
		windowType = WT_AREACOPY;
		resizable = false;
		visible = true;

		h = 250;
		w = 350;
		title = GetMsg("wm/areacopy/TITLE");
		center();

		defaultObject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);


		addLabel("lblAreaCModel",		15,20,GetMsg("wm/areacopy/MODELS"));
		addLabel("lblAreaCTextures",	15,40,GetMsg("wm/areacopy/TEXTURES"));
		addLabel("lblAreaCHeight",		15,60,GetMsg("wm/areacopy/HEIGHT"));
		addLabel("lblAareaCGat",		15,80,GetMsg("wm/areacopy/GAT"));
		addLabel("lblAareaCWalls",		15,100,GetMsg("wm/areacopy/WALLS"));
		addLabel("lblAareaCLights",		15,120,GetMsg("wm/areacopy/LIGHTS"));
		addLabel("lblAareaCEffects",	15,140,GetMsg("wm/areacopy/EFFECTS"));
		addLabel("lblAareaCSounds",		15,160,GetMsg("wm/areacopy/SOUNDS"));

		addCheckBox("objects",	120,20,true);
		addCheckBox("textures", 120,40,true);
		addCheckBox("height",	120,60,true);
		addCheckBox("gat",		120,80,true);
		addCheckBox("walls",	120,100,true);
		addCheckBox("lights",	120,120,true);
		addCheckBox("effects",	120,140,true);
		addCheckBox("sounds",	120,160,true);
		objects["OkButton"] = new cWindowOkButton(this);
	}	
};

#endif

