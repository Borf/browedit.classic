#ifndef __SOUNDSELECTWINDOW_H__
#define __SOUNDSELECTWINDOW_H__

#include "window.h"

#include <SDL/SDL_mixer.h>
#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowlistbox.h"
#include "windowtree.h"
#include "windowscrollpanel.h"
#include "windowpicturebox.h"
#include "rsmeditwindow.h"

#include <filesystem.h>
extern cFileSystem fs;
extern string rodir;
extern cWindow* draggingwindow;
extern cWindowObject* draggingobject;
extern vector<pair<string, string> > translations;
extern void mainloop();

extern vector<string> soundfiles;

class cSoundSelectWindow : public cWindow
{
	class cWindowSoundCatSelect : public cWindowTree
	{
	public:
		cWindowSoundCatSelect(cWindow* parent, vector<cWindowTree::cTreeNode*> n, TiXmlDocument &skin) : cWindowTree(parent, n,skin)
		{
			
		}

		void click()
		{
			unsigned int i;
			cWindowTree::click();
			cWindowDataListBox<string>* box = (cWindowDataListBox<string>*)parent->objects["sounds"];
			box->values.clear();
			box->data.clear();
			box->SetInt(-2,0);

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
				box->values.push_back(p.first);
				box->data.push_back(p.second);
				
			}
			parent->resizeto(parent->pw(), parent->ph());
			draggingwindow = NULL;
			draggingobject = NULL;
		}
	};


	class cSoundList : public cWindowDataListBox<string>
	{
	public:
		cSoundList(cWindow* parent, TiXmlDocument &skin) : cWindowDataListBox<string>(parent,skin)
		{
		}
		
		void doubleclick()
		{
			parent->objects["play"]->click();
		}
	};


	class cWindowPlayButton : public cWindowButton
	{
	public:
		cWindowPlayButton(cWindow* w, TiXmlDocument &skin) : cWindowButton(w,skin)
		{
			alignment = ALIGN_BOTTOMRIGHT;
			moveto(0,0);
			resizeto(100,20);
			text = "Play";
		}
		void click()
		{
			if(text == "Play")
			{
				int selected = parent->objects["sounds"]->GetInt(-1);
				if(selected >= ((cWindowDataListBox<string>*)parent->objects["sounds"])->data.size())
					return;

				string filename = ((cWindowDataListBox<string>*)parent->objects["sounds"])->data[selected];

				text = "Stop";

				Mix_Chunk *sample;

				cFile* pFile = fs.open(rodir+"data/wav/" + filename);
				sample=Mix_QuickLoad_WAV((BYTE*)pFile->data);
				Mix_Volume(-1,MIX_MAX_VOLUME);
				Mix_PlayChannel(0, sample, 0);
				while(Mix_Playing(-1) > 0 && text == "Stop")
				{
					mainloop();
				}
				if(text == "Play")
					Mix_HaltChannel(-1);
				Mix_FreeChunk(sample);
				pFile->close();
				text = "Play";
			}
			else
			{
				text = "Play";
			}
		}
	};
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* w, TiXmlDocument &skin) : cWindowButton(w,skin)
		{
			alignment = ALIGN_BOTTOMRIGHT;
			moveto(100,0);
			resizeto(100,20);
			text = "Ok";
		}
		void click()
		{
			int selected = parent->objects["sounds"]->GetInt(-1);
			if(selected >= ((cWindowDataListBox<string>*)parent->objects["sounds"])->data.size())
				return;

			string filename = ((cWindowDataListBox<string>*)parent->objects["sounds"])->data[selected];

			cSound* selectedSound = ((cSoundSelectWindow*)parent)->selectedSound;
			if(selectedSound)
			{
				selectedSound->fileName = filename;
			}
			else
			{
				cVector3 newPos = ((cSoundSelectWindow*)parent)->newPos;
				cSound s;
				s.fileName = filename;
				s.name = ((cWindowDataListBox<string>*)parent->objects["sounds"])->values[selected];
				s.pos = cVector3(newPos.x/5, newPos.y-17, newPos.z/5);
				s.rotation = cVector3(0,-45,0);
				s.scale = cVector3(0.94f,0.94f,0.94f);
				s.unknown6[0] = (BYTE)0xc7;
				s.unknown6[1] = (BYTE)0xc1;
				s.unknown6[2] = (BYTE)0xb7;
				s.unknown6[3] = (BYTE)0xd0;
				s.unknown6[4] = (BYTE)0x4e;
				s.unknown6[5] = (BYTE)0x2d;
				s.unknown6[6] = (BYTE)0xe3;
				s.unknown6[7] = (BYTE)0x43;

				unsigned char rawData[40] =
				{
					0x20, 0xD0, 0x6E, 0x0F, 0xF0, 0x75, 0x42, 0x00, 0x14, 0xF1, 0x12, 0x00, 0x74, 0x00, 0x00, 0x00, 
					0xE1, 0xF1, 0x12, 0x00, 0xE0, 0xA0, 0xE9, 0x07, 0x40, 0xB0, 0xE1, 0x00, 0x20, 0xD0, 0x6E, 0x0F, 
					0x04, 0x00, 0x00, 0x00, 0xC0, 0xCC, 0xC6, 0xE5, 
				};
				s.todo1 = string((char*)rawData,40);
				s.name = "sound" + inttostring(rand() % 99999);
				s.repeatDelay = 0.5f;
				s.unknown2 = 70.0f;
				s.unknown3 = 1;
				s.unknown2 = 1;
				s.unknown5 = 1;
				s.unknown7 = -435.095f;
				s.unknown8 = 0;

				Graphics.world.sounds.push_back(s);
				undostack.push(new cUndoNewSound());
			}
			parent->close();

			cWindow* w = Graphics.WM.getwindow(WT_SOUNDOVERVIEW);
			if(w != NULL)
				w->userfunc(NULL);

		}
	};
	class cWindowCancelButton : public cWindowButton
	{
	public:
		cWindowCancelButton(cWindow* w, TiXmlDocument &skin) : cWindowButton(w,skin)
		{
			alignment = ALIGN_BOTTOMRIGHT;
			moveto(200,0);
			resizeto(100,20);
			text = "Cancel";
		}
		void click()
		{
			parent->close();
		}
	};

public:

	map<cWindowTree::cTreeNode*, vector<pair<string, string> >, less<cWindowTree::cTreeNode*> > items;

	cVector3 newPos;
	cSound* selectedSound;

	cSoundSelectWindow(cTexture* t, cFont* f, TiXmlDocument &skin, cVector3 pNewPos) : cWindow(t,f,skin)
	{
		selectedSound = NULL;
		newPos = pNewPos; 
		wtype = WT_SOUNDSELECT;
		closetype = HIDE;
		resizable = true;
		visible = true;
		modal = false;

		h = Graphics.h()-50;
		w = Graphics.w()-50;
		title = GetMsg("wm/soundselect/TITLE");
		center();

		cWindowObject* o;

		vector<cWindowTree::cTreeNode*> nodes;
		map<string, cWindowTree::cTreeNode*, less<string> > lookup;

		string line, pre, filename, cat, name;

		map<string, string, less<string> > translationcache;

		for(unsigned int i = 0; i < soundfiles.size(); i++)
		{
			cFile* pFile = fs.open(soundfiles[i]);
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
					string origcat = cat;
					for(unsigned int ii = 0; ii < translations.size(); ii++)
						cat = replace(cat, translations[ii].first, translations[ii].second);
					translationcache[origcat] = cat;
				}

				if(lookup.find(cat) == lookup.end())
				{
					if(cat.find("/") != string::npos)
					{
						string p = cat.substr(0, cat.rfind("/"));
						if(lookup.find(p) == lookup.end())
						{
							Log(1,0,"Invalid nesting in soundfile...");
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
						items[lookup[cat]].reserve(100);
					}
					items[lookup[cat]].push_back(pair<string,string>(name,filename));
				}
			}
		}

		o = new cWindowSoundCatSelect(this, nodes, skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(0,0);
		o->resizeto(400,400);
		objects["tree"] = o;

		o = new cSoundList(this, skin);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(0, 0);
		o->resizeto(100,100);
		objects["sounds"] = o;

		objects["play"] = new cWindowPlayButton(this,skin);
		objects["ok"] = new cSoundSelectWindow::cWindowOkButton(this,skin);
		objects["cancel"] = new cSoundSelectWindow::cWindowCancelButton(this,skin);


		objects["tree"]->click();

//		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this,skin);

		resizeto(w,h);
	}	

	void resizeto(int ww, int hh)
	{
		cWindow::resizeto(ww,hh);
		objects["tree"]->resizeto(200, innerh());
		objects["sounds"]->moveto(200, 0);
		objects["sounds"]->resizeto(innerw()-200, innerh()-20);
	}

	void* userfunc(void* param)
	{
		return &items[(cWindowTree::cTreeNode*)param];
	}


};

#endif
