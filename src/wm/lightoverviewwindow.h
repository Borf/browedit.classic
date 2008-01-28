#ifndef __LIGHTOVERVIEWWINDOW_H__
#define __LIGHTOVERVIEWWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowtree.h"
#include "windowinputbox.h"
#include "objectwindow.h"
#include "windowcheckbox.h"
extern cMenu* popupmenu;
#include "../menucommands.h" 


class cLightOverViewWindow : public cWindow
{
public:
	class cLightOverViewTreeNode : public cWindowTree::cTreeNode
	{
	public:
		cLightOverViewTreeNode() : cWindowTree::cTreeNode() {}
		cLightOverViewTreeNode(string t) : cWindowTree::cTreeNode(t) {}
		cLightOverViewTreeNode(string t, vector<cWindowTree::cTreeNode*> items) : cWindowTree::cTreeNode(t, items) {}
		bool		iscat;
		cLight		light;

		cLightOverViewTreeNode* addNodeTree(string n)
		{
			if(n.find("\\") == string::npos)
			{
				cLightOverViewTreeNode* node = new cLightOverViewTreeNode(n);
				addchild(node);
				return node;
			}
			else
			{
				string firstpart = n.substr(0, n.find("\\"));
				string secondpart = n.substr(n.find("\\")+1);
			
				for(unsigned int i = 0; i < children.size(); i++)
				{
					if(children[i]->text == firstpart)
					{
						return ((cLightOverViewTreeNode*)children[i])->addNodeTree(secondpart);
					}
				}

				cLightOverViewTreeNode* node = new cLightOverViewTreeNode(firstpart);
				addchild(node);
				node->iscat = true;
				return node->addNodeTree(secondpart);
				

			}			
		}
		void checkopening(vector<cWindowTree::cTreeNode*> list)
		{
			for(unsigned int i = 0; i < list.size(); i++)
			{
				if(list[i]->text == text)
				{
					this->open = list[i]->open;
					for(unsigned int ii = 0; ii < children.size(); ii++)
					{
						((cLightOverViewTreeNode*)children[ii])->checkopening(list[i]->children);
					}
				}
			}
		}

		cLightOverViewTreeNode* getnode(cLight m)
		{
			if(light == m)
				return this;
			for(unsigned int i = 0; i < children.size(); i++)
			{
				cLightOverViewTreeNode* n = ((cLightOverViewTreeNode*)children[i])->getnode(m);
				if(n != NULL)
					return n;
			}
			return NULL;
		}
	
	};



	class cLightOverViewTree : public cWindowTree
	{
	public:
		cLightOverViewTree(cWindow* parent, vector<cWindowTree::cTreeNode*> nodes) : cWindowTree(parent, nodes)
		{
			moveto(5,20);
			resizeto(400, parent->ph()-30);
			alignment = ALIGN_TOPLEFT;
		}

		void rightclick()
		{
			cWindowTree::rightclick();
			popupmenu = new cMenu();
			popupmenu->parent = NULL;
			popupmenu->drawstyle = 1;
			popupmenu->x = (int)mousex;
			popupmenu->y = (int)mousey;
			popupmenu->w = 150;
			popupmenu->opened = true;
			cMenuItem* mm;
			ADDMENUITEM(mm,popupmenu,"Go to",		&MenuCommand_removefavlight); //new
			ADDMENUITEM(mm,popupmenu,"Insert light",		&MenuCommand_addfavlight); //new
			ADDMENUITEM(mm,popupmenu,"Insert category",		&MenuCommand_addfavlightcat); //new
		}

		void onchange()
		{
			unsigned int i;
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
			
			if(!((cLightOverViewTreeNode*)node)->iscat)
			{
				Graphics.camerapointer.x = -5*((cLightOverViewTreeNode*)node)->light.pos.x;
				Graphics.camerapointer.y = Graphics.world.height*-10+5*((cLightOverViewTreeNode*)node)->light.pos.z;

				for(i = 0; i < Graphics.world.lights.size(); i++)
				{
					if(Graphics.world.lights[i] == ((cLightOverViewTreeNode*)node)->light)
						Graphics.selectedobject = i;
				}
			}
		}
		void getobject(cLight m)
		{
			for(unsigned int i = 0; i < nodes.size(); i++)
			{
				
				cTreeNode* n = ((cLightOverViewTreeNode*)nodes[i])->getnode(m);
				cTreeNode* nn = n;
				if(n != NULL)
				{
					n = n->parent;
					while(n != NULL)
					{
						n->open = true;
						n = n->parent;
					}
					return;
					
					vector<string> values;
					for(i = 0; i < nodes.size(); i++)
						nodes[i]->getdata(values);

					i = 0;
					int yyy = realy()+h-5-12;
					while(yyy+10 > realy() && i < values.size())
					{
						i++;
						yyy-=12;
					}

					selected = nn->getselectionnr();
					if(selected < liststart || selected > liststart+(int)i)
						liststart = min((int)values.size()-i+1 , selected-(i/2));
					if(liststart < 0)
						liststart = 0;
					break;
				}
			}

		}
	};



	static void addobjects(cLightOverViewTreeNode* parent, bool root)
	{

		if(root)
		{
			cLightOverViewTreeNode* windowtreenode = new cLightOverViewTreeNode("lights");
			windowtreenode->iscat = true;
			parent->addchild(windowtreenode);

			for(unsigned int i = 0; i < Graphics.world.lights.size(); i++)
			{
				cLightOverViewTreeNode* node = windowtreenode->addNodeTree(Graphics.world.lights[i].name);
				node->iscat = false;
				node->light = Graphics.world.lights[i];
			}
		}
	}

	cLightOverViewWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_LIGHTOVERVIEW;
		visible = true;
		closetype = HIDE;
		notransparency = true;


		resizable = false;
		movable = false;
		h = Graphics.h();
		w = 256;
		title = GetMsg("wm/overview/TITLE");
		y = 0;
		x = Graphics.w() - w;

//		cWindowObject* o;
		objects["close"] = new cWindowCloseButton(this);


		vector<cWindowTree::cTreeNode*> nodes;
		objects["list"] = new cLightOverViewTree(this, nodes);
	

		resizeto(w,h);
		userfunc(NULL);
	}

	void* userfunc(void* p)
	{
		vector<cWindowTree::cTreeNode*> nodes;
		cLightOverViewTreeNode* windownode = new cLightOverViewTreeNode("root");

		addobjects(windownode, true);
		windownode->sort();

		((cLightOverViewTreeNode*)windownode->children[0])->checkopening(((cWindowTree*)objects["list"])->nodes);
		for(unsigned int i = 0; i < windownode->children.size(); i++)
		{
			windownode->children[i]->parent = NULL;
			nodes.push_back(windownode->children[i]);
			windownode->children[i] = NULL;
		}
		delete windownode;
		
		((cWindowTree*)objects["list"])->nodes = nodes;
		return NULL;
	}

	void resizeto(int ww, int hh)
	{
		cWindow::resizeto(ww, hh);
		objects["list"]->resizeto(ww-10, hh-25);
	}
};

#endif
