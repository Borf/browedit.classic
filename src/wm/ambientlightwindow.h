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
	cAmbientLightWindowOkButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
	{
		alignment = ALIGN_BOTTOM;
		moveto(0, 0);
		resizeto(100, 20);
		text = "Ok";
	}
	virtual ~cAmbientLightWindowOkButton() {}
	void click()
	{
		Graphics.world.ambientLight.ambientr = atoi(parent->objects["ambientr"]->GetText(0).c_str());
		Graphics.world.ambientLight.ambientg = atoi(parent->objects["ambientg"]->GetText(0).c_str());
		Graphics.world.ambientLight.ambientb = atoi(parent->objects["ambientb"]->GetText(0).c_str());

		Graphics.world.ambientLight.diffuse.x = atof(parent->objects["diffuser"]->GetText(0).c_str());
		Graphics.world.ambientLight.diffuse.y = atof(parent->objects["diffuseg"]->GetText(0).c_str());
		Graphics.world.ambientLight.diffuse.z = atof(parent->objects["diffuseb"]->GetText(0).c_str());

		Graphics.world.ambientLight.shadow.x = atof(parent->objects["shadowr"]->GetText(0).c_str());
		Graphics.world.ambientLight.shadow.y = atof(parent->objects["shadowg"]->GetText(0).c_str());
		Graphics.world.ambientLight.shadow.z = atof(parent->objects["shadowb"]->GetText(0).c_str());

		Graphics.world.ambientLight.alpha = atof(parent->objects["alpha"]->GetText(0).c_str());

		parent->close();
	}
};


class cAmbientLightWindow : public cWindow
{
public:
	cAmbientLightWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		wtype = WT_AMBIENTLIGHT;
		resizable = false;
		visible = true;

		h = 150;
		w = 310+skinOffLeft+skinOffRight;
		title = GetMsg("wm/ambient/TITLE");
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this,skin);
		objects["close"] = new cWindowCloseButton(this,skin);

		cWindowObject* o;

		addlabel("lblAmbient", 0,0,GetMsg("wm/ambient/AMBIENT"));
		addlabel("lblDiffuse", 0,20,GetMsg("wm/ambient/DIFFUSE"));
		addlabel("lblShadow",0,40,GetMsg("wm/ambient/SHADOW"));
		addlabel("lblAlpha", 0,60,GetMsg("wm/ambient/ALPHA"));

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,0);
		o->resizeto(70,20);
		objects["ambientr"] = o;

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,0);
		o->resizeto(70,20);
		objects["ambientg"] = o;
		
		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,0);
		o->resizeto(70,20);
		objects["ambientb"] = o;
		///////////////////////////////////////////////

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,20);
		o->resizeto(70,20);
		objects["diffuser"] = o;

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,20);
		o->resizeto(70,20);
		objects["diffuseg"] = o;

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,20);
		o->resizeto(70,20);
		objects["diffuseb"] = o;
		///////////////////////////////////////////////
		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,40);
		o->resizeto(70,20);
		objects["shadowr"] = o;

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,40);
		o->resizeto(70,20);
		objects["shadowg"] = o;

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(240,40);
		o->resizeto(70,20);
		objects["shadowb"] = o;
		/////////////////////////////////////////////////

		o = new cWindowInputBox(this,skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(100,60);
		o->resizeto(210,20);
		objects["alpha"] = o;

		objects["OkButton"] = new cAmbientLightWindowOkButton(this,skin);
	}	
};

#endif
