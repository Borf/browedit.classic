#ifndef __MESSAGEWINDOW_H__
#define __MESSAGEWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlistbox.h"
#include "windowtree.h"
#include "windowscrollpanel.h"
#include "windowpicturebox.h"

#include "../filesystem.h"
extern cFileSystem fs;
extern string rodir;

extern vector<string> texturefiles;

class cTextureWindow : public cWindow
{
	class cWindowTextureCatSelect : public cWindowTree
	{
	public:
		cWindowTextureCatSelect(cWindow* parent, vector<cWindowTree::cTreeNode*> n) : cWindowTree(parent, n)
		{
			
		}

		void click()
		{
			int i;
			cWindowTree::click();
			cWindowScrollPanel* box = (cWindowScrollPanel*)parent->objects["textures"];
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
				cWindowObject* o = new cWindowPictureBox(parent);
				o->alignment = ALIGN_TOPLEFT;
				o->moveto(i*130, 32);
				o->resizeto(128,128);
				o->SetText(0,rodir + "data\\texture\\" + v[i].second);
				box->objects.push_back(o);
			}
			parent->resizeto(parent->pw(), parent->ph());
			
		}
	};



public:

	map<cWindowTree::cTreeNode*, vector<pair<string, string> >, less<cWindowTree::cTreeNode*> > items;

	cTextureWindow()
	{
		wtype = WT_MESSAGE;
		resizable = true;
		visible = true;
		modal = false;

		h = 500;
		w = 700;
		title = "Texture Select";
		center();

		cWindowObject* o;

		vector<cWindowTree::cTreeNode*> nodes;
		map<string, cWindowTree::cTreeNode*, less<string> > lookup;

		for(int i = 0; i < texturefiles.size(); i++)
		{
			cFile* pFile = fs.open(texturefiles[i]);
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
							Log(1,0,"Invalid nesting in texturefile...");
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

		o = new cWindowTextureCatSelect(this, nodes);
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
		objects["textures"] = o;

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		resizeto(w,h);
	}	

	void resizeto(int ww, int hh)
	{
		cWindow::resizeto(ww,hh);
		objects["tree"]->resizeto(ww / 2, hh-30);
		cWindowScrollPanel* panel = (cWindowScrollPanel*)objects["textures"];
		panel->moveto((ww / 2)+20, 20);
		panel->resizeto((ww / 2)-20, hh-30);
		panel->innerwidth = (ww / 2) - 20;

		int x = 0;
		int y = 0;
		for(int i = 0; i < panel->objects.size(); i++)
		{
			panel->objects[i]->moveto(x,y);
			x+=130;
			if(x > panel->innerwidth-18)
			{
				x = 0;
				y += 130;
			}
		}
		panel->scrollposx = 0;
		panel->scrollposy = 0;
		panel->innerheight = y+130;

		panel->innerheight += (panel->innerheight % (panel->ph() - 16));

		
	}

	void* userfunc(void* param)
	{
		return &items[(cWindowTree::cTreeNode*)param];
	}

};

#endif
