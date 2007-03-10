#ifndef __MODELSWINDOW_H__
#define __MODELSWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlistbox.h"
#include "windowtree.h"
#include "windowscrollpanel.h"
#include "windowpicturebox.h"

#include "../RSMModel.h"
#include "../filesystem.h"
extern cFileSystem fs;
extern string rodir;
extern cWindow* draggingwindow;
extern cWindowObject* draggingobject;
extern eMode editmode;

extern vector<string> objectfiles;

class cModelsWindow : public cWindow
{
	class cWindowModel : public cWindowObject
	{
		cRSMModel* model;
		string data;
	public:
		cWindowModel(cWindow* parent) : cWindowObject(parent)
		{
			type = OBJECT_MODEL;
			model = NULL;
		}
		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
		{

			if(data != "" && model == NULL)
			{
				glPushMatrix();
				glLoadIdentity();
				model = new cRSMModel();
				model->load(rodir + data);
				model->pos = cVector3(0,0.7*w,1000);
				
				float sc = 0;
				sc = max(sc, model->bb2.bbmax[0] - model->bb2.bbmin[0]);
				sc = max(sc, model->bb2.bbmax[1] - model->bb2.bbmin[1]);
				sc = max(sc, model->bb2.bbmax[2] - model->bb2.bbmin[2]);
				sc = 1.5f*min(h,w) / sc;

				model->scale = cVector3(sc,sc,sc);

				model->rot = cVector3(0,0,0);
				model->bb2.bbrange[0] = 0;
				//model->bb2.bbmin[1] = 0;
				model->bb2.bbrange[2] = 0;
				glPopMatrix();
			}


			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);			// (X, Y, Width, Height)
			
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
			glPushMatrix();

			int xx, yy;
			xx = parent->px() + realx();
			yy = parent->py() + realy();
			glViewport(xx, yy+(cutoffbottom), w, h-(cutofftop) - (cutoffbottom));
			glLoadIdentity();
			glOrtho(-w,w,-h+2*cutoffbottom,h-2*cutofftop,-10000,10000);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glColor4f(0,0,0,1);
			glDisable(GL_TEXTURE_2D);
			glLineWidth(2);
			glBegin(GL_LINE_LOOP);
				glVertex2f(-w,-h);
				glVertex2f(w,-h);
				glVertex2f(w,h);
				glVertex2f(-w,h);
			glEnd();
			glLineWidth(1);
			glEnable(GL_TEXTURE_2D);
			glColor4f(1,1,1,1);

			if (model != NULL)
			{
				glTranslatef(0,0,1000);
				model->draw(false);
				glTranslatef(0,0,-1000);
				model->rot.y+=40*(Graphics.frameticks / 1000.0f);
			}

			glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}


		void click()
		{
			if(Graphics.previewmodel != NULL)
			{
				if(Graphics.previewmodel->filename == model->filename)
				{
					if (editmode == MODE_OBJECTS)
						Graphics.previewcolor = 0;
					parent->close();
					return;
				}
				delete Graphics.previewmodel;
			}
			Graphics.previewmodel = new cRSMModel();
			Graphics.previewmodel->load(rodir + data);
			Graphics.previewmodel->rot = cVector3(0,0,0);
			Graphics.previewmodel->scale = cVector3(4,4,4);
			Graphics.previewmodel->pos = cVector3(40,-40,-40);
			Graphics.previewcolor = 200;

		}

		void SetText(int i, string s)
		{
			if(i == 1)
			{
				data = s;
			}
		}

		string GetText(int i)
		{
			return data;
		}

		void drag()
		{
			parent->objects["zdragger"]->moveto(mousex-parent->px()-256,mousey-(Graphics.h()-parent->ph()-parent->py()));
			parent->objects["zdragger"]->SetText(1, data);
		}

		void SetInt(int i, int id)
		{
			data = "";
			if (model != NULL)
			{
				delete model;
				model = NULL;
			}
		}
		

		~cWindowModel()
		{
			if(model != NULL)
				delete model;
			model = NULL;
		}
	};


	class cWindowModelCatSelect : public cWindowTree
	{
		int originalselection;
	public:
		cWindowModelCatSelect(cWindow* parent, vector<cWindowTree::cTreeNode*> n) : cWindowTree(parent, n)
		{
			originalselection = -1;
		}

		void click()
		{
			int i;
			cWindowTree::click();
			cWindowScrollPanel* box = (cWindowScrollPanel*)parent->objects["models"];
			for(i = 0; i < box->objects.size(); i++)
				delete box->objects[i];
			box->objects.clear();
			int a = selected;
			cWindowTree::cTreeNode* node;
			for(i = 0; i < nodes.size(); i++)
			{
				 node = nodes[i]->getnode(a);
				 if(node != NULL)
					 break;
			}

			vector<pair<string, string> > v;
			v = *((vector<pair<string, string> >*)parent->userfunc(node));

			for(i = 0; i < v.size(); i++)
			{
				pair<string, string> p = v[i];
				cWindowObject* o = new cWindowModel(parent);
				o->alignment = ALIGN_TOPLEFT;
				o->moveto(i*130, 32);
				o->resizeto(128,128);
				o->SetText(0,rodir + p.second);
				o->SetText(1,p.second);
				o->setpopup(p.first);
				box->objects.push_back(o);
			}
			parent->resizeto(parent->pw(), parent->ph());
			draggingwindow = NULL;
			draggingobject = NULL;
		}
		void holddragover()
		{
			if(originalselection == -1)
				originalselection = selected;
			int i;
			vector<string> values;
			for(i = 0; i < nodes.size(); i++)
				nodes[i]->getdata(values);

			
			int xx = (int)mousex;
			xx -= realx();
			xx -= parent->px();
			int yy = Graphics.h()-(int)mousey;
			yy -= realy();
			yy -= parent->py();

			if (xx < w - 14)
			{ // in the box
				int s = selected;
				selected = liststart + ((h-yy-3) / 12);
				if (selected > (int)values.size() || selected < 0)
					selected = s;
			}
		}
		void dragover()
		{
			int i;
			int a = selected;
			cTreeNode* node = NULL;
			for(i = 0; i < nodes.size(); i++)
			{
				 node = nodes[i]->getnode(a);
				 if(node != NULL)
					 break;
			}
			a = originalselection;
			cTreeNode* nodeorig = NULL;
			for(i = 0; i < nodes.size(); i++)
			{
				 nodeorig = nodes[i]->getnode(a);
				 if(nodeorig != NULL)
					 break;
			}

			if(node != NULL && nodeorig != NULL)
			{
				string orig = "";
				string dest = "";
				cTreeNode* n = node;
				while(n != NULL)
				{
					dest = "/" + n->text + dest;
					n = n->parent;
				}
				dest = dest.substr(1);
				n = nodeorig;
				while(n != NULL)
				{
					orig = "/" + n->text + orig;
					n = n->parent;
				}
				orig = orig.substr(1);
				
				

				Graphics.WM.MessageBox("Moving: " + draggingobject->GetText(0) + " from " + orig + " to " + dest);
			}
			

			selected = originalselection;
			originalselection = -1;
		}

	};
	


public:

	map<cWindowTree::cTreeNode*, vector<pair<string, string> >, less<cWindowTree::cTreeNode*> > items;

	cModelsWindow()
	{
		wtype = WT_MODELS;
		closetype = HIDE;
		resizable = true;
		visible = true;
		modal = false;

		h = Graphics.h()-50;
		w = Graphics.w()-50;
		title = "Model Select";
		center();

		cWindowObject* o;

		vector<cWindowTree::cTreeNode*> nodes;
		map<string, cWindowTree::cTreeNode*, less<string> > lookup;

		for(int i = 0; i < objectfiles.size(); i++)
		{
			cFile* pFile = fs.open(objectfiles[i]);
			while(!pFile->eof())
			{
				string line = pFile->readline();
				if (line == "")
					continue;
				string pre = line.substr(0, line.find("|"));
				string filename = line.substr(line.find("|")+1);

				string cat = pre.substr(0, pre.rfind("/"));
				string name = pre.substr(pre.rfind("/")+1);

				if(lookup.find(cat) == lookup.end())
				{
					if(cat.find("/") != string::npos)
					{
						string p = cat.substr(0, cat.rfind("/"));
						if(lookup.find(p) == lookup.end())
						{
							Log(1,0,"Invalid nesting in objectfile...");
						}
						else
						{
							cWindowTree::cTreeNode* n = new cWindowTree::cTreeNode(cat.substr(cat.rfind("/")+1));
							lookup[cat] = n;
							lookup[p]->addchild(n);
						}
					}
					else
					{
						cWindowTree::cTreeNode* n = new cWindowTree::cTreeNode(cat);
						lookup[cat] = n;
						nodes.push_back(n);
					}
				}
				if(filename != "")
				{
					if (items.find(lookup[cat]) == items.end())
					{
						items[lookup[cat]] = vector<pair<string, string> >();
					}
					items[lookup[cat]].push_back(pair<string,string>(name,filename));
				}
			}
		}

		o = new cWindowModelCatSelect(this, nodes);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(20,20);
		o->resizeto(400,400);
		objects["tree"] = o;

/*		o = new cWindowListBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(20,20);
		o->resizeto(100,100);
		objects["textures"] = o;*/

		o = new cWindowScrollPanel(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(20, 20);
		o->resizeto(100,100);
		objects["models"] = o;

		o = new cWindowModel(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(9999,9999);
		o->resizeto(128,128);
		//o->SetText(0,"data/dragger.tga");
		objects["zdragger"] = o;

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		resizeto(w,h);
	}	

	void resizeto(int ww, int hh)
	{
		cWindow::resizeto(ww,hh);
		objects["tree"]->resizeto(200, hh-30);
		cWindowScrollPanel* panel = (cWindowScrollPanel*)objects["models"];
		panel->moveto(220, 20);
		panel->resizeto(ww-220, hh-30);
		panel->innerwidth = ww-220;

		int x = 0;
		int y = 0;
		for(int i = 0; i < panel->objects.size(); i++)
		{
			panel->objects[i]->moveto(x,y);
			x+=130;
			if(x+130 > panel->innerwidth-18)
			{
				x = 0;
				y += 130;
			}
		}
		panel->scrollposx = 0;
		panel->scrollposy = 0;
		panel->innerheight = y+130;
	}
	void stopdrag()
	{
		objects["zdragger"]->moveto(-4000,-4000);
		objects["zdragger"]->SetInt(0,0);
		((cWindowScrollPanel*)objects["models"])->draggingobject = NULL;

	}	

	void* userfunc(void* param)
	{
		return &items[(cWindowTree::cTreeNode*)param];
	}

};

#endif
