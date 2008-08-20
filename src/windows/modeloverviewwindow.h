#ifndef __MODELOVERVIEWWINDOW_H__
#define __MODELOVERVIEWWINDOW_H__

#include <wm/window.h>

#include <wm/windowobject.h>
#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowlabel.h>
#include <wm/windowtree.h>
#include <wm/windowinputbox.h>
#include "objectwindow.h"
#include <wm/windowcheckbox.h>
extern cMenu* popupmenu;
#include "../menucommands.h" 


class cModelOverViewWindow : public cWindow
{
public:
	class cModelOverViewTreeNode : public cWindowTree::cTreeNode
	{
	public:
		cModelOverViewTreeNode() : cWindowTree::cTreeNode() {}
		cModelOverViewTreeNode(std::string t) : cWindowTree::cTreeNode(t) {}
		cModelOverViewTreeNode(std::string t, std::vector<cWindowTree::cTreeNode*> items) : cWindowTree::cTreeNode(t, items) {}
		bool		iscat;

		int type;
		cRSMModel* model;

		cModelOverViewTreeNode* addNodeTree(std::string n)
		{
			if(n.find("\\") == std::string::npos)
			{
				cModelOverViewTreeNode* node = new cModelOverViewTreeNode(n);
				addchild(node);
				return node;
			}
			else
			{
				std::string firstpart = n.substr(0, n.find("\\"));
				std::string secondpart = n.substr(n.find("\\")+1);
			
				for(unsigned int i = 0; i < children.size(); i++)
				{
					if(children[i]->text == firstpart)
					{
						return ((cModelOverViewTreeNode*)children[i])->addNodeTree(secondpart);
					}
				}

				cModelOverViewTreeNode* node = new cModelOverViewTreeNode(firstpart);
				addchild(node);
				node->iscat = true;
				return node->addNodeTree(secondpart);
				

			}			
		}

		cModelOverViewTreeNode* getnode(cRSMModel* m)
		{
			if(model == m)
				return this;
			for(unsigned int i = 0; i < children.size(); i++)
			{
				cModelOverViewTreeNode* n = ((cModelOverViewTreeNode*)children[i])->getnode(m);
				if(n != NULL)
					return n;
			}
			return NULL;
		}
	
	};



	class cModelOverViewTree : public cWindowTree
	{
	public:
		cModelOverViewTree(cWindow* parent, std::vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument &skin) : cWindowTree(parent, nodes, skin)
		{
			moveTo(0,0);
			resizeTo(parent->innerWidth(), parent->innerHeight());
			alignment = ALIGN_TOPLEFT;
		}

		void rightClick()
		{
			cWindowTree::rightClick();
			popupmenu = new cMenu();
			popupmenu->parent = NULL;
			popupmenu->drawstyle = 1;
			popupmenu->x = (int)mouseX;
			popupmenu->y = (int)mouseY;
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
			
			if(!((cModelOverViewTreeNode*)node)->iscat)
			{
				Graphics.camerapointer.x = -5*((cModelOverViewTreeNode*)node)->model->pos.x;
				Graphics.camerapointer.y = Graphics.world.height*-10+5*((cModelOverViewTreeNode*)node)->model->pos.z;

				for(i = 0; i < Graphics.world.models.size(); i++)
				{
					if(Graphics.world.models[i] == ((cModelOverViewTreeNode*)node)->model)
						Graphics.selectedObject = i;
				}
			}
		}
		void getobject(cRSMModel* m)
		{
			for(unsigned int i = 0; i < nodes.size(); i++)
			{
				
				cTreeNode* n = ((cModelOverViewTreeNode*)nodes[i])->getnode(m);
				cTreeNode* nn = n;
				if(n != NULL)
				{
					n = n->parent;
					while(n != NULL)
					{
						n->open = true;
						n = n->parent;
					}

					std::vector<std::string> values;
					for(i = 0; i < nodes.size(); i++)
						nodes[i]->getdata(values);
					
					i = 0;
					int yyy = realY()+h-5-12;
					while(yyy+10 > realY() && i < values.size())
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



	static void addobjects(cModelOverViewTreeNode* parent, bool root)
	{

		if(root)
		{
			cModelOverViewTreeNode* windowtreenode = new cModelOverViewTreeNode("objects");
			windowtreenode->iscat = true;
			parent->addchild(windowtreenode);

			for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
			{
				cModelOverViewTreeNode* node = windowtreenode->addNodeTree(Graphics.world.models[i]->name);
				node->iscat = false;
				node->model = Graphics.world.models[i];
			}
		}
	}

	cModelOverViewWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		windowType = WT_MODELOVERVIEW;
		visible = true;
		closeType = HIDE;
		noTransparency = true;


		resizable = false;
		movable = false;
		h = Graphics.h();
		w = 256;
		title = GetMsg("wm/overview/TITLE");
		y = 0;
		x = Graphics.w() - w;

//		cWindowObject* o;
		objects["close"] = new cWindowCloseButton(this,skin);


		std::vector<cWindowTree::cTreeNode*> nodes;
		objects["list"] = new cModelOverViewTree(this, nodes, skin);
	

		resizeTo(w,h);
		userfunc(NULL);
	}

	void* userfunc(void* p)
	{
		std::vector<cWindowTree::cTreeNode*> nodes;
		cModelOverViewTreeNode* windownode = new cModelOverViewTreeNode("root");

		addobjects(windownode, true);
		windownode->sort();

		for(unsigned int i = 0; i < windownode->children.size(); i++)
		{
			windownode->children[i]->parent = NULL;
			nodes.push_back(windownode->children[i]);
			windownode->children[i] = NULL;
		}
		delete windownode;
		
		((cWindowTree*)objects["list"])->nodes = nodes;
		((cWindowTree*)objects["list"])->validCache = false;
		return NULL;
	}

	void resizeTo(int ww, int hh)
	{
		cWindow::resizeto(ww, hh);
		objects["list"]->resizeTo(innerWidth(), innerHeight());
	}
};

#endif
