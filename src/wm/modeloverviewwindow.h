#ifndef __MODELOVERVIEWWINDOW_H__
#define __MODELOVERVIEWWINDOW_H__

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
#include "menucommands.h" 


class cModelOverViewWindow : public cWindow
{
public:
	class cModelOverViewTreeNode : public cWindowTree::cTreeNode
	{
	public:
		cModelOverViewTreeNode() : cWindowTree::cTreeNode() {}
		cModelOverViewTreeNode(string t) : cWindowTree::cTreeNode(t) {}
		cModelOverViewTreeNode(string t, vector<cWindowTree::cTreeNode*> items) : cWindowTree::cTreeNode(t, items) {}
		bool		iscat;

		int type;
		cRSMModel* model;

		cModelOverViewTreeNode* addNodeTree(string n)
		{
			if(n.find("\\") == string::npos)
			{
				cModelOverViewTreeNode* node = new cModelOverViewTreeNode(n);
				addchild(node);
				return node;
			}
			else
			{
				string firstpart = n.substr(0, n.find("\\"));
				string secondpart = n.substr(n.find("\\")+1);
			
				for(int i = 0; i < children.size(); i++)
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
		void checkopening(vector<cWindowTree::cTreeNode*> list)
		{
			for(unsigned int i = 0; i < list.size(); i++)
			{
				if(list[i]->text == text)
				{
					this->open = list[i]->open;
					for(unsigned int ii = 0; ii < children.size(); ii++)
					{
						((cModelOverViewTreeNode*)children[ii])->checkopening(list[i]->children);
					}
				}
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
		cModelOverViewTree(cWindow* parent, vector<cWindowTree::cTreeNode*> nodes) : cWindowTree(parent, nodes)
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
			popupmenu->x = mousex;
			popupmenu->y = mousey;
			popupmenu->w = 150;
			popupmenu->opened = true;
			cMenuItem* mm;
			ADDMENUITEM(mm,popupmenu,"Go to",		&MenuCommand_removefavlight); //new
			ADDMENUITEM(mm,popupmenu,"Insert light",		&MenuCommand_addfavlight); //new
			ADDMENUITEM(mm,popupmenu,"Insert category",		&MenuCommand_addfavlightcat); //new
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
			
			if(!((cModelOverViewTreeNode*)node)->iscat)
			{
				Graphics.camerapointer.x = -5*((cModelOverViewTreeNode*)node)->model->pos.x;
				Graphics.camerapointer.y = Graphics.world.height*-10+5*((cModelOverViewTreeNode*)node)->model->pos.z;

				for(i = 0; i < Graphics.world.models.size(); i++)
				{
					if(Graphics.world.models[i] == ((cModelOverViewTreeNode*)node)->model)
						Graphics.selectedobject = i;
				}
			}
		}
		void getobject(cRSMModel* m)
		{
			for(int i = 0; i < nodes.size(); i++)
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

					
					int i;
					vector<string> values;
					for(i = 0; i < nodes.size(); i++)
						nodes[i]->getdata(values);

					i = 0;
					int yyy = realy()+h-5-12;
					while(yyy+10 > realy() && i < (int)values.size())
					{
						i++;
						yyy-=12;
					}

					selected = nn->getselectionnr();
					if(selected < liststart || selected > liststart+i)
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

			for(int i = 0; i < Graphics.world.models.size(); i++)
			{
				cModelOverViewTreeNode* node = windowtreenode->addNodeTree(Graphics.world.models[i]->name);
				node->iscat = false;
				node->model = Graphics.world.models[i];
			}
		}
	}

	cModelOverViewWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_MODELOVERVIEW;
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
		objects["list"] = new cModelOverViewTree(this, nodes);
	

		resizeto(w,h);
		userfunc(NULL);
	}

	void* userfunc(void* p)
	{
		vector<cWindowTree::cTreeNode*> nodes;
		cModelOverViewTreeNode* windownode = new cModelOverViewTreeNode("root");

		addobjects(windownode, true);
		windownode->sort();

		((cModelOverViewTreeNode*)windownode->children[0])->checkopening(((cWindowTree*)objects["list"])->nodes);
		for(int i = 0; i < windownode->children.size(); i++)
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
