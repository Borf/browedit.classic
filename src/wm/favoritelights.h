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



class cFavoriteLightsWindow : public cWindow
{
public:
	class cFavoriteTreeNode : public cWindowTree::cTreeNode
	{
	public:
		cFavoriteTreeNode() : cWindowTree::cTreeNode() {}
		cFavoriteTreeNode(string t) : cWindowTree::cTreeNode(t) {}
		cFavoriteTreeNode(string t, vector<cWindowTree::cTreeNode*> items) : cWindowTree::cTreeNode(t, items) {}


		vector<int> keys;

	};

	
	void addlights(cFavoriteTreeNode* parent, TiXmlNode* n)
	{
		while(n != NULL)
		{
			if(strcmp(n->Value(), "light") == 0)
			{
				cFavoriteTreeNode* windowtreenode = new cFavoriteTreeNode(n->FirstChildElement("name")->FirstChild()->Value());
				parent->addchild(windowtreenode);

				vector<int> keys;
				TiXmlNode* el = n;
				while(el != NULL)
				{
					TiXmlNode* pel = el->Parent();
					if(pel != NULL)
					{
						TiXmlNode* child = pel->FirstChild();
						for(int i = 0; child != NULL; i++)
						{
							if(child == el)
							{
								keys.push_back(i);
								break;
							}
							child=child->NextSibling();
						}
					}
					el = el->Parent();
				}
				windowtreenode->keys = keys;


			}
			else
			{
				cFavoriteTreeNode* windowtreenode = new cFavoriteTreeNode(n->Value());
				windowtreenode->open = true;
				parent->addchild(windowtreenode);
				addlights(windowtreenode, n->FirstChildElement());
			}
			n = n->NextSiblingElement();
		}
	}


	class cFavoritesTree : public cWindowTree
	{
	public:
		cFavoritesTree(cWindow* parent, vector<cWindowTree::cTreeNode*> nodes) : cWindowTree(parent, nodes)
		{
			moveto(5,20);
			resizeto(200, parent->ph()-30);
			alignment = ALIGN_TOPLEFT;
		}
		void onchange()
		{
			int i;
			int a = selected;
			cWindowTree::cTreeNode* node;
			for(i = 0; i < nodes.size(); i++)
			{
				 node = nodes[i]->getnode(a);
				 if(node != NULL)
					 break;
			}
			if(node == NULL)
				return;


			if(((cFavoriteLightsWindow*)parent)->currentkey.size() > 0)
			{
				vector<int> key = ((cFavoriteLightsWindow*)parent)->currentkey;
				TiXmlNode* n = favoritelights.FirstChild();
				for(i = 0; i < key[key.size()-1]; i++)
					n = n->NextSibling();

				for(i = key.size()-2; i > -1; i--)
				{
					n = n->FirstChild();
					for(int ii = 0; ii < key[i]; ii++)
						n = n->NextSibling();
				}
				n->FirstChildElement("name")->FirstChild()->SetValue(parent->objects["name"]->GetText(0).c_str());
				n->FirstChildElement("color")->SetAttribute("r", parent->objects["colorr"]->GetText(0).c_str());
				n->FirstChildElement("color")->SetAttribute("g", parent->objects["colorg"]->GetText(0).c_str());
				n->FirstChildElement("color")->SetAttribute("b", parent->objects["colorb"]->GetText(0).c_str());
				n->FirstChildElement("brightness")->FirstChild()->SetValue(parent->objects["intensity"]->GetText(0).c_str());
				n->FirstChildElement("range")->FirstChild()->SetValue(parent->objects["range"]->GetText(0).c_str());
				n->FirstChildElement("maxlight")->FirstChild()->SetValue(parent->objects["lightfalloff"]->GetText(0).c_str());
				n->FirstChildElement("lightfalloff")->FirstChild()->SetValue(parent->objects["lightfalloff"]->GetText(0).c_str());
				n->FirstChildElement("givesshadow")->FirstChild()->SetValue(parent->objects["castshadow"]->GetText(0).c_str());
				n->FirstChildElement("height")->FirstChild()->SetValue(parent->objects["height"]->GetText(0).c_str());
			}

			if(node->children.size() != 0)
			{
				parent->objects["name"]->SetText(0, "");
				parent->objects["colorr"]->SetText(0, "");
				parent->objects["colorg"]->SetText(0, "");
				parent->objects["colorb"]->SetText(0, "");
				parent->objects["intensity"]->SetText(0, "");
				parent->objects["range"]->SetText(0, "");
				parent->objects["maxlightincrement"]->SetText(0, "");
				parent->objects["lightfalloff"]->SetText(0, "");
				parent->objects["castshadow"]->SetText(0, "");
				parent->objects["height"]->SetText(0, "");
				((cFavoriteLightsWindow*)parent)->currentkey.clear();
				return;
			}
			vector<int> keys = ((cFavoriteTreeNode*)node)->keys;
			((cFavoriteLightsWindow*)parent)->currentkey = keys;

			TiXmlNode* n = favoritelights.FirstChild();
			for(i = 0; i < keys[keys.size()-1]; i++)
				n = n->NextSibling();

			for(i = keys.size()-2; i > -1; i--)
			{
				n = n->FirstChild();
				for(int ii = 0; ii < keys[i]; ii++)
					n = n->NextSibling();
			}
				
			parent->objects["name"]->SetText(0, n->FirstChildElement("name")->FirstChild()->Value());
			parent->objects["colorr"]->SetText(0, n->FirstChildElement("color")->Attribute("r"));
			parent->objects["colorg"]->SetText(0, n->FirstChildElement("color")->Attribute("g"));
			parent->objects["colorb"]->SetText(0, n->FirstChildElement("color")->Attribute("b"));
			parent->objects["intensity"]->SetText(0, n->FirstChildElement("brightness")->FirstChild()->Value());
			parent->objects["range"]->SetText(0, n->FirstChildElement("range")->FirstChild()->Value());
			parent->objects["maxlightincrement"]->SetText(0, n->FirstChildElement("maxlight")->FirstChild()->Value());
			parent->objects["lightfalloff"]->SetText(0, n->FirstChildElement("lightfalloff")->FirstChild()->Value());
			parent->objects["castshadow"]->SetText(0, n->FirstChildElement("givesshadow")->FirstChild()->Value());
			parent->objects["height"]->SetText(0, n->FirstChildElement("height")->FirstChild()->Value());
		}
	};


	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* parent) : cWindowButton(parent)
		{
			alignment = ALIGN_BOTTOM;
			moveto(100, 20);
			resizeto(100, 20);
			text = GetMsg("wm/OK");
		}

		void click()
		{
			((cWindowTree*)parent->objects["list"])->onchange();
			favoritelights.SaveFile("data/lights.txt");
			parent->close();
		}
	};
	class cWindowCancelButton : public cWindowButton
	{
	public:
		cWindowCancelButton(cWindow* parent) : cWindowButton(parent)
		{
			alignment = ALIGN_BOTTOM;
			moveto(-100, 20);
			resizeto(100, 20);
			text = GetMsg("wm/CANCEL");
		}
		virtual ~cWindowCancelButton() {}
		void click()
		{
			favoritelights = fs.getxml("data/lights.txt");
			parent->close();
		}
	};


	vector<int> currentkey;
	cFavoriteLightsWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_FAVLIGHTS;
		resizable = false;
		visible = true;

		h = 270;
		w = 550;
		title = GetMsg("wm/favlight/TITLE");
		center();

		defaultobject = "OkButton";

		cWindowObject* o;
		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);


		vector<cWindowTree::cTreeNode*> nodes;
		cFavoriteTreeNode* windownode = new cFavoriteTreeNode("root");
		TiXmlNode* n = favoritelights.FirstChildElement();
		addlights(windownode, n);
		cWindowTree::cTreeNode* root = windownode->children[0];
		windownode->children.clear(); // to prevend the children from being deleted;
		delete windownode;
		root->parent = NULL;
		nodes.push_back(root);

		objects["list"] = new cFavoritesTree(this, nodes);

		addlabel("lblName", 220, 60, GetMsg("wm/favlight/NAME"));
		addlabel("lblColor", 220, 80, GetMsg("wm/favlight/COLOR"));
		addlabel("lblIntensity", 220, 100, GetMsg("wm/favlight/INTENSITY"));
		addlabel("lblRange", 220, 120, GetMsg("wm/favlight/RANGE"));
		addlabel("lblMaxlightincrement", 220, 140, GetMsg("wm/favlight/MAXLIGHTINCREMENT"));
		addlabel("lblLightfalloff", 220, 160, GetMsg("wm/favlight/LIGHTFALLOFF"));
		addlabel("lblCastsShadow", 220, 180, GetMsg("wm/favlight/CASTSSHADOW"));
		addlabel("lblHeight", 220, 200, GetMsg("wm/favlight/HEIGHT"));
		
		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(370,60);
		o->resizeto(140,20);
		objects["name"] = o;
	
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

		o = new cWindowCheckBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(370,182);
		objects["castshadow"] = o;

		o = new cWindowInputBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(370,200);
		o->resizeto(140,20);
		objects["height"] = o;


		((cWindowTree*)objects["list"])->onchange();

		objects["OkButton"] = new cWindowOkButton(this);
		objects["CancelButton"] = new cWindowCancelButton(this);
		objects["CancelButton"]->moveto(0,20);
	}	
};

#endif
