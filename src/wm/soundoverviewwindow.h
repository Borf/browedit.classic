#ifndef __SOUNDOVERVIEWWINDOW_H__
#define __SOUNDOVERVIEWWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowtree.h"
#include "windowinputbox.h"
#include "objectwindow.h"
#include "windowcheckbox.h"
//extern cMenu* popupmenu;
#include "../menucommands.h" 


class cSoundOverViewWindow : public cWindow
{
public:
	class cSoundOverViewTreeNode : public cWindowTree::cTreeNode
	{
	public:
		cSoundOverViewTreeNode() : cWindowTree::cTreeNode() {}
		cSoundOverViewTreeNode(string t) : cWindowTree::cTreeNode(t) {}
		cSoundOverViewTreeNode(string t, vector<cWindowTree::cTreeNode*> items) : cWindowTree::cTreeNode(t, items) {}
		bool		iscat;
		cSound		sound;

		cSoundOverViewTreeNode* addNodeTree(string n)
		{
			if(n.find("\\") == string::npos)
			{
				cSoundOverViewTreeNode* node = new cSoundOverViewTreeNode(n);
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
						return ((cSoundOverViewTreeNode*)children[i])->addNodeTree(secondpart);
					}
				}

				cSoundOverViewTreeNode* node = new cSoundOverViewTreeNode(firstpart);
				addchild(node);
				node->iscat = true;
				return node->addNodeTree(secondpart);
				

			}			
		}

		cSoundOverViewTreeNode* getnode(cSound m)
		{
			if(sound == m)
				return this;
			for(unsigned int i = 0; i < children.size(); i++)
			{
				cSoundOverViewTreeNode* n = ((cSoundOverViewTreeNode*)children[i])->getnode(m);
				if(n != NULL)
					return n;
			}
			return NULL;
		}
	
	};



	class cSoundOverViewTree : public cWindowTree
	{
	public:
		cSoundOverViewTree(cWindow* parent, vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument &skin) : cWindowTree(parent, nodes, skin)
		{
			moveto(0,0);
			resizeto(parent->innerw(), parent->innerh());
			alignment = ALIGN_TOPLEFT;
		}

/*		void rightclick()
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
			ADDMENUITEM(mm,popupmenu,"Go to",		&MenuCommand_removefavsound); //new
			ADDMENUITEM(mm,popupmenu,"Insert sound",		&MenuCommand_addfavsound); //new
			ADDMENUITEM(mm,popupmenu,"Insert category",		&MenuCommand_addfavsoundcat); //new
		}*/

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
			
			if(!((cSoundOverViewTreeNode*)node)->iscat)
			{
				Graphics.camerapointer.x = -5*((cSoundOverViewTreeNode*)node)->sound.pos.x;
				Graphics.camerapointer.y = Graphics.world.height*-10+5*((cSoundOverViewTreeNode*)node)->sound.pos.z;

				for(i = 0; i < Graphics.world.sounds.size(); i++)
				{
					if(Graphics.world.sounds[i] == ((cSoundOverViewTreeNode*)node)->sound)
						Graphics.selectedobject = i;
				}
			}
		}
		void getobject(cSound m)
		{
			for(unsigned int i = 0; i < nodes.size(); i++)
			{
				
				cTreeNode* n = ((cSoundOverViewTreeNode*)nodes[i])->getnode(m);
				cTreeNode* nn = n;
				if(n != NULL)
				{
					n = n->parent;
					while(n != NULL)
					{
						n->open = true;
						n = n->parent;
					}

					selected = nn->getselectionnr();
					vector<string> values;
					for(i = 0; i < nodes.size() && i < selected; i++)
						nodes[i]->getdata(values);

					i = 0;
					int yyy = realy()+h-5-12;
					while(yyy+10 > realy() && i < values.size())
					{
						i++;
						yyy-=12;
					}

					if(selected < liststart || selected > liststart+(int)i)
						liststart = min((int)values.size()-i+1 , selected-(i/2));
					if(liststart < 0)
						liststart = 0;
					break;
				}
			}

		}
	};



	static void addobjects(cSoundOverViewTreeNode* parent, bool root)
	{

		if(root)
		{
			cSoundOverViewTreeNode* windowtreenode = new cSoundOverViewTreeNode("sounds");
			windowtreenode->iscat = true;
			parent->addchild(windowtreenode);

			for(unsigned int i = 0; i < Graphics.world.sounds.size(); i++)
			{
				cSoundOverViewTreeNode* node = windowtreenode->addNodeTree(Graphics.world.sounds[i].name);
				node->iscat = false;
				node->sound = Graphics.world.sounds[i];
			}
		}
	}

	cSoundOverViewWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		wtype = WT_SOUNDOVERVIEW;
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
		objects["close"] = new cWindowCloseButton(this,skin);


		vector<cWindowTree::cTreeNode*> nodes;
		objects["list"] = new cSoundOverViewTree(this, nodes,skin);
	

		resizeto(w,h);
		userfunc(NULL);
	}

	void* userfunc(void* p)
	{
		vector<cWindowTree::cTreeNode*> nodes;
		cSoundOverViewTreeNode* windownode = new cSoundOverViewTreeNode("root");

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
		return NULL;
	}

	void resizeto(int ww, int hh)
	{
		cWindow::resizeto(ww, hh);
		objects["list"]->resizeto(innerw(), innerh());
	}
};

#endif
