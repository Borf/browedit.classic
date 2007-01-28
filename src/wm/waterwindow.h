#ifndef __WATERWINDOW_H__
#define __WATERWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"


class cWaterWindowOkButton : public cWindowButton
{
public:
	cWaterWindowOkButton()
	{
		alignment = ALIGN_BOTTOM;
		moveto(0, 20);
		resizeto(100, 20);
		text = "            Ok";
	}
	virtual ~cWaterWindowOkButton() {}
	void click()
	{
		Graphics.world.water.amplitude = atof(parent->objects["amplitude"]->GetText(0).c_str());
		Graphics.world.water.height = atof(parent->objects["height"]->GetText(0).c_str());
		Graphics.world.water.phase = atof(parent->objects["phase"]->GetText(0).c_str());
		Graphics.world.water.surfacecurve = atof(parent->objects["surfacecurve"]->GetText(0).c_str());
		Graphics.world.water.texcycle = atoi(parent->objects["texcycle"]->GetText(0).c_str());
		Graphics.world.water.type = atoi(parent->objects["type"]->GetText(0).c_str());
		parent->close();
	}
};


class cWaterWindow : public cWindow
{
public:
	cWaterWindow()
	{
		wtype = WT_WATER;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = "Water";
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
		o->SetText(0, "Amplitude:");
		objects["lblAmplitude"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 40);
		o->resizeto(320, 100);
		o->SetText(0, "Height:");
		objects["lblHeight"] = o;


		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 60);
		o->resizeto(320, 100);
		o->SetText(0, "Phase:");
		objects["lblPhase"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 80);
		o->resizeto(320, 100);
		o->SetText(0, "SurfaceCycle:");
		objects["lblSurfaceCycle"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 100);
		o->resizeto(320, 100);
		o->SetText(0, "TextureCycle:");
		objects["lblTexCycle"] = o;

		o = new cWindowLabel();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(15, 120);
		o->resizeto(320, 100);
		o->SetText(0, "Type:");
		objects["lblType"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,20);
		o->resizeto(70,20);
		objects["amplitude"] = o;
		
		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,40);
		o->resizeto(70,20);
		objects["height"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,60);
		o->resizeto(70,20);
		objects["phase"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,80);
		o->resizeto(70,20);
		objects["surfacecurve"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,100);
		o->resizeto(70,20);
		objects["texcycle"] = o;

		o = new cWindowInputBox();
		o->parent = this;
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(120,120);
		o->resizeto(70,20);
		objects["type"] = o;
		
		
		o = new cWaterWindowOkButton();
		o->parent = this;
		objects["OkButton"] = o;
	}	
};

#endif
