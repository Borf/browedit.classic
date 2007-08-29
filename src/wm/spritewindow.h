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


extern cFileSystem fs;

class cSpriteWindow : public cWindow
{
public:
	TiXmlDocument sprites;


	class cWindowSprite : public cWindowObject
	{
	public:
		cWindowSprite(cWindow* parent) : cWindowObject(parent)
		{
			alignment = ALIGN_TOPLEFT;
			moveto(10,20);
			resizeto(120,170);
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
			
			addxml(tree, n->FirstChildElement(), NULL);
		}
		void addxml(cWindowTree* tree, TiXmlNode* n, cWindowTree::cTreeNode* treenode)
		{
			while(n != NULL)
			{
				if(strcmp(n->Value(), "sprite") == 0)
				{
					cWindowTree::cTreeNode* node = new cWindowTree::cTreeNode(n->FirstChild()->Value());
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
					addxml(NULL, n->FirstChildElement(), node);

				}
				n = n->NextSiblingElement();
			}
		}
	};

	cSpriteWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		cWindowObject* o;
		wtype = WT_SPRITE;
		resizable = false;
		visible = true;

		h = 200;
		w = 350;
		title = GetMsg("wm/sprite/TITLE");
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		objects["spritewindow"] = new cWindowSprite(this);

		objects["tabpanel"] = new cTabPanel(this);
		vector<cWindowTree::cTreeNode*> nodes;
		o = new cWindowTree(this,nodes);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(135, 40);
		o->resizeto(w - 145, h- 50);
		objects["tree"] = o;

		sprites = fs.getxml("sprites.xml");

	}


};

#endif
