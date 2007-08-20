#ifndef __RSMEDITWINDOW_H__
#define __RSMEDITWINDOW_H__

#include <SDL/SDL_syswm.h>
#include "../wm/window.h"

#include "../wm/windowobject.h"
#include "../wm/windowroundbutton.h"
#include "../wm/windowbutton.h"
#include "../wm/windowlabel.h"
extern cFileSystem fs;


extern string rodir;


class cRSMEditWindow : public cWindow
{
	class cWindowOpenButton : public cWindowButton
	{
	public:
		cWindowOpenButton(cWindow* parent) : cWindowButton(parent)
		{
			alignment = ALIGN_TOPLEFT;
			moveto(5, 20);
			resizeto(50, 20);
			text = "Open";
		}
		void click()
		{
		#ifdef WIN32
			char curdir[255];
			getcwd(curdir, 255);
			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version)	;
			SDL_GetWMInfo(&wmInfo);
			HWND hWnd = wmInfo.window;
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;

			strcpy(((cRSMEditWindow*)parent)->filename, replace(((cRSMEditWindow*)parent)->filename, "/", "\\").c_str());
			ofn.lpstrFile = ((cRSMEditWindow*)parent)->filename;
			ofn.nMaxFile = 256;
			ofn.lpstrFilter = "All\0*.*\0RO Models\0*.rsm\0";
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
			if (GetOpenFileName(&ofn))
			{

				chdir(curdir);
				((cRSMEditWindow*)parent)->open();


			}


		#endif
		}
	};
	class cWindowSaveButton : public cWindowButton
	{
	public:
		cWindowSaveButton(cWindow* parent) : cWindowButton(parent)
		{
			alignment = ALIGN_TOPLEFT;
			moveto(55, 20);
			resizeto(50, 20);
			text = "Save";
		}
		void click()
		{
			parent->close();
		}
	};
	class cWindowSaveAsButton : public cWindowButton
	{
	public:
		cWindowSaveAsButton(cWindow* parent) : cWindowButton(parent)
		{
			alignment = ALIGN_TOPLEFT;
			moveto(105, 20);
			resizeto(50, 20);
			text = "Save As";
		}
		void click()
		{
			string oldfilename = ((cRSMEditWindow*)parent)->filename;
			int i;
			cFile* pFile = fs.open(((cRSMEditWindow*)parent)->filename);

			char buffer[100];
			char header[100];
			pFile->read(header, 6); // header
			if (header[5] == 4)
				pFile->read(header+6, 25); // unknown
			else
				pFile->read(header+6, 24); // unknown

			pFile->read(buffer, 4); // ntextures;
			long nTextures;
			memcpy((char*)&nTextures, buffer, 4);

			for(i = 0; i < nTextures; i++)
				pFile->read(buffer, 40);

			string rest = "";
			while(!pFile->eof())
			{
				char buf[1024];
				int read = pFile->read(buf, 1024);
				rest += string(buf, read);
			}

			pFile->close();

			#ifdef WIN32
				char curdir[255];
				getcwd(curdir, 255);
				SDL_SysWMinfo wmInfo;
				SDL_VERSION(&wmInfo.version);
				SDL_GetWMInfo(&wmInfo);
				HWND hWnd = wmInfo.window;
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				strcpy(((cRSMEditWindow*)parent)->filename, replace(((cRSMEditWindow*)parent)->filename, "/", "\\").c_str());
				ofn.lpstrFile = ((cRSMEditWindow*)parent)->filename;
				ofn.nMaxFile = 256;
				ofn.lpstrFilter = "All\0*.*\0RO models\0*.rsm\0";
				ofn.nFilterIndex = 2;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
				if (!GetSaveFileName(&ofn))
				{
					strcpy(((cRSMEditWindow*)parent)->filename, oldfilename.c_str());
					chdir(curdir);
				}
				else
				{
					if (strcmp(((cRSMEditWindow*)parent)->filename+strlen(((cRSMEditWindow*)parent)->filename)-4, ".rsm") != 0)
					{ 
						strcpy(((cRSMEditWindow*)parent)->filename, (string(((cRSMEditWindow*)parent)->filename) + ".rsm").c_str());
					}
					chdir(curdir);

					
					ofstream pFile2;
					pFile2.open(((cRSMEditWindow*)parent)->filename, ios_base::out | ios_base::binary);
					pFile2.write(header, header[5] == 4 ? 31 : 30);


					cWindowModel* model = ((cWindowModel*)parent->objects["model"]);
					long nTextures = model->model->textures.size();
					pFile2.write((char*)&nTextures, 4);
					for(i = 0; i < model->model->textures.size(); i++)
					{
						char bufje[40];
						ZeroMemory(bufje, 40);
						string t = model->model->textures[i]->getfilename();
						t = t.substr(rodir.length()+13);
						strcpy(bufje, t.c_str());
						pFile2.write(bufje, 40);
					}

					pFile2.write(rest.c_str(), rest.length()-1);

					pFile2.close();





				}
			#endif


		}
	};

	class cWindowModel : public cWindowObject
	{
	public:
		cRSMModel* model;
		string data;
		float roty;
		long oldy;
		cWindowModel(cWindow* parent) : cWindowObject(parent)
		{
			type = OBJECT_MODEL;
			model = NULL;
			roty=0;
			oldy = -1;
		}
		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
		{
			int i;
			if(cutoffbottom < 0)
				cutoffbottom = 0;
			glEnable(GL_DEPTH_TEST);
			if(data != "" && model == NULL)
			{
				glPushMatrix();
				glLoadIdentity();
				model = new cRSMModel();
				model->load(data);
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


				cWindowScrollPanel* scroll = (cWindowScrollPanel*)parent->objects["scroll"];

				for(i = 0; i < model->textures.size(); i++)
				{
					cWindowObject* o = new cWindowModelTexture(parent,i);
					o->SetText(0, model->textures[i]->getfilename());
					o->moveto(0, 130*i);
					scroll->objects.push_back(o);
					scroll->innerheight = 130*i+130;
				}
				scroll->scrollposy = 0;
				scroll->scrollposx = 0;
				scroll->innerwidth = 128;



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
				model->draw(false);
				model->rot.y+=40*(Graphics.frameticks / 1000.0f);
				model->rot.x = roty;
			}

			glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			GLfloat colors[4];
			glGetFloatv(GL_CURRENT_COLOR, colors);
			glDisable(GL_DEPTH_TEST);

		}
		void drag()
		{
			if(oldy != -1)
				roty += (oldy-mousey) / 2.0f;
			oldy = mousey;
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

		void scrollup()
		{
			model->scale = model->scale * 1.1f;
		}
		void scrolldown()
		{
			model->scale = model->scale / 1.1f;
		}
	};



	class cWindowModelTexture : public cWindowPictureBox
	{
	public:
		int i;
		cWindowModelTexture(cWindow* parent, int ii) : cWindowPictureBox(parent)
		{
			resizeto(128,128);
			alignment = ALIGN_TOPLEFT;
			i = ii;
		}

		void click()
		{
			((cRSMEditWindow*)parent)->selected = i;
			SDL_Event ev;
			ev.type = SDL_KEYDOWN;
			ev.key.keysym.sym = SDLK_t;
			SDL_PushEvent(&ev);
			ev.type = SDL_KEYUP;
			ev.key.keysym.sym = SDLK_t;
			SDL_PushEvent(&ev);
		}

	};


public:

	char filename[255];
	int selected;


	cRSMEditWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		strcpy(filename, string(rodir + "data\\model\\프론테라\\분수대.rsm").c_str());
		wtype = WT_RSMEDIT;
		resizable = true;
		visible = true;

		h = 400;
		w = 450;
		title = "RSM Editor";
		center();

		defaultobject = "OkButton";

		objects["rollup"] = new cWindowRollupButton(this);
		objects["close"] = new cWindowCloseButton(this);

		objects["OpenButton"] = new cWindowOpenButton(this);
		objects["SaveButton"] = new cWindowSaveButton(this);
		objects["SaveAsButton"] = new cWindowSaveAsButton(this);

		cWindowObject* o = new cWindowScrollPanel(this);
		o->moveto(w-138, 50);
		o->resizeto(140, h-60);
		o->alignment = ALIGN_TOPLEFT;
		((cWindowScrollPanel*)o)->innerheight = 1000;
		((cWindowScrollPanel*)o)->innerwidth = 128;

		objects["scroll"] = o;


		open();
	}	

	void resizeto(int ww, int hh)
	{
		cWindow::resizeto(ww,hh);
		objects["model"]->resizeto(w-150, h-60);
		objects["scroll"]->moveto(w-138, 50);
		objects["scroll"]->resizeto(140, h-60);
	}


	void open()
	{
		int i;
		if (objects.find("model") != objects.end())
		{
			delete objects["model"];
			objects.erase(objects.find("model"));
		}

		cWindowModel* o = new cWindowModel(this);
		o->SetText(1, filename);
		o->moveto(10,50);
		o->resizeto(w-140, h-60);
		o->alignment = ALIGN_TOPLEFT;

		objects["model"] = o;

		cWindowScrollPanel* scroll = (cWindowScrollPanel*)objects["scroll"];

		for(i = 0; i < scroll->objects.size(); i++)
			delete scroll->objects[i];
		scroll->objects.clear();

	}

	void stopdrag()
	{
		((cWindowModel*)objects["model"])->oldy = -1;

	}

	void changetexture(string newtexture)
	{
		cWindowModel* model = ((cWindowModel*)objects["model"]);

		Log(3,0,"Old tid: %i", model->model->textures[selected]->texid());
		TextureCache.unload(model->model->textures[selected]);
		model->model->textures[selected] = TextureCache.load(rodir + newtexture);
		Log(3,0,"new vtid: %i", model->model->textures[selected]->texid());

		cWindowScrollPanel* scroll = (cWindowScrollPanel*)objects["scroll"];
		delete scroll->objects[selected];
		cWindowObject* o = new cWindowModelTexture(this,selected);
		o->SetText(0, rodir + newtexture);
		o->moveto(0, 130*selected);
		scroll->objects[selected] = o;



	}
};

#endif
