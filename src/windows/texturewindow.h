#ifndef __TEXTUREWINDOW_H__
#define __TEXTUREWINDOW_H__

#include <wm/window.h>

#include <wm/windowobject.h>
#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowlistbox.h>
#include <wm/windowtree.h>
#include <wm/windowscrollpanel.h>
#include <wm/windowpicturebox.h>
#include "rsmeditwindow.h"

#include <filesystem.h>
extern cFileSystem fs;
extern std::string rodir;
extern cWindow* draggingwindow;
extern cWindowObject* draggingObject;
extern std::vector<std::pair<std::string, std::string> > translations;


extern std::vector<std::string> texturefiles;

class cTextureWindow : public cWindow
{
	class cWindowTexture : public cWindowPictureBox
	{
		std::string data;
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
					t->RoFilename2 = std::string(buf,40);
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
					t->RoFilename2 = std::string(buf,40);
					t->texture = TextureCache.load(rodir + "data\\texture\\" + data);
					Graphics.world.textures.push_back(t);
					Graphics.texturestart = Graphics.world.textures.size() - 2;
				}
			}
		}
		void setText(int i, std::string s)
		{
			cWindowPictureBox::setText(i, s);
			if(i == 1)
			{
				data = s;
			}
		}
	};


	class cWindowTextureCatSelect : public cWindowTree
	{
	public:
		cWindowTextureCatSelect(cWindow* parent, std::vector<cWindowTree::cTreeNode*> n, TiXmlDocument &skin) : cWindowTree(parent, n,skin)
		{
			
		}

		void click()
		{
			unsigned int i;
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

			std::vector<std::pair<std::string, std::string> > v;
			v = *((std::vector<std::pair<std::string, std::string> >*)parent->userfunc(node));

			for(i = 0; i < v.size(); i++)
			{
				std::pair<std::string, std::string> p = v[i];
				cWindowObject* o = new cWindowTexture(parent);
				o->alignment = ALIGN_TOPLEFT;
				o->moveTo(i*130, 32);
				o->resizeTo(128,128);
				o->setText(0,rodir + "data\\texture\\" + p.second);
				o->setText(1,p.second);
				o->setPopup(p.first);
				box->objects.push_back(o);
			}
			parent->resizeto(parent->getWidth(), parent->getHeight());
			draggingwindow = NULL;
			draggingObject = NULL;
		}
	};



public:

	std::map<cWindowTree::cTreeNode*, std::vector<std::pair<std::string, std::string> >, std::less<cWindowTree::cTreeNode*> > items;

	int iconsize;

	cTextureWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		iconsize = 128;
		windowType = WT_TEXTURE;
		closeType = HIDE;
		resizable = true;
		visible = true;
		modal = false;

		h = Graphics.h()-50;
		w = Graphics.w()-50;
		title = GetMsg("wm/texture/TITLE");
		center();
		initprops("textures");

		cWindowObject* o;

		std::vector<cWindowTree::cTreeNode*> nodes;
		std::map<std::string, cWindowTree::cTreeNode*, std::less<std::string> > lookup;

		std::string line, pre, filename, cat, name;

		std::map<std::string, std::string, std::less<std::string> > translationcache;

		for(unsigned int i = 0; i < texturefiles.size(); i++)
		{
			cFile* pFile = fs.open(texturefiles[i]);
			if(pFile == NULL)
				continue;
			while(!pFile->eof())
			{
				line = pFile->readLine();
				if (line == "")
					continue;
				pre = line.substr(0, line.find("|"));
				filename = line.substr(line.find("|")+1);

				cat = pre.substr(0, pre.rfind("/"));
				name = pre.substr(pre.rfind("/")+1);

				if(translationcache.find(cat) != translationcache.end())
					cat = translationcache[cat];
				else
				{
					std::string origcat = cat;
					for(unsigned int ii = 0; ii < translations.size(); ii++)
						cat = replace(cat, translations[ii].first, translations[ii].second);
					translationcache[origcat] = cat;
				}

				if(lookup.find(cat) == lookup.end())
				{
					if(cat.find("/") != std::string::npos)
					{
						std::string p = cat.substr(0, cat.rfind("/"));
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
						items[lookup[cat]] = std::vector<std::pair<std::string, std::string> >();
						items[lookup[cat]].reserve(100);
					}
					items[lookup[cat]].push_back(std::pair<std::string,std::string>(name,filename));
				}
			}
		}

		o = new cWindowTextureCatSelect(this, nodes, skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveTo(0,0);
		o->resizeTo(400,400);
		objects["tree"] = o;

/*		o = new cWindowListBox(this);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(20,20);
		o->resizeto(100,100);
		objects["textures"] = o;*/

		o = new cWindowScrollPanel(this, skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveTo(0, 0);
		o->resizeTo(100,100);
		objects["textures"] = o;

//		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this,skin);

		resizeto(w,h);
	}	

	void resizeto(int ww, int hh)
	{
		cWindow::resizeto(ww,hh);
		objects["tree"]->resizeTo(200, innerHeight());
		cWindowScrollPanel* panel = (cWindowScrollPanel*)objects["textures"];
		panel->moveTo(200, 0);
		panel->resizeTo(innerWidth()-200, innerHeight());
		panel->innerwidth = innerWidth()-200;

		int x = 0;
		int y = 0;
		for(unsigned int i = 0; i < panel->objects.size(); i++)
		{
			panel->objects[i]->moveTo(x,y);
			panel->objects[i]->resizeTo(iconsize,iconsize);
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

	bool onKeyDown(int keyid, bool shift)
	{
		bool b = cWindow::onKeyDown(keyid, shift);
		if(!b)
		{
			if(keyid == SDLK_MINUS)
			{
				iconsize=(int)(iconsize/1.5);
				if(iconsize < 1)
					iconsize = 1;
				resizeto(w,h);
			}
			if(keyid == SDLK_EQUALS)
			{
				iconsize=(int)(iconsize*1.5);
				resizeto(w,h);
			}
		}
		return b;
	}

};

#endif
