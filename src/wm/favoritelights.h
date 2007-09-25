#ifndef __FAVORITELIGHTSWINDOW_H__
#define __FAVORITELIGHTSWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowtree.h"
#include "windowinputbox.h"
#include "objectwindow.h"
#include "windowcheckbox.h"
extern TiXmlDocument favoritelights;


void addlights(cWindowTree::cTreeNode* parent, TiXmlNode* n)
{
	while(n != NULL)
	{
		if(strcmp(n->Value(), "light") == 0)
		{
			cWindowTree::cTreeNode* windowtreenode = new cWindowTree::cTreeNode(n->FirstChildElement("name")->FirstChild()->Value());
			parent->addchild(windowtreenode);
		}
		else
		{
			cWindowTree::cTreeNode* windowtreenode = new cWindowTree::cTreeNode(n->Value());
			windowtreenode->open = true;
			parent->addchild(windowtreenode);
			addlights(windowtreenode, n->FirstChildElement());
		}
		n = n->NextSiblingElement();
	}
}

class cFavoriteLightsWindow : public cWindow
{
public:
	cFavoriteLightsWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_FAVLIGHTS;
		resizable = false;
		visible = true;

		h = 260;
		w = 550;
		title = GetMsg("wm/favlight/TITLE");
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);


		vector<cWindowTree::cTreeNode*> nodes;

		cWindowTree::cTreeNode* windownode = new cWindowTree::cTreeNode("root");

		TiXmlNode* n = favoritelights.FirstChildElement();

		addlights(windownode, n);

		cWindowTree::cTreeNode* root = windownode->children[0];
		windownode->children.clear(); // to prevend the children from being deleted;
		delete windownode;

		root->parent = NULL;
		
		

		nodes.push_back(root);



		o = new cWindowTree(this, nodes);
		o->moveto(5,20);
		o->resizeto(200, h-30);
		o->alignment = ALIGN_TOPLEFT;
		objects["list"] = o;

		addlabel("lblPos", 220, 60, GetMsg("wm/light/POSITION"));
		addlabel("lblColor", 220, 80, GetMsg("wm/light/COLOR"));
		addlabel("lblIntensity", 220, 100, GetMsg("wm/light/INTENSITY"));
		addlabel("lblRange", 220, 120, GetMsg("wm/light/RANGE"));
		addlabel("lblMaxlightincrement", 220, 140, GetMsg("wm/light/MAXLIGHTINCREMENT"));
		addlabel("lblLightfalloff", 220, 160, GetMsg("wm/light/LIGHTFALLOFF"));
		addlabel("lblCastsShadow", 220, 180, GetMsg("wm/light/CASTSSHADOW"));
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(300,60);
		o->resizeto(70,20);
		objects["posx"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(370,60);
		o->resizeto(70,20);
		objects["posy"] = o;
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(440,60);
		o->resizeto(70,20);
		objects["posz"] = o;
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(300,80);
		o->resizeto(70,20);
		objects["colorr"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(370,80);
		o->resizeto(70,20);
		objects["colorg"] = o;
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(440,80);
		o->resizeto(70,20);
		objects["colorb"] = o;
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(370,100);
		o->resizeto(140,20);
		objects["intensity"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(370,120);
		o->resizeto(140,20);
		objects["range"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(370,140);
		o->resizeto(140,20);
		objects["maxlightincrement"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(370,160);
		o->resizeto(140,20);
		objects["lightfalloff"] = o;

		o = new cWindowBoolCheckBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(170,182);
		objects["castshadow"] = o;


		objects["OkButton"] = new cWindowOkButton(this);
		objects["CancelButton"] = new cWindowCancelButton(this);
		objects["CancelButton"]->moveto(0,20);
	}	
};

#endif
