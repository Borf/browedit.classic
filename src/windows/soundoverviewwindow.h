#ifndef __SOUNDOVERVIEWWINDOW_H__
#define __SOUNDOVERVIEWWINDOW_H__

#include <wm/window.h>

#include <wm/windowobject.h>
#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowlabel.h>
#include <wm/windowtree.h>
#include <wm/windowinputbox.h>
#include "objectwindow.h"
#include <wm/windowcheckbox.h>
//extern cMenu* popupmenu;
#include "../menucommands.h" 


class cSoundOverViewWindow : public cWindow
{
public:
	class cSoundOverViewTreeNode : public cWindowTree::cTreeNode
	{
	public:
		cSoundOverViewTreeNode() : cWindowTree::cTreeNode() {}
		cSoundOverViewTreeNode(std::string t) : cWindowTree::cTreeNode(t) {}
		cSoundOverViewTreeNode(std::string t, std::vector<cWindowTree::cTreeNode*> items) : cWindowTree::cTreeNode(t, items) {}
		bool		iscat;
		cSound		sound;

		cSoundOverViewTreeNode* addNodeTree(std::string n)
		{
			if(n.find("\\") == std::string::npos)
			{
				cSoundOverViewTreeNode* node = new cSoundOverViewTreeNode(n);
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
		cSoundOverViewTree(cWindow* parent, std::vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument &skin) : cWindowTree(parent, nodes, skin)
		{
			moveTo(0,0);
			resizeTo(parent->innerWidth(), parent->innerHeight());
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
						Graphics.selectedObject = i;
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
					std::vector<std::string> values;
					for(i = 0; i < nodes.size() && i < selected; i++)
						nodes[i]->getdata(values);

					i = 0;
					int yyy = realY()+h-5-12;
					while(yyy+10 > realY() && i < values.size())
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
		windowType = WT_SOUNDOVERVIEW;
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
		objects["list"] = new cSoundOverViewTree(this, nodes,skin);
	

		resizeTo(w,h);
		userfunc(NULL);
	}

	void* userfunc(void* p)
	{
		std::vector<cWindowTree::cTreeNode*> nodes;
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

	void resizeTo(int ww, int hh)
	{
		cWindow::resizeto(ww, hh);
		objects["list"]->resizeTo(innerWidth(), innerHeight());
	}
};

#endif
