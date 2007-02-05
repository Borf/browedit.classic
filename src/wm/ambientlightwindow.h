#ifndef __AMBIENTLIGHTWINDOW_H__
#define __AMBIENTLIGHTWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"


class cAmbientLightWindowOkButton : public cWindowButton
{
public:
	cAmbientLightWindowOkButton()
	{
		alignment = ALIGN_BOTTOM;
		moveto(0, 20);
		resizeto(100, 20);
		text = "            Ok";
	}
	virtual ~cAmbientLightWindowOkButton() {}
	void click()
	{
		Graphics.world.ambientlight.ambientr = atoi(parent->objects["ambientr"]->GetText(0).c_str());
		Graphics.world.ambientlight.ambientg = atoi(parent->objects["ambientg"]->GetText(0).c_str());
		Graphics.world.ambientlight.ambientb = atoi(parent->objects["ambientb"]->GetText(0).c_str());

		Graphics.world.ambientlight.diffuse.x = atof(parent->objects["diffuser"]->GetText(0).c_str());
		Graphics.world.ambientlight.diffuse.y = atof(parent->objects["diffuseg"]->GetText(0).c_str());
		Graphics.world.ambientlight.diffuse.z = atof(parent->objects["diffuseb"]->GetText(0).c_str());

		Graphics.world.ambientlight.shadow.x = atof(parent->objects["shadowr"]->GetText(0).c_str());
		Graphics.world.ambientlight.shadow.y = atof(parent->objects["shadowg"]->GetText(0).c_str());
		Graphics.world.ambientlight.shadow.z = atof(parent->objects["shadowb"]->GetText(0).c_str());

		Graphics.world.ambientlight.alpha = atof(parent->objects["alpha"]->GetText(0).c_str());

		parent->close();
	}
};


class cAmbientLightWindow : public cWindow
{
public:
	cAmbientLightWindow()
	{
		wtype = WT_AMBIENTLIGHT;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = "Ambient Light";
		center();

		defaultobject = "OkButton";

		cWindowObject* o = new cWindowRollupButton();
		o->parent = this;
		objects["rollup"] = o;

		o = new cWindowCloseButton();
		o->parent = this;
		objects["close"] = o;


		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 20);
		o->resizeto(320, 100);
		o->SetText(0, "Ambient:");
		objects["lblAmbient"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 40);
		o->resizeto(320, 100);
		o->SetText(0, "Diffuse:");
		objects["lblDiffuse"] = o;


		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 60);
		o->resizeto(320, 100);
		o->SetText(0, "Shadow:");
		objects["lblShadow"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 80);
		o->resizeto(320, 100);
		o->SetText(0, "Alpha:");
		objects["lblAlpha"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,20);
		o->resizeto(70,20);
		objects["ambientr"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(190,20);
		o->resizeto(70,20);
		objects["ambientg"] = o;
		
		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(260,20);
		o->resizeto(70,20);
		objects["ambientb"] = o;
		///////////////////////////////////////////////

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,40);
		o->resizeto(70,20);
		objects["diffuser"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(190,40);
		o->resizeto(70,20);
		objects["diffuseg"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(260,40);
		o->resizeto(70,20);
		objects["diffuseb"] = o;
		///////////////////////////////////////////////
		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,60);
		o->resizeto(70,20);
		objects["shadowr"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(190,60);
		o->resizeto(70,20);
		objects["shadowg"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(260,60);
		o->resizeto(70,20);
		objects["shadowb"] = o;
		/////////////////////////////////////////////////

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,80);
		o->resizeto(210,20);
		objects["alpha"] = o;

		
		o = new cAmbientLightWindowOkButton();
		o->parent = this;
		objects["OkButton"] = o;
	}	
};

#endif
