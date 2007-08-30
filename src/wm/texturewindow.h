#ifndef __TEXTUREWINDOW_H__
#define __TEXTUREWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlistbox.h"
#include "windowtree.h"
#include "windowscrollpanel.h"
#include "windowpicturebox.h"
#include "rsmeditwindow.h"

#include "../filesystem.h"
extern cFileSystem fs;
extern string rodir;
extern cWindow* draggingwindow;
extern cWindowObject* draggingobject;


extern vector<string> texturefiles;

class cTextureWindow : public cWindow
{
	class cWindowTexture : public cWindowPictureBox
	{
		string data;
	public:
		cWindowTexture(cWindow* parent) : cWindowPictureBox(parent)
		{

		}

		void click()
		{
			cWindow* w = Graphics.WM.getwindow(WT_RSMEDIT);
			if(w != NULL)
			{
				((cRSMEditWindow*)w)->changetexture("data\\texture\\" + data);
				parent->close();
			}
			else
			{
				if(SDL_GetModState() & KMOD_SHIFT)
				{
					int id = Graphics.texturestart + (int)(Graphics.selectionstart.y - 32) / 288;
					TextureCache.unload(Graphics.world.textures[id]->texture);
					delete Graphics.world.textures[id];

					cTextureContainer* t = new cTextureContainer();
					t->RoFilename = data;
					char buf[40];
					ZeroMemory(buf, 40);
					sprintf(buf, "%i%i", rand(), rand());
					t->RoFilename2 = string(buf,40);
					t->texture = TextureCache.load(rodir + "data\\texture\\" + data);
					Graphics.world.textures[id] = t;

				}
				else
				{
					cTextureContainer* t = new cTextureContainer();
					t->RoFilename = data;
					char buf[40];
					ZeroMemory(buf, 40);
					sprintf(buf, "%i%i", rand(), rand());
					t->RoFilename2 = string(buf,40);
					t->texture = TextureCache.load(rodir + "data\\texture\\" + data);
					Graphics.world.textures.push_back(t);
					Graphics.texturestart = Graphics.world.textures.size() - 2;
				}
			}
		}
		void SetText(int i, string s)
		{
			cWindowPictureBox::SetText(i, s);
			if(i == 1)
			{
				data = s;
			}
		}
	};


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
				pair<string, string> p = v[i];
				cWindowObject* o = new cWindowTexture(parent);
				o->alignment = ALIGN_TOPLEFT;
				o->moveto(i*130, 32);
				o->resizeto(128,128);
				o->SetText(0,rodir + "data\\texture\\" + p.second);
				o->SetText(1,p.second);
				o->setpopup(p.first);
				box->objects.push_back(o);
			}
			parent->resizeto(parent->pw(), parent->ph());
			draggingwindow = NULL;
			draggingobject = NULL;
		}
	};



public:

	map<cWindowTree::cTreeNode*, vector<pair<string, string> >, less<cWindowTree::cTreeNode*> > items;

	int iconsize;

	cTextureWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		iconsize = 128;
		wtype = WT_TEXTURE;
		closetype = HIDE;
		resizable = true;
		visible = true;
		modal = false;

		h = Graphics.h()-50;
		w = Graphics.w()-50;
		title = GetMsg("wm/texture/TITLE");
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

//		objects["rollup"] = new cWindowRollupButton(this);
//		objects["close"] = new cWindowCloseButton(this);

		resizeto(w,h);
	}	

	void resizeto(int ww, int hh)
	{
		cWindow::resizeto(ww,hh);
		objects["tree"]->resizeto(200, hh-30);
		cWindowScrollPanel* panel = (cWindowScrollPanel*)objects["textures"];
		panel->moveto(220, 20);
		panel->resizeto(ww-220, hh-30);
		panel->innerwidth = ww-220;

		int x = 0;
		int y = 0;
		for(int i = 0; i < panel->objects.size(); i++)
		{
			panel->objects[i]->moveto(x,y);
			panel->objects[i]->resizeto(iconsize,iconsize);
			x+=iconsize+2;
			if(x+iconsize+2 > panel->innerwidth-18)
			{
				x = 0;
				y += iconsize+2;
			}
		}
		panel->scrollposx = 0;
		panel->scrollposy = 0;
		panel->innerheight = y+iconsize+2;
	}

	void* userfunc(void* param)
	{
		return &items[(cWindowTree::cTreeNode*)param];
	}

	bool onkeydown(int keyid, bool shift)
	{
		bool b = cWindow::onkeydown(keyid, shift);
		if(!b)
		{
			if(keyid == SDLK_MINUS)
			{
				iconsize/=1.5;
				if(iconsize < 1)
					iconsize = 1;
				resizeto(w,h);
			}
			if(keyid == SDLK_EQUALS)
			{
				iconsize*=1.5;
				resizeto(w,h);
			}
		}
		return b;
	}

};

#endif
