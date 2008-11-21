#include <common.h>
#ifdef WIN32
#include <commdlg.h>
#include <shellapi.h>
#endif
#include <fstream>
#include "menucommands.h"
#include "graphics.h"
#include <SDL/SDL_syswm.h>
#include "undo.h"
#include "filesystem.h"
#include "windows/waterwindow.h"
#include "windows/ambientlightwindow.h"
#include "windows/keybindwindow.h"
#include "windows/rsmeditwindow.h"
#include "windows/favoritelights.h"
#include "windows/modeloverviewwindow.h"
#include "windows/lightoverviewwindow.h"
#include "windows/progresswindow.h"
#include "windows/filewindow.h"
#include "windows/mapswindow.h"
#include "plugins/base/base.h"
#include "settings.h"

int process_events( );
extern std::vector<std::string> objectfiles;
extern cMenu* mode;
extern cMenu* editdetail;
extern cMenu* speed;
extern cMenu* models;
extern cMenu* currentobject;
extern TiXmlDocument sprites;
//extern double mouseclickx, mouseclicky, mouseclickz;
extern bool boundingBoxCollisions;

void mainloop();
#include <bthread.h>
extern cBMutex* renderMutex;

#include <curl/curl.h>


cMenuItem* selectedeffect = NULL;

MENUCOMMAND(new)
{
	int newWidth = atoi(cWM::inputWindow("Width", "100").c_str());
	if(newWidth == 0)
		return false;
	int newHeight = atoi(cWM::inputWindow("Height", "100").c_str());
	if(newHeight == 0)
		return false;


	if(newHeight < 8 || newWidth < 8)
	{
		cWM::showMessage("Cannot make a map with a height or width lower then 8");
		return false;
	}
	
	cGraphics::newWorld();
	strcpy(cGraphics::world->fileName,(cSettings::roDir + "data\\untitled").c_str());
	cGraphics::world->newEmpty(newWidth, newHeight);
	if(!cGraphics::world->loaded)
		MenuCommand_close(src);
	cGraphics::updateMenu();
	
	return true;
}



MENUCOMMAND(open)
{
#ifdef WIN32
	char curdir[100];
	getcwd(curdir, 100);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version)	;
	SDL_GetWMInfo(&wmInfo);
	HWND hWnd = wmInfo.window;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;

	if(cGraphics::world)
		strcpy(cGraphics::world->fileName, replace(cGraphics::world->fileName, "/", "\\").c_str());
	char buf[256];

	ZeroMemory(buf, 256);
	if(cGraphics::world)
		strcpy(buf, cGraphics::world->fileName);
	else
		strcpy(buf, (cSettings::roDir + "data\\prontera.rsw").c_str());
	ofn.lpstrFile = buf;
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = "All\0*.*\0RO Maps\0*.rsw\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	if (GetOpenFileName(&ofn))
	{

		cGraphics::newWorld();
		strcpy(cGraphics::world->fileName,buf);
		while(cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] != '.')
			cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] = '\0';
		cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] = '\0';
		chdir(curdir);
		cGraphics::world->load();
		if(!cGraphics::world->loaded)
			MenuCommand_close(src);
		cGraphics::updateMenu();
	}
#else


#endif
	return true;
}




void openfunc(std::string param)
{
	cGraphics::newWorld();
	ZeroMemory(cGraphics::world->fileName, 128);
	memcpy(cGraphics::world->fileName, param.c_str(), param.length());
	cGraphics::world->load();
	if(!cGraphics::world->loaded)
		MenuCommand_close(NULL);
	cGraphics::updateMenu();
}

MENUCOMMAND(opengrf)
{
	cWM::addWindow(new cFileWindow(openfunc));
	return true;
}

MENUCOMMAND(close)
{
	cGraphics::world->unload();
	delete cGraphics::worldContainer;
	cGraphics::worlds.erase(cGraphics::worlds.begin());
	
	cGraphics::worldContainer = NULL;
	cGraphics::world = NULL;

	if(cGraphics::worlds.size() > 0)
	{
		cGraphics::worldContainer = cGraphics::worlds[0];
		cGraphics::world = cGraphics::worldContainer->world;
	}

	cGraphics::updateMenu();

	return true;
}


MENUCOMMAND(save)
{
	if(!cGraphics::world)
		return false;
#ifdef WIN32
	if(cGraphics::world->fileName[0] == '\0')
	{
		char curdir[100];
		getcwd(curdir, 100);
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWMInfo(&wmInfo);
		HWND hWnd = wmInfo.window;
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		strcpy(cGraphics::world->fileName, replace(cGraphics::world->fileName, "/", "\\").c_str());
		ofn.lpstrFile = cGraphics::world->fileName;
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
		ofn.nFilterIndex = 2;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
		if (GetSaveFileName(&ofn))
		{
			while(cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] != '.')
				cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] = '\0';
			cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] = '\0';
			cGraphics::world->save();
		}
		chdir(curdir);
	}
	else
		cGraphics::world->save();
#else

#endif

	return true;
}


MENUCOMMAND(quicksave)
{
	if(!cGraphics::world)
		return false;
	cGraphics::world->quickSave = true;
#ifdef WIN32
	if(cGraphics::world->fileName[0] == '\0')
	{
		char curdir[100];
		getcwd(curdir, 100);
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWMInfo(&wmInfo);
		HWND hWnd = wmInfo.window;
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		strcpy(cGraphics::world->fileName, replace(cGraphics::world->fileName, "/", "\\").c_str());
		ofn.lpstrFile = cGraphics::world->fileName;
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
		ofn.nFilterIndex = 2;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
		if (GetSaveFileName(&ofn))
		{
			while(cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] != '.')
				cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] = '\0';
			cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] = '\0';
			cGraphics::world->save();
		}
		chdir(curdir);
	}
	else
		cGraphics::world->save();
#else

#endif

	cGraphics::world->quickSave = false;

	return true;
}

MENUCOMMAND(saveAs)
{
	if(!cGraphics::world)
		return false;
#ifdef WIN32
	char curdir[100];
	getcwd(curdir, 100);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWMInfo(&wmInfo);
	HWND hWnd = wmInfo.window;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	strcpy(cGraphics::world->fileName, replace(cGraphics::world->fileName, "/", "\\").c_str());
	ofn.lpstrFile = cGraphics::world->fileName;
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
	if (!GetSaveFileName(&ofn))
	{
		cGraphics::world->fileName[0] = '\0';
		chdir(curdir);
	}
	else
	{
		if (strcmp(cGraphics::world->fileName+strlen(cGraphics::world->fileName)-4, ".rsw") == 0)
		{ 
			while(cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] != '.')
				cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] = '\0';
			cGraphics::world->fileName[strlen(cGraphics::world->fileName)-1] = '\0';
		}
		chdir(curdir);
		cGraphics::world->save();

	}
#else
	std::string input = cWM::inputWindow("Filename (so not add .rsw): ", cGraphics::world->fileName);
	if(input != "")
	{
		sprintf(cGraphics::world->fileName, "%s", input.c_str());
		cGraphics::world->save();
		
	}

#endif
	return true;
}
MENUCOMMAND(exit)
{
	cSettings::running = false;
	return true;
}	
MENUCOMMAND(undo)
{
	if(!cGraphics::world)
		return false;
	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_u;
	SDL_PushEvent(&ev);
	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_u;
	SDL_PushEvent(&ev);
	return true;
}


int ClassifyPoint(cVector3 point, cVector3 pO, cVector3 pN)
{
	cVector3 TempVect;
	TempVect.x = pO.x - point.x;
	TempVect.y = pO.y - point.y;
	TempVect.z = pO.z - point.z;
	cVector3 dir = TempVect;
	GLfloat d = dir.dot(pN);;
	
	if (d < -0.00001f)
		return 1;
	else
		if (d > 0.00001f)
			return -1;
		return 0;
}


MENUCOMMAND(random1)
{
	if(!cGraphics::world)
		return false;
	int height = atoi(cWM::inputWindow("Height:").c_str());
	int smooth  = atoi(cWM::inputWindow("Smoothing level (use 5-10 for decent results)").c_str());
	if(height == 0)
	{
		cWM::showMessage("You must enter a height bigger then 0");
		return true;
	}

	cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(0,0,cGraphics::world->width, cGraphics::world->height));
	int x,y;
	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cGraphics::world->cubes[y][x].tileOtherSide = -1;
			cGraphics::world->cubes[y][x].tileSide = -1;
		}
	}
	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
		//	cGraphics::world->cubes[2*y][2*x].tileUp = 1;

			cGraphics::world->cubes[y][x].cell1 = height/2-rand()%height;
			cGraphics::world->cubes[y][x].cell2 = height/2-rand()%height;
			cGraphics::world->cubes[y][x].cell3 = height/2-rand()%height;
			cGraphics::world->cubes[y][x].cell4 = height/2-rand()%height;

/*			cGraphics::world->cubes[2*y][2*x-1].cell2 =	cGraphics::world->cubes[2*y][2*x].cell1;
			cGraphics::world->cubes[2*y-1][2*x-1].cell4 =	cGraphics::world->cubes[2*y][2*x].cell1;
			cGraphics::world->cubes[2*y-1][2*x].cell3 =	cGraphics::world->cubes[2*y][2*x].cell1;


			cGraphics::world->cubes[2*y][2*x+1].cell1 =	cGraphics::world->cubes[2*y][2*x].cell2;
			cGraphics::world->cubes[2*y-1][2*x+1].cell3 =	cGraphics::world->cubes[2*y][2*x].cell2;
			cGraphics::world->cubes[2*y-1][2*x].cell4 =	cGraphics::world->cubes[2*y][2*x].cell2;
		

			cGraphics::world->cubes[2*y][2*x-1].cell4 =	cGraphics::world->cubes[2*y][2*x].cell3;
			cGraphics::world->cubes[2*y+1][2*x-1].cell2 =	cGraphics::world->cubes[2*y][2*x].cell3;
			cGraphics::world->cubes[2*y+1][2*x].cell1 =	cGraphics::world->cubes[2*y][2*x].cell3;

			cGraphics::world->cubes[2*y][2*x+1].cell3 =	cGraphics::world->cubes[2*y][2*x].cell4;
			cGraphics::world->cubes[2*y+1][2*x+1].cell1 =	cGraphics::world->cubes[2*y][2*x].cell4;
			cGraphics::world->cubes[2*y+1][2*x].cell2 =	cGraphics::world->cubes[2*y][2*x].cell4;
		*/
		}
	}

	eMode m = cSettings::editMode;
	cSettings::editMode = MODE_HEIGHTDETAIL;

	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_s;
	for(int i = 0; i < smooth; i++)
		SDL_PushEvent(&ev);

	int b = cGraphics::worldContainer->settings.brushSizeDetailHeight;
	cGraphics::cMouse::x3d = cGraphics::world->width*5;
	cGraphics::cMouse::z3d = cGraphics::world->height*5;
	cGraphics::worldContainer->settings.brushSizeDetailHeight = cGraphics::world->width+cGraphics::world->height;
	
	process_events();
	cGraphics::worldContainer->settings.brushSizeDetailHeight = b;


	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_s;
	SDL_PushEvent(&ev);
	cSettings::editMode = m;

	return true;
}

MENUCOMMAND(random2)
{
	if(!cGraphics::world)
		return false;
	unsigned int i;
	unsigned int smooth  = 3;//atoi(cWM::inputWindow("Smoothing level (use 5-10 for decent results)").c_str());

	cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(0,0,cGraphics::world->width, cGraphics::world->height));
	float x,y;

	cGraphics::world->tiles.clear();
	for(int tex = 0; tex < 3; tex++)
	{
		for(y = 0; y < 4; y++)
		{
			for(x = 0; x < 4; x++)
			{
				cTile t;
				t.lightmap = 0;
				t.texture = tex;
				t.u1 = x/4.0;
				t.v1 = y/4.0;
				t.u2 = (x+1)/4.0;
				t.v2 = (y)/4.0;
				t.u3 = (x)/4.0;
				t.v3 = (y+1)/4.0;
				t.u4 = (x+1)/4.0;
				t.v4 = (y+1)/4.0;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				cGraphics::world->tiles.push_back(t);
			}
		}
	}
	cGraphics::draw();
	SDL_GL_SwapBuffers();
	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cGraphics::world->cubes[(int)y][(int)x].tileOtherSide = -1;
			cGraphics::world->cubes[(int)y][(int)x].tileSide = -1;
			cGraphics::world->cubes[(int)y][(int)x].tileUp = 0;
		}
	}
	cGraphics::draw();
	SDL_GL_SwapBuffers();

	if(!cGraphics::world->makeLightmapsUnique())
		return false;
	if(!cGraphics::world->blackLightmaps())
		return false;


	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cGraphics::world->cubes[(int)y][(int)x].cell1 = -64;
			cGraphics::world->cubes[(int)y][(int)x].cell2 = -64;
			cGraphics::world->cubes[(int)y][(int)x].cell3 = -64;
			cGraphics::world->cubes[(int)y][(int)x].cell4 = -64;
		}
	}

	
//	x = 1 + (rand()%((cGraphics::world->width/10)-2));
//	y = 1 + (rand()%((cGraphics::world->height/10)-2));

	x = cGraphics::world->width/2;
	y = cGraphics::world->height/2;

	
	int a = rand()%360;
	int lasta = a;
	int reali = 0;
	bool filledenough = false;
	while(!filledenough) //(cGraphics::world->height+cGraphics::world->width) / 25
	{
		cGraphics::draw();
		SDL_GL_SwapBuffers();
		reali++;
		a += (rand()%180)-90;
		while(a < 0)
			a+=360;
		while(a > 360)
			a-=360;

		lasta = a;

		int c = rand() % 25+25;

		float curve = ((rand() % 100)-50) / 50.0f;
		for(int ii = 0; ii < c; ii++)
		{
			a += (int)curve;
			while(a < 0)
				a+=360;
			while(a > 360)
				a-=360;

			bool water = false;//rand() % 20 == 0;

			for(int xx = 0; xx < 10; xx++)
			{
				for(int yy = 0; yy < 10; yy++)
				{
					cGraphics::world->cubes[(int)y+yy][(int)x+xx].cell1 = water ? 30 : 0;//rand()%25;
					cGraphics::world->cubes[(int)y+yy][(int)x+xx].cell2 = water ? 30 : 0;//rand()%25;
					cGraphics::world->cubes[(int)y+yy][(int)x+xx].cell3 = water ? 30 : 0;//rand()%25;
					cGraphics::world->cubes[(int)y+yy][(int)x+xx].cell4 = water ? 30 : 0;//rand()%25;
				}
			}

			x+=cos(a* (PI/180.0f));
			y+=sin(a* (PI/180.0f));


			if(y < 5)
			{
				y = 5;
				break;
			}
			if(y >= (cGraphics::world->height)-15)
			{
				y = (cGraphics::world->height)-15;
				break;
			}
			if(x < 5)
			{
				x = 5;
				break;
			}
			if(x >= (cGraphics::world->width)-15)
			{
				x = (cGraphics::world->width)-15;
				break;
			}
		}
		

		int count = 0;
		for(int yy = 0; yy < cGraphics::world->height; yy++)
		{
			for(int xx = 0; xx < cGraphics::world->width; xx++)
			{
				if(cGraphics::world->cubes[yy][xx].cell1 == 0)
					count++;
			}
		}
		if(count > cGraphics::world->height*cGraphics::world->width / 2)
			filledenough = true;

	}

	std::vector<std::string>	randommodels;
	for(i = 0; i < objectfiles.size(); i++)
	{
		cFile* pFile = cFileSystem::open(objectfiles[i]);
		while(!pFile->eof())
		{
			std::string line = pFile->readLine();
			if (line == "")
				continue;
			std::string pre = line.substr(0, line.find("|"));
			std::string filename = line.substr(line.find("|")+1);

			std::string cat = pre.substr(0, pre.rfind("/"));
			std::string name = pre.substr(pre.rfind("/")+1);

			if(cat == "Random2 Objects")
				randommodels.push_back(filename);
		}
	}
	for(i = 0; i < cGraphics::world->models.size(); i++)
		delete cGraphics::world->models[i];
	cGraphics::world->models.clear();

	for(i = 0; i < 50; i++)
	{
		cRSMModel* model = new cRSMModel();
		model->load(cSettings::roDir +  randommodels[rand() % randommodels.size()]);

		bool ok = false;
		while(!ok || cGraphics::world->cubes[(int)(model->pos.z/2)][(int)(model->pos.x/2)].cell1 != 0)
		{
			model->pos = cVector3(rand()%(cGraphics::world->width*2), 0, rand()%(cGraphics::world->height*2));
			ok = true;
			for(int x = -4; x < 4; x++)
			{
				for(int y = -4; y < 4; y++)
				{
					if(model->pos.z/2+x < 0 || model->pos.z/2+x >= cGraphics::world->height-1 ||
						model->pos.x/2+y < 0 || model->pos.x/2+y >= cGraphics::world->width-1)
						continue;
					if(cGraphics::world->cubes[(int)(model->pos.z/2+x)][(int)(model->pos.x/2+y)].cell1 != 0)
						ok = false;
				}
			}

		}


		model->pos.y = cGraphics::world->cubes[(int)(model->pos.z/2)][(int)(model->pos.x/2)].cell1;
		model->scale = cVector3(1,1,1);
		model->rot = cVector3(0,rand()%360,0);
		cGraphics::world->models.push_back(model);
	}


	eMode m = cSettings::editMode;
	cSettings::editMode = MODE_HEIGHTDETAIL;

	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_s;
	for(i = 0; i < smooth; i++)
		SDL_PushEvent(&ev);

	int b = cGraphics::worldContainer->settings.brushSizeDetailHeight;
	cGraphics::cMouse::x3d = cGraphics::world->width*5;
	cGraphics::cMouse::z3d = cGraphics::world->height*5;
	cGraphics::worldContainer->settings.brushSizeDetailHeight = cGraphics::world->width+cGraphics::world->height;
	
	process_events();
	cGraphics::draw();
	SDL_GL_SwapBuffers();

	cGraphics::worldContainer->settings.brushSizeDetailHeight = b;


	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_s;
	SDL_PushEvent(&ev);



	cSettings::editMode = m;

	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			if((cGraphics::world->cubes[(int)y][(int)x].cell1 <= -8 || cGraphics::world->cubes[(int)y][(int)x].cell2 <= -8 || cGraphics::world->cubes[(int)y][(int)x].cell3  <= -8|| cGraphics::world->cubes[(int)y][(int)x].cell4 <= -8) && cGraphics::world->cubes[(int)y][(int)x].cell1 > -63)
				cGraphics::world->cubes[(int)y][(int)x].tileUp= 32 + ((int)x%4) + 4*((int)y%4);
			else if(cGraphics::world->cubes[(int)y][(int)x].cell1 >= -63)
				cGraphics::world->cubes[(int)y][(int)x].tileUp= 16 + ((int)x%4) + 4*((int)y%4);
		}
	}

	cGraphics::world->water.height = 12;






	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cCube* c = &cGraphics::world->cubes[(int)y][(int)x];
			cGraphics::world->gattiles[2*(int)y][2*(int)x].type = (c->cell1+c->cell2+c->cell3+c->cell4) < -10 ? '\1' : '\0';
			cGraphics::world->gattiles[2*(int)y][2*(int)x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) < -10 ? '\1' : '\0';
			cGraphics::world->gattiles[2*(int)y+1][2*(int)x].type = (c->cell1+c->cell2+c->cell3+c->cell4) < -10 ? '\1' : '\0';
			cGraphics::world->gattiles[2*(int)y+1][2*(int)x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) < -10 ? '\1' : '\0';
		}

	}

	MenuCommand_gatheight(src);



	for(x = 0; x < cGraphics::world->width; x++)
		for(y = 0; y < cGraphics::world->height; y++)
		{
			cGraphics::world->cubes[(int)y][(int)x].maxHeight = -99999;
			cGraphics::world->cubes[(int)y][(int)x].minHeight = 99999;
		}

	for(i = 0; i < cGraphics::world->models.size(); i++)
		cGraphics::world->models[i]->draw(false,false,true);


	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			if(cGraphics::world->cubes[(int)y][(int)x].maxHeight != -99999)
			{
				cGraphics::world->gattiles[2*(int)y][2*(int)x].type = '\1';
				cGraphics::world->gattiles[2*(int)y][2*(int)x+1].type = '\1';
				cGraphics::world->gattiles[2*(int)y+1][2*(int)x].type = '\1';
				cGraphics::world->gattiles[2*(int)y+1][2*(int)x+1].type = '\1';
			}
		}

	}
	cGraphics::world->root->recalculate();


	Log(3,0,"Made %i iterations", reali);

	return true;
}



class cIntQuad
{
public:
	int x;
	int y;
	int w;
	int h;
	std::vector<int> connections;
	cIntQuad(int xx, int yy, int ww, int hh)
	{
		x = xx;
		y = yy;
		w = ww;
		h = hh;
	}
};

MENUCOMMAND(random3)
{
	if(!cGraphics::world)
		return false;
	unsigned int i;
	int xx,yy;

	cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(0,0,cGraphics::world->width, cGraphics::world->height));
	float x,y;

	cGraphics::world->tiles.clear();
	for(int tex = 0; tex < 5; tex++)
	{
		for(y = 0; y < 4; y++)
		{
			for(x = 0; x < 4; x++)
			{
				cTile t;
				t.lightmap = 0;
				t.texture = tex;
				t.u1 = x/4.0;
				t.v1 = y/4.0;
				t.u2 = (x+1)/4.0;
				t.v2 = (y)/4.0;
				t.u3 = (x)/4.0;
				t.v3 = (y+1)/4.0;
				t.u4 = (x+1)/4.0;
				t.v4 = (y+1)/4.0;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				cGraphics::world->tiles.push_back(t);
			}
		}
	}
	
	
	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cGraphics::world->cubes[(int)y][(int)x].tileOtherSide = -1;
			cGraphics::world->cubes[(int)y][(int)x].tileSide = -1;
			cGraphics::world->cubes[(int)y][(int)x].tileUp = 48 + ((int)x%4) + 4*((int)y%4);
		}
	}


	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cGraphics::world->cubes[(int)y][(int)x].cell1 = 16;
			cGraphics::world->cubes[(int)y][(int)x].cell2 = 16;
			cGraphics::world->cubes[(int)y][(int)x].cell3 = 16;
			cGraphics::world->cubes[(int)y][(int)x].cell4 = 16;
		}
	}

	
	cGraphics::draw();
	SDL_GL_SwapBuffers();

	
	int lasta = 0;
	int reali = 0;

	int w,h;

	
	std::vector<cIntQuad> islands;

	bool filledenough = false;
	unsigned int lastsize = 0;
	while(!filledenough)
	{
		if(lastsize != islands.size())
		{
			cGraphics::draw();
			SDL_GL_SwapBuffers();
			lastsize = islands.size();
		}
		int island = -1;
		reali++;
		if(reali > 10000)
			break;

		w = 10+rand() % 30;
		h = 10+rand() % 30;

		if(islands.size() > 0)
		{
			island = rand() % islands.size();
			if(islands[island].connections.size() > 1)
				island = rand() % islands.size();


			int a = rand()%4;
			if(a == lasta || (a+2)%4 == lasta)
				a = rand()%4;
			lasta = a;

			if(a == 0)
			{
				x = islands[island].x + islands[island].w + 4;
				y = islands[island].y;
			}
			if(a == 1)
			{
				x = islands[island].x;
				y = islands[island].y + islands[island].h + 4;
			}
			if(a == 2)
			{
				x = islands[island].x;
				y = islands[island].y - h - 4;
			}
			if(a == 3)
			{
				x = islands[island].x - w - 4;
				y = islands[island].y;
			}

		}
		else
		{
			x = (cGraphics::world->width-w)/2;
			y = (cGraphics::world->height-h)/2;
		}

		if(!(x + w >= cGraphics::world->width-1 || y+h >= cGraphics::world->height-1 || x <= 1 || y <= 1))
		{
			int takencount = 0;
			for(xx = (int)x; xx < (int)x+w; xx++)
			{
				for(yy = (int)y; yy < (int)y+h; yy++)
				{
					if(cGraphics::world->cubes[yy][xx].cell1 == 0)
						takencount++;
				}
			}
			if(takencount < 3)
			{
				for(xx = (int)x; xx < (int)x+w; xx++)
				{
					for(yy = (int)y; yy < (int)y+h; yy++)
					{
						cGraphics::world->cubes[yy][xx].cell1 = 0;//rand()%25;
						cGraphics::world->cubes[yy][xx].cell2 = 0;//rand()%25;
						cGraphics::world->cubes[yy][xx].cell3 = 0;//rand()%25;
						cGraphics::world->cubes[yy][xx].cell4 = 0;//rand()%25;
						cGraphics::world->cubes[yy][xx].tileUp = 32 + (xx%4) + 4*(yy%4);
					}
				}
				if(island != -1)
					islands[island].connections.push_back(islands.size());

				islands.push_back(cIntQuad((int)x,(int)y,w,h));
				if(island != -1)
					islands[islands.size()-1].connections.push_back(island);
			}
			int count = 0;
			for(int yy = 0; yy < cGraphics::world->height; yy++)
			{
				for(int xx = 0; xx < cGraphics::world->width; xx++)
				{
					if(cGraphics::world->cubes[yy][xx].cell1 == 0)
						count++;
				}
			}
			if(count > cGraphics::world->height*cGraphics::world->width / 2)
				filledenough = true;

		}
	}

	for(i = 0; i < cGraphics::world->models.size(); i++)
		delete cGraphics::world->models[i];
	cGraphics::world->models.clear();


	for(i = 0; i < islands.size(); i++)
	{
		for(unsigned int ii = 0; ii < islands[i].connections.size(); ii++)
		{
			cGraphics::draw();
			SDL_GL_SwapBuffers();
			x = islands[islands[i].connections[ii]].x;
			y = islands[islands[i].connections[ii]].y;
			w = islands[islands[i].connections[ii]].w;
			h = islands[islands[i].connections[ii]].h;

			xx = islands[i].x;
			yy = islands[i].y;

			if(xx - (x+w) == 4)
				x+=w;
			else if(yy - (y+h) == 4)
				y+=h;
			else if(y - (yy+islands[i].h) == 4)
				yy+=islands[i].h;
			else if(x - (xx+islands[i].w) == 4)
				xx+=islands[i].w;


			if(xx == x)
			{
				if(w < islands[i].w)
					x = xx = xx + w/2;
				else
					x = xx = xx + islands[i].w/2;
			}
			if(yy == y)
			{
				if(h < islands[i].h)
					y = yy = yy + h/2;
				else
					y = yy = yy + islands[i].h/2;
			}


			cRSMModel* model = new cRSMModel();
			model->load(cSettings::roDir +  "data\\model\\郴何家前\\枚促府.rsm");
			model->pos.x = (xx+x) + ((xx == x) ? 1 : 0);
			model->pos.z = (yy+y) + ((yy == y) ? 1 : 0);
			model->pos.y = 10;
			model->scale = cVector3((xx == x) ? 0.95f : 1,1,(yy == y) ? 0.95f : 1);
			model->rot = cVector3(0,xx==x ? 0 : 90,0);
			cGraphics::world->models.push_back(model);

		}
	}

	cGraphics::worldContainer->settings.selectionstart.y = 320;
	cGraphics::worldContainer->settings.texturestart = 0;
	MenuCommand_addwalls(src);


	cGraphics::world->water.height = 8;





	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cCube* c = &cGraphics::world->cubes[(int)y][(int)x];
			cGraphics::world->gattiles[2*(int)y][2*(int)x].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			cGraphics::world->gattiles[2*(int)y][2*(int)x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			cGraphics::world->gattiles[2*(int)y+1][2*(int)x].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			cGraphics::world->gattiles[2*(int)y+1][2*(int)x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
		}

	}

	MenuCommand_gatheight(src);



	for(x = 0; x < cGraphics::world->width; x++)
		for(y = 0; y < cGraphics::world->height; y++)
		{
			cGraphics::world->cubes[(int)y][(int)x].maxHeight = -99999;
			cGraphics::world->cubes[(int)y][(int)x].minHeight = 99999;
		}

	for(i = 0; i < cGraphics::world->models.size(); i++)
		cGraphics::world->models[i]->draw(false,false,true);


	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			if(cGraphics::world->cubes[(int)y][(int)x].maxHeight != -99999)
			{
				cGraphics::world->gattiles[2*(int)y][2*(int)x].type = '\0';
				cGraphics::world->gattiles[2*(int)y][2*(int)x+1].type = '\0';
				cGraphics::world->gattiles[2*(int)y+1][2*(int)x].type = '\0';
				cGraphics::world->gattiles[2*(int)y+1][2*(int)x+1].type = '\0';

				cGraphics::world->gattiles[2*(int)y][2*(int)x].cell1 = 	cGraphics::world->gattiles[2*(int)y][2*(int)x].cell2 = 	cGraphics::world->gattiles[2*(int)y][2*(int)x].cell3 = 	cGraphics::world->gattiles[2*(int)y][2*(int)x].cell4 = 0;
				cGraphics::world->gattiles[2*(int)y][2*(int)x+1].cell1 = 	cGraphics::world->gattiles[2*(int)y][2*(int)x+1].cell2 = 	cGraphics::world->gattiles[2*(int)y][2*(int)x+1].cell3	= 	cGraphics::world->gattiles[2*(int)y][2*(int)x+1].cell4 = 0;
				cGraphics::world->gattiles[2*(int)y+1][2*(int)x].cell1 = 	cGraphics::world->gattiles[2*(int)y+1][2*(int)x].cell2 = 	cGraphics::world->gattiles[2*(int)y+1][2*(int)x].cell3	= 	cGraphics::world->gattiles[2*(int)y+1][2*(int)x].cell4 = 0;
				cGraphics::world->gattiles[2*(int)y+1][2*(int)x+1].cell1 = cGraphics::world->gattiles[2*(int)y+1][2*(int)x+1].cell2 = cGraphics::world->gattiles[2*(int)y+1][2*(int)x+1].cell3 = cGraphics::world->gattiles[2*(int)y+1][2*(int)x+1].cell4 = 0;
			}
		}

	}
	cGraphics::world->root->recalculate();


	return true;
}



MENUCOMMAND(random4)
{
	if(!cGraphics::world)
		return false;
	unsigned int i;
	int x,y;

	cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(0,0,cGraphics::world->width, cGraphics::world->height));


	cGraphics::world->tiles.clear();
	for(int tex = 0; tex < 5; tex++)
	{
		for(y = 0; y < 4; y++)
		{
			for(x = 0; x < 4; x++)
			{
				cTile t;
				t.lightmap = 0;
				t.texture = tex;
				t.u1 = x/4.0;
				t.v1 = y/4.0;
				t.u2 = (x+1)/4.0;
				t.v2 = (y)/4.0;
				t.u3 = (x)/4.0;
				t.v3 = (y+1)/4.0;
				t.u4 = (x+1)/4.0;
				t.v4 = (y+1)/4.0;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				cGraphics::world->tiles.push_back(t);
			}
		}
	}

	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cGraphics::world->cubes[y][x].tileOtherSide = -1;
			cGraphics::world->cubes[y][x].tileSide = -1;
			cGraphics::world->cubes[y][x].tileUp = 16 + ((int)x%4) + 4*((int)y%4);
		}
	}

	float xding = rand() % 50 + 20;
	float yding = rand() % 50 + 20;
	float zding = rand() % 50 + 20;

	float xding2 = rand() % 50 + 50;
	float yding2 = rand() % 50 + 50;

	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{

			float f = xding2*sin(y/xding)+yding2*cos(x/yding) + zding*cos(x/zding+y/zding);
			f = floor(f / 32.0f)*32;

			cGraphics::world->cubes[y][x].cell1 = f;
			cGraphics::world->cubes[y][x].cell2 = f;
			cGraphics::world->cubes[y][x].cell3 = f;
			cGraphics::world->cubes[y][x].cell4 = f;
		}
	}
	eMode m = cSettings::editMode;
	cSettings::editMode = MODE_HEIGHTDETAIL;

	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_s;
	for(i = 0; i < 3; i++)
		SDL_PushEvent(&ev);

	int b = cGraphics::worldContainer->settings.brushSizeDetailHeight;
	cGraphics::cMouse::x3d = cGraphics::world->width*5;
	cGraphics::cMouse::z3d = cGraphics::world->height*5;
	cGraphics::worldContainer->settings.brushSizeDetailHeight = cGraphics::world->width+cGraphics::world->height;
	
	process_events();
	cGraphics::worldContainer->settings.brushSizeDetailHeight = b;


	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_s;
	SDL_PushEvent(&ev);
	cSettings::editMode = m;




	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			if(fabs(cGraphics::world->cubes[y][x].cell1 - cGraphics::world->cubes[y][x].cell2) > 5 ||
				fabs(cGraphics::world->cubes[y][x].cell1 - cGraphics::world->cubes[y][x].cell3) > 5 ||
				fabs(cGraphics::world->cubes[y][x].cell1 - cGraphics::world->cubes[y][x].cell4) > 5 ||
				fabs(cGraphics::world->cubes[y][x].cell2 - cGraphics::world->cubes[y][x].cell3) > 5 ||
				fabs(cGraphics::world->cubes[y][x].cell2 - cGraphics::world->cubes[y][x].cell4) > 5 ||
				fabs(cGraphics::world->cubes[y][x].cell3 - cGraphics::world->cubes[y][x].cell4) > 5)
				cGraphics::world->cubes[y][x].tileUp = 32 + ((int)x%4) + 4*((int)y%4);
		}
	}


	std::vector<std::string>	randommodels;
	for(i = 0; i < objectfiles.size(); i++)
	{
		cFile* pFile = cFileSystem::open(objectfiles[i]);
		while(!pFile->eof())
		{
			std::string line = pFile->readLine();
			if (line == "")
				continue;
			std::string pre = line.substr(0, line.find("|"));
			std::string filename = line.substr(line.find("|")+1);

			std::string cat = pre.substr(0, pre.rfind("/"));
			std::string name = pre.substr(pre.rfind("/")+1);

			if(cat == "randomtrees")
				randommodels.push_back(filename);
		}
	}



	if(randommodels.size() > 0)
	{
		for(i = 0; i < 1000; i++)
		{
			cRSMModel* model = new cRSMModel();
			model->load(cSettings::roDir +  randommodels[rand() % randommodels.size()]);

			model->pos = cVector3(rand()%(cGraphics::world->width*2), 0, rand()%(cGraphics::world->height*2));

			while(cGraphics::world->cubes[(int)(model->pos.z/2)][(int)(model->pos.x/2)].tileUp > 32)
				model->pos = cVector3(rand()%(cGraphics::world->width*2), 0, rand()%(cGraphics::world->height*2));


			model->pos.y = cGraphics::world->cubes[(int)(model->pos.z/2)][(int)(model->pos.x/2)].cell1;
			model->scale = cVector3(1,1,1);
			model->rot = cVector3(0,0,0);
			cGraphics::world->models.push_back(model);
		}
	}

	return true;
}



MENUCOMMAND(mode)
{
	if(!cGraphics::world)
		return false;
	std::string title = src->title;
	for(unsigned int i = 0; i < mode->items.size(); i++)
		mode->items[i]->ticked = false;
	src->ticked = true;

	if(title == GetMsg("menu/editmode/GLOBALHEIGHTEDIT"))
	{
		cSettings::editMode = MODE_HEIGHTGLOBAL;
		if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::world->textures.size())
			cGraphics::worldContainer->settings.texturestart = 0;
	}
	else if (title == GetMsg("menu/editmode/DETAILTERRAINEDIT"))
	{
		cSettings::editMode = MODE_HEIGHTDETAIL;
		if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::world->textures.size())
			cGraphics::worldContainer->settings.texturestart = 0;
	}
	else if (title == GetMsg("menu/editmode/TEXTUREEDIT"))
	{
		cSettings::editMode = MODE_TEXTURE;
		if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::world->textures.size())
			cGraphics::worldContainer->settings.texturestart = 0;
	}
	else if (title == GetMsg("menu/editmode/WALLEDIT"))
	{
		cSettings::editMode = MODE_WALLS;
	}
	else if (title == GetMsg("menu/editmode/OBJECTEDIT"))
	{
		cSettings::editMode = MODE_OBJECTS;
		if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::world->textures.size())
			cGraphics::worldContainer->settings.texturestart = 0;
	}
	else if (title == GetMsg("menu/editmode/GATEDIT"))
	{
		cSettings::editMode = MODE_GAT;
		if (cGraphics::worldContainer->settings.texturestart >= 6)
			cGraphics::worldContainer->settings.texturestart = 0;
	}
	else if (title == GetMsg("menu/editmode/WATEREDIT"))
	{
		cSettings::editMode = MODE_WATER;
		cGraphics::worldContainer->settings.texturestart = cGraphics::world->water.type;
	}
	else if (title == GetMsg("menu/editmode/EFFECTSEDIT"))
	{
		cSettings::editMode = MODE_EFFECTS;
		cGraphics::worldContainer->settings.selectedObject = -1;
	}
	else if (title == GetMsg("menu/editmode/SOUNDSEDIT"))
	{
		cSettings::editMode = MODE_SOUNDS;
	}
	else if (title == GetMsg("menu/editmode/LIGHTSEDIT"))
	{
		cSettings::editMode = MODE_LIGHTS;
	}
	else if (title == GetMsg("menu/editmode/OBJECTGROUPEDIT"))
	{
		cSettings::editMode = MODE_OBJECTGROUP;
	}
	else if (title == GetMsg("menu/editmode/SPRITEEDIT"))
	{
		cSettings::editMode = MODE_SPRITE;
	}
	return true;
}

MENUCOMMAND(mode_detail)
{
	if(!cGraphics::world)
		return false;
	unsigned int i;
	for(i = 0; i < mode->items.size(); i++)
		mode->items[i]->ticked = (mode->items[i]->title == "Detail Terrain Edit" ? true : false);
	for(i = 0; i < editdetail->items.size(); i++)
		editdetail->items[i]->ticked = false;
	src->ticked = true;
	cSettings::editMode = MODE_HEIGHTDETAIL;
	cGraphics::worldContainer->settings.brushSizeDetailHeight = atoi(src->title.c_str());

	return true;
}
MENUCOMMAND(speed)
{
	for(unsigned int i =0 ; i < speed->items.size(); i++)
		speed->items[i]->ticked = false;
	src->ticked = true;
	cSettings::paintSpeed = atof(src->title.c_str());
	return true;
}

MENUCOMMAND(model)
{
	if(!cGraphics::world)
		return false;
	delete cGraphics::previewModel;
	cGraphics::previewModel = new cRSMModel();
	cGraphics::previewModel->load(cSettings::roDir + src->data);
	cGraphics::previewModel->rot = cVector3(0,0,0);
	cGraphics::previewModel->scale = cVector3(4,4,4);

	cGraphics::previewModel->pos = cVector3(40,-40,-40);

	if (cSettings::editMode != MODE_OBJECTS)
		cGraphics::previewColor = 200;
	currentobject = src;
	return true;
}


MENUCOMMAND(quadtree)
{
	if(!cGraphics::world)
		return false;
	int x,y;
	for(x = 0; x < cGraphics::world->width; x++)
		for(y = 0; y < cGraphics::world->height; y++)
		{
			cGraphics::world->cubes[y][x].maxHeight = -99999;
			cGraphics::world->cubes[y][x].minHeight = 99999;
		}

	for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
	{
		Log(3,0,GetMsg("CALCMODEL"), i, cGraphics::world->models.size(), (i/(float)cGraphics::world->models.size())*100);
		cGraphics::world->models[i]->draw(false,false,true);
	}


	cGraphics::world->root->recalculate();
	return true;
}

MENUCOMMAND(gatheight)
{
	if(!cGraphics::world)
		return false;
	cGraphics::worldContainer->undoStack->push(new cUndoGatHeightEdit(0,0,cGraphics::world->gattiles[0].size(), cGraphics::world->gattiles.size()));
	int x,y;
	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cCube* c = &cGraphics::world->cubes[y][x];
			cGraphics::world->gattiles[2*y][2*x].cell1 = (c->cell1+c->cell1) / 2.0f;
			cGraphics::world->gattiles[2*y][2*x].cell2 = (c->cell1+c->cell2) / 2.0f;
			cGraphics::world->gattiles[2*y][2*x].cell3 = (c->cell1+c->cell3) / 2.0f;
			cGraphics::world->gattiles[2*y][2*x].cell4 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;

			cGraphics::world->gattiles[2*y][2*x+1].cell1 = (c->cell1+c->cell2) / 2.0f;
			cGraphics::world->gattiles[2*y][2*x+1].cell2 = (c->cell2+c->cell2) / 2.0f;
			cGraphics::world->gattiles[2*y][2*x+1].cell3 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			cGraphics::world->gattiles[2*y][2*x+1].cell4 = (c->cell4+c->cell2) / 2.0f;

			cGraphics::world->gattiles[2*y+1][2*x+1].cell1 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			cGraphics::world->gattiles[2*y+1][2*x+1].cell2 = (c->cell4 + c->cell2) / 2.0f;
			cGraphics::world->gattiles[2*y+1][2*x+1].cell3 = (c->cell4 + c->cell3) / 2.0f;
			cGraphics::world->gattiles[2*y+1][2*x+1].cell4 = (c->cell4 + c->cell4) / 2.0f;

			cGraphics::world->gattiles[2*y+1][2*x].cell1 = (c->cell3 + c->cell1) / 2.0f;
			cGraphics::world->gattiles[2*y+1][2*x].cell2 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			cGraphics::world->gattiles[2*y+1][2*x].cell3 = (c->cell3 + c->cell3) / 2.0f;
			cGraphics::world->gattiles[2*y+1][2*x].cell4 = (c->cell3 + c->cell4) / 2.0f;
		}

	}
	
	return true;
}

MENUCOMMAND(gatcollision2)
{
	if(!cGraphics::world)
		return false;
	MenuCommand_gatheight(src);
	int x, y;
	unsigned int i;
	int ww = cGraphics::w();
	ww -= 256;
	int hh = cGraphics::h()-20;

	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,ww,hh);						// Reset The Current Viewport

	float camrad = 10;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	gluPerspective(45.0f,(GLfloat)ww/(GLfloat)hh,10.0f,10000.0f);
	gluLookAt(  -cGraphics::worldContainer->camera.pointer.x + cGraphics::worldContainer->camera.height*sin(cGraphics::worldContainer->camera.rot),
				camrad+cGraphics::worldContainer->camera.height,
				-cGraphics::worldContainer->camera.pointer.y + cGraphics::worldContainer->camera.height*cos(cGraphics::worldContainer->camera.rot),
				-cGraphics::worldContainer->camera.pointer.x,camrad + cGraphics::worldContainer->camera.height * (cGraphics::worldContainer->camera.angle/10.0f),-cGraphics::worldContainer->camera.pointer.y,
				0,1,0);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
//	glTranslatef(0,0,cGraphics::world->height*10);
//	glScalef(1,1,-1);

	for(i = 0; i < cGraphics::world->models.size(); i++)
		cGraphics::world->models[i]->precollides();

	Log(3,0, "Done Model boundingbox calculations");

	for(x = 0; x < cGraphics::world->width*2; x++)
	{
		for(y = 0; y < cGraphics::world->height*2; y++)
		{
			cGraphics::world->gattiles[y][x].type = 0;
		}
	}
	

	for(x = 0; x < cGraphics::world->width; x++)
		for(y = 0; y < cGraphics::world->height; y++)
		{
			cGraphics::world->cubes[y][x].maxHeight = -99999;
			cGraphics::world->cubes[y][x].minHeight = 99999;
		}

	for(i = 0; i < cGraphics::world->models.size(); i++)
	{
		Log(3,0,GetMsg("CALCMODEL"), i, cGraphics::world->models.size(), (i/(float)cGraphics::world->models.size())*100);
		cGraphics::world->models[i]->draw(false,false,true);
	}


	for(x = 0; x < cGraphics::world->width*2; x++)
	{
		for(y = 0; y < cGraphics::world->height*2; y++)
		{
			if(cGraphics::world->cubes[y/2][x/2].maxHeight == -99999 || cGraphics::world->cubes[y/2][x/2].minHeight == 99999)
				continue;

			
			cGraphics::worldContainer->camera.pointer.x = -5*x + 2.5;
//			cGraphics::worldContainer->camera.pointer.y = -5*(2*cGraphics::world->height-y) + 2.5;


			cVector3 worldpos = cVector3(	5*x+2.5, 
											-5000,
											5*y+2.5);
			
			cVector3 highup = worldpos + cVector3(0, 10000, 0);
			for(unsigned int ii = 0; ii < cGraphics::world->models.size(); ii++)
			{
				if(cGraphics::world->models[ii]->collides(worldpos, highup))
				{
					cGraphics::world->gattiles[y][x].type = 1;
					break;
				}
			}
		}
		mainloop();
	}
	for(x = 0; x < cGraphics::world->width; x++)
		for(y = 0; y < cGraphics::world->height; y++)
		{
			cGraphics::world->cubes[y][x].maxHeight = 0;
			cGraphics::world->cubes[y][x].minHeight = 0;
		}


	return true;
}



//cVector3 lightpos = cVector3(-20000,20000,-20000);
bool selectonly;
bool lightonly;
bool noshadow;

MENUCOMMAND(dolightmaps)
{
	selectonly = true;
	lightonly = false;
	noshadow = false;
	return MenuCommand_dolightmaps2(src);
}
MENUCOMMAND(dolightmapsall)
{
	selectonly = false;
	lightonly = false;
	noshadow = false;
	return MenuCommand_dolightmaps2(src);
}
MENUCOMMAND(dolightmapslights)
{
	selectonly = false;
	lightonly = true;
	noshadow = false;
	return MenuCommand_dolightmaps2(src);
}

MENUCOMMAND(dolightmapsnoshadow)
{
	selectonly = false;
	lightonly = false;
	noshadow = true;
	return MenuCommand_dolightmaps2(src);
	
}

class cWindowPreviewButton : public cWindowButton
{
public:
	cWindowPreviewButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin) : cWindowButton(parent, skin)
	{
		alignment = ALIGN_TOPLEFT;
		moveTo(0,20);
		resizeTo(100,20);
		text = "Preview";
	}
	void onClick()
	{
		cGraphics::world->loaded = !cGraphics::world->loaded;
	}
};

inline void setLightIntensity(BYTE* buf, int yy, int xx, cVector3 worldpos, std::vector<std::vector<int> >* = NULL );





class cLightmapWorker : public cBThread
{
	cBThread* parent;
	int index;
public:
	cLightmapWorker(cBThread* p, int i)
	{
		parent = p;
		index = i;
	}
	void worker()
	{
		parent->signal(index);
		int mySignal = 0;
		while(mySignal != -1)
		{
			waitForSignal();
			mySignal = getSignal();
				
			int x,y;
			x = mySignal % cGraphics::world->width;
			y = mySignal / cGraphics::world->height;
			
///////////////
			cCube* c = &cGraphics::world->cubes[y][x];
			if(selectonly && !c->selected)
			{
				parent->signal();
				return;
			}
			
			
			//w->objects["progress"]->setInt(0, y*cGraphics::world->width + x);
			
			if(c->tileUp != -1)
			{
				cGraphics::world->tiles[c->tileUp].lightsWithShadow.resize(cGraphics::world->lights.size());
				BYTE* buf = (BYTE*)cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileUp].lightmap]->buf;
				ZeroMemory(buf,255);
				for(int yy = 1; yy < 7; yy++)
				{
					for(int xx = 1; xx < 7; xx++)
					{
						float fx = (xx-1)/6.0f;
						float fy = (yy-1)/6.0f;
						cVector3 worldpos = cVector3(	10*x+(10/6.0)*(xx-1), 
							-((c->cell1*(1-fx)+c->cell2*(fx)) + (c->cell1*(fy)+c->cell3*(1-fy))-c->cell1),
							10*y+(10/6.0)*(yy-1));
						setLightIntensity(buf, yy, xx, worldpos, &cGraphics::world->tiles[c->tileUp].lightsWithShadow);
					}
				}
				cGraphics::world->realLightmaps[y][x]->reset();
			}
			
			if(c->tileSide != -1)
			{
				cGraphics::world->tiles[c->tileSide].lightsWithShadow.resize(cGraphics::world->lights.size());
				BYTE* buf = (BYTE*)cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileSide].lightmap]->buf;
				ZeroMemory(buf,256);				
				for(int yy = 0; yy < 8; yy++)
				{
					for(int xx = 0; xx < 8; xx++)
					{
//						float fx = (xx-1)/6.0f;
						float fy = (yy-1)/6.0f;
						
						cCube* c2 = &cGraphics::world->cubes[y+1][x];
						
						cVector3 worldpos = cVector3(	10*x+(10/6.0)*(xx-1), 
							-((1-fy)*c->cell3 + (fy)*c2->cell1),
							10*y+10);
						
						setLightIntensity(buf, yy, xx, worldpos, &cGraphics::world->tiles[c->tileSide].lightsWithShadow);
					}
				}
				cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileSide].lightmap]->del();
				cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileSide].lightmap]->del2();
			}
			
			if(c->tileOtherSide != -1)
			{
				cGraphics::world->tiles[c->tileOtherSide].lightsWithShadow.resize(cGraphics::world->lights.size());
				BYTE* buf = (BYTE*)cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileOtherSide].lightmap]->buf;
				ZeroMemory(buf,256);
				
				for(int yy = 0; yy < 8; yy++)
				{
					for(int xx = 0; xx < 8; xx++)
					{
//						float fx = (xx-1)/6.0f;
						float fy = (yy-1)/6.0f;
						
						cCube* c2 = &cGraphics::world->cubes[y][x+1];
						
						cVector3 worldpos = cVector3(	10*x+10, 
							-((1-fy)*c->cell4 + (fy)*c2->cell3),
							10*y+(10/6.0)*(7-xx));
						setLightIntensity(buf, yy, xx, worldpos, &cGraphics::world->tiles[c->tileOtherSide].lightsWithShadow);
						
					}
				}
				cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileOtherSide].lightmap]->del();
				cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileOtherSide].lightmap]->del2();
			}




//////////////////			
			parent->signal(index);
		}
	}
};





class cLightmapMonitor : public cBThread
{
public:
	void worker()
	{
		cBThread* t[10];

		for(int i = 0; i < 10; i++)
		{
			t[i] = new cLightmapWorker(this,i);
			t[i]->start();
		}
		
		
		
		for(int x = 0; x < cGraphics::world->width; x++)
		{
			for(int y = 0; y < cGraphics::world->height; y++)
			{
				waitForSignal();
				int mySignal = getSignal();
				t[mySignal]->signal(cGraphics::world->width*y + x);
			}
		}
	}
};









MENUCOMMAND(dolightmaps2)
{
	if(!cGraphics::world)
		return false;

	unsigned int i;

	bool rendering = true;


	cProgressWindow* w = new cProgressWindow(&rendering);
	cWM::addWindow(w);
	w->objects["progress"]->setInt(1,0);
	w->objects["progress"]->setInt(2,cGraphics::world->height * cGraphics::world->width);
	w->objects["toggle"] = new cWindowPreviewButton(w);

	mainloop();

	Log(3,0,"Starting Lightmap Clearing");
	if(!cGraphics::world->makeLightmapsUnique())
		return false;
	//cGraphics::world->blackLightmaps();
	Log(3,0,"Done initializing for lightmaps...");

	int ww = cGraphics::w();
	ww -= 256;
	int hh = cGraphics::h()-20;

	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,ww,hh);						// Reset The Current Viewport

	float camrad = 10;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	gluPerspective(45.0f,(GLfloat)ww/(GLfloat)hh,10.0f,10000.0f);
	gluLookAt(  -cGraphics::worldContainer->camera.pointer.x + cGraphics::worldContainer->camera.height*sin(cGraphics::worldContainer->camera.rot),
				camrad+cGraphics::worldContainer->camera.height,
				-cGraphics::worldContainer->camera.pointer.y + cGraphics::worldContainer->camera.height*cos(cGraphics::worldContainer->camera.rot),
				-cGraphics::worldContainer->camera.pointer.x,camrad + cGraphics::worldContainer->camera.height * (cGraphics::worldContainer->camera.angle/10.0f),-cGraphics::worldContainer->camera.pointer.y,
				0,1,0);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
//	glTranslatef(0,0,cGraphics::world->height*10);
//	glScalef(1,1,-1);

	for(i = 0; i < cGraphics::world->models.size(); i++)
		cGraphics::world->models[i]->precollides();

	cGraphics::world->loaded = false;

	Log(3,0, "Done Model boundingbox calculations");
	unsigned long timer = tickcount();


#if 1
	int x,y;
	for(y = 0; y < cGraphics::world->height && rendering; y++)
	{
		for(x = 0; x < cGraphics::world->width && rendering; x++)
		{
			cCube* c = &cGraphics::world->cubes[y][x];
			if(selectonly && !c->selected)
				continue;
			w->objects["progress"]->setInt(0, y*cGraphics::world->width + x);
			
			if(c->tileUp != -1)
			{
				BYTE* buf = (BYTE*)cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileUp].lightmap]->buf;
				ZeroMemory(buf,255);
				for(int yy = 1; yy < 7; yy++)
				{
					for(int xx = 1; xx < 7; xx++)
					{
						float fx = (xx-1)/6.0f;
						float fy = (yy-1)/6.0f;
						cVector3 worldpos = cVector3(	10*x+(10/6.0)*(xx-1), 
														-((c->cell1*(1-fx)+c->cell2*(fx)) + (c->cell1*(fy)+c->cell3*(1-fy))-c->cell1),
														10*y+(10/6.0)*(yy-1));
						setLightIntensity(buf, yy, xx, worldpos);
					}
				}
				cGraphics::world->realLightmaps[y][x]->reset();
			}
			
			if(c->tileSide != -1)
			{
				BYTE* buf = (BYTE*)cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileSide].lightmap]->buf;
				ZeroMemory(buf,256);				
				for(int yy = 0; yy < 8; yy++)
				{
					for(int xx = 0; xx < 8; xx++)
					{
//						float fx = (xx-1)/6.0f;
						float fy = (yy-1)/6.0f;
						
						cCube* c2 = &cGraphics::world->cubes[y+1][x];
						
						cVector3 worldpos = cVector3(	10*x+(10/6.0)*(xx-1), 
							-((1-fy)*c->cell3 + (fy)*c2->cell1),
							10*y+10);
						
						setLightIntensity(buf, yy, xx, worldpos);
					}
				}
				cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileSide].lightmap]->del();
				cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileSide].lightmap]->del2();
			}

			if(c->tileOtherSide != -1)
			{
				BYTE* buf = (BYTE*)cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileOtherSide].lightmap]->buf;
				ZeroMemory(buf,256);

				for(int yy = 0; yy < 8; yy++)
				{
					for(int xx = 0; xx < 8; xx++)
					{
//						float fx = (xx-1)/6.0f;
						float fy = (yy-1)/6.0f;

						cCube* c2 = &cGraphics::world->cubes[y][x+1];

						cVector3 worldpos = cVector3(	10*x+10, 
														-((1-fy)*c->cell4 + (fy)*c2->cell3),
														10*y+(10/6.0)*(7-xx));
						setLightIntensity(buf, yy, xx, worldpos);
						
					}
				}
				cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileOtherSide].lightmap]->del();
				cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileOtherSide].lightmap]->del2();
			}		
			
			
			
			
			
			
			
			if(cGraphics::world->loaded)
			{
				cGraphics::worldContainer->camera.pointer.x = -10*x + 5;
				cGraphics::worldContainer->camera.pointer.y = -10*(cGraphics::world->height-y) + 5;
			}
			if(tickcount() - timer > 100)
			{
				mainloop();
				timer = tickcount();
			}
		}
	}
#else //threading! :D
	boundingBoxCollisions = true;
	cBThread* t = new cLightmapMonitor();
	t->start();
	t->wait();
	boundingBoxCollisions = false;

	//if(cWM::confirmWindow("Done first step of lightmap calculation, do you want to continue?"))
	{
		
	}

#endif


	cGraphics::world->fixGridding();
	cGraphics::world->loaded = true;
	
	
	w->close();
	
	//	return true;
	
	
	/*
	for(i = 0; i < cGraphics::world->models.size(); i++)
	{
	Log(3,0,"Doing model %i out of %i (%.2f%%)", i, cGraphics::world->models.size(), (i/(float)cGraphics::world->models.size())*100);
	cGraphics::world->models[i]->draw(false,false,false, true);
	}*/
	
	/*	float t;
	for(x = 0; x < cGraphics::world->width; x++)
	{
	Log(3,0,"%f%%", (x/(float)cGraphics::world->width)*100.0f);
	for(y = 0; y < cGraphics::world->height; y++)
	{
	int tile = cGraphics::world->cubes[y][x].tileUp;
	if (tile != -1)
			{
				float cellheight = -cGraphics::world->cubes[y][x].cell1;
				cLightmap* l = cGraphics::world->lightmaps[cGraphics::world->tiles[tile].lightmap];
				for(int xx = 0; xx < 6; xx++)
				{
					for(int yy = 0; yy < 6; yy++)
					{
						cVector3 pos = cVector3(10*x+10*(xx/6.0),cellheight, 10*y+10*(yy/6.0));
						char* lightmappos = &l->buf[xx + (8*yy)+1+8];

						for(int xxx = max(0,x - 1); xxx <= min(cGraphics::world->width-1,x+1); xxx++)
						{
							for(int yyy = max(0,y - 1); yyy <= min(cGraphics::world->height-1,y+1); yyy++)
							{
								if(*lightmappos == 127)
									break;
								if (xxx == x && yyy == y)
									continue;
								cCube* c = &cGraphics::world->cubes[yyy][xxx];
								cVector3 triangle[6];
								triangle[2] = cVector3(xxx*10+10, -c->cell2, yyy*10);
								triangle[1] = cVector3(xxx*10, -c->cell3, yyy*10-10);
								triangle[0] = cVector3(xxx*10+10, -c->cell4, yyy*10-10);

								triangle[5] = cVector3(xxx*10, -c->cell4, yyy*10-10);
								triangle[4] = cVector3(xxx*10+10, -c->cell2, yyy*10);
								triangle[3] = cVector3(xxx*10, -c->cell1, yyy*10);

								if (LineIntersectPolygon(triangle, 3, lightpos, pos, t))
								{
									if (t < 1)
										*lightmappos = 127;//((BYTE)l->buf[xx + (8*yy)+1+8]) / 1.6;
								}
								else if (LineIntersectPolygon(triangle+3, 3, lightpos, pos, t))
								{
									if (t < 1)
										*lightmappos = 127;//((BYTE)l->buf[xx + (8*yy)+1+8]) / 1.6;
								}

								if (c->tileOtherSide != -1)
								{
									triangle[2] = cVector3(xxx*10+10,-c->cell4,yyy*10);
									triangle[1] = cVector3(xxx*10+10,-(c+1)->cell1,yyy*10);
									triangle[0] = cVector3(xxx*10+10,-(c+1)->cell3,yyy*10+10);

									triangle[3] = cVector3(xxx*10+10,-(c+1)->cell1,yyy*10+10);
									triangle[4] = cVector3(xxx*10+10,-c->cell4,yyy*10+10);
									triangle[5] = cVector3(xxx*10+10,-c->cell2,yyy*10);

									if (LineIntersectPolygon(triangle, 3, lightpos, pos, t))
									{
										if (t < 1)
											*lightmappos = 127;//((BYTE)l->buf[xx + (8*yy)+1+8]) / 1.6;
									}
									else if (LineIntersectPolygon(triangle+3, 3, lightpos, pos, t))
									{
										if (t < 1)
											*lightmappos = 127;//((BYTE)l->buf[xx + (8*yy)+1+8]) / 1.6;
									}
								}
								if (c->tileSide != -1 && y < cGraphics::world->width - 1)
								{

									triangle[0] = cVector3(xxx*10,-c->cell3,yyy*10+10);
									triangle[1] = cVector3(xxx*10+10,-c->cell4,yyy*10+10);
									triangle[2] = cVector3(xxx*10,-cGraphics::world->cubes[y+1][x].cell1,yyy*10+10);
									
									
									triangle[3] = cVector3(xxx*10+10,-cGraphics::world->cubes[y+1][x].cell2,yyy*10+10);
									triangle[4] = cVector3(xxx*10,-cGraphics::world->cubes[y+1][x].cell1,yyy*10+10);
									triangle[5] = cVector3(xxx*10+10,-c->cell4,yyy*10+10);

									if (LineIntersectPolygon(triangle, 3, lightpos, pos, t))
									{
										if(t < 1)
											*lightmappos = 127;//((BYTE)l->buf[xx + (8*yy)+1+8]) / 1.6;
									}
									else if (LineIntersectPolygon(triangle+3, 3, lightpos, pos, t))
									{
										if(t < 1)
											*lightmappos = 127;//((BYTE)l->buf[xx + (8*yy)+1+8]) / 1.6;
									}
								}
							
							}
						}
					}
				}

			}
				
		}
	}
*/
	return true;
}


BYTE* getLightMap(int x, int y)
{
	cCube* c = &cGraphics::world->cubes[y/6][x/6];
	if(c->tileUp != -1)
	{
		cTile* t = &cGraphics::world->tiles[c->tileUp];
		if(t->lightmap != -1)
		{
			cLightmap* l = cGraphics::world->lightmaps[t->lightmap];
			return (BYTE*)l->buf + (8*((y%6)+1) + ((x%6)+1));
		}
	}
	return NULL;
}

MENUCOMMAND(smoothlightmaps)
{
	if(!cGraphics::world)
		return false;
	std::string strFactor = cWM::inputWindow("Smoothing factor:", "1");
	if(strFactor == "")
		return true;
	float factor = atof(strFactor.c_str());
	int x,y;
	char* buf = new char[cGraphics::world->height*6*cGraphics::world->width*6];
	for(x = 0; x < cGraphics::world->width*6; x++)
	{
		for(y = 0; y < cGraphics::world->height*6; y++)
		{
			int total = 0;
			float count = 0;
			for(int xx = -1; xx < 2; xx++)
			{
				for(int yy = -1; yy < 2; yy++)
				{
					if(x+xx >= 0 && y+yy >= 0 && x+xx < cGraphics::world->width*6 && y+yy < cGraphics::world->height*6)
					{
						if(xx == 0 && yy == 0)
						{
							if(getLightMap(x+xx,y+yy) != NULL)
							{
								total += factor * *getLightMap(x+xx,y+yy);
								count+=factor;
							}
						}
						else
						{
							if(getLightMap(x+xx,y+yy) != NULL)
							{
								total += *getLightMap(x+xx,y+yy);
								count++;
							}
						}
					}
				}
			}
			buf[x+cGraphics::world->width*6*y] = min(max(total / count,0),255);
		}
	}

	for(x = 0; x < cGraphics::world->width*6; x++)
	{
		for(y = 0; y < cGraphics::world->height*6; y++)
		{
			if(getLightMap(x,y) != NULL)
				*getLightMap(x,y) = buf[x+cGraphics::world->width*6*y];
		}
	}
	for(x = 0; x < cGraphics::world->width; x++)
		for(y = 0; y < cGraphics::world->height; y++)
			cGraphics::world->realLightmaps[y][x]->reset();
	cGraphics::world->fixGridding();


	return true;
}

MENUCOMMAND(fixcolors)
{
	if(!cGraphics::world)
		return false;
	int x,y;
	for(x = 0; x < cGraphics::world->width; x++)
		for(y = 0; y < cGraphics::world->height; y++)
		{
			int tile = cGraphics::world->cubes[y][x].tileUp;
			if(tile != -1)
			{
				cGraphics::world->tiles[tile].color[0] = '\255';
				cGraphics::world->tiles[tile].color[1] = '\255';
				cGraphics::world->tiles[tile].color[2] = '\255';
				cGraphics::world->tiles[tile].color[3] = '\255';
			}			
		}

	return true;
}


MENUCOMMAND(savelightmaps)
{
	if(!cGraphics::world)
		return false;
	cGraphics::world->saveLightmap();
	return true;
}

MENUCOMMAND(loadlightmaps)
{

	if(!cGraphics::world)
		return false;
	if(!cGraphics::world->makeLightmapsUnique())
		return false;
	cGraphics::world->loadLightmap();
	cGraphics::world->fixGridding();

	return true;
}


MENUCOMMAND(addwalls)
{
	if(!cGraphics::world)
		return false;
	int x,y;
	for(x = 0; x < cGraphics::world->width-1; x++)
	{
		for(y = 0; y < cGraphics::world->height-1; y++)
		{
			cCube* c = &cGraphics::world->cubes[y][x];
			if (c->tileOtherSide == -1)
			{
				if (c->cell4 != (c+1)->cell1 && c->cell2 != (c+1)->cell3)
				{
					cTile t;
					t.color[0] = (char)255;
					t.color[1] = (char)255;
					t.color[2] = (char)255;
					t.color[3] = (char)255;
					t.texture = cGraphics::worldContainer->settings.texturestart + ((int)cGraphics::worldContainer->settings.selectionstart.y - 32) / 288;
					t.lightmap = 0;
					t.u1 = 0;
					t.v1 = 0;

					t.u2 = 1;
					t.v2 = 0;
					
					t.u3 = 0;
					t.v3 = 1;
					
					t.u4 = 1;
					t.v4 = 1;
					cGraphics::world->tiles.push_back(t);
					cGraphics::world->cubes[y][x].tileOtherSide = cGraphics::world->tiles.size()-1;

					cGraphics::cMouse::x3d = x*10+5;
					cGraphics::cMouse::z3d = y*10+5;
					SDL_Event ev;
					ev.type = SDL_KEYDOWN;
					ev.key.keysym.sym = SDLK_w;
					ev.key.keysym.mod = KMOD_LALT;
					cProcessManagement::walledit_process_events(ev);

				}
			}
			if (c->tileSide == -1)
			{
				if (c->cell4 != cGraphics::world->cubes[y+1][x].cell1 && c->cell3 != cGraphics::world->cubes[y+1][x].cell2)
				{
					cTile t;
					t.color[0] = (char)255;
					t.color[1] = (char)255;
					t.color[2] = (char)255;
					t.color[3] = (char)255;
					t.texture = cGraphics::worldContainer->settings.texturestart + ((int)cGraphics::worldContainer->settings.selectionstart.y - 32) / 288;
					t.lightmap = 0;
					t.u1 = 0;
					t.v1 = 0;

					t.u2 = 1;
					t.v2 = 0;
					
					t.u3 = 0;
					t.v3 = 1;
					
					t.u4 = 1;
					t.v4 = 1;
					cGraphics::world->tiles.push_back(t);
					cGraphics::world->cubes[y][x].tileSide = cGraphics::world->tiles.size()-1;

					cGraphics::cMouse::x3d = x*10+5;
					cGraphics::cMouse::z3d = y*10+5;
					SDL_Event ev;
					ev.type = SDL_KEYDOWN;
					ev.key.keysym.sym = SDLK_w;
					ev.key.keysym.mod = (SDLMod)0;
					cProcessManagement::walledit_process_events(ev);
			
				}
			}
		}
	}
	return true;
}

MENUCOMMAND(gatcollision)
{
	if(!cGraphics::world)
		return false;
	int x,y;
	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cCube* c = &cGraphics::world->cubes[y][x];
			cGraphics::world->gattiles[2*y][2*x].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			cGraphics::world->gattiles[2*y][2*x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			cGraphics::world->gattiles[2*y+1][2*x].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			cGraphics::world->gattiles[2*y+1][2*x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
		}

	}
	return true;
}


MENUCOMMAND(cleanuplightmaps)
{
	if(!cGraphics::world)
		return false;
	unsigned int i;
	std::vector<int> newvalue;
	std::map<int, bool, std::less<int> > used;
	for(i = 0; i < cGraphics::world->lightmaps.size(); i++)
	{
		for(unsigned int ii = 0; ii < i; ii++)
		{
			if(memcmp(cGraphics::world->lightmaps[i]->buf, cGraphics::world->lightmaps[ii]->buf, 256) == 0)
			{
				newvalue.push_back(ii);
				break;
			}
		}
		if (newvalue.size() <= i)
			newvalue.push_back(i);
	}

	for(i = 0; i < cGraphics::world->tiles.size(); i++)
	{
		cGraphics::world->tiles[i].lightmap = newvalue[cGraphics::world->tiles[i].lightmap];
	}
	return true;
}


MENUCOMMAND(tempfunc)
{
	if(!cGraphics::world)
		return false;
	glColor4f(1,1,1,0.7f);
	glEnable(GL_BLEND);

	cTile t;
	t.color[0] = (char)255;
	t.color[1] = (char)255;
	t.color[2] = (char)255;
	t.color[3] = (char)255;
	t.texture = cGraphics::worldContainer->settings.texturestart + ((int)cGraphics::worldContainer->settings.selectionstart.y - 32) / 288;
	t.lightmap = -1;
	t.u1 = 0;
	t.v1 = 0;

	t.u2 = 1;
	t.v2 = 0;
	
	t.u3 = 0;
	t.v3 = 1;
	
	t.u4 = 1;
	t.v4 = 1;
	cGraphics::world->tiles.push_back(t);
	cGraphics::world->tiles.push_back(t);
	int x,y;
	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cCube* c = &cGraphics::world->cubes[y][x];
			if(cGraphics::world->lightmaps[cGraphics::world->tiles[c->tileUp].lightmap]->buf[10] == 0)
			{
				c->cell1 = -20;
				c->cell2 = -20;
				c->cell3 = -20;
				c->cell4 = -20;

				c->tileUp = cGraphics::world->tiles.size()-2;
				c->tileSide = -1;
				c->tileOtherSide = -1;
			}
			else
			{
				c->tileSide = -1;
				c->tileOtherSide = -1;
			}

		}

	}
	return true;

}


bool mouseovertexture(cMenu* src)
{
	if (cGraphics::texturePreview == NULL || cGraphics::texturePreview->getfilename() != cSettings::roDir + "data\\texture\\" + ((cMenuItem*)src)->data)
	{
		cGraphics::texturePreview = cTextureCache::load(cSettings::roDir + "data\\texture\\" + ((cMenuItem*)src)->data);
		return false;
	}
	else
	{
		cTextureCache::unload(cGraphics::texturePreview);
		cGraphics::texturePreview = NULL;
		return true;
	}
}
bool mouseouttexture(cMenu* src)
{
	if (cGraphics::texturePreview != NULL)
	{
		cTextureCache::unload(cGraphics::texturePreview);
		cGraphics::texturePreview = NULL;
	}
	return true;
}

MENUCOMMAND(effect)
{
	if(!cGraphics::world)
		return false;
	if (selectedeffect != NULL)
		selectedeffect->ticked = false;
	src->ticked = true;
	if (cGraphics::worldContainer->settings.selectedObject != -1)
	{
		cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].type = atoi(src->data.c_str());
		cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].readablename = src->title;
	}
	selectedeffect = src;
	return true;
}

MENUCOMMAND(toggle)
{
	if(!cGraphics::world)
		return false;
	if(src->pdata)
	{
		*((bool*)src->pdata) = !*((bool*)src->pdata);
		if(cGraphics::worldContainer)
			cGraphics::worldContainer->view = cGraphics::view;

		src->ticked = *((bool*)src->pdata);
	}
	else
		src->ticked = !src->ticked;
	return true;
}

MENUCOMMAND(water)
{
	if(!cGraphics::world)
		return false;
	char buf[100];
	cWindow* w = new cWaterWindow();
	sprintf(buf, "%f", cGraphics::world->water.amplitude);		w->objects["amplitude"]->setText(0,buf);
	sprintf(buf, "%f", cGraphics::world->water.height);		w->objects["height"]->setText(0,buf);
	sprintf(buf, "%f", cGraphics::world->water.phase);			w->objects["phase"]->setText(0,buf);
	sprintf(buf, "%f", cGraphics::world->water.surfaceCurve);	w->objects["surfacecurve"]->setText(0,buf);
	sprintf(buf, "%i", cGraphics::world->water.type);			w->objects["type"]->setText(0,buf);
	cWM::addWindow(w);
	return true;
}

MENUCOMMAND(cleantextures)
{
	if(!cGraphics::world)
		return false;
	cGraphics::world->clean();
	std::vector<bool> used;
	int i;
	used.resize(cGraphics::world->textures.size(), false);


	for(i = 0; i < (int)cGraphics::world->tiles.size(); i++)
		used[cGraphics::world->tiles[i].texture] = true;
	
	for(i = (int)used.size()-1; i >= 0; i--)
	{
		if (!used[i])
		{
			for(unsigned int ii = 0; ii < cGraphics::world->tiles.size(); ii++)
			{
				if(cGraphics::world->tiles[i].texture > i)
					cGraphics::world->tiles[i].texture--;
			}
			cTextureCache::unload(cGraphics::world->textures[i]->texture);
			delete cGraphics::world->textures[i];
			cGraphics::world->textures.erase(cGraphics::world->textures.begin() + i);
		}
	}
	cGraphics::worldContainer->settings.texturestart = 0;
	return true;
}

MENUCOMMAND(ambientlight)
{
	if(!cGraphics::world)
		return false;
	char buf[100];
	cWindow* w = new cAmbientLightWindow();
	sprintf(buf, "%i", cGraphics::world->ambientLight.ambientr);		w->objects["ambientr"]->setText(0,buf);
	sprintf(buf, "%i", cGraphics::world->ambientLight.ambientg);		w->objects["ambientg"]->setText(0,buf);
	sprintf(buf, "%i", cGraphics::world->ambientLight.ambientb);		w->objects["ambientb"]->setText(0,buf);

	sprintf(buf, "%f", cGraphics::world->ambientLight.diffuse.x);		w->objects["diffuser"]->setText(0,buf);
	sprintf(buf, "%f", cGraphics::world->ambientLight.diffuse.y);		w->objects["diffuseg"]->setText(0,buf);
	sprintf(buf, "%f", cGraphics::world->ambientLight.diffuse.z);		w->objects["diffuseb"]->setText(0,buf);

	sprintf(buf, "%f", cGraphics::world->ambientLight.shadow.x);		w->objects["shadowr"]->setText(0,buf);
	sprintf(buf, "%f", cGraphics::world->ambientLight.shadow.y);		w->objects["shadowg"]->setText(0,buf);
	sprintf(buf, "%f", cGraphics::world->ambientLight.shadow.z);		w->objects["shadowb"]->setText(0,buf);
	
	sprintf(buf, "%f", cGraphics::world->ambientLight.alpha);			w->objects["alpha"]->setText(0,buf);

	cWM::addWindow(w);
	return true;
}


MENUCOMMAND(texturewindow)
{
	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_t;
	SDL_PushEvent(&ev);
	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_t;
	SDL_PushEvent(&ev);
	return true;
}

MENUCOMMAND(modelwindow)
{
	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_m;
	SDL_PushEvent(&ev);
	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_m;
	SDL_PushEvent(&ev);
	return true;
}

MENUCOMMAND(properties)
{
	if(!cGraphics::world)
		return false;
	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_RETURN;
	SDL_PushEvent(&ev);
	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_RETURN;
	SDL_PushEvent(&ev);
	return true;
}


MENUCOMMAND(preferences)
{
	cWM::addWindow(new cKeyBindWindow());
	return true;
}



MENUCOMMAND(fillarea)
{
	if(!cGraphics::world)
		return false;
	int x,y,i;
	std::map<int, bool, std::less<int> > used;

	cLightmap* map = new cLightmap();
	for(i = 0; i < 256; i++)
		map->buf[i] = i < 64 ? 255 : 0;
	cGraphics::world->lightmaps.push_back(map);
	map = new cLightmap();
	for(i = 0; i < 256; i++)
		map->buf[i] = i < 64 ? 255 : 0;
	cGraphics::world->lightmaps.push_back(map);


	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			if(cGraphics::world->cubes[y][x].selected)
			{
				int xx = x % 4;
				int yy = y % 4;
				cTile t;
				t.lightmap = cGraphics::world->textures.size()-1;
				t.texture = cGraphics::worldContainer->settings.texturestart + ((int)cGraphics::worldContainer->settings.selectionstart.y - 32) / 288;
				t.u1 = xx/4.0;
				t.v1 = yy/4.0;
				t.u2 = (xx+1)/4.0;
				t.v2 = (yy)/4.0;
				t.u3 = (xx)/4.0;
				t.v3 = (yy+1)/4.0;
				t.u4 = (xx+1)/4.0;
				t.v4 = (yy+1)/4.0;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				cGraphics::world->tiles.push_back(t);
				cGraphics::world->cubes[y][x].tileUp = cGraphics::world->tiles.size()-1;;
			}
		}
	}


	for(x = 0; x < cGraphics::world->width; x++)
	{
		for(y = 0; y < cGraphics::world->height; y++)
		{
			int tile = cGraphics::world->cubes[y][x].tileUp;
			if(used.find(tile) != used.end())
			{
				cTile t = cGraphics::world->tiles[tile];
				tile = cGraphics::world->tiles.size();
				cGraphics::world->tiles.push_back(t);
				cGraphics::world->cubes[y][x].tileUp = tile;
			}
			used[tile] = 1;
///////////////////////////////////////
			tile = cGraphics::world->cubes[y][x].tileSide;
			if (tile != -1)
			{
				if(used.find(tile) != used.end())
				{
					cTile t = cGraphics::world->tiles[tile];
					tile = cGraphics::world->tiles.size();
					cGraphics::world->tiles.push_back(t);
					cGraphics::world->cubes[y][x].tileSide = tile;
				}
				used[tile] = 1;
			}
/////////////////////////////////////
			tile = cGraphics::world->cubes[y][x].tileOtherSide;
			if (tile!= -1)
			{
				if(used.find(tile) != used.end())
				{
					cTile t = cGraphics::world->tiles[tile];
					tile = cGraphics::world->tiles.size();
					cGraphics::world->tiles.push_back(t);
					cGraphics::world->cubes[y][x].tileOtherSide = tile;
				}
				used[tile] = 1;
			}
		}
	}


	

	return true;
}



MENUCOMMAND(rsmedit)
{
	cWM::addWindow(new cRSMEditWindow());
	return true;
}

MENUCOMMAND(favlights)
{
	cWM::addWindow(new cFavoriteLightsWindow());
	return true;
}


MENUCOMMAND(exportmapfiles)
{
	if(!cGraphics::world)
		return false;
#ifdef WIN32
	CreateDirectory(cGraphics::world->fileName, NULL);
	CreateDirectory((cGraphics::world->fileName + std::string("\\texture\\")).c_str(), NULL);

	unsigned int i;
	std::ofstream pFile((std::string(cGraphics::world->fileName) + ".txt").c_str());
	for(i = 0; i < cGraphics::world->textures.size(); i++)
	{
		cFile* pF = cFileSystem::open(cSettings::roDir + "data\\texture\\" + cGraphics::world->textures[i]->RoFilename);
		if(pF->location != -1)
		{
			pF->close();
			continue;
		}
		pF->close();


		CopyFile((cSettings::roDir + "data\\texture\\" + cGraphics::world->textures[i]->RoFilename).c_str(), (std::string(cGraphics::world->fileName) + "\\texture\\" + cGraphics::world->textures[i]->RoFilename2).c_str(), false);
		pFile.write("texture\\", 8);
		pFile.write(cGraphics::world->textures[i]->RoFilename.c_str(), cGraphics::world->textures[i]->RoFilename.length());
		pFile.put('\r');
		pFile.put('\n');
	}

	std::map<std::string, bool, std::less<std::string> > usedmodels;

	for(i = 0; i < cGraphics::world->models.size(); i++)
	{
		if(usedmodels.find(cGraphics::world->models[i]->rofilename) != usedmodels.end())
			continue;

		cFile* pF = cFileSystem::open(cGraphics::world->models[i]->filename);
		if(pF->location != -1)
		{
			pF->close();
			continue;
		}
		pF->close();



		usedmodels[cGraphics::world->models[i]->rofilename] = true;
		pFile.write("model\\", 6);
		pFile.write(cGraphics::world->models[i]->rofilename.c_str(), cGraphics::world->models[i]->rofilename.length());
		pFile.put('\r');
		pFile.put('\n');

		for(unsigned int ii = 0; ii < cGraphics::world->models[i]->textures.size(); ii++)
		{
			std::string file = cGraphics::world->models[i]->textures[ii]->getfilename();
			cFile* pF = cFileSystem::open(file);
			if(pF->location != -1)
			{
				pF->close();
				continue;
			}
			pF->close();

			file = file.substr(cSettings::roDir.length()+5);
			pFile.write(file.c_str(), file.length());
			pFile.put('\r');
			pFile.put('\n');
		}
		
	}


	pFile.close();
	ShellExecute(NULL,"open",(std::string(cGraphics::world->fileName) + ".txt").c_str(),NULL,"c:\\",SW_SHOW);
#endif
	return true;
}




MENUCOMMAND(random5)
{
	if(!cGraphics::world)
		return false;
	if(cGraphics::world->textures.size() < 3)
	{
		cWM::showMessage("You need at least 3 textures for this generator (floor, top and walls)");
		return false;
	}
	unsigned int i;
	int xx,yy;

	cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(0,0,cGraphics::world->width, cGraphics::world->height));
	float x,y;

	
	for(i = 0; i < cGraphics::world->lightmaps.size(); i++)
		delete cGraphics::world->lightmaps[i];
	cGraphics::world->lightmaps.clear();

	cLightmap* l = new cLightmap();
	ZeroMemory(l->buf,255);
	cGraphics::world->lightmaps.push_back(l);

	cGraphics::world->tiles.clear();
	for(int tex = 0; tex < 3; tex++)
	{
		for(y = 0; y < 4; y++)
		{
			for(x = 0; x < 4; x++)
			{
				cTile t;
				t.lightmap = 0;
				t.texture = tex;
				t.u1 = x/4.0;
				t.v1 = y/4.0;
				t.u2 = (x+1)/4.0;
				t.v2 = (y)/4.0;
				t.u3 = (x)/4.0;
				t.v3 = (y+1)/4.0;
				t.u4 = (x+1)/4.0;
				t.v4 = (y+1)/4.0;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				cGraphics::world->tiles.push_back(t);
			}
		}
	}
	
	
	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			cGraphics::world->cubes[(int)y][(int)x].tileOtherSide = -1;
			cGraphics::world->cubes[(int)y][(int)x].tileSide = -1;
			cGraphics::world->cubes[(int)y][(int)x].tileUp = 0 + ((int)x%4) + 4*((int)y%4);
			cGraphics::world->cubes[(int)y][(int)x].minHeight = 99999;
			cGraphics::world->cubes[(int)y][(int)x].cell1 = -32;
			cGraphics::world->cubes[(int)y][(int)x].cell2 = -32;
			cGraphics::world->cubes[(int)y][(int)x].cell3 = -32;
			cGraphics::world->cubes[(int)y][(int)x].cell4 = -32;
		}
	}



	
	int lasta = 0;
	int reali = 0;

	int w,h;

	
	std::vector<cIntQuad> islands;

	bool filledenough = false;
	unsigned int oldsize = 0;
	while(!filledenough)
	{
		if(oldsize != islands.size())
		{
			cGraphics::draw();
			SDL_GL_SwapBuffers();
			oldsize = islands.size();
		}
		int island = -1;
		reali++;
		if(reali > 10000)
			break;

		w = 5+rand() % 10;
		h = 5+rand() % 10;

		if(islands.size() > 0)
		{
			island = rand() % islands.size();
			if(islands[island].connections.size() > 1)
				island = rand() % islands.size();


			int a = rand()%4;
			if(a == lasta || (a+2)%4 == lasta)
				a = rand()%4;
			lasta = a;

			if(a == 0)
			{
				x = islands[island].x + islands[island].w + 2;
				y = islands[island].y;
			}
			if(a == 1)
			{
				x = islands[island].x;
				y = islands[island].y + islands[island].h + 2;
			}
			if(a == 2)
			{
				x = islands[island].x;
				y = islands[island].y - h - 2;
			}
			if(a == 3)
			{
				x = islands[island].x - w - 2;
				y = islands[island].y;
			}

		}
		else
		{
			x = (cGraphics::world->width-w)/2;
			y = (cGraphics::world->height-h)/2;
		}

		if(!(x + w >= cGraphics::world->width-1 || y+h >= cGraphics::world->height-1 || x <= 1 || y <= 1))
		{
			int takencount = 0;
			for(xx = (int)x; xx < (int)x+w; xx++)
			{
				for(yy = (int)y; yy < (int)y+h; yy++)
				{
					if(cGraphics::world->cubes[yy][xx].cell1 == 0)
						takencount++;
				}
			}
			if(takencount < 3)
			{
				for(xx = (int)x; xx < (int)x+w; xx++)
				{
					for(yy = (int)y; yy < (int)y+h; yy++)
					{
						cGraphics::world->cubes[yy][xx].cell1 = 0;//rand()%25;
						cGraphics::world->cubes[yy][xx].cell2 = 0;//rand()%25;
						cGraphics::world->cubes[yy][xx].cell3 = 0;//rand()%25;
						cGraphics::world->cubes[yy][xx].cell4 = 0;//rand()%25;
						cGraphics::world->cubes[yy][xx].tileUp = 16 + (xx%4) + 4*(yy%4);
					}
				}
				if(island != -1)
					islands[island].connections.push_back(islands.size());

				islands.push_back(cIntQuad((int)x,(int)y,w,h));
				if(island != -1)
					islands[islands.size()-1].connections.push_back(island);
			}
			int count = 0;
			for(int yy = 0; yy < cGraphics::world->height; yy++)
			{
				for(int xx = 0; xx < cGraphics::world->width; xx++)
				{
					if(cGraphics::world->cubes[yy][xx].cell1 == 0)
						count++;
				}
			}
			if(count > cGraphics::world->height*cGraphics::world->width / 2)
				filledenough = true;

		}
	}


	for(i = 0; i < islands.size(); i++)
	{
		cGraphics::draw();
		SDL_GL_SwapBuffers();
		for(unsigned int ii = 0; ii < islands[i].connections.size(); ii++)
		{

			x = islands[islands[i].connections[ii]].x;
			y = islands[islands[i].connections[ii]].y;
			w = islands[islands[i].connections[ii]].w;
			h = islands[islands[i].connections[ii]].h;

			xx = islands[i].x;
			yy = islands[i].y;

			if(xx - (x+w) == 5)
				x+=w;
			else if(yy - (y+h) == 5)
				y+=h;
			else if(y - (yy+islands[i].h) == 5)
				yy+=islands[i].h;
			else if(x - (xx+islands[i].w) == 5)
				xx+=islands[i].w;


			if(xx == x)
			{
				if(w < islands[i].w)
					x = xx = xx + w/2;
				else
					x = xx = xx + islands[i].w/2;
			}
			if(yy == y)
			{
				if(h < islands[i].h)
					y = yy = yy + h/2;
				else
					y = yy = yy + islands[i].h/2;
			}


			while(xx != x || yy != y)
			{
				for(int xxx = ((x == xx) ? -1 : 0); xxx < ((x==xx) ? 1 : 2); xxx++)
				{
					for(int yyy = ((y == yy) ? -1 : 0); yyy < ((y==yy) ? 1 : 2); yyy++)
					{
						cGraphics::world->cubes[yy+yyy][xx+xxx].cell1 = 0;//rand()%25;
						cGraphics::world->cubes[yy+yyy][xx+xxx].cell2 = 0;//rand()%25;
						cGraphics::world->cubes[yy+yyy][xx+xxx].cell3 = 0;//rand()%25;
						cGraphics::world->cubes[yy+yyy][xx+xxx].cell4 = 0;//rand()%25;
						cGraphics::world->cubes[yy+yyy][xx+xxx].tileUp = 16 + ((xx+xxx)%4) + 4*((yy+yyy)%4);
						if(xx > x)
							xx--;
						if(xx < x)
							xx++;
						if(yy > y)
							yy--;
						if(yy < y)
							yy++;

						cGraphics::world->cubes[yy+yyy][xx+xxx].cell1 = 0;//rand()%25;
						cGraphics::world->cubes[yy+yyy][xx+xxx].cell2 = 0;//rand()%25;
						cGraphics::world->cubes[yy+yyy][xx+xxx].cell3 = 0;//rand()%25;
						cGraphics::world->cubes[yy+yyy][xx+xxx].cell4 = 0;//rand()%25;
						cGraphics::world->cubes[yy+yyy][xx+xxx].tileUp = 16 + ((xx+xxx)%4) + 4*((yy+yyy)%4);
					}
				}
			}
		}
	}

	cGraphics::worldContainer->settings.selectionstart.y = 320;
	cGraphics::worldContainer->settings.texturestart = 1;
	MenuCommand_addwalls(src);


	MenuCommand_gatheight(src);
	MenuCommand_gatcollision(src);




/*	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			if((cGraphics::world->cubes[y][x].cell1 <= -8 || cGraphics::world->cubes[y][x].cell2 <= -8 || cGraphics::world->cubes[y][x].cell3  <= -8|| cGraphics::world->cubes[y][x].cell4 <= -8) && cGraphics::world->cubes[y][x].cell1 > -63)
			{
				cGraphics::world->cubes[y][x].tileUp= 50 + ((int)x%5) + 5*((int)y%5);
			}
		}
	}
*/
	cGraphics::world->water.height = 8;


	return true;
}





MENUCOMMAND(99dun)
{
	srand(atoi(cWM::inputWindow("Random Seed: ").c_str()));

	for(int i = 0; i < 10; i++)
	{
		int mode = rand() % 3;

		if(mode == 0)
		{
			sprintf(cGraphics::world->fileName, "%sdata\\random_pay", cSettings::roDir.c_str());
			cGraphics::world->load();
			MenuCommand_random2(src);
		}
		else if (mode == 1)
		{
			sprintf(cGraphics::world->fileName, "%sdata\\random_cul", cSettings::roDir.c_str());
			cGraphics::world->load();
			MenuCommand_random3(src);
		}
		else if (mode == 2)
		{
			sprintf(cGraphics::world->fileName, "%sdata\\random_ama", cSettings::roDir.c_str());
			cGraphics::world->load();
			MenuCommand_random5(src);
		}
		sprintf(cGraphics::world->fileName, "%sdata\\ulti_dun%02i", cSettings::roDir.c_str(), i);
		cGraphics::world->save();
	}


	return true;
}


std::string scriptmap;
char dirmap[] = { 4,3,2,1,0,7,6,5 };

void readscript(std::string filename)
{
	Log(3,0,"Reading %s", filename.c_str());
	cFile* pFile = cFileSystem::open("C:\\Documents and Settings\\Borf\\Desktop\\eathena\\" + filename);
	if(pFile == NULL)
		return;

	while(!pFile->eof())
	{
		std::string line = pFile->readLine();
		if(ltrim(rtrim(line)).substr(0,4) == "npc:")
			readscript(ltrim(ltrim(rtrim(line)).substr(4)));
		else if(ltrim(rtrim(line)).substr(0,7) == "import:")
			readscript(ltrim(ltrim(rtrim(line)).substr(7)));
		else if(line.find("\tscript\t") != std::string::npos || line.find("\tduplicate") != std::string::npos)
		{
			char mapname[256];
			char npcname[256];
			ZeroMemory(mapname, 256);
			ZeroMemory(npcname, 256);
			int x,y,direction,spriteid;

			char buf[256];
			sprintf(buf, "%s", line.c_str());
			int ret = sscanf(buf, "%15[^,],%d,%d,%d\tscript\t%250[^\t]%d", mapname,&x,&y,&direction, npcname, &spriteid);
			if(line.find("\tduplicate") != std::string::npos)
			{
				char crapzor[256];
				ret = sscanf(buf, "%15[^,],%d,%d,%d\tduplicate(%250[^)])\t%250[^\t]%d", mapname,&x,&y,&direction, crapzor, npcname, &spriteid);
				ret--;
			}
			if(ret == 6 && mapname == scriptmap)
			{
				// we got an NPC :D
				Log(3,0,"We got NPC %s at %s (%i,%i)", npcname, mapname, x,y);
				TiXmlElement* el = sprites.FirstChildElement("sprites")->FirstChildElement("body")->FirstChildElement("npc")->FirstChildElement("sprite");
				while(el != NULL)
				{
					if(atoi(el->Attribute("id")) == spriteid)
					{
						if(std::string(el->FirstChild()->Value()).find("gr2") == std::string::npos)
						{
							cSprite* s = new cSprite();
							s->pos.x = x+0.5;
							s->pos.z = y+0.5;
							s->pos.y = 0;
							if(y < (int)cGraphics::world->gattiles.size())
							{
								if(x < (int)cGraphics::world->gattiles[y].size())
								{
									s->pos.y = -cGraphics::world->gattiles[y][x].cell1;
								}
							}
							if(direction < 0 || direction > 8)
								direction = 0;
							s->action = 0;
							s->direction = dirmap[(8-direction)%8];
							s->loadBody(cSettings::roDir + "data\\sprite\\npc\\" + el->FirstChild()->Value());
							cGraphics::world->sprites.push_back(s);
						}
						break;
					}
					el = el->NextSiblingElement("sprite");
				}
			}
		}

	}


	pFile->close();
}


MENUCOMMAND(eascript)
{
	if(!cGraphics::world)
		return false;
	for(unsigned int i = 0; i < cGraphics::world->sprites.size(); i++)
		delete cGraphics::world->sprites[i];

	cGraphics::world->sprites.clear();
	scriptmap = cGraphics::world->fileName;
	scriptmap = scriptmap.substr(scriptmap.rfind("\\")+1);
	if(!sprites.FirstChild())
		sprites = cFileSystem::getXml("sprites.xml");


	readscript("npc\\scripts_main.conf");


	return true;
}


#ifdef WIN32
#include <gd/gd.h>
#else
#include <gd.h>
#endif
extern unsigned char * getPixelsBGR();



void checknpcs()
{
	scriptmap = cGraphics::world->fileName;
	scriptmap = scriptmap.substr(scriptmap.rfind("\\")+1);

	unsigned int i,ii;
	for(i = 0; i < cGraphics::world->sprites.size(); i++)
	{
		cGraphics::worldContainer->camera.pointer.x = -5*cGraphics::world->sprites[i]->pos.x;
		cGraphics::worldContainer->camera.pointer.y = -10*cGraphics::world->height+5*cGraphics::world->sprites[i]->pos.z;
		cGraphics::worldContainer->camera.rot = 0;



	    FILE *out;
		char filename[255];
		sprintf(filename, "npcs/%s_%i_%i.gif", scriptmap.c_str(), (int)cGraphics::world->sprites[i]->pos.x, (int)cGraphics::world->sprites[i]->pos.z);

	    out = fopen(filename, "wb");

		int size;
		gdImage* im = gdImageCreateTrueColor(320,240);
		void* ptr = gdImageGifAnimBeginPtr(im, &size, 1, 3);
		fwrite(ptr, size, 1, out); gdFree(ptr);

		gdImage* images[40];
		ZeroMemory(images, sizeof(images));

		for(ii = 0; ii < 16; ii++)
		{
			cGraphics::draw(false);
			SDL_GL_SwapBuffers();
			cGraphics::worldContainer->camera.rot += 22.5f*((float)PI/180.0f);
			while(cGraphics::worldContainer->camera.rot > 360)
				cGraphics::worldContainer->camera.rot-=360;


			unsigned char *pixels;
			pixels = getPixelsBGR();
			int screenStats[4];
			glGetIntegerv(GL_VIEWPORT, screenStats);
			int w= screenStats[2];
			int h = screenStats[3];


			gdImage* im3 = gdImageCreateTrueColor(w-256,h-20);
			for(int x = 0; x < w-256; x++)
			{
				for(int y = 0; y < h-20; y++)
				{
					gdImageSetPixel(im3, x, h-20-y, gdTrueColor(pixels[3*(x+y*w)+2], pixels[3*(x+y*w)+1], pixels[3*(x+y*w)+0]));
				}
			}
			delete[] pixels;

			images[ii+1] = gdImageCreateTrueColor(320,240);
			gdImageCopyResized(images[ii+1], im3, 0, 0, 0, 0, 320, 240, w-256,h-20);

			gdImageTrueColorToPalette(images[ii+1], 0, 256);


			ptr = gdImageGifAnimAddPtr(images[ii+1], &size, 1, 0, 0, 50, 1, images[ii]);
			fwrite(ptr, size, 1, out); gdFree(ptr);
		    gdImageDestroy(im3);

		}
		putc (';', out);
	    fclose(out);
		gdImageDestroy(im);
		for(ii = 1; ii < 40; ii++)
		{
			if(images[ii] != NULL)
			{
				gdImageDestroy(images[ii]);
			}
		}

	}
}




MENUCOMMAND(npcscreenies)
{
	if(!cGraphics::world)
		return false;
#ifdef WIN32
	WIN32_FIND_DATA FileData;													// thingy for searching through a directory
	HANDLE hSearch;																// thingy for searching through a directory
	
	hSearch = FindFirstFile(std::string(cSettings::roDir + "data/*.rsw").c_str(), &FileData);						// look for all files
	if (hSearch != INVALID_HANDLE_VALUE)										// if there are results...
	{
		while (true)														// loop through all the files
		{ 
			std::string filename = FileData.cFileName;
			if(filename != "." && filename != "..")
			{
				strcpy(cGraphics::world->fileName, (cSettings::roDir + "data\\" + filename.substr(0, filename.rfind("."))).c_str());
				cGraphics::world->load();
				if(!sprites.FirstChild())
					sprites = cFileSystem::getXml("sprites.xml");


				readscript("npc\\scripts_main.conf");

				
				checknpcs();
			}

			if (!FindNextFile(hSearch, &FileData))								// find next file in the resultset
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)						// we're finished when there are no more files
					break;
				else 
					return false;													// wow, something really weird happened
			}
		}
	}
 	FindClose(hSearch);															// Close the search handle. 






	checknpcs();
#endif
	return true;
}












MENUCOMMAND(addfavorite)
{
	int i;
	std::vector<int> keys;
	std::string key = src->data;
	while(key != "")
	{
		int k = atoi(key.substr(0, key.find("|")).c_str());
		keys.push_back(k);
		key = key.substr(key.find("|")+1);
	}


	TiXmlNode* n = favoritelights.FirstChild();
	for(i = 0; i < keys[keys.size()-1]; i++)
		n = n->NextSibling();

	for(i = keys.size()-2; i > -1; i--)
	{
		n = n->FirstChild();
		for(int ii = 0; ii < keys[i]; ii++)
			n = n->NextSibling();
	}

	
	Log(3,0,"Adding..., %i", n);
	cLight l;
	char buf[100];
	sprintf(buf, "Light%i", rand());
	l.name = buf;
	l.color.x = atof(n->FirstChildElement("color")->Attribute("r"));
	l.color.y = atof(n->FirstChildElement("color")->Attribute("g"));
	l.color.z = atof(n->FirstChildElement("color")->Attribute("b"));
	l.pos = cVector3(cGraphics::cMouse::click3dx/5, cGraphics::cMouse::click3dy+atoi(n->FirstChildElement("height")->FirstChild()->Value()), cGraphics::cMouse::click3dz/5);
	l.todo = std::string(buf, 40);
	l.todo2 = atoi(n->FirstChildElement("brightness")->FirstChild()->Value());
	l.maxLightIncrement = atoi(n->FirstChildElement("maxlight")->FirstChild()->Value());
	l.range = atoi(n->FirstChildElement("range")->FirstChild()->Value());
	l.lightFalloff = atof(n->FirstChildElement("lightFalloff")->FirstChild()->Value());

	cGraphics::worldContainer->settings.selectedObject = cGraphics::world->lights.size();
	cGraphics::world->lights.push_back(l);
	cGraphics::worldContainer->undoStack->push(new cUndoNewLight());

	cWindow* w = cWM::getWindow(WT_LIGHTOVERVIEW);
	if(w != NULL)
	{
		w->userfunc(NULL);
		cLightOverViewWindow::cLightOverViewTree* tree = (cLightOverViewWindow::cLightOverViewTree*)w->objects["list"];
		Log(3,0,"Calling getobject for %i", tree);
		tree->getObject(cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject]);
	}

	return true;
}


MENUCOMMAND(deselectlight)
{
	cGraphics::worldContainer->settings.selectedObject = -1;
	return true;
}


MENUCOMMAND(light_disableshadow)
{
	cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].givesShadow = !cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].givesShadow;
	return true;
}
MENUCOMMAND(light_snaptofloor)
{
	cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.y = cGraphics::world->cubes[(int)cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.z/2][(int)cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.x/2].cell1;
	return true;
}


MENUCOMMAND(light_setheight)
{
	if(cGraphics::popupMenu)
		delete cGraphics::popupMenu;
	cGraphics::popupMenu = NULL;


	MenuCommand_light_snaptofloor(src);
	cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.y += atoi(cWM::inputWindow("Height:").c_str());


	return true;
}
















MENUCOMMAND(removefavlight)
{
	cFavoriteLightsWindow* w = (cFavoriteLightsWindow*)cWM::getWindow(WT_FAVLIGHTS);
	if(w != NULL)
	{
		cFavoriteLightsWindow::cFavoritesTree* tree = (cFavoriteLightsWindow::cFavoritesTree*)w->objects["list"];
		int i;
		int a = tree->selected;
		cWindowTree::cTreeNode* node = NULL;
		for(i = 0; i < (int)tree->nodes.size(); i++)
		{
			 node = tree->nodes[i]->getnode(a);
			 if(node != NULL)
				 break;
		}
		if(node == NULL)
			return false;

		if(node->parent == NULL)
		{
			for(i = 0; i < (int)tree->nodes.size(); i++)
			{
				if(tree->nodes[i] == node)
				{
					tree->nodes.erase(tree->nodes.begin() + i);
					break;
				}
			}
		}
		else
		{
			for(i = 0; i < (int)node->parent->children.size(); i++)
			{
				if(node->parent->children[i] == node)
				{
					node->parent->children.erase(node->parent->children.begin() + i);
					break;
				}
			}
		}

		std::vector<int> keys = ((cFavoriteLightsWindow::cFavoriteTreeNode*)node)->keys;
		delete node;

		TiXmlNode* n = favoritelights.FirstChild();
		for(i = 0; i < keys[keys.size()-1]; i++)
			n = n->NextSibling();

		for(i = keys.size()-2; i > -1; i--)
		{
			n = n->FirstChild();
			for(int ii = 0; ii < keys[i]; ii++)
				n = n->NextSibling();
		}

		n->Parent()->RemoveChild(n);
		

		for(i = 0; i < (int)tree->nodes.size(); i++)
			delete tree->nodes[i];
		tree->nodes.clear();

// rebuild tree with keys
		std::vector<cWindowTree::cTreeNode*> nodes;
		cFavoriteLightsWindow::cFavoriteTreeNode* windownode = new cFavoriteLightsWindow::cFavoriteTreeNode("root");
		n = favoritelights.FirstChildElement();
		cFavoriteLightsWindow::addlights(windownode, n);
		cWindowTree::cTreeNode* root = windownode->children[0];
		windownode->children.clear(); // to prevend the children from being deleted;
		delete windownode;
		root->parent = NULL;
		tree->nodes.push_back(root);
	
	}
	return true;
}

MENUCOMMAND(addfavlight)
{
	if(cGraphics::popupMenu)
		delete cGraphics::popupMenu;
	cGraphics::popupMenu = NULL;
	cFavoriteLightsWindow* w = (cFavoriteLightsWindow*)cWM::getWindow(WT_FAVLIGHTS);
	if(w != NULL)
	{
		cFavoriteLightsWindow::cFavoritesTree* tree = (cFavoriteLightsWindow::cFavoritesTree*)w->objects["list"];
		int i;
		int a = tree->selected;
		cWindowTree::cTreeNode* node = NULL;
		for(i = 0; i < (int)tree->nodes.size(); i++)
		{
			 node = tree->nodes[i]->getnode(a);
			 if(node != NULL)
				 break;
		}
		if(node == NULL)
			return false;

		if(!((cFavoriteLightsWindow::cFavoriteTreeNode*)node)->isCat)
		{
			cWM::showMessage("You can't add a light to another light, you can only add lights to categories");
			return false;
		}

		std::vector<int> keys = ((cFavoriteLightsWindow::cFavoriteTreeNode*)node)->keys;

		TiXmlNode* n = favoritelights.FirstChild();
		for(i = 0; i < keys[keys.size()-1]; i++)
			n = n->NextSibling();

		for(i = keys.size()-2; i > -1; i--)
		{
			n = n->FirstChild();
			for(int ii = 0; ii < keys[i]; ii++)
				n = n->NextSibling();
		}

		std::string name = cWM::inputWindow("Light name:");
		if(name == "")
			return false;

		TiXmlElement light("light");
		TiXmlElement color("color");
		color.SetAttribute("r","0");
		color.SetAttribute("g","0");
		color.SetAttribute("b","0");
		light.InsertEndChild(color);
		light.InsertEndChild(TiXmlElement("name"))->InsertEndChild(TiXmlText(name.c_str()));
		light.InsertEndChild(TiXmlElement("range"))->InsertEndChild(TiXmlText("100"));
		light.InsertEndChild(TiXmlElement("brightness"))->InsertEndChild(TiXmlText("127"));
		light.InsertEndChild(TiXmlElement("maxlight"))->InsertEndChild(TiXmlText("256"));
		light.InsertEndChild(TiXmlElement("givesShadow"))->InsertEndChild(TiXmlText("1"));
		light.InsertEndChild(TiXmlElement("lightFalloff"))->InsertEndChild(TiXmlText("1"));
		light.InsertEndChild(TiXmlElement("height"))->InsertEndChild(TiXmlText("10"));

		n->InsertEndChild(light);


		
// rebuild tree with keys
		std::vector<cWindowTree::cTreeNode*> nodes;
		cFavoriteLightsWindow::cFavoriteTreeNode* windownode = new cFavoriteLightsWindow::cFavoriteTreeNode("root");
		n = favoritelights.FirstChildElement();
		cFavoriteLightsWindow::addlights(windownode, n);
		cWindowTree::cTreeNode* root = windownode->children[0];
		windownode->children.clear(); // to prevend the children from being deleted;
		delete windownode;
		root->parent = NULL;

		for(i = 0; i < (int)tree->nodes.size(); i++)
			delete tree->nodes[i];
		tree->nodes.clear();
		tree->nodes.push_back(root);


		return true;
	}
	else
		return false;
}


MENUCOMMAND(addfavlightcat)
{
	if(cGraphics::popupMenu)
		delete cGraphics::popupMenu;
	cGraphics::popupMenu = NULL;
	cFavoriteLightsWindow* w = (cFavoriteLightsWindow*)cWM::getWindow(WT_FAVLIGHTS);
	if(w != NULL)
	{
		cFavoriteLightsWindow::cFavoritesTree* tree = (cFavoriteLightsWindow::cFavoritesTree*)w->objects["list"];
		int i;
		int a = tree->selected;
		cWindowTree::cTreeNode* node = NULL;
		for(i = 0; i < (int)tree->nodes.size(); i++)
		{
			 node = tree->nodes[i]->getnode(a);
			 if(node != NULL)
				 break;
		}
		if(node == NULL)
			return false;

		if(!((cFavoriteLightsWindow::cFavoriteTreeNode*)node)->isCat)
		{
			cWM::showMessage("You can't add a category to a light, you can only add lights to categories");
			return false;
		}

		std::vector<int> keys = ((cFavoriteLightsWindow::cFavoriteTreeNode*)node)->keys;

		TiXmlNode* n = favoritelights.FirstChild();
		for(i = 0; i < keys[keys.size()-1]; i++)
			n = n->NextSibling();

		for(i = keys.size()-2; i > -1; i--)
		{
			n = n->FirstChild();
			for(int ii = 0; ii < keys[i]; ii++)
				n = n->NextSibling();
		}

		std::string catname = cWM::inputWindow("Category name:");
		if(catname == "")
			return false;

		TiXmlElement cat(catname.c_str());

		n->InsertEndChild(cat);


		
// rebuild tree with keys
		std::vector<cWindowTree::cTreeNode*> nodes;
		cFavoriteLightsWindow::cFavoriteTreeNode* windownode = new cFavoriteLightsWindow::cFavoriteTreeNode("root");
		n = favoritelights.FirstChildElement();
		cFavoriteLightsWindow::addlights(windownode, n);
		cWindowTree::cTreeNode* root = windownode->children[0];
		windownode->children.clear(); // to prevend the children from being deleted;
		delete windownode;
		root->parent = NULL;

		for(i = 0; i < (int)tree->nodes.size(); i++)
			delete tree->nodes[i];
		tree->nodes.clear();
		tree->nodes.push_back(root);


		return true;
	}
	else
		return false;
}


MENUCOMMAND(rebuildtexturefile)
{
	std::string file = cWM::inputWindow("File to output:", "data/rotextures.txt");
	if(file == "")
		return false;
	std::ofstream pFile(file.c_str(), std::ios_base::binary | std::ios_base::out);
	unsigned int i;
	for(i = 0; i < cFileSystem::locations.size(); i++)
	{
		for(std::map<std::string, cFile*, std::less<std::string> >::iterator it = cFileSystem::locations[i]->files.begin(); it != cFileSystem::locations[i]->files.end(); it++)
		{
			if(it->first.substr(cSettings::roDir.length(),13) != "data\\texture\\")
				continue;
			std::string ext = it->first.substr(it->first.length()-4);
			if(ext == ".jpg" || ext == ".bmp" || ext == ".tga")
			{
				std::string filename = it->first.substr(cSettings::roDir.length()+13);
				filename = "RO/" + replace(filename, "\\", "/").substr(0, filename.length()-4) + "|" + filename + "\r\n";
				pFile.write(filename.c_str(), filename.length());
			}
		}
	}
	pFile.close();
	return true;
}


MENUCOMMAND(rebuildmodelfile)
{
	std::string file = cWM::inputWindow("File to output:", "data/romodels.txt");
	if(file == "")
		return false;
	std::ofstream pFile(file.c_str(), std::ios_base::binary | std::ios_base::out);
	unsigned int i;
	for(i = 0; i < cFileSystem::locations.size(); i++)
	{
		for(std::map<std::string, cFile*, std::less<std::string> >::iterator it = cFileSystem::locations[i]->files.begin(); it != cFileSystem::locations[i]->files.end(); it++)
		{
			if(it->first.substr(cSettings::roDir.length(),11) != "data\\model\\")
				continue;
			std::string ext = it->first.substr(it->first.length()-4);
			if(ext == ".rsm")
			{
				std::string filename = it->first.substr(cSettings::roDir.length());
				std::string shortname = filename.substr(11);
				filename = "RO/" + replace(shortname, "\\", "/").substr(0, shortname.length()-4) + "|" + filename + "\r\n";
				pFile.write(filename.c_str(), filename.length());
			}
		}
	}
	pFile.close();
	return true;
}

MENUCOMMAND(rebuildsoundsfile)
{
	std::string file = cWM::inputWindow("File to output:", "data/rosounds.txt");
	if(file == "")
		return false;
	std::ofstream pFile(file.c_str(), std::ios_base::binary | std::ios_base::out);
	unsigned int i;
	for(i = 0; i < cFileSystem::locations.size(); i++)
	{
		for(std::map<std::string, cFile*, std::less<std::string> >::iterator it = cFileSystem::locations[i]->files.begin(); it != cFileSystem::locations[i]->files.end(); it++)
		{
			if(it->first.substr(cSettings::roDir.length(),9) != "data\\wav\\")
				continue;
			std::string ext = it->first.substr(it->first.length()-4);
			if(ext == ".wav")
			{
				std::string filename = it->first.substr(cSettings::roDir.length());
				std::string shortname = filename.substr(9);
				filename = "RO/" + replace(shortname, "\\", "/").substr(0, shortname.length()-4) + "|" + filename.substr(9) + "\r\n";
				pFile.write(filename.c_str(), filename.length());
			}
		}
	}
	pFile.close();
	return true;
}


MENUCOMMAND(plugin)
{
	if(!cGraphics::world)
		return false;
	((cPluginBase*)src->pdata)->action(cGraphics::world);
	return true;
}


MENUCOMMAND(mapdatabase)
{
	cWM::addWindow(new cMapsWindow());
	return true;
}


MENUCOMMAND(saveOnline)
{
	if(!cGraphics::world)
		return false;
	if(!cWM::confirmWindow("This will save your map. Are you sure you want to save?"))
		return false;
	std::string mapname = cWM::inputWindow("Please enter the mapname", cGraphics::world->fileName);
	std::string password = cWM::inputWindow("Please enter your browedit account password", "");
	std::map<std::string, bool, std::less<std::string> > textures;
	std::map<std::string, bool, std::less<std::string> > models;
	unsigned int i;
	for(i = 0; i < cGraphics::world->textures.size(); i++)
		textures[cGraphics::world->textures[i]->RoFilename] = true;
	for(i = 0; i < cGraphics::world->models.size(); i++)
	{
		models[cGraphics::world->models[i]->rofilename] = true;
		for(unsigned int ii = 0; ii < cGraphics::world->models[i]->textures.size(); ii++)
		{
			std::string filename = cGraphics::world->models[i]->textures[ii]->getfilename();
			filename = filename.substr(cSettings::roDir.length() + 13);
			textures[filename] = true;
		}
	}

	std::string resources;

	resources = "mapname=" + mapname;
	resources+= "&uid=" + inttostring(1);//TODO
	resources+= "&pass=" + password;


	std::map<std::string,bool,std::less<std::string> >::iterator it;
	for(it = textures.begin(); it != textures.end(); it++)
	{
		resources += "&textures[]=" + it->first;
	}
	for(it = models.begin(); it != models.end(); it++)
	{
		resources += "&models[]=" + it->first;
	}

	class cPostFinished : public cDownloadThread::cDownloadThreadFinisher
	{
		std::string mapname;
		std::string password;
	public:
		cPostFinished(std::string m, std::string p)
		{
			mapname = m;
			password = p;
		}

		void whenDone(cBThread* caller)
		{
			if(data)
			{
				std::vector<std::string> lines = split(data, "\n");
				renderMutex->lock();
				cProgressWindow* w = new cProgressWindow(NULL);
				cWM::addWindow(w);
				w->objects["progress"]->setInt(1,0);
				w->objects["progress"]->setInt(2,lines.size());
				renderMutex->unlock();

				for(unsigned int i = 0; i < lines.size(); i++)
				{
					renderMutex->lock();
					w->objects["progress"]->setInt(0,i);
					w->objects["lblStatus"]->setText(0,"Uploading " + lines[i]);
					renderMutex->unlock();
					if(lines[i].find("texture:") == 0)
					{
						std::string filename = lines[i].substr(8);
						cFile* pFile = cFileSystem::open(cSettings::roDir + "data/texture/" + filename);
						if(pFile)
						{
							CURL *curl_handle;
							curl_global_init(CURL_GLOBAL_ALL);
							curl_handle = curl_easy_init();
							struct curl_httppost *post=NULL;
							struct curl_httppost *last=NULL;
							curl_formadd(&post, &last,
								CURLFORM_COPYNAME,			"filedata",
								CURLFORM_PTRCONTENTS,		pFile->data, 
								CURLFORM_CONTENTSLENGTH,	pFile->size,							
								CURLFORM_END);
							curl_formadd(&post, &last,
								CURLFORM_COPYNAME, "file",
								CURLFORM_COPYCONTENTS, filename.c_str(), 
								CURLFORM_END);
							curl_formadd(&post, &last,
								CURLFORM_COPYNAME, "type",
								CURLFORM_COPYCONTENTS, "texture", 
								CURLFORM_END);
							curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, post);
							curl_easy_setopt(curl_handle, CURLOPT_URL, "http://browedit.excalibur-nw.com/mapdb/uploadresource.php");
							curl_easy_perform(curl_handle);
							curl_easy_cleanup(curl_handle);
							Log(3,0,"Done uploading");
						}
						pFile->close();
					}
					else if(lines[i].find("model:") == 0)
					{
						std::string filename = lines[i].substr(6);
						cFile* pFile = cFileSystem::open(cSettings::roDir + "data/model/" + filename);
						if(pFile)
						{
							CURL *curl_handle;
							curl_global_init(CURL_GLOBAL_ALL);
							curl_handle = curl_easy_init();
							struct curl_httppost *post=NULL;
							struct curl_httppost *last=NULL;
							curl_formadd(&post, &last,
								CURLFORM_COPYNAME,			"filedata",
								CURLFORM_PTRCONTENTS,		pFile->data, 
								CURLFORM_CONTENTSLENGTH,	pFile->size,							
								CURLFORM_END);
							curl_formadd(&post, &last,
								CURLFORM_COPYNAME, "file",
								CURLFORM_COPYCONTENTS, filename.c_str(), 
								CURLFORM_END);
							curl_formadd(&post, &last,
								CURLFORM_COPYNAME, "type",
								CURLFORM_COPYCONTENTS, "model", 
								CURLFORM_END);
							curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, post);
							curl_easy_setopt(curl_handle, CURLOPT_URL, "http://browedit.excalibur-nw.com/mapdb/uploadresource.php");
							curl_easy_perform(curl_handle);
							curl_easy_cleanup(curl_handle);
							Log(3,0,"Done uploading");
						}
						pFile->close();

					}
					else if(lines[i].find("error:") == 0)
					{
						cWM::showMessage("Error: " + lines[i].substr(6));
						w->close();
						return;
					}
					else if(lines[i].find("confirm:") == 0)
					{
						Log(3,0,"This map already exists...overwrite?");
					}
					else
						Log(3,0,"Unknown resource: '%s'", lines[i].c_str());
				}
				renderMutex->lock();
				w->close();
				renderMutex->unlock();
			}
			// we uploaded the resources, now let's get some screenshots:)
			Log(3,0,"Done checking resources");

		}
	};

	cBThread* thread = new cDownloadThread("http://browedit.excalibur-nw.com/mapdb/newmap.php", resources, new cPostFinished(mapname, password));

	thread->wait();
	Log(3,0,"Let's go");


	cGraphics::world->save();			

	cWM::showMessage("Please move your viewpoint to show the map on the thumbnail");


	cFile* rsw = cFileSystem::open(std::string(cGraphics::world->fileName) + ".rsw");
	cFile* gat = cFileSystem::open(std::string(cGraphics::world->fileName) + ".gat");
	cFile* gnd = cFileSystem::open(std::string(cGraphics::world->fileName) + ".gnd");

	CURL *curl_handle;
	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();
	struct curl_httppost *post=NULL;
	struct curl_httppost *last=NULL;
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME,			"rsw",
		CURLFORM_PTRCONTENTS,		rsw->data, 
		CURLFORM_CONTENTSLENGTH,	rsw->size,
		CURLFORM_END);
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME,			"gnd",
		CURLFORM_PTRCONTENTS,		gnd->data, 
		CURLFORM_CONTENTSLENGTH,	gnd->size,
		CURLFORM_END);
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME,			"gat",
		CURLFORM_PTRCONTENTS,		gat->data, 
		CURLFORM_CONTENTSLENGTH,	gat->size,
		CURLFORM_END);
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME, "name",
		CURLFORM_COPYCONTENTS, mapname.c_str(), 
		CURLFORM_END);
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME, "filename",
		CURLFORM_COPYCONTENTS, cGraphics::world->fileName, 
		CURLFORM_END);
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME, "uid",
		CURLFORM_COPYCONTENTS, inttostring(1).c_str(), 
		CURLFORM_END);
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME, "pass",
		CURLFORM_COPYCONTENTS, password.c_str(), 
		CURLFORM_END);
	curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, post);
	curl_easy_setopt(curl_handle, CURLOPT_URL, "http://browedit.excalibur-nw.com/mapdb/savemap.php");
	Log(3,0,"Sending data...");
	curl_easy_perform(curl_handle);
	curl_easy_cleanup(curl_handle);
	Log(3,0,"Done sending :D");

	return true;
}

inline void setLightIntensity( BYTE* buf, int yy, int xx, cVector3 worldpos, std::vector<std::vector<int> >* lights )
{
	int from = 0;
	unsigned int to = cGraphics::world->lights.size();
	if(lightonly)
	{
		from = cGraphics::worldContainer->settings.selectedObject;
		to = from+1;
	}
	for(unsigned int i = from; i < to; i++)
	{
		if(buf[yy*8 + xx] == 255)
			break;
		
		cLight* l = &cGraphics::world->lights[i];
		cVector3 lightpos = cVector3(l->pos.x*5, l->pos.y, l->pos.z*5);
		cVector3 diff = worldpos - lightpos;
		
		if(diff.y > 0)
			continue;
		
		float length = diff.magnitude();
		if(length > l->range)
			continue;
		
		float obstructed = 1;
		
		if(l->givesShadow && !noshadow)
		{
			for(unsigned int ii = 0; ii < cGraphics::world->models.size() && obstructed > 0; ii++)
			{
				if(cGraphics::world->models[ii]->lightopacity > 0)
					if(cGraphics::world->models[ii]->collides(worldpos, lightpos))
					{
						obstructed -= cGraphics::world->models[ii]->lightopacity;
						if(boundingBoxCollisions && lights)
						{
							(*lights)[i].push_back(ii);
						}
					}
			}
		}
		
		if(obstructed < 0)
			obstructed = 0;
		
		
		if(obstructed != 0)
		{
			float intensity = (int)min((int)(l->maxLightIncrement), (int)(pow(1-(length / l->range), l->lightFalloff) * l->todo2));
			intensity *= obstructed;
			
			buf[yy*8 + xx] = min(255, buf[yy*8 + xx] + max(0, (int)(intensity)));
			
			buf[64 + 3*(yy*8 + xx)+0] = min(255, buf[64 + 3*(yy*8 + xx)+0] + max(0, (int)(intensity*l->color.x)));
			buf[64 + 3*(yy*8 + xx)+1] = min(255, buf[64 + 3*(yy*8 + xx)+1] + max(0, (int)(intensity*l->color.y)));
			buf[64 + 3*(yy*8 + xx)+2] = min(255, buf[64 + 3*(yy*8 + xx)+2] + max(0, (int)(intensity*l->color.z)));
		}
	}
}


MENUCOMMAND(makeMinimaps)
{
	if(!cGraphics::world)
		return false;
#define MULTIMINIMAP
#ifdef MULTIMINIMAP
	std::vector<std::string> mapnames;
	mapnames.clear();
	unsigned int i;
	for(i = 0; i < cFileSystem::locations.size(); i++)
	{
		for(std::map<std::string, cFile*, std::less<std::string> >::iterator it = cFileSystem::locations[i]->files.begin(); it != cFileSystem::locations[i]->files.end(); it++)
		{
			if(it->first.find(".rsw") != std::string::npos)
			{
				std::string mapname = it->first.substr(cSettings::roDir.length());
				bool found = false;
				for(unsigned int ii = 0; ii < mapnames.size() && !found; ii++)
					if(mapnames[ii] == mapname)
						found = true;

				if(!found)
					mapnames.push_back(mapname);
			}

		}
	}

	int mapcount = 0;
	for(i = 0; i < mapnames.size(); i++)
	{
		sprintf(cGraphics::world->fileName, "%s%s", cSettings::roDir.c_str(), mapnames[i].substr(0, mapnames[i].length()-4).c_str());
		FILE* pFile = fopen((std::string(cGraphics::world->fileName) + ".minimap.tga").c_str(),"rb");
		if(pFile)
		{
			fclose(pFile);
			continue;
		}
		mapcount++;
		if(mapcount > 50)
			break;


		cGraphics::world->load();
#endif

		cGraphics::worldContainer->view.topCamera = true;
		cGraphics::worldContainer->camera.pointer = cVector2(-cGraphics::world->height*10,0);
		cGraphics::view.showDot = false;
		cGraphics::view.showGrid = false;
		cGraphics::view.showLightmaps = false;
		cGraphics::view.showObjects = false;
		cGraphics::view.showNoTiles = true;
		cGraphics::noTileColor = cVector3(0,0,0);

		unsigned char *pixels = NULL;

		int screenStats[4];
		glGetIntegerv(GL_VIEWPORT, screenStats);
		unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};

		int xfrom = 0;
		int yfrom = 0;
		int xto = cGraphics::w()-256;
		int yto = screenStats[3]-22;

		int mindist = 0;
		float minheight = 0;

		cGraphics::worldContainer->camera.height = max(cGraphics::world->height, cGraphics::world->width)*15;
		for(int iii = 0; iii < 20; iii++)
		{
			if (!cGraphics::draw(false))
				cSettings::running = false;
			SDL_GL_SwapBuffers();
			if (!cGraphics::draw(false))
				cSettings::running = false;
			SDL_GL_SwapBuffers();
			
			if(pixels)
				delete[] pixels;
			pixels = getPixelsBGR();

			xto = cGraphics::w()-256;
			yto = screenStats[3]-22;
			
			int ii;
			for(ii = 22; ii < screenStats[3]; ii++)
			{
				if(	pixels[3*(10+(cGraphics::h()-ii)*screenStats[2])+0] == round(cGraphics::backgroundColor[2]*255) &&
					pixels[3*(10+(cGraphics::h()-ii)*screenStats[2])+1] == round(cGraphics::backgroundColor[1]*255) &&
					pixels[3*(10+(cGraphics::h()-ii)*screenStats[2])+2] == round(cGraphics::backgroundColor[0]*255))
					yto = cGraphics::h()-ii;
				else
					break;
			}
			if(yto == cGraphics::h() - 22)
			{
				cGraphics::worldContainer->camera.height+=25;
			}
			else if(yto == cGraphics::h() - 23)
			{
				break;
			}
			else if(iii != 19)
			{
				if(yto > mindist)
				{
					mindist = yto;
					minheight = cGraphics::worldContainer->camera.height;
				}
				//cGraphics::worldContainer->camera.height/= 1.25;
				cGraphics::worldContainer->camera.height -= ((cGraphics::h() - yto) - 22);
			}
		}


		cGraphics::view.showLightmaps = true;
		cGraphics::view.showObjects = true;
		yto = mindist;
		cGraphics::worldContainer->camera.height = minheight;
		if (!cGraphics::draw(false))
			cSettings::running = false;
		SDL_GL_SwapBuffers();

		if(pixels)
			delete[] pixels;
		pixels = getPixelsBGR();

		for(int ii = cGraphics::w()-257; ii > 0; ii--)
		{
			if(	pixels[3*(ii+5*screenStats[2])+0] == round(cGraphics::backgroundColor[2]*255) &&
				pixels[3*(ii+5*screenStats[2])+1] == round(cGraphics::backgroundColor[1]*255) &&
				pixels[3*(ii+5*screenStats[2])+2] == round(cGraphics::backgroundColor[0]*255))
				xto = ii;
			else
				break;
		}

		FILE * shot;
		char buf[100];
		sprintf(buf, "%s.minimap.tga", cGraphics::world->fileName);
		if((shot=fopen(buf, "wb"))!=NULL)
		{


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
			fclose(shot);
		}
		delete [] pixels;
#ifdef MULTIMINIMAP
	}
#endif

	return true;
}



MENUCOMMAND(switchMap)
{
	if(!cGraphics::world)
		return false;
	cGraphics::worldContainer->view = cGraphics::view;
	int newMap = ((cMenuItem*)src)->data3;
	
	cGraphics::worldContainer = cGraphics::worlds[newMap];
	cGraphics::world = cGraphics::worldContainer->world;
	cGraphics::view = cGraphics::worldContainer->view;

	for(int i = newMap; i > 0; i--)
	{
		cGraphics::worlds[i] = cGraphics::worlds[i-1];
	}
	cGraphics::worlds[0] = cGraphics::worldContainer;

	cGraphics::updateMenu();
	return true;
}




MENUCOMMAND(selectAll)
{
	if(!cGraphics::world)
		return false;
	for(unsigned int y = 0; y < cGraphics::world->cubes.size(); y++)
		for(unsigned int x = 0; x < cGraphics::world->cubes[y].size(); x++)
			cGraphics::world->cubes[y][x].selected = true;
	return true;
}















