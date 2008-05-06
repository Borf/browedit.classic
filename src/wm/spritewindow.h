#ifndef __SPRITEWINDOW_H__
#define __SPRITEWINDOW_H__

#include <tinyxml/tinyxml.h>
#include <filesystem.h>

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"
#include "objectwindow.h"
#include "windowtabpanel.h"
#include "windowtree.h"
#include "../sprite.h"

extern string rodir;
extern cFileSystem fs;
TiXmlDocument sprites;

class cSpriteWindow : public cWindow
{
public:
	class cSpriteInfo
	{
	public:
		string filename;
		cSpriteInfo()
		{
		}
		cSpriteInfo(string f)
		{
			filename = f;
		}
	};

	map<cWindowTree::cTreeNode*, cSpriteInfo, less<cWindowTree::cTreeNode*> >	lookupmap;
	bool male;


	class cWindowSprite : public cWindowObject
	{
	public:
		cSprite* sprite;
		cWindowSprite(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent, skin.FirstChildElement("skin")->FirstChildElement("frame"))
		{
			alignment = ALIGN_TOPLEFT;
			moveto(0,0);
			resizeto(120,160);
			sprite = new cSprite();
		}
		void draw(int,int,int,int)
		{
			GLfloat colors[4];
			glGetFloatv(GL_CURRENT_COLOR, colors);
			int xx, yy;
			xx = realx();
			yy = realy();

			cWindowObject::draw(0,0,0,0);

			sprite->pos = cVector3(xx/5 + w/2/5,yy+h/4,0);
			sprite->scale = 1;
			sprite->draw();

		}
	};

	class cTabPanel : public cWindowTabPanel
	{
	public:
		cTabPanel(cWindow* parent, TiXmlDocument &skin) : cWindowTabPanel(parent, skin)
		{
			alignment = ALIGN_TOPLEFT;
			tabs.clear();
			tabs.push_back("Body");
			tabs.push_back("Face");
			tabs.push_back("Upper");
			tabs.push_back("Mid");
			tabs.push_back("Low");
			moveto(120, 0);
			resizeto(parent->innerw() - 120,parent->innerh());
		}
		void tabchange(int oldtab)
		{
			cWindowTree* tree = (cWindowTree*)parent->objects["tree"];
			tree->nodes.clear();
			
			TiXmlNode* n = sprites.FirstChildElement("sprites");
			if(selectedtab == 0)
				n = n->FirstChildElement("body");
			else if(selectedtab == 1)
				n = n->FirstChildElement("face");
			else
				n = n->FirstChildElement("headgear");
			((cSpriteWindow*)parent)->lookupmap.clear();
			if(selectedtab != 0)
				tree->nodes.push_back(new cWindowTree::cTreeNode("Nothing"));
			addxml(tree, n->FirstChildElement(), NULL, n->ToElement()->Attribute("directory") != NULL ? n->ToElement()->Attribute("directory") : "");

			tree->selected = 0;

			if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite == NULL)
				return;
			

			string name = "";
			if(selectedtab == 0 && ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->body != NULL)
			{
				name = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->body->filename;
				name = name.substr(rodir.length() + 12);
			}
			if(selectedtab == 1 && ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head != NULL)
			{
				name = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head->filename;
				name = name.substr(rodir.length() + 12);
			}
			if(selectedtab == 2 && ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras.size() > 0)
			{
				if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[0] != NULL)
				{
					name = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[0]->filename;
					name = name.substr(name.rfind("\\")+1);
					name = name.substr(name.find("_"));
				}
			}
			if(selectedtab == 3 && ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras.size() > 1)
			{
				if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[1] != NULL)
				{
					name = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[1]->filename;
					name = name.substr(name.rfind("\\")+1);
					name = name.substr(name.find("_"));
				}
			}
			if(selectedtab == 4 && ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras.size() > 2)
			{
				if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[2] != NULL)
				{
					name = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[2]->filename;
					name = name.substr(name.rfind("\\")+1);
					name = name.substr(name.find("_"));
				}
			}
			

			cWindowTree::cTreeNode* node = NULL;
			for(map<cWindowTree::cTreeNode*, cSpriteInfo, less<cWindowTree::cTreeNode*> >::iterator it = ((cSpriteWindow*)parent)->lookupmap.begin(); it != ((cSpriteWindow*)parent)->lookupmap.end(); it++)
			{
				string f = it->second.filename;
				if(it->second.filename == name)
				{
					node = it->first;
				}
			}

			

			cWindowTree::cTreeNode* nn = node;
			while(node != NULL)
			{
				node->open = true;
				node = node->parent;
			}

			node = nn;
			if(node != NULL)
			{
				unsigned int i;
				tree->selected = 0;
				vector<cWindowTree::cTreeNode*> values;
				for(i = 0; i < tree->nodes.size(); i++)
					tree->nodes[i]->getnodes(values);

				for(i = 0; i < values.size(); i++)
				{
					if(values[i] == node)
						tree->selected = i;
				}

				tree->liststart = tree->selected - ((tree->ph()/2) / 20);
				if(tree->liststart < 0)
					tree->liststart = 0;

				int yy = tree->realy()+h-5-12;
				while(yy+10 > tree->realy() && i < values.size())
				{
					i++;
					yy-=12;
				}

				if(tree->ph()/12 > (int)values.size())
					tree->liststart = 0;

				if(tree->liststart >= (int)values.size() - (tree->ph()/12))
					tree->liststart = values.size() - (tree->ph()/12);



			}


		}
		void addxml(cWindowTree* tree, TiXmlNode* n, cWindowTree::cTreeNode* treenode, string dir = "")
		{
			while(n != NULL)
			{
				if(strcmp(n->Value(), "sprite") == 0)
				{
					cWindowTree::cTreeNode* node = new cWindowTree::cTreeNode(n->ToElement()->Attribute("name"));
					
					((cSpriteWindow*)parent)->lookupmap[node] = cSpriteInfo(dir + n->FirstChild()->Value());
					if(treenode == NULL)
						tree->nodes.push_back(node);
					else
						treenode->addchild(node);
				}
				else
				{
					cWindowTree::cTreeNode* node = new cWindowTree::cTreeNode(n->Value());
					if(treenode == NULL)
						tree->nodes.push_back(node);
					else
						treenode->addchild(node);
					char* d = (char*)n->ToElement()->Attribute("directory");
					addxml(NULL, n->FirstChildElement(), node, dir + (d != NULL ? d : ""));

				}
				n = n->NextSiblingElement();
			}
		}
	};


	class cTree : public cWindowTree
	{
	public:
		cTree(cWindow* parent, vector<cTreeNode*> n, TiXmlDocument &skin) : cWindowTree(parent, n,skin)
		{

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

			if(node->children.size() == 0)
			{

				intptr_t selectedtab = parent->objects["tabpanel"]->GetInt(0);
				if(((cSpriteWindow*)parent)->lookupmap.find(node) == ((cSpriteWindow*)parent)->lookupmap.end())
				{
					if(selectedtab == 1)
					{
						delete ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head;
						((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head = NULL;
					}
					if(selectedtab > 1)
					{
						delete ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[selectedtab-2];
						((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[selectedtab-2] = NULL;
					}
					return;
				}
				else
				{
					cSpriteWindow::cSpriteInfo* info = &((cSpriteWindow*)parent)->lookupmap[node];
				//	Graphics.WM.MessageBox("Selected: " + info->filename);
					if(selectedtab == 0)
						((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->loadbody(rodir + "data\\sprite\\" + info->filename);
					if(selectedtab == 1)
						((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->loadhead(rodir + "data\\sprite\\" + info->filename);
					if(selectedtab == 2)
						((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->setextra(0, rodir + "data\\sprite\\악세사리\\" + (((cSpriteWindow*)parent)->male ? "남\\남" : "여\\여")+ info->filename);
					if(selectedtab == 3)
						((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->setextra(1, rodir + "data\\sprite\\악세사리\\" + (((cSpriteWindow*)parent)->male ? "남\\남" : "여\\여")+ info->filename);
					if(selectedtab == 4)
						((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->setextra(2, rodir + "data\\sprite\\악세사리\\" + (((cSpriteWindow*)parent)->male ? "남\\남" : "여\\여")+ info->filename);
				}
			}
			

		}
	};


	class cActionChangeButton : public cWindowButton
	{
	public:
		cActionChangeButton(cWindow* p, TiXmlDocument &skin) : cWindowButton(p,skin)
		{
			alignment = ALIGN_TOPLEFT;
			moveto(0, 200);
			resizeto(120, 20);
			text = "Action 1";
		}
		void click()
		{
			((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->action = (((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->action + 1) % 12;
			char buf[32];
			sprintf(buf, "Action: %i", ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->action);
			text = buf;
		}
	};

	class cDirectionButton : public cWindowButton
	{
		int direction;
	public:
		cDirectionButton(cWindow* p, int dir, TiXmlDocument &skin) : cWindowButton(p,skin)
		{
			direction = dir;
			const char* directions[] = { "S", "SE","E","NE","N","NW","W","SW" };
			text = directions[dir];
			resizeto(50,20);
			alignment = ALIGN_TOPLEFT;
			moveto(35+(int)(35*sin(dir/8.0f*2*PI)), 300+     (dir<5 ? (-20*dir) : -20*(8-dir))     );

		}

		void click()
		{
			((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->direction = direction;
		}
	};

	class cOkButton : public cWindowButton
	{
	public:
		cOkButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
		{
			alignment = ALIGN_TOPLEFT;
			moveto(0, parent->innerh()-20);
			resizeto(120, 20);
			text = "Ok";
		}
		void click()
		{
			if((int)Graphics.world.sprites.size() > Graphics.selectedobject && Graphics.selectedobject != -1)
			{

				if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head != NULL)
					Graphics.world.sprites[Graphics.selectedobject]->loadhead(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head->filename);
				else
				{
					delete Graphics.world.sprites[Graphics.selectedobject]->head;
					Graphics.world.sprites[Graphics.selectedobject]->head = NULL;
				}
				Graphics.world.sprites[Graphics.selectedobject]->loadbody(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->body->filename);

				for(unsigned int i = 0; i < ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras.size(); i++)
				{
					if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[i] != NULL)
					{
						Graphics.world.sprites[Graphics.selectedobject]->setextra(i, ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[i]->filename);
					}
					else
					{
						if(Graphics.world.sprites[Graphics.selectedobject]->extras.size() > i)
							if(Graphics.world.sprites[Graphics.selectedobject]->extras[i] != NULL)
							{
								delete Graphics.world.sprites[Graphics.selectedobject]->extras[i];
								Graphics.world.sprites[Graphics.selectedobject]->extras[i] = NULL;
							}
					}
				}
				Graphics.world.sprites[Graphics.selectedobject]->action = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->action;
				Graphics.world.sprites[Graphics.selectedobject]->direction = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->direction;
			}
			
			parent->close();
		}
	};
	class cCancelButton : public cWindowButton
	{
	public:
		cCancelButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent, skin)
		{
			alignment = ALIGN_TOPLEFT;
			moveto(0, parent->innerh()-40);
			resizeto(120, 20);
			text = "Cancel";
		}
		void click()
		{
			parent->close();
		}
	};


	cSpriteWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		male = true;
		cWindowObject* o;
		wtype = WT_SPRITE;
		resizable = false;
		visible = true;
		male = true;

		h = 400;
		w = 550;
		title = GetMsg("wm/sprite/TITLE");
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this,skin);
		objects["close"] = new cWindowCloseButton(this,skin);

		objects["spritewindow"] = new cWindowSprite(this,skin);

		objects["tabpanel"] = new cTabPanel(this, skin);
		vector<cWindowTree::cTreeNode*> nodes;
		o = new cTree(this,nodes, skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(125, 30);
		o->resizeto(innerw() - 130, innerh()- 35);
		objects["tree"] = o;
		objects["actionbutton"] = new cActionChangeButton(this,skin);
		objects["ok"] = new cOkButton(this,skin);
		objects["cancel"] = new cCancelButton(this,skin);

		for(int i = 0; i < 8; i++)
		{
			char buf[10];
			sprintf(buf, "dir%i", i);
			objects[buf] = new cDirectionButton(this, i,skin);
		}

		if(!sprites.FirstChild())
			sprites = fs.getxml("sprites.xml");

		((cTabPanel*)objects["tabpanel"])->tabchange(-1);


	}


};

#endif
