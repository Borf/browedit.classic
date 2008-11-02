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



class cAreaCopyWindow : public cWindow
{
private:
	class cAreaCopyWindowOkButton : public cWindowButton
	{

	public:
		cAreaCopyWindowOkButton(cWindow* parent, TiXmlDocument* skin = NULL) : cWindowButton(parent,skin)
		{
			alignment = ALIGN_BOTTOM;
			moveTo(0, 20);
			resizeTo(100, 20);
			text = GetMsg("wm/OK");
		}
		virtual ~cAreaCopyWindowOkButton() {}
		void click()
		{

/*			Graphics.world.areac.AreaCModel = atof(parent->objects["amplitude"]->GetText(0).c_str());
			Graphics.world.areac.AreaCTextures = atof(parent->objects["height"]->GetText(0).c_str());
			Graphics.world.areac.AreaCHeight = atof(parent->objects["phase"]->GetText(0).c_str());
			Graphics.world.areac.AreaCGat = atof(parent->objects["surfacecurve"]->GetText(0).c_str());
			Graphics.world.areac.AreaCLights = atoi(parent->objects["type"]->GetText(0).c_str());
			parent->close();
*/
		}
	};
public:
	cAreaCopyWindow() : cWindow()
	{
		windowType = WT_AREACOPY;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = GetMsg("wm/areacopy/TITLE");
		center();

		defaultObject = "OkButton";

		cWindowObject* o;
		
		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);


		addLabel("lblAreaCModel", 15,20,GetMsg("wm/areacopy/MODELS"));
		addLabel("lblAreaCTextures", 15,40,GetMsg("wm/areacopy/TEXTURES"));
		addLabel("lblAreaCHeight", 15,60,GetMsg("wm/areacopy/HEIGHT"));
		addLabel("lblAareaCGat", 15,80,GetMsg("wm/areacopy/GAT"));
		addLabel("lblAreaCLights", 15,100,GetMsg("wm/areacopy/LIGHTS"));
		addLabel("lblAreaCEffects", 15,120,GetMsg("wm/areacopy/EFFECTS"));
	
	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(120,20);
	o->setInt(0,1);
	objects["Models"] = o;
//	return o;
  
	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(120,40);
	o->setInt(0,1);
	objects["Textures"] = o;
//	return o;

	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(120,60);
	o->setInt(0,1);
	objects["Height"] = o;
//	return o;

	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(120,80);
	o->setInt(0,1);
	objects["Gat"] = o;
//	return o;

	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(120,100);
	o->setInt(0,1);
	objects["Lights"] = o;
//	return o;

	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(120,120);
	o->setInt(0,1);
	objects["Effects"] = o;
//	return o;

		objects["OkButton"] = new cAreaCopyWindowOkButton(this);
	}	
};

#endif

