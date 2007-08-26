#ifndef __AREACOPYWINDOW_H__
#define __AREACOPYWINDOW_H__

#include <SDL/SDL_syswm.h>
#include "window.h"
#include "window.cpp"
#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"
#include "windowcheckbox.h"
extern cFileSystem fs;



class cAreaCopyWindow : public cWindow
{
private:
	class cAreaCopyWindowOkButton : public cWindowButton
	{
	public:
		cAreaCopyWindowOkButton(cWindow* parent) : cWindowButton(parent)
		{
			alignment = ALIGN_BOTTOM;
			moveto(0, 20);
			resizeto(100, 20);
			text = msgtable[WINDOW_OK];
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
	cAreaCopyWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_AREACOPY;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = msgtable[WINDOW_AREACOPY_TITLE];
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		
		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);


		addlabel("lblAreaCModel", 15,20,msgtable[WINDOW_AREACOPY_MODELS]);
		addlabel("lblAreaCTextures", 15,80,msgtable[WINDOW_AREACOPY_TEXTURES]);
		addlabel("lblAreaCHeight", 15,60,msgtable[WINDOW_AREACOPY_HEIGHT]);
		addlabel("lblAareaCGat", 15,40,msgtable[WINDOW_AREACOPY_GAT]);
		addlabel("lblAreaCLights", 15,100,msgtable[WINDOW_AREACOPY_LIGHTS]);
		addlabel("lblAreaCEffects", 15,120,msgtable[WINDOW_AREACOPY_EFFECTS]);
	
	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(120,20);
	o->SetInt(0,1);
	objects["Models"] = o;
//	return o;
  
	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(120,40);
	o->SetInt(0,1);
	objects["Textures"] = o;
//	return o;

	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(120,60);
	o->SetInt(0,1);
	objects["Height"] = o;
//	return o;

	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(120,80);
	o->SetInt(0,1);
	objects["Gat"] = o;
//	return o;

	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(120,80);
	o->SetInt(0,1);
	objects["Lights"] = o;
//	return o;

	o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(120,100);
	o->SetInt(0,1);
	objects["Effects"] = o;
//	return o;

		objects["OkButton"] = new cAreaCopyWindowOkButton(this);
	}	
};

#endif

