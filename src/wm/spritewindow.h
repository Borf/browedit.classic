#ifndef __SPRITEWINDOW_H__
#define __SPRITEWINDOW_H__

#include "tinyxml/tinyxml.h"
#include "filesystem.h"

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

class cSpriteWindow : public cWindow
{
public:
	TiXmlDocument sprites;
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
		cWindowSprite(cWindow* parent) : cWindowObject(parent)
		{
			alignment = ALIGN_TOPLEFT;
			moveto(10,20);
			resizeto(120,170);
			sprite = new cSprite();
		}
		void draw(int,int,int,int)
		{
			GLfloat colors[4];
			glGetFloatv(GL_CURRENT_COLOR, colors);
			int xx, yy;
			xx = realx();
			yy = realy();

			glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
			glBegin(GL_QUADS);
				glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+3, yy+3);
				glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-3, yy+3);
				glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-3, yy+h-3);
				glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+3, yy+h-3);

		// borders
				glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+w-4, yy+4);
				glTexCoord2f((258.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx+w, yy+4);
				glTexCoord2f((258.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx+w, yy+h-4);
				glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+w-4, yy+h-4);

				glTexCoord2f((387.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx, yy+4);
				glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
				glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+4, yy+h-4);
				glTexCoord2f((387.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx, yy+h-4);

				glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+4, yy+h);
				glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+h-4);
				glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+w-4, yy+h-4);
				glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+w-4, yy+h);
				
				glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
				glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+4, yy);
				glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w-4, yy);
				glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+w-4, yy+4);
		//corners
				glTexCoord2f((258.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx, yy+h-4);
				glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+4, yy+h-4);
				glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+4, yy+h);
				glTexCoord2f((258.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx, yy+h);

				glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+w-4, yy+h-4);
				glTexCoord2f((387.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx+w, yy+h-4);
				glTexCoord2f((387.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+w, yy+h);
				glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w-4, yy+h);

				glTexCoord2f((258.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx, yy);
				glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+4, yy);
				glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
				glTexCoord2f((258.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx, yy+4);

				glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+w-4, yy);
				glTexCoord2f((387.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+w, yy);
				glTexCoord2f((387.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx+w, yy+4);
				glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+w-4, yy+4);	
			glEnd();
			glDisable(GL_TEXTURE_2D);

			sprite->pos = cVector3(xx/5 + w/2/5,yy+h/4,0);
			sprite->scale = 1;
			sprite->draw();

		}
	};

	class cTabPanel : public cWindowTabPanel
	{
	public:
		cTabPanel(cWindow* parent) : cWindowTabPanel(parent)
		{
			alignment = ALIGN_TOPLEFT;
			tabs.clear();
			tabs.push_back("Body");
			tabs.push_back("Face");
			tabs.push_back("Upper");
			tabs.push_back("Mid");
			tabs.push_back("Low");
			moveto(135, 20);
			resizeto(parent->pw() - 145,20);
		}
		void tabchange(int oldtab)
		{
			cWindowTree* tree = (cWindowTree*)parent->objects["tree"];
			tree->nodes.clear();
			
			TiXmlNode* n = ((cSpriteWindow*)parent)->sprites.FirstChildElement("sprites");
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
		}
		void addxml(cWindowTree* tree, TiXmlNode* n, cWindowTree::cTreeNode* treenode, string dir = "")
		{
			while(n != NULL)
			{
				if(strcmp(n->Value(), "sprite") == 0)
				{
					cWindowTree::cTreeNode* node = new cWindowTree::cTreeNode(n->FirstChild()->Value());
					
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
		cTree(cWindow* parent, vector<cTreeNode*> n) : cWindowTree(parent, n)
		{

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

			if(node->children.size() == 0)
			{
				cSpriteWindow::cSpriteInfo* info = &((cSpriteWindow*)parent)->lookupmap[node];
			//	Graphics.WM.MessageBox("Selected: " + info->filename);

				int selectedtab = parent->objects["tabpanel"]->GetInt(0);
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
	};

	cSpriteWindow(cTexture* t, cFont* f) : cWindow(t,f)
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

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		objects["spritewindow"] = new cWindowSprite(this);

		objects["tabpanel"] = new cTabPanel(this);
		vector<cWindowTree::cTreeNode*> nodes;
		o = new cTree(this,nodes);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(135, 40);
		o->resizeto(w - 145, h- 50);
		objects["tree"] = o;

		sprites = fs.getxml("sprites.xml");

		((cTabPanel*)objects["tabpanel"])->tabchange(-1);

	}


};

#endif
