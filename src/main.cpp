#include <common.h>

#include <SDL/SDL_mixer.h>
#include <fstream>
#include <list>
#include <algorithm>
#include <utility>
#include <math.h>
#include <time.h>
#include <tinyxml/tinyxml.h>


#include "font.h"
#include "filesystem.h"
#include "menu.h"
#include "undo.h"
#include "menucommands.h"
#include "windows/lightwindow.h"
#include "windows/texturewindow.h"
#include "windows/modelswindow.h"
#include "windows/modeloverviewwindow.h"
#include "windows/lightoverviewwindow.h"
#include "windows/soundoverviewwindow.h"
#include "windows/texturetoolswindow.h"
#include "windows/minimapwindow.h"
#include "plugins/base/base.h"
#include "interfaceimplementation.h"
#include "bmutex.h"
#include "settings.h"
#include "texturecache.h"
#include "clipboard.h"

#ifdef WIN32
#include <winsock.h>
#include <windows.h>
#endif


#ifdef __MINGW32__
#include <GL/glext.h>											// We use a define from this file: GL_BGRA_EXT
#endif

cBMutex* renderMutex;

void MakeUndo();
void Undo();
int cWM::movement;

void ChangeGrid();
void UpdateTriangleMenu();
void CleanSurfaces();
int process_events();
long tilex,tiley;
long lastmotion;
bool	doneAction = true;
TiXmlDocument favoritelights;


unsigned long keys[SDLK_LAST-SDLK_FIRST];
std::vector<std::pair<std::string, std::string> > translations;

bool mouseouttexture(cMenu*);
bool mouseovertexture(cMenu*);
std::vector<cPluginBase*> plugins;


cMenu* currentobject;
cMenu* snaptofloor;
cMenu* lastmenu = NULL;
cMenu* openMaps;

int cursorsize = 1;

cMenu* mode;
cMenu* editdetail;
cMenu* speed;
cMenu* models;

std::map<int, cMenu*, std::less<int> >	effects;
cMenu* effectsmenu;

long lasttimer;


cBrowInterfaceImplementation browInterface;

unsigned char * getPixelsBGR()
{
  int screenStats[4];
  glGetIntegerv(GL_VIEWPORT, screenStats);
  unsigned char *pixels;
  pixels = new unsigned char[screenStats[2]*screenStats[3]*3];
//  glReadBuffer( GL_BACK );
  glReadPixels(0, 0, screenStats[2], screenStats[3], GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
  return pixels;
}


void mainloop()
{
	renderMutex->lock();
	if(lasttimer + cSettings::paintSpeed < SDL_GetTicks())
	{
		if(cSettings::editMode == MODE_HEIGHTDETAIL && cGraphics::menu->inWindow((int)cGraphics::cMouse::x, cGraphics::h()-(int)cGraphics::cMouse::y) == NULL)
		{
			if (cGraphics::cMouse::lbuttondown || cGraphics::cMouse::rbuttondown)
			{
				int posx = tilex;
				int posy = tiley;
				bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
				float mmin = 99999999.0f;
				float mmax = -9999999.0f;
				if (ctrl)
				{
					for(int x = posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x++)
					{
						for(int y = posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y++)
						{
							if(x >= 0 && y >= 0 && x < cGraphics::world->width && y < cGraphics::world->height)
							{
								cCube* c = &cGraphics::world->cubes[y][x];
								mmin = min(min(min(min(mmin, c->cell1),c->cell2),c->cell3),c->cell4);
								mmax = max(max(max(max(mmax, c->cell1),c->cell2),c->cell3),c->cell4);
							}
						}
					}
				}

				for(int x = posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x++)
				{
					for(int y = posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y++)
					{
						if(x >= 0 && y >= 0 && x < cGraphics::world->width && y < cGraphics::world->height)
						{
							cCube* c = &cGraphics::world->cubes[y][x];
							if(cGraphics::cMouse::lbuttondown && !cGraphics::cMouse::rbuttondown)
							{
								if (!cGraphics::slope || (x > posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)) && y > posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f))
									c->cell1-=1;
								if (!cGraphics::slope || (x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1) && y > posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f))
									c->cell2-=1;
								if (!cGraphics::slope || (x > posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)) && y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1)
									c->cell3-=1;
								if (!cGraphics::slope || (x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1) && y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1)
									c->cell4-=1;
								if(ctrl)
								{
									c->cell1 = max(mmin,c->cell1);
									c->cell2 = max(mmin,c->cell2);
									c->cell3 = max(mmin,c->cell3);
									c->cell4 = max(mmin,c->cell4);
								}
							}
							if(cGraphics::cMouse::lbuttondown && cGraphics::cMouse::rbuttondown)
							{
								if (!cGraphics::slope || (x > posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)) && y > posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f))
									c->cell1+=1;
								if (!cGraphics::slope || (x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1) && y > posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f))
									c->cell2+=1;
								if (!cGraphics::slope || (x > posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)) && y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1)
									c->cell3+=1;
								if (!cGraphics::slope || (x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1) && y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1)
									c->cell4+=1;
								if(ctrl)
								{
									c->cell1 = min(mmax,c->cell1);
									c->cell2 = min(mmax,c->cell2);
									c->cell3 = min(mmax,c->cell3);
									c->cell4 = min(mmax,c->cell4);
								}
							}
						}
					}
				}
				cGraphics::world->calcVertexNormals(posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f), posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f), posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f), posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f));
				lasttimer = SDL_GetTicks();
			}
		}
	}

	if(cGraphics::cMouse::rbuttondown && (keys[SDLK_w] || keys[SDLK_s] || keys[SDLK_a] || keys[SDLK_d]))
	{
		cVector2 v = cVector2((keys[SDLK_a] ? 1 : 0) - (keys[SDLK_d] ? 1 : 0), (keys[SDLK_w] ? 1 : 0) - (keys[SDLK_s] ? 1 : 0));
		v = v * (cGraphics::getFrameTicks()/5.0f);
		v.rotate(-cGraphics::worldContainer->camera.rot / PI * 180.0f);
		cGraphics::worldContainer->camera.pointer = cGraphics::worldContainer->camera.pointer + v;
	}
	
	

	process_events( );

	unsigned long currenttime = SDL_GetTicks();
	for(int i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
	{
		if(keys[i] != 0)
		{
			if (keys[i] < currenttime)
			{
				keys[i] = currenttime + 50;
				SDL_Event ev;
				ev.type = SDL_KEYDOWN;
				ev.key.keysym.sym = (SDLKey)(i + SDLK_FIRST);
				SDL_PushEvent(&ev);
			}
		}
	}

	static eMode lasteditmode = cSettings::editMode;
	
	if(cSettings::editMode != lasteditmode)
	{
		lasteditmode = cSettings::editMode;
		cWindow* w = cWM::getWindow(WT_MODELOVERVIEW);
		if(cSettings::editMode == MODE_OBJECTS)
		{
			if(w == NULL)
				cWM::addWindow(new cModelOverViewWindow());
			else
			{
				w->userfunc(NULL);
				w->show();
			}
		}
		else if(w != NULL)
			w->close();


		w = cWM::getWindow(WT_LIGHTOVERVIEW);
		if (cSettings::editMode == MODE_LIGHTS)
		{
			if(w == NULL)
				cWM::addWindow(new cLightOverViewWindow());
			else
			{
				w->userfunc(NULL);
				w->show();
			}
		}
		else if(w != NULL)
			w->close();

		w = cWM::getWindow(WT_SOUNDOVERVIEW);
		if (cSettings::editMode == MODE_SOUNDS)
		{
			if(w == NULL)
				cWM::addWindow(new cSoundOverViewWindow());
			else
			{
				w->userfunc(NULL);
				w->show();
			}
		}
		else if(w != NULL)
			w->close();

		w = cWM::getWindow(WT_TEXTURETOOLS);
		if (cSettings::editMode == MODE_TEXTUREPAINT)
		{
			if(w == NULL)
				cWM::addWindow(new cTextureToolsWindow());
			else
			{
				w->show();
			}
		}
		else if(w != NULL)
			w->close();
		


	}

	if (!cGraphics::draw())
		cSettings::running = false;
	SDL_GL_SwapBuffers();
	renderMutex->unlock();
	Sleep(1);
}





void additem(std::map<std::string, cMenu*, std::less<std::string> > &itemsm, std::map<cMenu*, int, std::less<cMenu*> > &levelm, std::string cat)
{
	cMenu* root = models;
	std::string catname = cat;
	if(cat.find("/") != std::string::npos)
	{
		if(itemsm.find(cat.substr(0, cat.rfind("/"))) == itemsm.end())
			additem(itemsm, levelm, cat.substr(0, cat.rfind("/")));
		root = itemsm[cat.substr(0, cat.rfind("/"))];
		catname = cat.substr(cat.rfind("/")+1);
	}
	
	cMenu* submenu;
	ADDMENU(submenu,		root, catname + "...",				450 + 100*(levelm[root]+1),100);
	itemsm[cat] = submenu;
	levelm[submenu] = levelm[root] + 1;
}



bool translationcomp(std::pair<std::string, std::string> a, std::pair<std::string, std::string> b)
{
	return a.first.length() > b.first.length();
}

#ifdef __MINGW32__
// mingw32 complains about not finding WinMain, so let's just give him what he want!
// Code adapted and modified from Allegro
int main(int argc, char *argv[]);

//int WinMain(void *hInst, void *hPrev, char *Cmd, int nShow)
int WinMain(HINSTANCE hInst,HINSTANCE hPrev, LPSTR Cmd,int nShow) 
{
	char *argbuf;
	char *cmdline;
	char **argv;
	int argc;
	int argc_max;
	int i, q;

	/* can't use parameter because it doesn't include the executable name */
	cmdline = GetCommandLine();
	i = strlen(cmdline) + 1;
	argbuf = (char*)malloc(i);
	memcpy(argbuf, cmdline, i);

	argc = 0;
	argc_max = 64; // mess with me at your own risk
	argv = (char**)malloc(sizeof(char *) * argc_max);
	if (!argv) {
		free(argbuf);
		return 1; // OUT OF MEMORY
	}

	i=0;

	/* parse commandline into argc/argv format */
	while (argbuf[i]) {
		while ((argbuf[i]) && (argbuf[i]==' '))
			i++;
		if (argbuf[i]) {
			if ((argbuf[i] == '\'') || (argbuf[i] == '"')) {
				q = argbuf[i++];
				if (!argbuf[i]) break;
			} else {
				q = 0;
			}
			argv[argc++] = &argbuf[i];
			if (argc >= argc_max) {
				argc_max += 64;
				argv = (char**)realloc(argv, sizeof(char *) * argc_max);
				if (!argv) {
					free(argbuf);
					return 1; // OUT OF MEMORY
				}
			}

			while ((argbuf[i]) && ((q) ? (argbuf[i] != q) : (argbuf[i]!= ' ')))
				i++;

			if (argbuf[i]) {
				argbuf[i] = 0;
				i++;
			}
		}
	}

	argv[argc] = NULL;

	/* call the application entry point */
	i = main(argc, argv);

	free(argv);
	free(argbuf);

	return i;
}
#endif

cWindow* XmlWindow(std::string s)
{
	return cWM::xmlWindow(s);
}


int main(int argc, char *argv[])
{
#if 0
	char* debugtest;
	if(debugtest == (char*)0xcccccccc)
		return 0;
#endif


	int i;
	log_open("log_worldeditor.txt","worldedit",2);
	cFile* pFile = cFileSystem::open("config.txt");
	if (pFile == NULL)
	{
		Log(2,0,"Error opening configfile, trying one directory up");
		chdir("..");
		pFile = cFileSystem::open("config.txt");
		if(pFile == NULL)
		{
			Log(1,0,"Could not find configfile one directory up, stopping");
			return 0;
		}
		log_close();
		log_open("log_worldeditor.txt","worldedit",2);
	}
	cSettings::configFileName = pFile->readLine();
	pFile->close();

	cSettings::config = cFileSystem::getXml(cSettings::configFileName);
	if(cSettings::config.Error())
	{
		Log(1,0,"Could not load config xml: %s at %i:%i", cSettings::config.ErrorDesc(), cSettings::config.ErrorCol(), cSettings::config.ErrorRow());
		Log(2,0,"Browedit will most likely crash");

	}
	std::string language = cSettings::config.FirstChildElement("config")->FirstChildElement("language")->FirstChild()->Value();
	language = language.substr(language.find("=")+1);
	cSettings::msgTable = cFileSystem::getXml("data/" + language + ".txt");

	cMenu* mm;


	models = new cMenu();
//	models->parent = NULL;
//	models->title = cSettings::msgTable[MENU_MODELS]; 
	models->item = false; 
	models->drawStyle = 1; 
	models->y = 20; 
	models->x = 0; 
	models->w = 50; 
	

	std::map<std::string, cMenu*, std::less<std::string> > itemsm;
	std::map<cMenu*, int, std::less<cMenu*> > levelm;
	levelm[models] = 0;
	

	TiXmlElement* el = cSettings::config.FirstChildElement("config")->FirstChildElement();


	int windowWidth = 1024,windowHeight = 768, windowBpp = 32;
	bool windowFullscreen = true;

	while(el != NULL)
	{
		std::string option = el->Value();

		if(option == "ro")
		{
			cSettings::roDir = el->Attribute("directory");
			TiXmlElement* el2 = el->FirstChildElement("grf");
			while(el2 != NULL)
			{
				cFileSystem::loadPackedFile(el2->FirstChild()->Value());
				el2 = el2->NextSiblingElement("grf");
			}

		}
		if(option == "graphics")
		{
			TiXmlElement* el2 = el->FirstChildElement();
			while(el2 != NULL)
			{
					 if(strcmp(el2->Value(),					"resx") == 0)
					windowWidth = atoi(el2->FirstChild()->Value());
				else if(strcmp(el2->Value(),					"resy") == 0)
					windowHeight = atoi(el2->FirstChild()->Value());
				else if(strcmp(el2->Value(),					"fullscreen") == 0)
					windowFullscreen = strcmp(el2->FirstChild()->Value(),"true") == 0;
				else if(strcmp(el2->Value(),					"bpp") == 0)
					windowBpp = atoi(el2->FirstChild()->Value());
				else if(strcmp(el2->Value(),					"font") == 0)
					cSettings::fontName = el2->FirstChild()->Value();
				else if(strcmp(el2->Value(),					"skin") == 0)
					cSettings::skinFile = el2->FirstChild()->Value();
				else if(strcmp(el2->Value(),					"bgcolor") == 0)
					cGraphics::backgroundColor = hex2floats(el2->FirstChild()->Value());
				else if(strcmp(el2->Value(),					"notilecolor") == 0)
					cGraphics::noTileColor = hex2floats(el2->FirstChild()->Value());
				else if(strcmp(el2->Value(),					"gattransparency") == 0)
					cGraphics::gatTransparency = atof(el2->FirstChild()->Value());
				else if(strcmp(el2->Value(),					"camerasmoothing") == 0)
					cSettings::cameraSmoothing = atof(el2->FirstChild()->Value());

				el2 = el2->NextSiblingElement();

			}
		}
		if(option == "gattiles")
		{
			TiXmlElement* el2 = el->FirstChildElement("tile");
			while(el2 != NULL)
			{
				cGraphics::gatTiles.push_back(atoi(el2->FirstChild()->Value()));
				el2 = el2->NextSiblingElement("tile");
			}
		}
		if(option == "files")
		{
			TiXmlElement* el2;
			
			el2 = el->FirstChildElement("models");
			if(el2)
			{
				TiXmlElement* model = el2->FirstChildElement("model");
				while(model != NULL)
				{
					std::string value = model->FirstChild()->Value();
					cSettings::objectFiles.push_back(value);
					cFile* pFile2 = cFileSystem::open(value);
					if (pFile2 != NULL)
					{
						Log(3,0,GetMsg("file/LOADING"), value.c_str()); // Loading file
						while(!pFile2->eof())
						{
							std::string line = pFile2->readLine();
							std::string pre = line.substr(0, line.find("|"));
							std::string filename = line.substr(line.find("|")+1);

							std::string cat = pre.substr(0, pre.rfind("/"));
							std::string menuname = pre.substr(pre.rfind("/")+1);

							if (cat != "" && itemsm.find(cat) == itemsm.end())
							{
								additem(itemsm, levelm, cat);
							}
							if(filename != "")
							{
								ADDMENUITEMDATA2(mm,itemsm[cat],menuname, &MenuCommand_model, filename, pre);
							}
							
						}
						Log(3,0,GetMsg("file/DONELOADING"), value.c_str()); // Done Loading file
						pFile2->close();
					}
					else
						Log(1,0,GetMsg("file/COULDNOTOPEN"), value.c_str()); // could not open %s

					model = model->NextSiblingElement("model");
				}
			}
			el2 = el->FirstChildElement("textures");
			if(el2)
			{
				TiXmlElement* texture = el2->FirstChildElement("texture");
				while(texture != NULL)
				{
					cSettings::textureFiles.push_back(texture->FirstChild()->Value());
					texture = texture->NextSiblingElement("texture");
				}
			}
			el2 = el->FirstChildElement("sounds");
			if(el2)
			{
				TiXmlElement* sound = el2->FirstChildElement("sound");
				while(sound != NULL)
				{
					cSettings::soundFiles.push_back(sound->FirstChild()->Value());
					sound = sound->NextSiblingElement("sound");
				}
			}
		}
		else if (option == "undo")
			cSettings::undoSize = atoi(el->Attribute("size"));
		el = el->NextSiblingElement();
	}			


	pFile = cFileSystem::open("data/korean2english.txt");
	while(!pFile->eof())
	{
		std::string a = pFile->readLine();
		std::string b = pFile->readLine();
		translations.push_back(std::pair<std::string, std::string>(a,b));
	}
//	mergesort<std::pair<std::string, std::string> >(translations, translationcomp);
	std::sort(translations.begin(), translations.end(), translationcomp);
	pFile->close();


	itemsm.clear();
	levelm.clear();

	models->sort();
	
	favoritelights = cFileSystem::getXml("data/lights.txt");

	if (!cGraphics::init(windowWidth, windowHeight, windowBpp, windowFullscreen))
		return 1;
#ifdef WIN32
	if(GetSystemMetrics(80) > 1)
		SetWindowPos(GetConsoleHwnd(), GetConsoleHwnd(), GetSystemMetrics(SM_CXSCREEN),0,0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
#endif

	cMenu* file;
	cMenu* rnd;
	cMenu* view;
	cMenu* edit;
	cMenu* windows;

	cMenu::initSkin(cWM::skin);
	cGraphics::menu = new cMenu();
	cGraphics::menu->title = "root";
	cGraphics::menu->item = false;
	cGraphics::menu->drawStyle = 0;
	cGraphics::menu->opened = true;
	cGraphics::menu->x = 0;
	cGraphics::menu->y = 0;
	cGraphics::menu->w = cGraphics::w();
	
	int posx = 0;
	ADDMENU2(file,		cGraphics::menu, GetMsg("menu/file/TITLE"),		posx); // File
	ADDMENU2(rnd,		cGraphics::menu, GetMsg("menu/generate/TITLE"),	posx); // Generate
	ADDMENU2(view,		cGraphics::menu, GetMsg("menu/view/TITLE"),		posx); // view
	ADDMENU2(mode,		cGraphics::menu, GetMsg("menu/editmode/TITLE"),	posx); // edit mode
	ADDMENU2(edit,		cGraphics::menu, GetMsg("menu/edit/TITLE"),		posx); // edit
	//ADDMENU2(models,		menu, cSettings::msgTable[MENU_MODELS],	posx); // models
//	models->parent = menu;
//	menu->items.push_back(models);
//	models->x = posx;
//	models->w = cGraphics::font->textlen(models->title)+10;
//	posx+=models->w;
	ADDMENU2(effectsmenu,cGraphics::menu, GetMsg("menu/effects/TITLE"),	posx); // effects
	ADDMENU2(windows,	cGraphics::menu, GetMsg("menu/windows/TITLE"),	posx); // windows
	ADDMENU2(openMaps,	cGraphics::menu,	"Open Maps", posx); // open maps

	ADDMENUITEM(mm,file,GetMsg("menu/file/NEW"),							&MenuCommand_new); //new
	ADDMENUITEM(mm,file,GetMsg("menu/file/OPEN"),							&MenuCommand_open); //open
	ADDMENUITEM(mm,file,GetMsg("menu/file/OPENGRF"),						&MenuCommand_opengrf); //open
	ADDMENUITEM(mm,file,GetMsg("menu/file/CLOSE"),							&MenuCommand_close); //close
	ADDMENUITEM(mm,file,GetMsg("menu/file/SAVE"),							&MenuCommand_save); //save
	ADDMENUITEM(mm,file,GetMsg("menu/file/QUICKSAVE"),						&MenuCommand_quicksave); //save
	ADDMENUITEM(mm,file,GetMsg("menu/file/SAVEAS"),							&MenuCommand_saveAs); //save as
#ifdef _DEBUG
	ADDMENUITEM(mm,file,GetMsg("menu/file/SAVEONLINE"),						&MenuCommand_saveOnline);
	ADDMENUITEM(mm,file,"Make Minimaps",									&MenuCommand_makeMinimaps);
#endif
	ADDMENUITEM(mm,file,GetMsg("menu/file/EXPORTLIGHTMAPS"),				&MenuCommand_savelightmaps); // export lightmaps
	ADDMENUITEM(mm,file,GetMsg("menu/file/IMPORTLIGHTMAPS"),				&MenuCommand_loadlightmaps); // import lightmaps
	ADDMENUITEM(mm,file,GetMsg("menu/file/EXPORTMAPFILES"),					&MenuCommand_exportmapfiles);
	ADDMENUITEM(mm,file,GetMsg("menu/file/REBUILDTEXTUREFILE"),				&MenuCommand_rebuildtexturefile);
	ADDMENUITEM(mm,file,GetMsg("menu/file/REBUILDMODELFILE"),				&MenuCommand_rebuildmodelfile);
	ADDMENUITEM(mm,file,GetMsg("menu/file/REBUILDSOUNDSFILE"),				&MenuCommand_rebuildsoundsfile);
#ifdef _DEBUG
	ADDMENUITEM(mm,file,GetMsg("menu/file/MAPDATABASE"),					&MenuCommand_mapdatabase); //save as
#endif
	ADDMENUITEM(mm,file,GetMsg("menu/file/EXIT"),							&MenuCommand_exit); // exit
	
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/RANDOM1"),					&MenuCommand_random1); // random1 Hills
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/RANDOM2"),					&MenuCommand_random2); // random2 Valleys 
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/RANDOM4"),					&MenuCommand_random4); // random4 Mountains 
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/RANDOM5"),					&MenuCommand_random5); // random4 Mountains 
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/MAZESTUFF"),					&MenuCommand_tempfunc); // Maze stuff
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/QUADTREE"),					&MenuCommand_quadtree); // Quadtree
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/CALCULATELIGHTMAPS"),			&MenuCommand_dolightmapsall); // Lightmaps
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/CALCULATELIGHTMAPSLOCAL"),	&MenuCommand_dolightmaps); // Selected lightmaps
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/CALCULATELIGHTMAPSLIGHT"),	&MenuCommand_dolightmapslights); // Selected light
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/CALCULATELIGHTMAPSNOSHADOW"),	&MenuCommand_dolightmapsnoshadow); // 
	ADDMENUITEM(mm,rnd, "Smooth Lightmap",	&MenuCommand_smoothlightmaps); // 
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/DUN99"),						&MenuCommand_99dun); // 99 level dungeon
	ADDMENUITEM(mm,rnd, "eAthena Script",									&MenuCommand_eascript);
	ADDMENUITEM(mm,rnd, "NPC stuff",										&MenuCommand_npcscreenies);

	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/GRID"),					&MenuCommand_toggle, (void*)&cGraphics::view.showGrid); //grid
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/OBJECTS"),				&MenuCommand_toggle, (void*)&cGraphics::view.showObjects);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/TRANSPARENTOBJECTS"),		&MenuCommand_toggle, (void*)&cGraphics::view.showObjectsAsTransparent);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/BOUNDINGBOXES"),			&MenuCommand_toggle, (void*)&cGraphics::view.showBoundingBoxes);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/LIGHTMAPS"),				&MenuCommand_toggle, (void*)&cGraphics::view.showLightmaps);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/TILECOLORS"),				&MenuCommand_toggle, (void*)&cGraphics::view.showTileColors);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/SHOWWATER"),				&MenuCommand_toggle, (void*)&cGraphics::view.showWater);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/INVISIBLETILES"),			&MenuCommand_toggle, (void*)&cGraphics::view.showNoTiles);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/WATERANIMATION"),			&MenuCommand_toggle, (void*)&cGraphics::view.showWaterAnimation);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/GATTILES"),				&MenuCommand_toggle, (void*)&cGraphics::view.showGat);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/SHOWDOT"),				&MenuCommand_toggle, (void*)&cGraphics::view.showDot);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/SHOWSPRITES"),			&MenuCommand_toggle, (void*)&cGraphics::view.showSprites);
	ADDMENUITEMDATALINK(mm,view,"Show all light spheres",					&MenuCommand_toggle, (void*)&cGraphics::view.showAllLights);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/TOPCAMERA"),				&MenuCommand_toggle, (void*)&cGraphics::view.topCamera);
	ADDMENUITEMDATALINK(mm,view,GetMsg("menu/view/SIDECAMERA"),				&MenuCommand_toggle, (void*)&cGraphics::view.sideCamera);

	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/TEXTUREEDIT"),				&MenuCommand_mode);
	mm->ticked = true;
	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/GLOBALHEIGHTEDIT"),			&MenuCommand_mode);
	ADDMENU(editdetail,mode,GetMsg("menu/editmode/DETAILTERRAINEDIT"),		400,100);

	ADDMENUITEM(mm, editdetail, "1",										&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "2",										&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "4",										&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "8",										&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "16",										&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "32",										&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "64",										&MenuCommand_mode_detail);

	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/WALLEDIT"),					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/OBJECTEDIT"),					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/GATEDIT"),					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/WATEREDIT"),					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/EFFECTSEDIT"),				&MenuCommand_mode);
	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/SOUNDSEDIT"),					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/LIGHTSEDIT"),					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/OBJECTGROUPEDIT"),			&MenuCommand_mode);
	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/SPRITEEDIT"),					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,GetMsg("menu/editmode/TEXTUREPAINTEDIT"),			&MenuCommand_mode);


	ADDMENU(speed,edit, GetMsg("menu/edit/SPEED"),						480, 100);
	ADDMENUITEM(mm,speed,"5",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"10",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"25",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"50",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"100",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"250",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"500",												&MenuCommand_speed);
	ADDMENUITEMDATAP(mm,edit,GetMsg("menu/edit/SLOPING"),					&MenuCommand_toggle, (void*)&cGraphics::slope);
	ADDMENUITEMDATAP(snaptofloor,edit,GetMsg("menu/edit/SNAPOBJECTS"),		&MenuCommand_toggle, NULL);
	snaptofloor->ticked = true;

	ADDMENUITEM(mm,edit,GetMsg("menu/edit/FILLAREA"),						&MenuCommand_fillarea);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/GATHEIGHT"),						&MenuCommand_gatheight);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/RESETCOLORS"),					&MenuCommand_fixcolors);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/ADDWALLS"),						&MenuCommand_addwalls);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/GATCOLLISION"),					&MenuCommand_gatcollision);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/GATCOLLISION")+std::string("2"),		&MenuCommand_gatcollision2);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/CLEANLIGHTMAPS"),					&MenuCommand_cleanuplightmaps);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/REMOVETEXTURES"),					&MenuCommand_cleantextures);
	ADDMENUITEMDATALINK(mm,edit,GetMsg("menu/edit/CLEARLIGHTMAPSONEDIT"),		&MenuCommand_toggle, (void*)&cGraphics::clearLightmaps);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/SELECTALL"),						&MenuCommand_selectAll);

	ADDMENUITEM(mm,windows,GetMsg("menu/windows/AMBIENTLIGHTING"),			&MenuCommand_ambientlight);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/MODELWINDOW"),				&MenuCommand_modelwindow);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/TEXTURES"),					&MenuCommand_texturewindow);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/PROPERTIES"),				&MenuCommand_properties);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/WATER"),					&MenuCommand_water);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/PREFERENCES"),				&MenuCommand_preferences);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/RSMEDITOR"),				&MenuCommand_rsmedit);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/FAVLIGHTS"),				&MenuCommand_favlights);


//	WIN32_FIND_DATA FileData;													// thingy for searching through a directory
//	HANDLE hSearch;																// thingy for searching through a directory
//load plugins	
	
#ifdef WIN32
	WIN32_FIND_DATA fileData;
	HANDLE hSearch = FindFirstFile("plugins/*.dll", &fileData);						// look for all files
	if (hSearch != INVALID_HANDLE_VALUE)										// if there are results...
	{
		while (true)														// loop through all the files
		{ 
			std::string filename = fileData.cFileName;
			if(filename != "." && filename != "..")
			{
				Log(3,0,"Loading plugin '%s'", fileData.cFileName);
				HMODULE hlib;
				hlib = LoadLibraryEx(("plugins/" + filename).c_str(), NULL,0);
				cPluginBase** (__cdecl* getInstances)(void);
				getInstances = (cPluginBase**(__cdecl*)(void))GetProcAddress(hlib, "getInstances");
				if(!getInstances)
				{
					Log(2,0,"%s is not a valid plugin", fileData.cFileName);
					if (!FindNextFile(hSearch, &fileData))								// find next file in the resultset
					{
						if (GetLastError() == ERROR_NO_MORE_FILES)						// we're finished when there are no more files
							break;
						else 
							break;														// wow, something really weird happened
					}
					continue;
				}
				int instanceCount = *((int*)GetProcAddress(hlib, "getInstanceCount"));

				cPluginBase** pluginInstances = getInstances();

				for(i = 0; i < instanceCount; i++)
				{
					plugins.push_back(pluginInstances[i]);
					pluginInstances[i]->setInterface(&browInterface);

					cMenu* p = cGraphics::menu;
					std::string s = pluginInstances[i]->menu;
					std::string past = "";
					
					while(s.find("/") != std::string::npos)
					{
						cMenu* pp = p->find(GetMsg("menu/" + past + s.substr(0,s.find("/")) + "/TITLE"),false);
						if(!pp)
						{
						//	if(p == cGraphics::menu) //root
							{
								ADDMENU2(pp,p, GetMsg("menu/" + past + s.substr(0,s.find("/")) + "/TITLE"),	posx);
							}
							
						}
						p = pp;
						past += s.substr(0, s.find("/")) + "/";
						s = s.substr(s.find("/")+1);
					}
					if(!p)
						Log(1,0,"Couldn't find parent!");
					ADDMENUITEMDATAP(mm,p,GetMsg("menu/" + std::string(pluginInstances[i]->menu)),	&MenuCommand_plugin, (void*)pluginInstances[i]);
				}
			}

			if (!FindNextFile(hSearch, &fileData))								// find next file in the resultset
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)						// we're finished when there are no more files
					break;
				else 
					break;														// wow, something really weird happened
			}
		}
	}
 	FindClose(hSearch);															// Close the search handle. 
#endif




	cGraphics::cMouse::lastlclick = 0;
	cGraphics::cMouse::lastrclick = 0;

	Log(3,0,GetMsg("file/LOADING"), "data/keymap.txt");
	pFile = cFileSystem::open("data/cSettings::keyMap.txt");
	if(pFile == NULL)
	{
		Log(3,0,"Keymap file not found, writing default");
		std::ofstream pFile2("data/cSettings::keyMap.txt");
		for(i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
		{
			char buf[100];
			sprintf(buf, "%i\n", i);
			pFile2.write(buf, strlen(buf));
			
		}
		pFile2.close();
		pFile = cFileSystem::open("data/keymap.txt");

	}
	for(i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
	{
		if(pFile->eof())
		{
			cSettings::keyMap[i] = i;
		}
		else
			cSettings::keyMap[i] = atoi(pFile->readLine().c_str());
	}

	pFile->close();
	Log(3,0,GetMsg("file/DONELOADING"), "data/keymap.txt");


	float effectCount = cGraphics::h() / 21;
	Log(3,0,GetMsg("file/LOADING"), "data/effects.txt");
	std::vector<cMenu*> effectssubmenu;

	pFile = cFileSystem::open("data/effects.txt");
	i = 0;
	while(pFile && !pFile->eof())
	{
		std::string line = pFile->readLine();
		if(line.find("|") != std::string::npos)
		{
			if (effectssubmenu.size() <= floor(i/effectCount))
			{
				effectssubmenu.resize(effectssubmenu.size()+1);
				char buf[100];
				sprintf(buf, "%i - %i", (int) (floor(i/effectCount)*effectCount), (int)((floor(i/effectCount)+1)*effectCount)-1);
				ADDMENU(effectssubmenu.back(),		effectsmenu, buf,				0,100);
			}

			int id = atoi(line.substr(0,line.find("|")).c_str());
			std::string val = line.substr(line.find("|")+1);

			char buf[255];
			sprintf(buf, "%i. %s", id, val.c_str());

			ADDMENUITEMDATA(mm,effectssubmenu[(int)floor(i/effectCount)],buf, &MenuCommand_effect, line.substr(0,line.find("|")));

			i++;
		
		}
	}

	pFile->close();
	Log(3,0,GetMsg("file/DONELOADING"), "data/effects.txt");



	cGraphics::newWorld();
	Log(3,0,GetMsg("DONEINIT"));
	cGraphics::world->newWorld();
	if(cSettings::config.FirstChildElement("config")->FirstChildElement("firstmap"))
		strcpy(cGraphics::world->fileName, std::string(cSettings::roDir + "data\\" + cSettings::config.FirstChildElement("config")->FirstChildElement("firstmap")->FirstChild()->Value()).c_str());
	else
		strcpy(cGraphics::world->fileName, std::string(cSettings::roDir + "data\\comodo").c_str());

	if(argc > 1)
	{
		strcpy(cGraphics::world->fileName, std::string(cSettings::roDir + "data\\" + argv[1]).c_str());
		cGraphics::world->load();
	}
#ifndef WIN32
//	cGraphics::world->load();
#endif

#ifdef _DEBUG
	if(argc == 1)
		cGraphics::world->load();
//	cGraphics::world->importalpha();
#endif

	if(!cGraphics::world->loaded)
	{
		MenuCommand_close(NULL);
	}

	cGraphics::updateMenu();

	for(i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
		keys[i] = 0;


	Mix_OpenAudio(11025,MIX_DEFAULT_FORMAT,2,1024);
	Mix_AllocateChannels(1);
		
	
	lasttimer = SDL_GetTicks();
	renderMutex = new cBMutex();
	while( cSettings::running )
		mainloop();

	
	// Shutdown
	delete renderMutex;
	Mix_CloseAudio();
	cGraphics::closeAndCleanup();				// Kill The Window
	for(i = 0; i < (int)cGraphics::worlds.size(); i++)
		delete cGraphics::worlds[i];

	cGraphics::worlds.clear();
	cGraphics::worldContainer = NULL;
	cGraphics::world = NULL;

	if(cClipBoard::currentClipBoard)
		delete cClipBoard::currentClipBoard;


	cTextureCache::status();

	log_close();
	return 0;							// Exit The Program
}

/**
 * Main Process Management loop, decides what process handlers to call
 */
int process_events()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			if(cWM::confirmWindow("Are you sure you want to quit?"))
				cSettings::running = false;
			break;
		case SDL_KEYUP:
			keys[event.key.keysym.sym-SDLK_FIRST] = 0;
			if (cWM::onKeyUp(event.key.keysym.sym, (event.key.keysym.mod&KMOD_SHIFT) != 0))
				return 0;
#ifdef _DEBUG
			if(cSettings::keyMap[event.key.keysym.sym] == SDLK_ESCAPE)
				cSettings::running = false;
#endif
			switch (event.key.keysym.sym)
			{
			case SDLK_F4:
				if(SDL_GetModState() & KMOD_ALT)
				{
#ifdef WIN32
					PostQuitMessage(0);
#else
					exit(0);
#endif
				}
				break;

			default:
				break;
			}
			break;
		case SDL_KEYDOWN:
			if(keys[event.key.keysym.sym-SDLK_FIRST] == 0)
				keys[event.key.keysym.sym-SDLK_FIRST] = SDL_GetTicks() + 400;
			if(cWM::onKeyDown(event.key.keysym.sym, (event.key.keysym.mod&KMOD_SHIFT) != 0))
				return 0;
			if (strlen(SDL_GetKeyName(event.key.keysym.sym)) == 1 || event.key.keysym.sym == SDLK_SPACE)
			{
				if (event.key.keysym.unicode > 0 && event.key.keysym.unicode < 128)
					if (cWM::onChar((char)event.key.keysym.unicode, (event.key.keysym.mod&KMOD_SHIFT) != 0))
						return 0;
			}
			break;

		default:
			break;
		}
		
		
		int go = cProcessManagement::main_process_events(event);

		if(!cGraphics::world)
			break;

		if(go == 0)
		{
			switch(cSettings::editMode)
			{
			case MODE_TEXTURE:			cProcessManagement::textureedit_process_events(event);		break;
			case MODE_HEIGHTDETAIL:		cProcessManagement::detailheightedit_process_events(event);	break;
			case MODE_HEIGHTGLOBAL:		cProcessManagement::globalheightedit_process_events(event);	break;
			case MODE_WALLS:			cProcessManagement::walledit_process_events(event);			break;
			case MODE_OBJECTS:			cProcessManagement::objectedit_process_events(event);		break;
			case MODE_GAT:				cProcessManagement::gatedit_process_events(event);			break;
			case MODE_WATER:			cProcessManagement::wateredit_process_events(event);		break;
			case MODE_EFFECTS:			cProcessManagement::effectedit_process_events(event);		break;
			case MODE_SOUNDS:			cProcessManagement::soundedit_process_events(event);		break;
			case MODE_LIGHTS:			cProcessManagement::lightedit_process_events(event);		break;
			case MODE_OBJECTGROUP:		cProcessManagement::objectgroupedit_process_events(event);	break;
			case MODE_SPRITE:			cProcessManagement::spriteedit_process_events(event);		break;
			case MODE_TEXTUREPAINT:		cProcessManagement::texturepaintedit_process_events(event);	break;
			}
		}

		if(event.type == SDL_MOUSEMOTION)
		{
			cGraphics::cMouse::xOld = cGraphics::cMouse::x;
			cGraphics::cMouse::yOld = cGraphics::cMouse::y;
		}
	}
	return 0;
}


cWindow* lastdragoverwindow = NULL;


/**
 * Main mode process handler
 */
int cProcessManagement::main_process_events(SDL_Event &event)
{
	switch(event.type)
	{
	case SDL_MOUSEMOTION:
		{
			if(cGraphics::cMouse::x != event.motion.x || cGraphics::cMouse::y != event.motion.y)
				lastmotion = SDL_GetTicks();

			cGraphics::cMouse::x = event.motion.x;
			cGraphics::cMouse::y = event.motion.y;
			cMenu* m = cGraphics::menu->inWindow((int)cGraphics::cMouse::x, cGraphics::h()-(int)cGraphics::cMouse::y);


			if(cWM::movement > 4)
			{
				tilex = (int)cGraphics::cMouse::x3d / 10;
				tiley = (int)cGraphics::cMouse::z3d / 10;
			}
			if(m != NULL)
				return 1;

			cWM::movement++;

			if (cWM::movement > 0)
			{
				if (cWM::draggingWindow != NULL)
				{
					cWM::draggingWindow->drag();
					cWM::drag(cWM::draggingWindow);
				}
				else
				{
					if (cWM::draggingObject != NULL)
					{
						cWM::draggingObject->drag();

						cWindow* w = cWM::inWindow();
						if (w != NULL)
							w->onHoldDragOver();
						else if (w == NULL && lastdragoverwindow != NULL)
							lastdragoverwindow->onHoldDragOver();
						lastdragoverwindow = w;
					}
				}
			}

			if(cGraphics::menu->inWindow((int)cGraphics::cMouse::xStart, cGraphics::h()-(int)cGraphics::cMouse::yStart) != NULL)
				return 1;

			if(!cGraphics::world)
				return 0;

			if (cGraphics::cMouse::rbuttondown && !cGraphics::cMouse::lbuttondown)
			{
				if(SDL_GetModState() & KMOD_SHIFT)
				{
					if (SDL_GetModState() & KMOD_CTRL)
					{
						cGraphics::worldContainer->camera.angle += (cGraphics::cMouse::yOld - cGraphics::cMouse::y) / 10.0f;
						cGraphics::worldContainer->camera.angle = max(min(cGraphics::worldContainer->camera.angle, (float)20), (float)-10);
						cGraphics::worldContainer->camera.rot += (cGraphics::cMouse::xOld - cGraphics::cMouse::x) / 100.0f;
						while(cGraphics::worldContainer->camera.rot < 0)
							cGraphics::worldContainer->camera.rot+=2*(float)PI;
						while(cGraphics::worldContainer->camera.rot > 2*PI)
							cGraphics::worldContainer->camera.rot-=2*(float)PI;
					}
					else
					{
						if(cGraphics::worldContainer->view.topCamera)
						{
							cGraphics::worldContainer->camera.height += (cGraphics::cMouse::yOld - cGraphics::cMouse::y) / 2.0f;
							cGraphics::worldContainer->camera.height = max(min(cGraphics::worldContainer->camera.height, (float)15000), (float)-5);
							if(cGraphics::worldContainer->camera.height != -5 && cGraphics::worldContainer->camera.height != 15000)
							{
								cGraphics::worldContainer->camera.pointer.x -= (cGraphics::cMouse::yOld - cGraphics::cMouse::y) / 4.0f;
								cGraphics::worldContainer->camera.pointer.y += (cGraphics::cMouse::yOld - cGraphics::cMouse::y) / 4.0f;
							}
						}
						else
						{
							cGraphics::worldContainer->camera.height += (cGraphics::cMouse::yOld - cGraphics::cMouse::y) / 2.0f;
							cGraphics::worldContainer->camera.height = max(min(cGraphics::worldContainer->camera.height, (float)15000), (float)-5);
						}
					
						cGraphics::worldContainer->camera.rot += (cGraphics::cMouse::xOld - cGraphics::cMouse::x) / 100.0f;
						if(cGraphics::worldContainer->view.sideCamera)
						{
							if(cGraphics::worldContainer->camera.rot <  (float)(1.6*PI))
								cGraphics::worldContainer->camera.rot = (float)(1.6*PI);
							if(cGraphics::worldContainer->camera.rot >= (float)(1.8*PI))
								cGraphics::worldContainer->camera.rot = (float)(1.8*PI);
						}						
						while(cGraphics::worldContainer->camera.rot < 0)
							cGraphics::worldContainer->camera.rot+=2*(float)PI;
						while(cGraphics::worldContainer->camera.rot > 2*PI)
							cGraphics::worldContainer->camera.rot-=2*(float)PI;
					}
				}
			/*	else if (SDL_GetModState() & KMOD_CTRL)
				{
					if (!(cGraphics::worldContainer->settings.selectionstart3d == cGraphics::worldContainer->settings.selectionend3d))
					{
					}
				}*/
				else
				{
					if(!cGraphics::worldContainer->view.topCamera)
					{
						cVector2 v = cVector2((cGraphics::cMouse::xOld - cGraphics::cMouse::x),  (cGraphics::cMouse::yOld - cGraphics::cMouse::y));
						v.rotate(-cGraphics::worldContainer->camera.rot / PI * 180.0f);
						cGraphics::worldContainer->camera.pointer = cGraphics::worldContainer->camera.pointer - v;
					}
					else
					{
						cGraphics::worldContainer->camera.pointer.x -= (cGraphics::cMouse::yOld - cGraphics::cMouse::y);
						cGraphics::worldContainer->camera.pointer.y -= (cGraphics::cMouse::xOld - cGraphics::cMouse::x);

					} 
				} 
			}//---------------------- right mouse down end reference
			else if (cGraphics::cMouse::lbuttondown && !cGraphics::cMouse::rbuttondown)
			{
				if(cGraphics::cMouse::xStart > cGraphics::w()-256)
				{
					int offset = cGraphics::w()%32;

					cGraphics::worldContainer->settings.selectionstart.x = floor(cGraphics::cMouse::xStart / 32.0)*32;
					cGraphics::worldContainer->settings.selectionstart.y = floor(cGraphics::cMouse::yStart / 32.0)*32;
					cGraphics::worldContainer->settings.selectionend.x = (int)ceil((cGraphics::cMouse::x-offset) / 32.0)*32;
					cGraphics::worldContainer->settings.selectionend.y = (int)ceil((cGraphics::cMouse::y-offset) / 32.0)*32;

					cGraphics::worldContainer->settings.selectionstart.x += cGraphics::w()%32;
					cGraphics::worldContainer->settings.selectionend.x += cGraphics::w()%32;

					if(cGraphics::worldContainer->settings.selectionstart.x > cGraphics::worldContainer->settings.selectionend.x)
					{
						float f = cGraphics::worldContainer->settings.selectionstart.x;
						cGraphics::worldContainer->settings.selectionstart.x = cGraphics::worldContainer->settings.selectionend.x;
						cGraphics::worldContainer->settings.selectionend.x = f;
					}
					if(cGraphics::worldContainer->settings.selectionstart.y > cGraphics::worldContainer->settings.selectionend.y)
					{
						float f = cGraphics::worldContainer->settings.selectionstart.y;
						cGraphics::worldContainer->settings.selectionstart.y = cGraphics::worldContainer->settings.selectionend.y;
						cGraphics::worldContainer->settings.selectionend.y = f;
					}
					return 1;
				}
			}
			return 0;
		} 
		case SDL_MOUSEBUTTONDOWN:
			{
			cWM::movement = 0;
			cGraphics::cMouse::xStart = cGraphics::cMouse::x = event.motion.x;
			cGraphics::cMouse::yStart = cGraphics::cMouse::y = event.motion.y;

			cGraphics::cMouse::x3dStart = cGraphics::cMouse::x3d;
			cGraphics::cMouse::y3dStart = cGraphics::cMouse::y3d;
			cGraphics::cMouse::z3dStart = cGraphics::cMouse::z3d;

			tilex = (int)cGraphics::cMouse::x3d / 10;
			tiley = (int)cGraphics::cMouse::z3d / 10;

			if(event.button.button == 4)
			{ // scroll up
				cWindow* w = cWM::inWindow();
				if(w != NULL)
					w->onScrollUp();
				else if(cGraphics::worldContainer)
				{
					if(cGraphics::worldContainer->view.topCamera)
					{
						float diff = cGraphics::worldContainer->camera.height;
						cGraphics::worldContainer->camera.height*=1.1f;
						diff -= cGraphics::worldContainer->camera.height;
						cGraphics::worldContainer->camera.height = max(min(cGraphics::worldContainer->camera.height, (float)15000), (float)-5);
						if(cGraphics::worldContainer->camera.height != -5 && cGraphics::worldContainer->camera.height != 15000)
						{
							cGraphics::worldContainer->camera.pointer.x += diff/2.0f;
							cGraphics::worldContainer->camera.pointer.y -= diff/2.0f;
 						}
					}
					else
					{
						cGraphics::worldContainer->camera.height*=1.1f;
						cGraphics::worldContainer->camera.height = max(min(cGraphics::worldContainer->camera.height, (float)15000), (float)-5);
					}
				}
				return 1;
			}

			if(event.button.button == 5)
			{ // scroll down
				cWindow* w = cWM::inWindow();
				if(w != NULL)
					w->onScrollDown();
				else if(cGraphics::worldContainer)
				{
					if(cGraphics::worldContainer->view.topCamera)
					{
						float diff = cGraphics::worldContainer->camera.height;
						cGraphics::worldContainer->camera.height/=1.1f;
						diff -= cGraphics::worldContainer->camera.height;
						cGraphics::worldContainer->camera.height = max(min(cGraphics::worldContainer->camera.height, (float)15000), (float)-5);
						if(cGraphics::worldContainer->camera.height != -5 && cGraphics::worldContainer->camera.height != 15000)
						{
							cGraphics::worldContainer->camera.pointer.x += diff/2.0f;
							cGraphics::worldContainer->camera.pointer.y -= diff/2.0f;
						}
					}
					else
					{
						cGraphics::worldContainer->camera.height/=1.1f;
						cGraphics::worldContainer->camera.height = max(min(cGraphics::worldContainer->camera.height, (float)15000), (float)-5);
					}
				}
				return 1;
			}


			cGraphics::cMouse::doubleClick = false;
			if (SDL_GetTicks() - cGraphics::cMouse::lastlclick < 250)
				cGraphics::cMouse::doubleClick = true;

		
			cMenu* m = cGraphics::menu->inWindow((int)cGraphics::cMouse::x, cGraphics::h()-(int)cGraphics::cMouse::y);
		
			cMenu* pm = NULL;
			if(cGraphics::popupMenu != NULL)
			{
				pm = cGraphics::popupMenu->inWindow((int)cGraphics::cMouse::x, cGraphics::h()-(int)cGraphics::cMouse::y);
				return 1;
			}

			if(cGraphics::cMouse::doubleClick && m == NULL && pm == NULL && event.button.button == SDL_BUTTON_LEFT)
			{
				return 1;
			}
			if (cWM::movement < 3 && !cGraphics::cMouse::doubleClick && m == NULL && pm == NULL && event.button.button == SDL_BUTTON_LEFT)
			{
				cWM::draggingObject = NULL;
				cWM::draggingWindow = NULL;
				if (cWM::inWindow() != NULL)
				{
					cWindow* w = cWM::inWindow();
					if (!w->inObject())
					{ // drag this window
						cGraphics::dragoffsetx = cGraphics::cMouse::x - w->getX();
						cGraphics::dragoffsety = (cGraphics::h()-cGraphics::cMouse::y) - w->py2();
						cWM::click(false);
						cWM::draggingWindow = cWM::inWindow();
						if(cGraphics::cMouse::xStart < cWM::draggingWindow->getX()+cWM::draggingWindow->getWidth() && cGraphics::cMouse::xStart > cWM::draggingWindow->getX()+cWM::draggingWindow->getWidth() - DRAGBORDER)
							cWM::draggingWindow->startresisingxy();
						if((cGraphics::h()-cGraphics::cMouse::yStart) > cWM::draggingWindow->getY() && (cGraphics::h()-cGraphics::cMouse::yStart) < cWM::draggingWindow->getY() + DRAGBORDER)
							cWM::draggingWindow->startresizingyx();
						if(cGraphics::cMouse::xStart > cWM::draggingWindow->getX() && cGraphics::cMouse::xStart < cWM::draggingWindow->getX() + DRAGBORDER)
							cWM::draggingWindow->startresisingx();
						if((cGraphics::h()-cGraphics::cMouse::yStart) < cWM::draggingWindow->getY()+cWM::draggingWindow->getHeight() && (cGraphics::h()-cGraphics::cMouse::yStart) > cWM::draggingWindow->getY()+cWM::draggingWindow->getHeight() - DRAGBORDER)
							cWM::draggingWindow->startresizingy();
						return 1;
					}
					else
					{ // drag this component
						cWM::click(false);
						cWM::draggingObject = w->inObject();
						cGraphics::dragoffsetx = cGraphics::cMouse::x - w->getX() - w->inObject()->realX();
						cGraphics::dragoffsety = (cGraphics::h()-cGraphics::cMouse::y) - w->getY() - w->inObject()->realY();
					}
					return 1;
				}
				else
				{
					cWM::defocus();
				}
			}			
			
			if(event.button.button == SDL_BUTTON_LEFT)
				cGraphics::cMouse::lbuttondown = true;
			else // rbutton
				cGraphics::cMouse::rbuttondown = true;
			if(m != NULL)
				return 1;
			if(pm != NULL)
				return 1;
			return 0;
			}
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == 4 || event.button.button == 5)
				break;
			
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				cMenu* m = cGraphics::menu->inWindow((int)cGraphics::cMouse::x, cGraphics::h()-(int)cGraphics::cMouse::y);
				cMenu* pm = NULL;
				if(cGraphics::popupMenu != NULL)
					pm = cGraphics::popupMenu->inWindow((int)cGraphics::cMouse::x, cGraphics::h()-(int)cGraphics::cMouse::y);
				doneAction = true;
				cGraphics::cMouse::lbuttondown = false;
				cGraphics::cMouse::x = event.motion.x;
				cGraphics::cMouse::y = event.motion.y;
				cWindow* w = cWM::inWindow();
				if (cWM::draggingWindow != NULL && m == NULL)
				{
					cWM::draggingWindow->stopresizing();
				}
				cWM::draggingWindow = NULL;
				if (cWM::movement <= 1 && m == NULL && cGraphics::popupMenu == NULL)
					cWM::click(true);
				if (cWM::draggingObject != NULL && m == NULL)
				{
					if(cWM::inWindow() != NULL)
						cWM::inWindow()->onDragOver();
					if(cWM::draggingObject != NULL)
						cWM::draggingObject->parent->onStopDrag();
					cWM::draggingObject = NULL;
				}

				cGraphics::cMouse::lbuttondown = false;
				if (SDL_GetTicks() - cGraphics::cMouse::lastlclick < 250)
				{
					cGraphics::cMouse::doubleClick = true;
					cGraphics::cMouse::lastlclick = SDL_GetTicks();
					if(m == NULL)
						cWM::onDoubleClick();
				}
				else
					cGraphics::cMouse::lastlclick = SDL_GetTicks();
				cGraphics::menu->unMouseOver();
				if(pm != NULL)
					pm->unMouseOver();
				if(pm == NULL && cGraphics::popupMenu != NULL)
				{
					delete cGraphics::popupMenu;
					cGraphics::popupMenu = NULL;
				}
				if (m == NULL)
				{
					cGraphics::menu->closeMenu();
					cGraphics::menu->opened = true;
				}
				if (m != NULL && m->opened)
				{
					m->click((int)cGraphics::cMouse::x, cGraphics::h()-(int)cGraphics::cMouse::y);
					return 1;
				}
				else if (pm != NULL && pm->opened)
				{
					pm->click((int)cGraphics::cMouse::x, cGraphics::h()-(int)cGraphics::cMouse::y);
					if(!pm->opened)
					{
						delete cGraphics::popupMenu;
						cGraphics::popupMenu = NULL;
					}
					return 1;
				}
				else // no menu
				{
					if(w != NULL)
						return 1;
				}
			}
			else // right button
			{
				cMenu* m = cGraphics::menu->inWindow((int)cGraphics::cMouse::x, cGraphics::h()-(int)cGraphics::cMouse::y);
				cMenu* pm = NULL;
				if(cGraphics::popupMenu != NULL)
					pm = cGraphics::popupMenu->inWindow((int)cGraphics::cMouse::x, cGraphics::h()-(int)cGraphics::cMouse::y);
				cGraphics::menu->unMouseOver();
				if(pm != NULL)
					pm->unMouseOver();
				if (m == NULL)
				{
					cGraphics::menu->closeMenu();
					cGraphics::menu->opened = true;
				}
				if(pm == NULL && cGraphics::popupMenu != NULL)
				{
					delete cGraphics::popupMenu;
					cGraphics::popupMenu = NULL;
					return 1;
				}

				cGraphics::cMouse::rbuttondown = false;
				cGraphics::cMouse::doubleClick = false;
				if (cWM::movement < 2)
				{
					if(cWM::inWindow() != NULL)
					{
						cWM::onRightClick();
						return 1;
					}
				}
				long l = SDL_GetTicks();
				if (l - cGraphics::cMouse::lastrclick < 250)
				{
					cGraphics::cMouse::doubleClick = true;
					cGraphics::cMouse::lastrclick = 0;
				}
				else
					cGraphics::cMouse::lastrclick = SDL_GetTicks();
				if(cGraphics::cMouse::doubleClick && cWM::movement < 3 && cGraphics::worldContainer)
				{
					cGraphics::worldContainer->camera.rot = 0;
				}
				cGraphics::cMouse::lastrclick = SDL_GetTicks();
				cGraphics::cMouse::rbuttondown = false;
				cGraphics::cMouse::x = event.motion.x;
				cGraphics::cMouse::y = event.motion.y;

				if(cWM::movement < 3 && (cSettings::editMode == MODE_OBJECTS || cSettings::editMode == MODE_EFFECTS))
				{
					cGraphics::worldContainer->settings.selectedObject = -1;
					return 1;
				}
			}
			return 0;				
			break;
		case SDL_KEYDOWN:
			{
				bool found = false;
				for(int i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
				{
					if (cSettings::keyMap[i] == event.key.keysym.sym+SDLK_FIRST)
					{
						event.key.keysym.sym = (SDLKey)(i+SDLK_FIRST);
						found = true;
						break;
					}
				}
				if(!found)
					return 1;
			}

			if(!cGraphics::worldContainer)
				break;

			switch (event.key.keysym.sym)
			{
			case SDLK_r:
				cGraphics::world->calcVertexNormals();
				break;
			case SDLK_SPACE:
				if (cGraphics::previewColor > 20)
					cGraphics::previewColor = 20;
				break;
			case SDLK_g:
				cGraphics::view.showGrid = !cGraphics::view.showGrid;
				break;
			case SDLK_l:
				MenuCommand_toggle((cMenuItem*)cGraphics::menu->find("Lightmaps"));
				break;
			case SDLK_w:
				if(cGraphics::cMouse::rbuttondown)
				{
					return 1;
				}
				if(SDL_GetModState() & KMOD_META)
				{
					MenuCommand_toggle((cMenuItem*)cGraphics::menu->find("Water"));
					return 1;
				}
				break;
			case SDLK_o:
			{
				if((event.key.keysym.mod&KMOD_SHIFT) == 0)
					cGraphics::view.showObjects = !cGraphics::view.showObjects;
				else
					cGraphics::view.showObjectsAsTransparent = !cGraphics::view.showObjectsAsTransparent;
				break;
			}
			case SDLK_INSERT:
				{
					cGraphics::quadtreeView++;
					if (cGraphics::quadtreeView > 5)
						cGraphics::quadtreeView = 5;
				}
				break;
			case SDLK_DELETE:
				{
					cGraphics::quadtreeView--;
					if (cGraphics::quadtreeView < -1)
						cGraphics::quadtreeView = -1;
				}
				break;
			case SDLK_F1:
				if((event.key.keysym.mod&KMOD_SHIFT) == 0 && cGraphics::world)
				{
					cSettings::editMode = MODE_TEXTURE;
					if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::world->textures.size())
						cGraphics::worldContainer->settings.texturestart = 0;
				}
				else
				{
					cSettings::editMode = MODE_SPRITE;
				}
				break;
			case SDLK_F2:
				if((event.key.keysym.mod&KMOD_SHIFT) == 0 && cGraphics::world)
				{
					cSettings::editMode = MODE_HEIGHTGLOBAL;
					if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::world->textures.size())
						cGraphics::worldContainer->settings.texturestart = 0;
				}
				else if(cGraphics::world)
				{
					cSettings::editMode = MODE_TEXTUREPAINT;
					if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::world->textures.size())
						cGraphics::worldContainer->settings.texturestart = 0;
				}
				break;
			case SDLK_F3:
				if(!cGraphics::world)
					break;
				cSettings::editMode = MODE_HEIGHTDETAIL;
				if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::world->textures.size())
					cGraphics::worldContainer->settings.texturestart = 0;
				break;
			case SDLK_F4:
				if(!cGraphics::world)
					break;
				cSettings::editMode = MODE_WALLS;
				break;
			case SDLK_F5:
				if(!cGraphics::world)
					break;
				cSettings::editMode = MODE_OBJECTS;
				if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::world->textures.size())
					cGraphics::worldContainer->settings.texturestart = 0;
				break;
			case SDLK_F6:
				if(!cGraphics::world)
					break;
				cSettings::editMode = MODE_GAT;
				if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::gatTiles.size()-1)
					cGraphics::worldContainer->settings.texturestart = 0;
				break;
			case SDLK_F7:
				if(!cGraphics::world)
					break;
				cSettings::editMode = MODE_WATER;
				cGraphics::worldContainer->settings.texturestart = cGraphics::world->water.type;
				break;
			case SDLK_F8:
				if(!cGraphics::world)
					break;
				cSettings::editMode = MODE_EFFECTS;
				cGraphics::worldContainer->settings.selectedObject = -1;
				break;
			case SDLK_F9:
				if(!cGraphics::world)
					break;
				cSettings::editMode = MODE_SOUNDS;
				break;
			case SDLK_F10:
				if(!cGraphics::world)
					break;
				cSettings::editMode = MODE_LIGHTS;
				break;
			case SDLK_F11:
				if(!cGraphics::world)
					break;
				cSettings::editMode = MODE_OBJECTGROUP;
				break;
			case SDLK_1:
			case SDLK_2:
			case SDLK_3:
			case SDLK_4:
			case SDLK_5:
			case SDLK_6:
			case SDLK_7:
			case SDLK_8:
			case SDLK_9:
				if(SDL_GetModState() & KMOD_SHIFT)
					cGraphics::worldContainer->settings.gridoffsetx = (event.key.keysym.sym - SDLK_0) / 10.0f;
				else if(SDL_GetModState() & KMOD_CTRL)
					cGraphics::worldContainer->settings.gridoffsety = (event.key.keysym.sym - SDLK_0) / 10.0f;
				else
					cGraphics::worldContainer->settings.gridSize = (event.key.keysym.sym - SDLK_0) / 4.0f;
				break;
			case SDLK_0:
				if(SDL_GetModState() & KMOD_SHIFT)
					cGraphics::worldContainer->settings.gridoffsetx = 0;
				else if(SDL_GetModState() & KMOD_CTRL)
					cGraphics::worldContainer->settings.gridoffsety = 0;
				else
					cGraphics::worldContainer->settings.gridSize = 16 / 4.0f;
				break;
			case SDLK_u:
				if(cGraphics::worldContainer)
					cGraphics::worldContainer->undoStack->undo();
				break;
			case SDLK_t:
				{
					cWindow* w = cWM::getWindow(WT_TEXTURE);
					if (w == NULL)
						cWM::addWindow(new cTextureWindow());
					else
						cWM::toggleWindow(WT_TEXTURE);
					break;
				}
				break;
			case SDLK_m:
				{
					cWindow* w = cWM::getWindow(WT_MODELS);
					if (w == NULL)
						cWM::addWindow(new cModelsWindow());
					else
						cWM::toggleWindow(WT_MODELS);
				}
				break;
			case SDLK_KP0:
				{
					for(int i = 0; i < 360; i+=10)
					{
						SDL_Event ev;
						ev.type = SDL_KEYUP;
						ev.key.keysym.sym = SDLK_PRINT;
						ev.key.keysym.mod = (SDLMod)(KMOD_SHIFT | KMOD_CTRL);
						SDL_PushEvent(&ev);
						process_events();
						cGraphics::worldContainer->camera.rot += 10.0f*((float)PI/180.0f);
					}
					return true;
				}
				break;
#ifdef _DEBUG
			case SDLK_d:
				{
					if(cGraphics::cMouse::rbuttondown)
						return 1;
					if(event.key.keysym.mod & KMOD_CTRL)
						MenuCommand_mapdatabase(NULL);
				}
				break;
#endif
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			if(event.key.keysym.sym == SDLK_TAB && event.key.keysym.mod& KMOD_CTRL)
			{
				cWindow* w = cWM::getWindow(WT_MINIMAP);
				if (w == NULL)
					cWM::addWindow(new cMiniMapWindow()	);
				else
					cWM::toggleWindow(WT_MINIMAP);
			}
			else if(event.key.keysym.sym == SDLK_TAB)
			{
				if((event.key.keysym.mod & ~KMOD_NUM) == 0 && cGraphics::worlds.size() > 1)
					MenuCommand_switchMap((cMenuItem*)openMaps->items[1]);

			}

			if(event.key.keysym.sym == SDLK_PRINT || event.key.keysym.sym == SDLK_SYSREQ)
			{
				if((event.key.keysym.mod&KMOD_SHIFT) != 0)
				{
				if (!cGraphics::draw(false))
					cSettings::running = false;
				SDL_GL_SwapBuffers();

				}
#ifdef WIN32
				WIN32_FIND_DATA FileData;													// thingy for searching through a directory
				HANDLE hSearch;																// thingy for searching through a directory

				bool fFinished = false;														// not finished with looking yet....
				int nFiles = 0;
				hSearch = FindFirstFile("ScreenShots/*.*", &FileData);						// look for all files
				if (hSearch != INVALID_HANDLE_VALUE)										// if there are results...
				{
					while (!fFinished)														// loop through all the files
					{ 
						if (std::string(FileData.cFileName) != "." && std::string(FileData.cFileName) != ".." && std::string(FileData.cFileName) != "CVS")	// if this is a real folder, not . or .. or CVS
							nFiles++;
						if (!FindNextFile(hSearch, &FileData))								// find next file in the resultset
						{
							if (GetLastError() == ERROR_NO_MORE_FILES)						// we're finished when there are no more files
								fFinished = true; 
							else 
								return -1;													// wow, something really weird happened
						}
					}
				}
 				FindClose(hSearch);															// Close the search handle. 
#else
				int nFiles = rand();
#endif				
				unsigned char *pixels;
				pixels = getPixelsBGR();

				FILE * shot;
				char buf[100];
				sprintf(buf, "Screenshots/Shot%i.tga", nFiles);
				if((shot=fopen(buf, "wb"))!=NULL)
				{
					int screenStats[4];
					glGetIntegerv(GL_VIEWPORT, screenStats);
					unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};

					int xfrom = 0;
					int yfrom = 0;
					int xto = screenStats[2];
					int yto = screenStats[3];

					if((event.key.keysym.mod&KMOD_CTRL) != 0)
					{
						yfrom = 19;
						xto = cGraphics::w() - 256;
					}

					int w= xto - xfrom;
					int h= yto - yfrom;


					unsigned char Header[6]={((int)(w%256)),((int)(w/256)),((int)(h%256)),((int)(h/256)),24,0};
					fwrite(TGAheader, sizeof(unsigned char), 12, shot);
					fwrite(Header, sizeof(unsigned char), 6, shot);

					for(int y = yfrom; y < yto; y++)
					{
						for(int x = xfrom; x < xto; x++)
						{
							fwrite(pixels+3*(x+y*screenStats[2]), sizeof(unsigned char), 3, shot);
						}
					}
					delete [] pixels;
					fclose(shot);
				}
			}
			break;
		default:
			break;

	}
	return 0;
}
