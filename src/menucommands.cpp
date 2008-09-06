#include <common.h>
#include <commdlg.h>
#include <shellapi.h>
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

extern cGraphics Graphics;
extern bool running;
extern cUndoStack undostack;
extern eMode editmode;
extern int brushsize;
extern double mouse3dx, mouse3dy, mouse3dz;
int process_events( );
extern std::vector<std::string> objectfiles;
extern cFileSystem fs;
extern std::string rodir;
extern cMenu* mode;
extern cMenu* editdetail;
extern cMenu* speed;
extern cMenu* models;
extern cMenu* currentobject;
extern float paintspeed;
extern TiXmlDocument sprites;
extern double mouseclickx, mouseclicky, mouseclickz;
extern std::string message;
extern bool showmessage;
extern long userid;

void mainloop();
#include <bthread.h>
extern cBMutex* renderMutex;

#include <curl/curl.h>


cMenuItem* selectedeffect = NULL;

MENUCOMMAND(new)
{
	Graphics.WM.ShowMessage("This feature isn't working yet...");
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

	strcpy(Graphics.world.fileName, replace(Graphics.world.fileName, "/", "\\").c_str());
	ofn.lpstrFile = Graphics.world.fileName;
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = "All\0*.*\0RO Maps\0*.rsw\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	if (GetOpenFileName(&ofn))
	{
		while(Graphics.world.fileName[strlen(Graphics.world.fileName)-1] != '.')
			Graphics.world.fileName[strlen(Graphics.world.fileName)-1] = '\0';
		Graphics.world.fileName[strlen(Graphics.world.fileName)-1] = '\0';

		chdir(curdir);
		Graphics.world.load();
	}
#else


#endif
	return true;
}




void openfunc(std::string param)
{
	ZeroMemory(Graphics.world.fileName, 128);
	memcpy(Graphics.world.fileName, param.c_str(), param.length());
	Graphics.world.load();
}

MENUCOMMAND(opengrf)
{
	Graphics.WM.addwindow(new cFileWindow(Graphics.WM.texture, Graphics.WM.font, openfunc,Graphics.WM.skin));
	return true;
}

MENUCOMMAND(save)
{
#ifdef WIN32
	if(Graphics.world.fileName[0] == '\0')
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
		strcpy(Graphics.world.fileName, replace(Graphics.world.fileName, "/", "\\").c_str());
		ofn.lpstrFile = Graphics.world.fileName;
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
		ofn.nFilterIndex = 2;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
		if (GetSaveFileName(&ofn))
		{
			while(Graphics.world.fileName[strlen(Graphics.world.fileName)-1] != '.')
				Graphics.world.fileName[strlen(Graphics.world.fileName)-1] = '\0';
			Graphics.world.fileName[strlen(Graphics.world.fileName)-1] = '\0';
			Graphics.world.save();
		}
		chdir(curdir);
	}
	else
		Graphics.world.save();
#else

#endif

	return true;
}


MENUCOMMAND(quicksave)
{
	Graphics.world.quickSave = true;
#ifdef WIN32
	if(Graphics.world.fileName[0] == '\0')
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
		strcpy(Graphics.world.fileName, replace(Graphics.world.fileName, "/", "\\").c_str());
		ofn.lpstrFile = Graphics.world.fileName;
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
		ofn.nFilterIndex = 2;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
		if (GetSaveFileName(&ofn))
		{
			while(Graphics.world.fileName[strlen(Graphics.world.fileName)-1] != '.')
				Graphics.world.fileName[strlen(Graphics.world.fileName)-1] = '\0';
			Graphics.world.fileName[strlen(Graphics.world.fileName)-1] = '\0';
			Graphics.world.save();
		}
		chdir(curdir);
	}
	else
		Graphics.world.save();
#else

#endif

	Graphics.world.quickSave = false;

	return true;
}

MENUCOMMAND(saveAs)
{
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
	strcpy(Graphics.world.fileName, replace(Graphics.world.fileName, "/", "\\").c_str());
	ofn.lpstrFile = Graphics.world.fileName;
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
	if (!GetSaveFileName(&ofn))
	{
		Graphics.world.fileName[0] = '\0';
		chdir(curdir);
	}
	else
	{
		if (strcmp(Graphics.world.fileName+strlen(Graphics.world.fileName)-4, ".rsw") == 0)
		{ 
			while(Graphics.world.fileName[strlen(Graphics.world.fileName)-1] != '.')
				Graphics.world.fileName[strlen(Graphics.world.fileName)-1] = '\0';
			Graphics.world.fileName[strlen(Graphics.world.fileName)-1] = '\0';
		}
		chdir(curdir);
		Graphics.world.save();

	}
#else
	std::string input = Graphics.WM.InputWindow("Filename (so not add .rsw): ", Graphics.world.fileName);
	if(input != "")
	{
		sprintf(Graphics.world.fileName, "%s", input.c_str());
		Graphics.world.save();
		
	}

#endif
	return true;
}
MENUCOMMAND(exit)
{
	running = false;
	return true;
}	
MENUCOMMAND(undo)
{
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
	GLfloat d = dir.Dot(pN);;
	
	if (d < -0.00001f)
		return 1;
	else
		if (d > 0.00001f)
			return -1;
		return 0;
}


MENUCOMMAND(random1)
{
	int height = atoi(Graphics.WM.InputWindow("Height:").c_str());
	int smooth  = atoi(Graphics.WM.InputWindow("Smoothing level (use 5-10 for decent results)").c_str());
	if(height == 0)
	{
		Graphics.WM.ShowMessage("You must enter a height bigger then 0");
		return true;
	}

	undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));
	int x,y;
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[y][x].tileOtherSide = -1;
			Graphics.world.cubes[y][x].tileSide = -1;
		}
	}
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
		//	Graphics.world.cubes[2*y][2*x].tileUp = 1;

			Graphics.world.cubes[y][x].cell1 = height/2-rand()%height;
			Graphics.world.cubes[y][x].cell2 = height/2-rand()%height;
			Graphics.world.cubes[y][x].cell3 = height/2-rand()%height;
			Graphics.world.cubes[y][x].cell4 = height/2-rand()%height;

/*			Graphics.world.cubes[2*y][2*x-1].cell2 =	Graphics.world.cubes[2*y][2*x].cell1;
			Graphics.world.cubes[2*y-1][2*x-1].cell4 =	Graphics.world.cubes[2*y][2*x].cell1;
			Graphics.world.cubes[2*y-1][2*x].cell3 =	Graphics.world.cubes[2*y][2*x].cell1;


			Graphics.world.cubes[2*y][2*x+1].cell1 =	Graphics.world.cubes[2*y][2*x].cell2;
			Graphics.world.cubes[2*y-1][2*x+1].cell3 =	Graphics.world.cubes[2*y][2*x].cell2;
			Graphics.world.cubes[2*y-1][2*x].cell4 =	Graphics.world.cubes[2*y][2*x].cell2;
		

			Graphics.world.cubes[2*y][2*x-1].cell4 =	Graphics.world.cubes[2*y][2*x].cell3;
			Graphics.world.cubes[2*y+1][2*x-1].cell2 =	Graphics.world.cubes[2*y][2*x].cell3;
			Graphics.world.cubes[2*y+1][2*x].cell1 =	Graphics.world.cubes[2*y][2*x].cell3;

			Graphics.world.cubes[2*y][2*x+1].cell3 =	Graphics.world.cubes[2*y][2*x].cell4;
			Graphics.world.cubes[2*y+1][2*x+1].cell1 =	Graphics.world.cubes[2*y][2*x].cell4;
			Graphics.world.cubes[2*y+1][2*x].cell2 =	Graphics.world.cubes[2*y][2*x].cell4;
		*/
		}
	}

	eMode m = editmode;
	editmode = MODE_HEIGHTDETAIL;

	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_s;
	for(int i = 0; i < smooth; i++)
		SDL_PushEvent(&ev);

	int b = brushsize;
	mouse3dx = Graphics.world.width*5;
	mouse3dz = Graphics.world.height*5;
	brushsize = Graphics.world.width+Graphics.world.height;
	
	process_events();
	brushsize = b;


	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_s;
	SDL_PushEvent(&ev);
	editmode = m;

	return true;
}

MENUCOMMAND(random2)
{
	unsigned int i;
	unsigned int smooth  = 3;//atoi(Graphics.WM.InputWindow("Smoothing level (use 5-10 for decent results)").c_str());

	undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));
	float x,y;

	Graphics.world.tiles.clear();
	for(int tex = 0; tex < 3; tex++)
	{
		for(y = 0; y < 5; y++)
		{
			for(x = 0; x < 5; x++)
			{
				cTile t;
				t.lightmap = 1;
				t.texture = tex;
				t.u1 = x/5.0;
				t.v1 = y/5.0;
				t.u2 = (x+1)/5.0;
				t.v2 = (y)/5.0;
				t.u3 = (x)/5.0;
				t.v3 = (y+1)/5.0;
				t.u4 = (x+1)/5.0;
				t.v4 = (y+1)/5.0;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				Graphics.world.tiles.push_back(t);
			}
		}
	}
	Graphics.draw();
	SDL_GL_SwapBuffers();
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[(int)y][(int)x].tileOtherSide = -1;
			Graphics.world.cubes[(int)y][(int)x].tileSide = -1;
			Graphics.world.cubes[(int)y][(int)x].tileUp = 0;
		}
	}
	Graphics.draw();
	SDL_GL_SwapBuffers();


	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[(int)y][(int)x].cell1 = -64;
			Graphics.world.cubes[(int)y][(int)x].cell2 = -64;
			Graphics.world.cubes[(int)y][(int)x].cell3 = -64;
			Graphics.world.cubes[(int)y][(int)x].cell4 = -64;
		}
	}

	
//	x = 1 + (rand()%((Graphics.world.width/10)-2));
//	y = 1 + (rand()%((Graphics.world.height/10)-2));

	x = Graphics.world.width/2;
	y = Graphics.world.height/2;

	
	int a = rand()%360;
	int lasta = a;
	int reali = 0;
	bool filledenough = false;
	while(!filledenough) //(Graphics.world.height+Graphics.world.width) / 25
	{
		Graphics.draw();
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
					Graphics.world.cubes[(int)y+yy][(int)x+xx].cell1 = water ? 30 : 0;//rand()%25;
					Graphics.world.cubes[(int)y+yy][(int)x+xx].cell2 = water ? 30 : 0;//rand()%25;
					Graphics.world.cubes[(int)y+yy][(int)x+xx].cell3 = water ? 30 : 0;//rand()%25;
					Graphics.world.cubes[(int)y+yy][(int)x+xx].cell4 = water ? 30 : 0;//rand()%25;
				}
			}

			x+=cos(a* (PI/180.0f));
			y+=sin(a* (PI/180.0f));


			if(y < 5)
			{
				y = 5;
				break;
			}
			if(y >= (Graphics.world.height)-15)
			{
				y = (Graphics.world.height)-15;
				break;
			}
			if(x < 5)
			{
				x = 5;
				break;
			}
			if(x >= (Graphics.world.width)-15)
			{
				x = (Graphics.world.width)-15;
				break;
			}
		}
		

		int count = 0;
		for(int yy = 0; yy < Graphics.world.height; yy++)
		{
			for(int xx = 0; xx < Graphics.world.width; xx++)
			{
				if(Graphics.world.cubes[yy][xx].cell1 == 0)
					count++;
			}
		}
		if(count > Graphics.world.height*Graphics.world.width / 2)
			filledenough = true;

	}

	std::vector<std::string>	randommodels;
	for(i = 0; i < objectfiles.size(); i++)
	{
		cFile* pFile = fs.open(objectfiles[i]);
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
	for(i = 0; i < Graphics.world.models.size(); i++)
		delete Graphics.world.models[i];
	Graphics.world.models.clear();

	for(i = 0; i < 50; i++)
	{
		cRSMModel* model = new cRSMModel();
		model->load(rodir +  randommodels[rand() % randommodels.size()]);

		bool ok = false;
		while(!ok || Graphics.world.cubes[(int)(model->pos.z/2)][(int)(model->pos.x/2)].cell1 != 0)
		{
			model->pos = cVector3(rand()%(Graphics.world.width*2), 0, rand()%(Graphics.world.height*2));
			ok = true;
			for(int x = -4; x < 4; x++)
			{
				for(int y = -4; y < 4; y++)
				{
					if(model->pos.z/2+x < 0 || model->pos.z/2+x >= Graphics.world.height-1 ||
						model->pos.x/2+y < 0 || model->pos.x/2+y >= Graphics.world.width-1)
						continue;
					if(Graphics.world.cubes[(int)(model->pos.z/2+x)][(int)(model->pos.x/2+y)].cell1 != 0)
						ok = false;
				}
			}

		}


		model->pos.y = Graphics.world.cubes[(int)(model->pos.z/2)][(int)(model->pos.x/2)].cell1;
		model->scale = cVector3(1,1,1);
		model->rot = cVector3(0,rand()%360,0);
		Graphics.world.models.push_back(model);
	}


	eMode m = editmode;
	editmode = MODE_HEIGHTDETAIL;

	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_s;
	for(i = 0; i < smooth; i++)
		SDL_PushEvent(&ev);

	int b = brushsize;
	mouse3dx = Graphics.world.width*5;
	mouse3dz = Graphics.world.height*5;
	brushsize = Graphics.world.width+Graphics.world.height;
	
	process_events();
	Graphics.draw();
	SDL_GL_SwapBuffers();

	brushsize = b;


	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_s;
	SDL_PushEvent(&ev);



	editmode = m;

	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			if((Graphics.world.cubes[(int)y][(int)x].cell1 <= -8 || Graphics.world.cubes[(int)y][(int)x].cell2 <= -8 || Graphics.world.cubes[(int)y][(int)x].cell3  <= -8|| Graphics.world.cubes[(int)y][(int)x].cell4 <= -8) && Graphics.world.cubes[(int)y][(int)x].cell1 > -63)
				Graphics.world.cubes[(int)y][(int)x].tileUp= 50 + ((int)x%5) + 5*((int)y%5);
			else if(Graphics.world.cubes[(int)y][(int)x].cell1 >= -63)
				Graphics.world.cubes[(int)y][(int)x].tileUp= 25 + ((int)x%5) + 5*((int)y%5);
		}
	}

	Graphics.world.water.height = 12;






	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			cCube* c = &Graphics.world.cubes[(int)y][(int)x];
			Graphics.world.gattiles[2*(int)y][2*(int)x].type = (c->cell1+c->cell2+c->cell3+c->cell4) < -10 ? '\1' : '\0';
			Graphics.world.gattiles[2*(int)y][2*(int)x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) < -10 ? '\1' : '\0';
			Graphics.world.gattiles[2*(int)y+1][2*(int)x].type = (c->cell1+c->cell2+c->cell3+c->cell4) < -10 ? '\1' : '\0';
			Graphics.world.gattiles[2*(int)y+1][2*(int)x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) < -10 ? '\1' : '\0';
		}

	}

	MenuCommand_gatheight(src);



	for(x = 0; x < Graphics.world.width; x++)
		for(y = 0; y < Graphics.world.height; y++)
		{
			Graphics.world.cubes[(int)y][(int)x].maxHeight = -99999;
			Graphics.world.cubes[(int)y][(int)x].minHeight = 99999;
		}

	for(i = 0; i < Graphics.world.models.size(); i++)
		Graphics.world.models[i]->draw(false,false,true);


	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			if(Graphics.world.cubes[(int)y][(int)x].maxHeight != -99999)
			{
				Graphics.world.gattiles[2*(int)y][2*(int)x].type = '\1';
				Graphics.world.gattiles[2*(int)y][2*(int)x+1].type = '\1';
				Graphics.world.gattiles[2*(int)y+1][2*(int)x].type = '\1';
				Graphics.world.gattiles[2*(int)y+1][2*(int)x+1].type = '\1';
			}
		}

	}
	Graphics.world.root->recalculate();


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
	unsigned int i;
	int xx,yy;

	undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));
	float x,y;

	Graphics.world.tiles.clear();
	for(int tex = 0; tex < 5; tex++)
	{
		for(y = 0; y < 5; y++)
		{
			for(x = 0; x < 5; x++)
			{
				cTile t;
				t.lightmap = 1;
				t.texture = tex;
				t.u1 = x/5.0;
				t.v1 = y/5.0;
				t.u2 = (x+1)/5.0;
				t.v2 = (y)/5.0;
				t.u3 = (x)/5.0;
				t.v3 = (y+1)/5.0;
				t.u4 = (x+1)/5.0;
				t.v4 = (y+1)/5.0;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				Graphics.world.tiles.push_back(t);
			}
		}
	}
	
	
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[(int)y][(int)x].tileOtherSide = -1;
			Graphics.world.cubes[(int)y][(int)x].tileSide = -1;
			Graphics.world.cubes[(int)y][(int)x].tileUp = 75 + ((int)x%5) + 5*((int)y%5);
		}
	}


	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[(int)y][(int)x].cell1 = 16;
			Graphics.world.cubes[(int)y][(int)x].cell2 = 16;
			Graphics.world.cubes[(int)y][(int)x].cell3 = 16;
			Graphics.world.cubes[(int)y][(int)x].cell4 = 16;
		}
	}

	
	Graphics.draw();
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
			Graphics.draw();
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
			x = (Graphics.world.width-w)/2;
			y = (Graphics.world.height-h)/2;
		}

		if(!(x + w >= Graphics.world.width-1 || y+h >= Graphics.world.height-1 || x <= 1 || y <= 1))
		{
			int takencount = 0;
			for(xx = (int)x; xx < (int)x+w; xx++)
			{
				for(yy = (int)y; yy < (int)y+h; yy++)
				{
					if(Graphics.world.cubes[yy][xx].cell1 == 0)
						takencount++;
				}
			}
			if(takencount < 3)
			{
				for(xx = (int)x; xx < (int)x+w; xx++)
				{
					for(yy = (int)y; yy < (int)y+h; yy++)
					{
						Graphics.world.cubes[yy][xx].cell1 = 0;//rand()%25;
						Graphics.world.cubes[yy][xx].cell2 = 0;//rand()%25;
						Graphics.world.cubes[yy][xx].cell3 = 0;//rand()%25;
						Graphics.world.cubes[yy][xx].cell4 = 0;//rand()%25;
						Graphics.world.cubes[yy][xx].tileUp = 50 + (xx%5) + 5*(yy%5);
					}
				}
				if(island != -1)
					islands[island].connections.push_back(islands.size());

				islands.push_back(cIntQuad((int)x,(int)y,w,h));
				if(island != -1)
					islands[islands.size()-1].connections.push_back(island);
			}
			int count = 0;
			for(int yy = 0; yy < Graphics.world.height; yy++)
			{
				for(int xx = 0; xx < Graphics.world.width; xx++)
				{
					if(Graphics.world.cubes[yy][xx].cell1 == 0)
						count++;
				}
			}
			if(count > Graphics.world.height*Graphics.world.width / 2)
				filledenough = true;

		}
	}

	for(i = 0; i < Graphics.world.models.size(); i++)
		delete Graphics.world.models[i];
	Graphics.world.models.clear();


	for(i = 0; i < islands.size(); i++)
	{
		for(unsigned int ii = 0; ii < islands[i].connections.size(); ii++)
		{
			Graphics.draw();
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
			model->load(rodir +  "data\\model\\郴何家前\\枚促府.rsm");
			model->pos.x = (xx+x) + ((xx == x) ? 1 : 0);
			model->pos.z = (yy+y) + ((yy == y) ? 1 : 0);
			model->pos.y = 10;
			model->scale = cVector3((xx == x) ? 0.95f : 1,1,(yy == y) ? 0.95f : 1);
			model->rot = cVector3(0,xx==x ? 0 : 90,0);
			Graphics.world.models.push_back(model);

		}
	}

	Graphics.selectionstart.y = 320;
	Graphics.texturestart = 0;
	MenuCommand_addwalls(src);


	Graphics.world.water.height = 8;





	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			cCube* c = &Graphics.world.cubes[(int)y][(int)x];
			Graphics.world.gattiles[2*(int)y][2*(int)x].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			Graphics.world.gattiles[2*(int)y][2*(int)x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			Graphics.world.gattiles[2*(int)y+1][2*(int)x].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			Graphics.world.gattiles[2*(int)y+1][2*(int)x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
		}

	}

	MenuCommand_gatheight(src);



	for(x = 0; x < Graphics.world.width; x++)
		for(y = 0; y < Graphics.world.height; y++)
		{
			Graphics.world.cubes[(int)y][(int)x].maxHeight = -99999;
			Graphics.world.cubes[(int)y][(int)x].minHeight = 99999;
		}

	for(i = 0; i < Graphics.world.models.size(); i++)
		Graphics.world.models[i]->draw(false,false,true);


	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			if(Graphics.world.cubes[(int)y][(int)x].maxHeight != -99999)
			{
				Graphics.world.gattiles[2*(int)y][2*(int)x].type = '\0';
				Graphics.world.gattiles[2*(int)y][2*(int)x+1].type = '\0';
				Graphics.world.gattiles[2*(int)y+1][2*(int)x].type = '\0';
				Graphics.world.gattiles[2*(int)y+1][2*(int)x+1].type = '\0';

				Graphics.world.gattiles[2*(int)y][2*(int)x].cell1 = 	Graphics.world.gattiles[2*(int)y][2*(int)x].cell2 = 	Graphics.world.gattiles[2*(int)y][2*(int)x].cell3 = 	Graphics.world.gattiles[2*(int)y][2*(int)x].cell4 = 0;
				Graphics.world.gattiles[2*(int)y][2*(int)x+1].cell1 = 	Graphics.world.gattiles[2*(int)y][2*(int)x+1].cell2 = 	Graphics.world.gattiles[2*(int)y][2*(int)x+1].cell3	= 	Graphics.world.gattiles[2*(int)y][2*(int)x+1].cell4 = 0;
				Graphics.world.gattiles[2*(int)y+1][2*(int)x].cell1 = 	Graphics.world.gattiles[2*(int)y+1][2*(int)x].cell2 = 	Graphics.world.gattiles[2*(int)y+1][2*(int)x].cell3	= 	Graphics.world.gattiles[2*(int)y+1][2*(int)x].cell4 = 0;
				Graphics.world.gattiles[2*(int)y+1][2*(int)x+1].cell1 = Graphics.world.gattiles[2*(int)y+1][2*(int)x+1].cell2 = Graphics.world.gattiles[2*(int)y+1][2*(int)x+1].cell3 = Graphics.world.gattiles[2*(int)y+1][2*(int)x+1].cell4 = 0;
			}
		}

	}
	Graphics.world.root->recalculate();


	return true;
}



MENUCOMMAND(random4)
{
	unsigned int i;
	int x,y;

	undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));


	Graphics.world.tiles.clear();
	for(int tex = 0; tex < 5; tex++)
	{
		for(y = 0; y < 5; y++)
		{
			for(x = 0; x < 5; x++)
			{
				cTile t;
				t.lightmap = 1;
				t.texture = tex;
				t.u1 = x/5.0;
				t.v1 = y/5.0;
				t.u2 = (x+1)/5.0;
				t.v2 = (y)/5.0;
				t.u3 = (x)/5.0;
				t.v3 = (y+1)/5.0;
				t.u4 = (x+1)/5.0;
				t.v4 = (y+1)/5.0;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				Graphics.world.tiles.push_back(t);
			}
		}
	}

	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[y][x].tileOtherSide = -1;
			Graphics.world.cubes[y][x].tileSide = -1;
			Graphics.world.cubes[y][x].tileUp = 25 + ((int)x%5) + 5*((int)y%5);
		}
	}

	float xding = rand() % 50 + 20;
	float yding = rand() % 50 + 20;
	float zding = rand() % 50 + 20;

	float xding2 = rand() % 50 + 50;
	float yding2 = rand() % 50 + 50;

	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{

			float f = xding2*sin(y/xding)+yding2*cos(x/yding) + zding*cos(x/zding+y/zding);
			f = floor(f / 32.0f)*32;

			Graphics.world.cubes[y][x].cell1 = f;
			Graphics.world.cubes[y][x].cell2 = f;
			Graphics.world.cubes[y][x].cell3 = f;
			Graphics.world.cubes[y][x].cell4 = f;
		}
	}
	eMode m = editmode;
	editmode = MODE_HEIGHTDETAIL;

	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_s;
	for(i = 0; i < 3; i++)
		SDL_PushEvent(&ev);

	int b = brushsize;
	mouse3dx = Graphics.world.width*5;
	mouse3dz = Graphics.world.height*5;
	brushsize = Graphics.world.width+Graphics.world.height;
	
	process_events();
	brushsize = b;


	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_s;
	SDL_PushEvent(&ev);
	editmode = m;




	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			if(fabs(Graphics.world.cubes[y][x].cell1 - Graphics.world.cubes[y][x].cell2) > 5 ||
				fabs(Graphics.world.cubes[y][x].cell1 - Graphics.world.cubes[y][x].cell3) > 5 ||
				fabs(Graphics.world.cubes[y][x].cell1 - Graphics.world.cubes[y][x].cell4) > 5 ||
				fabs(Graphics.world.cubes[y][x].cell2 - Graphics.world.cubes[y][x].cell3) > 5 ||
				fabs(Graphics.world.cubes[y][x].cell2 - Graphics.world.cubes[y][x].cell4) > 5 ||
				fabs(Graphics.world.cubes[y][x].cell3 - Graphics.world.cubes[y][x].cell4) > 5)
				Graphics.world.cubes[y][x].tileUp = 50 + ((int)x%5) + 5*((int)y%5);
		}
	}


	std::vector<std::string>	randommodels;
	for(i = 0; i < objectfiles.size(); i++)
	{
		cFile* pFile = fs.open(objectfiles[i]);
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
			model->load(rodir +  randommodels[rand() % randommodels.size()]);

			model->pos = cVector3(rand()%(Graphics.world.width*2), 0, rand()%(Graphics.world.height*2));

			while(Graphics.world.cubes[(int)(model->pos.z/2)][(int)(model->pos.x/2)].tileUp > 50)
				model->pos = cVector3(rand()%(Graphics.world.width*2), 0, rand()%(Graphics.world.height*2));


			model->pos.y = Graphics.world.cubes[(int)(model->pos.z/2)][(int)(model->pos.x/2)].cell1;
			model->scale = cVector3(1,1,1);
			model->rot = cVector3(0,0,0);
			Graphics.world.models.push_back(model);
		}
	}

	return true;
}



MENUCOMMAND(mode)
{
	std::string title = src->title;
	for(unsigned int i = 0; i < mode->items.size(); i++)
		mode->items[i]->ticked = false;
	src->ticked = true;

	if(title == GetMsg("menu/editmode/GLOBALHEIGHTEDIT"))
	{
		editmode = MODE_HEIGHTGLOBAL;
		if (Graphics.texturestart >= (int)Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == GetMsg("menu/editmode/DETAILTERRAINEDIT"))
	{
		editmode = MODE_HEIGHTDETAIL;
		if (Graphics.texturestart >= (int)Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == GetMsg("menu/editmode/TEXTUREEDIT"))
	{
		editmode = MODE_TEXTURE;
		if (Graphics.texturestart >= (int)Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == GetMsg("menu/editmode/WALLEDIT"))
	{
		editmode = MODE_WALLS;
	}
	else if (title == GetMsg("menu/editmode/OBJECTEDIT"))
	{
		editmode = MODE_OBJECTS;
		if (Graphics.texturestart >= (int)Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == GetMsg("menu/editmode/GATEDIT"))
	{
		editmode = MODE_GAT;
		if (Graphics.texturestart >= 6)
			Graphics.texturestart = 0;
	}
	else if (title == GetMsg("menu/editmode/WATEREDIT"))
	{
		editmode = MODE_WATER;
		Graphics.texturestart = Graphics.world.water.type;
	}
	else if (title == GetMsg("menu/editmode/EFFECTSEDIT"))
	{
		editmode = MODE_EFFECTS;
		Graphics.selectedObject = -1;
	}
	else if (title == GetMsg("menu/editmode/SOUNDSEDIT"))
	{
		editmode = MODE_SOUNDS;
	}
	else if (title == GetMsg("menu/editmode/LIGHTSEDIT"))
	{
		editmode = MODE_LIGHTS;
	}
	else if (title == GetMsg("menu/editmode/OBJECTGROUPEDIT"))
	{
		editmode = MODE_OBJECTGROUP;
	}
	else if (title == GetMsg("menu/editmode/SPRITEEDIT"))
	{
		editmode = MODE_SPRITE;
	}
	return true;
}

MENUCOMMAND(grid)
{
	src->ticked = !src->ticked;
	Graphics.showgrid = src->ticked;
	return true;
}

MENUCOMMAND(mode_detail)
{
	unsigned int i;
	for(i = 0; i < mode->items.size(); i++)
		mode->items[i]->ticked = (mode->items[i]->title == "Detail Terrain Edit" ? true : false);
	for(i = 0; i < editdetail->items.size(); i++)
		editdetail->items[i]->ticked = false;
	src->ticked = true;
	editmode = MODE_HEIGHTDETAIL;
	brushsize = atoi(src->title.c_str());

	return true;
}
MENUCOMMAND(speed)
{
	for(unsigned int i =0 ; i < speed->items.size(); i++)
		speed->items[i]->ticked = false;
	src->ticked = true;
	paintspeed = atof(src->title.c_str());
	return true;
}

MENUCOMMAND(showobjects)
{
	src->ticked = !src->ticked;
	Graphics.showObjects = src->ticked;
	return true;
}


MENUCOMMAND(model)
{
	delete Graphics.previewModel;
	Graphics.previewModel = new cRSMModel();
	Graphics.previewModel->load(rodir + src->data);
	Graphics.previewModel->rot = cVector3(0,0,0);
	Graphics.previewModel->scale = cVector3(4,4,4);

	Graphics.previewModel->pos = cVector3(40,-40,-40);

	if (editmode != MODE_OBJECTS)
		Graphics.previewColor = 200;
	currentobject = src;
	return true;
}


MENUCOMMAND(slope)
{
	src->ticked = !src->ticked;
	Graphics.slope = src->ticked;
	return true;
}

MENUCOMMAND(quadtree)
{
	int x,y;
	for(x = 0; x < Graphics.world.width; x++)
		for(y = 0; y < Graphics.world.height; y++)
		{
			Graphics.world.cubes[y][x].maxHeight = -99999;
			Graphics.world.cubes[y][x].minHeight = 99999;
		}

	for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
	{
		Log(3,0,GetMsg("CALCMODEL"), i, Graphics.world.models.size(), (i/(float)Graphics.world.models.size())*100);
		Graphics.world.models[i]->draw(false,false,true);
	}


	Graphics.world.root->recalculate();
	return true;
}

MENUCOMMAND(gatheight)
{
	undostack.push(new cUndoGatHeightEdit(0,0,Graphics.world.gattiles[0].size(), Graphics.world.gattiles.size()));
	int x,y;
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			cCube* c = &Graphics.world.cubes[y][x];
			Graphics.world.gattiles[2*y][2*x].cell1 = (c->cell1+c->cell1) / 2.0f;
			Graphics.world.gattiles[2*y][2*x].cell2 = (c->cell1+c->cell2) / 2.0f;
			Graphics.world.gattiles[2*y][2*x].cell3 = (c->cell1+c->cell3) / 2.0f;
			Graphics.world.gattiles[2*y][2*x].cell4 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;

			Graphics.world.gattiles[2*y][2*x+1].cell1 = (c->cell1+c->cell2) / 2.0f;
			Graphics.world.gattiles[2*y][2*x+1].cell2 = (c->cell2+c->cell2) / 2.0f;
			Graphics.world.gattiles[2*y][2*x+1].cell3 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			Graphics.world.gattiles[2*y][2*x+1].cell4 = (c->cell4+c->cell2) / 2.0f;

			Graphics.world.gattiles[2*y+1][2*x+1].cell1 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			Graphics.world.gattiles[2*y+1][2*x+1].cell2 = (c->cell4 + c->cell2) / 2.0f;
			Graphics.world.gattiles[2*y+1][2*x+1].cell3 = (c->cell4 + c->cell3) / 2.0f;
			Graphics.world.gattiles[2*y+1][2*x+1].cell4 = (c->cell4 + c->cell4) / 2.0f;

			Graphics.world.gattiles[2*y+1][2*x].cell1 = (c->cell3 + c->cell1) / 2.0f;
			Graphics.world.gattiles[2*y+1][2*x].cell2 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			Graphics.world.gattiles[2*y+1][2*x].cell3 = (c->cell3 + c->cell3) / 2.0f;
			Graphics.world.gattiles[2*y+1][2*x].cell4 = (c->cell3 + c->cell4) / 2.0f;
		}

	}
	
	return true;
}

MENUCOMMAND(gatcollision2)
{
	MenuCommand_gatheight(src);
	int i, x, y;
	int ww = Graphics.w();
	ww -= 256;
	int hh = Graphics.h()-20;

	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,ww,hh);						// Reset The Current Viewport

	float camrad = 10;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	gluPerspective(45.0f,(GLfloat)ww/(GLfloat)hh,10.0f,10000.0f);
	gluLookAt(  -Graphics.camerapointer.x + Graphics.cameraheight*sin(Graphics.camerarot),
				camrad+Graphics.cameraheight,
				-Graphics.camerapointer.y + Graphics.cameraheight*cos(Graphics.camerarot),
				-Graphics.camerapointer.x,camrad + Graphics.cameraheight * (Graphics.cameraangle/10.0f),-Graphics.camerapointer.y,
				0,1,0);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
//	glTranslatef(0,0,Graphics.world.height*10);
//	glScalef(1,1,-1);

	for(i = 0; i < Graphics.world.models.size(); i++)
		Graphics.world.models[i]->precollides();

	Log(3,0, "Done Model boundingbox calculations");

	for(x = 0; x < Graphics.world.width*2; x++)
	{
		for(y = 0; y < Graphics.world.height*2; y++)
		{
			Graphics.world.gattiles[y][x].type = 0;
		}
	}
	

	for(x = 0; x < Graphics.world.width; x++)
		for(y = 0; y < Graphics.world.height; y++)
		{
			Graphics.world.cubes[y][x].maxHeight = -99999;
			Graphics.world.cubes[y][x].minHeight = 99999;
		}

	for(i = 0; i < Graphics.world.models.size(); i++)
	{
		Log(3,0,GetMsg("CALCMODEL"), i, Graphics.world.models.size(), (i/(float)Graphics.world.models.size())*100);
		Graphics.world.models[i]->draw(false,false,true);
	}


	for(x = 0; x < Graphics.world.width*2; x++)
	{
		for(y = 0; y < Graphics.world.height*2; y++)
		{
			if(Graphics.world.cubes[y/2][x/2].maxHeight == -99999 || Graphics.world.cubes[y/2][x/2].minHeight == 99999)
				continue;

			
			Graphics.camerapointer.x = -5*x + 2.5;
//			Graphics.camerapointer.y = -5*(2*Graphics.world.height-y) + 2.5;


			cVector3 worldpos = cVector3(	5*x+2.5, 
											-5000,
											5*y+2.5);
			
			cVector3 highup = worldpos + cVector3(0, 10000, 0);
			for(unsigned int ii = 0; ii < Graphics.world.models.size(); ii++)
			{
				if(Graphics.world.models[ii]->collides(worldpos, highup))
				{
					Graphics.world.gattiles[y][x].type = 1;
					break;
				}
			}
		}
		mainloop();
	}
	for(x = 0; x < Graphics.world.width; x++)
		for(y = 0; y < Graphics.world.height; y++)
		{
			Graphics.world.cubes[y][x].maxHeight = 0;
			Graphics.world.cubes[y][x].minHeight = 0;
		}


	return true;
}



cVector3 lightpos = cVector3(-20000,20000,-20000);
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
	cWindowPreviewButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent, skin)
	{
		alignment = ALIGN_TOPLEFT;
		moveTo(0,20);
		resizeTo(100,20);
		text = "Preview";
	}
	void click()
	{
		Graphics.world.loaded = !Graphics.world.loaded;
	}
};

inline void setLightIntensity(BYTE* buf, int yy, int xx, cVector3 worldpos );


MENUCOMMAND(dolightmaps2)
{
	int x,y;
	unsigned int i;

	bool rendering = true;


	cProgressWindow* w = new cProgressWindow(Graphics.WM.texture, Graphics.WM.font, &rendering, Graphics.WM.skin);
	Graphics.WM.addwindow(w);
	w->objects["progress"]->setInt(1,0);
	w->objects["progress"]->setInt(2,Graphics.world.height * Graphics.world.width);
	w->objects["toggle"] = new cWindowPreviewButton(w, Graphics.WM.skin);

	mainloop();

	Log(3,0,"Starting Lightmap Clearing");
	Graphics.world.makeLightmapsUnique();
	//Graphics.world.blackLightmaps();
	Log(3,0,"Done initializing for lightmaps...");

	int ww = Graphics.w();
	ww -= 256;
	int hh = Graphics.h()-20;

	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,ww,hh);						// Reset The Current Viewport

	float camrad = 10;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	gluPerspective(45.0f,(GLfloat)ww/(GLfloat)hh,10.0f,10000.0f);
	gluLookAt(  -Graphics.camerapointer.x + Graphics.cameraheight*sin(Graphics.camerarot),
				camrad+Graphics.cameraheight,
				-Graphics.camerapointer.y + Graphics.cameraheight*cos(Graphics.camerarot),
				-Graphics.camerapointer.x,camrad + Graphics.cameraheight * (Graphics.cameraangle/10.0f),-Graphics.camerapointer.y,
				0,1,0);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
//	glTranslatef(0,0,Graphics.world.height*10);
//	glScalef(1,1,-1);

	for(i = 0; i < Graphics.world.models.size(); i++)
		Graphics.world.models[i]->precollides();

	Graphics.world.loaded = false;


	unsigned long timer = tickcount();

	Log(3,0, "Done Model boundingbox calculations");
	for(y = 0; y < Graphics.world.height && rendering; y++)
	{
		for(x = 0; x < Graphics.world.width && rendering; x++)
		{
			cCube* c = &Graphics.world.cubes[y][x];
			if(selectonly && !c->selected)
				continue;
			w->objects["progress"]->setInt(0, y*Graphics.world.width + x);
			
			if(c->tileUp != -1)
			{
				BYTE* buf = (BYTE*)Graphics.world.lightmaps[Graphics.world.tiles[c->tileUp].lightmap]->buf;
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
				Graphics.world.realLightmaps[y][x]->reset();
			}
			
			if(c->tileSide != -1)
			{
				BYTE* buf = (BYTE*)Graphics.world.lightmaps[Graphics.world.tiles[c->tileSide].lightmap]->buf;
				ZeroMemory(buf,256);				
				for(int yy = 0; yy < 8; yy++)
				{
					for(int xx = 0; xx < 8; xx++)
					{
						float fx = (xx-1)/6.0f;
						float fy = (yy-1)/6.0f;
						
						cCube* c2 = &Graphics.world.cubes[y+1][x];
						
						cVector3 worldpos = cVector3(	10*x+(10/6.0)*(xx-1), 
							-((1-fy)*c->cell3 + (fy)*c2->cell1),
							10*y+10);
						
						setLightIntensity(buf, yy, xx, worldpos);
					}
				}
				Graphics.world.lightmaps[Graphics.world.tiles[c->tileSide].lightmap]->del();
				Graphics.world.lightmaps[Graphics.world.tiles[c->tileSide].lightmap]->del2();
			}

			if(c->tileOtherSide != -1)
			{
				BYTE* buf = (BYTE*)Graphics.world.lightmaps[Graphics.world.tiles[c->tileOtherSide].lightmap]->buf;
				ZeroMemory(buf,256);

				for(int yy = 0; yy < 8; yy++)
				{
					for(int xx = 0; xx < 8; xx++)
					{
						float fx = (xx-1)/6.0f;
						float fy = (yy-1)/6.0f;

						cCube* c2 = &Graphics.world.cubes[y][x+1];

						cVector3 worldpos = cVector3(	10*x+10, 
														-((1-fy)*c->cell4 + (fy)*c2->cell3),
														10*y+(10/6.0)*(7-xx));
						setLightIntensity(buf, yy, xx, worldpos);
						
					}
				}
				Graphics.world.lightmaps[Graphics.world.tiles[c->tileOtherSide].lightmap]->del();
				Graphics.world.lightmaps[Graphics.world.tiles[c->tileOtherSide].lightmap]->del2();
			}		
			
			
			
			
			
			
			
			if(Graphics.world.loaded)
			{
				Graphics.camerapointer.x = -10*x + 5;
				Graphics.camerapointer.y = -10*(Graphics.world.height-y) + 5;
			}
			if(tickcount() - timer > 100)
			{
				mainloop();
				timer = tickcount();
			}
		}
	}
	
	Graphics.world.fixGridding();
	Graphics.world.loaded = true;
	
	
	w->close();
	
	//	return true;
	
	
	/*
	for(i = 0; i < Graphics.world.models.size(); i++)
	{
	Log(3,0,"Doing model %i out of %i (%.2f%%)", i, Graphics.world.models.size(), (i/(float)Graphics.world.models.size())*100);
	Graphics.world.models[i]->draw(false,false,false, true);
	}*/
	
	/*	float t;
	for(x = 0; x < Graphics.world.width; x++)
	{
	Log(3,0,"%f%%", (x/(float)Graphics.world.width)*100.0f);
	for(y = 0; y < Graphics.world.height; y++)
	{
	int tile = Graphics.world.cubes[y][x].tileUp;
	if (tile != -1)
			{
				float cellheight = -Graphics.world.cubes[y][x].cell1;
				cLightmap* l = Graphics.world.lightmaps[Graphics.world.tiles[tile].lightmap];
				for(int xx = 0; xx < 6; xx++)
				{
					for(int yy = 0; yy < 6; yy++)
					{
						cVector3 pos = cVector3(10*x+10*(xx/6.0),cellheight, 10*y+10*(yy/6.0));
						char* lightmappos = &l->buf[xx + (8*yy)+1+8];

						for(int xxx = max(0,x - 1); xxx <= min(Graphics.world.width-1,x+1); xxx++)
						{
							for(int yyy = max(0,y - 1); yyy <= min(Graphics.world.height-1,y+1); yyy++)
							{
								if(*lightmappos == 127)
									break;
								if (xxx == x && yyy == y)
									continue;
								cCube* c = &Graphics.world.cubes[yyy][xxx];
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
								if (c->tileSide != -1 && y < Graphics.world.width - 1)
								{

									triangle[0] = cVector3(xxx*10,-c->cell3,yyy*10+10);
									triangle[1] = cVector3(xxx*10+10,-c->cell4,yyy*10+10);
									triangle[2] = cVector3(xxx*10,-Graphics.world.cubes[y+1][x].cell1,yyy*10+10);
									
									
									triangle[3] = cVector3(xxx*10+10,-Graphics.world.cubes[y+1][x].cell2,yyy*10+10);
									triangle[4] = cVector3(xxx*10,-Graphics.world.cubes[y+1][x].cell1,yyy*10+10);
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
	cCube* c = &Graphics.world.cubes[y/6][x/6];
	if(c->tileUp != -1)
	{
		cTile* t = &Graphics.world.tiles[c->tileUp];
		if(t->lightmap != -1)
		{
			cLightmap* l = Graphics.world.lightmaps[t->lightmap];
			return (BYTE*)l->buf + (8*((y%6)+1) + ((x%6)+1));
		}
	}
	return NULL;
}

MENUCOMMAND(smoothlightmaps)
{
	std::string strFactor = Graphics.WM.InputWindow("Smoothing factor:", "1");
	if(strFactor == "")
		return true;
	float factor = atof(strFactor.c_str());
	int x,y;
	char* buf = new char[Graphics.world.height*6*Graphics.world.width*6];
	for(x = 0; x < Graphics.world.width*6; x++)
	{
		for(y = 0; y < Graphics.world.height*6; y++)
		{
			int total = 0;
			float count = 0;
			for(int xx = -1; xx < 2; xx++)
			{
				for(int yy = -1; yy < 2; yy++)
				{
					if(x+xx >= 0 && y+yy >= 0 && x+xx < Graphics.world.width*6 && y+yy < Graphics.world.height*6)
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
			buf[x+Graphics.world.width*6*y] = min(max(total / count,0),255);
		}
	}

	for(x = 0; x < Graphics.world.width*6; x++)
	{
		for(y = 0; y < Graphics.world.height*6; y++)
		{
			if(getLightMap(x,y) != NULL)
				*getLightMap(x,y) = buf[x+Graphics.world.width*6*y];
		}
	}
	for(x = 0; x < Graphics.world.width; x++)
		for(y = 0; y < Graphics.world.height; y++)
			Graphics.world.realLightmaps[y][x]->reset();
	Graphics.world.fixGridding();


	return true;
}

MENUCOMMAND(fixcolors)
{
	int x,y;
	for(x = 0; x < Graphics.world.width; x++)
		for(y = 0; y < Graphics.world.height; y++)
		{
			int tile = Graphics.world.cubes[y][x].tileUp;
			if(tile != -1)
			{
				Graphics.world.tiles[tile].color[0] = '\255';
				Graphics.world.tiles[tile].color[1] = '\255';
				Graphics.world.tiles[tile].color[2] = '\255';
				Graphics.world.tiles[tile].color[3] = '\255';
			}			
		}

	return true;
}


MENUCOMMAND(savelightmaps)
{
	Graphics.world.savelightmap();
	return true;
}

MENUCOMMAND(loadlightmaps)
{

	Graphics.world.makeLightmapsUnique();
	Graphics.world.loadlightmap();
	Graphics.world.fixGridding();

	return true;
}


MENUCOMMAND(addwalls)
{
	int x,y;
	for(x = 0; x < Graphics.world.width-1; x++)
	{
		for(y = 0; y < Graphics.world.height-1; y++)
		{
			cCube* c = &Graphics.world.cubes[y][x];
			if (c->tileOtherSide == -1)
			{
				if (c->cell4 != (c+1)->cell1 && c->cell2 != (c+1)->cell3)
				{
					cTile t;
					t.color[0] = (char)255;
					t.color[1] = (char)255;
					t.color[2] = (char)255;
					t.color[3] = (char)255;
					t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
					t.lightmap = 0;
					t.u1 = 0;
					t.v1 = 0;

					t.u2 = 1;
					t.v2 = 0;
					
					t.u3 = 0;
					t.v3 = 1;
					
					t.u4 = 1;
					t.v4 = 1;
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileOtherSide = Graphics.world.tiles.size()-1;
				}
			}
			if (c->tileSide == -1)
			{
				if (c->cell4 != Graphics.world.cubes[y+1][x].cell1 && c->cell3 != Graphics.world.cubes[y+1][x].cell2)
				{
					cTile t;
					t.color[0] = (char)255;
					t.color[1] = (char)255;
					t.color[2] = (char)255;
					t.color[3] = (char)255;
					t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
					t.lightmap = 0;
					t.u1 = 0;
					t.v1 = 0;

					t.u2 = 1;
					t.v2 = 0;
					
					t.u3 = 0;
					t.v3 = 1;
					
					t.u4 = 1;
					t.v4 = 1;
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileSide = Graphics.world.tiles.size()-1;
				}
			}
		}
	}
	return true;
}

MENUCOMMAND(gatcollision)
{
	int x,y;
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			cCube* c = &Graphics.world.cubes[y][x];
			Graphics.world.gattiles[2*y][2*x].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			Graphics.world.gattiles[2*y][2*x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			Graphics.world.gattiles[2*y+1][2*x].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			Graphics.world.gattiles[2*y+1][2*x+1].type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
		}

	}
	return true;
}


MENUCOMMAND(cleanuplightmaps)
{
	unsigned int i;
	std::vector<int> newvalue;
	std::map<int, bool, std::less<int> > used;
	for(i = 0; i < Graphics.world.lightmaps.size(); i++)
	{
		for(unsigned int ii = 0; ii < i; ii++)
		{
			if(memcmp(Graphics.world.lightmaps[i]->buf, Graphics.world.lightmaps[ii]->buf, 256) == 0)
			{
				newvalue.push_back(ii);
				break;
			}
		}
		if (newvalue.size() <= i)
			newvalue.push_back(i);
	}

	for(i = 0; i < Graphics.world.tiles.size(); i++)
	{
		Graphics.world.tiles[i].lightmap = newvalue[Graphics.world.tiles[i].lightmap];
	}
	return true;
}


MENUCOMMAND(tempfunc)
{
	glColor4f(1,1,1,0.7f);
	glEnable(GL_BLEND);

	cTile t;
	t.color[0] = (char)255;
	t.color[1] = (char)255;
	t.color[2] = (char)255;
	t.color[3] = (char)255;
	t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
	t.lightmap = 0;
	t.u1 = 0;
	t.v1 = 0;

	t.u2 = 1;
	t.v2 = 0;
	
	t.u3 = 0;
	t.v3 = 1;
	
	t.u4 = 1;
	t.v4 = 1;
	Graphics.world.tiles.push_back(t);
	Graphics.world.tiles.push_back(t);
	int x,y;
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			cCube* c = &Graphics.world.cubes[y][x];
			if(Graphics.world.lightmaps[Graphics.world.tiles[c->tileUp].lightmap]->buf[10] == 0)
			{
				c->cell1 = -20;
				c->cell2 = -20;
				c->cell3 = -20;
				c->cell4 = -20;

				c->tileUp = Graphics.world.tiles.size()-2;
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

MENUCOMMAND(snaptofloor)
{
	src->ticked = !src->ticked;
	return true;
}


bool mouseovertexture(cMenu* src)
{
	if (Graphics.texturePreview == NULL || Graphics.texturePreview->getfilename() != rodir + "data\\texture\\" + ((cMenuItem*)src)->data)
	{
		Graphics.texturePreview = TextureCache.load(rodir + "data\\texture\\" + ((cMenuItem*)src)->data);
		return false;
	}
	else
	{
		TextureCache.unload(Graphics.texturePreview);
		Graphics.texturePreview = NULL;
		return true;
	}
}
bool mouseouttexture(cMenu* src)
{
	if (Graphics.texturePreview != NULL)
	{
		TextureCache.unload(Graphics.texturePreview);
		Graphics.texturePreview = NULL;
	}
	return true;
}

MENUCOMMAND(effect)
{
	if (selectedeffect != NULL)
		selectedeffect->ticked = false;
	src->ticked = true;
	if (Graphics.selectedObject != -1)
	{
		Graphics.world.effects[Graphics.selectedObject].type = atoi(src->data.c_str());
		Graphics.world.effects[Graphics.selectedObject].readablename = src->title;
	}
	selectedeffect = src;
	return true;
}

MENUCOMMAND(toggle)
{
	src->ticked = !src->ticked;
	*((bool*)src->pdata) = src->ticked;

	return true;
}

MENUCOMMAND(water)
{
	char buf[100];
	cWindow* w = new cWaterWindow(Graphics.WM.texture, Graphics.WM.font, Graphics.WM.skin);
	sprintf(buf, "%f", Graphics.world.water.amplitude);		w->objects["amplitude"]->setText(0,buf);
	sprintf(buf, "%f", Graphics.world.water.height);		w->objects["height"]->setText(0,buf);
	sprintf(buf, "%f", Graphics.world.water.phase);			w->objects["phase"]->setText(0,buf);
	sprintf(buf, "%f", Graphics.world.water.surfaceCurve);	w->objects["surfacecurve"]->setText(0,buf);
	sprintf(buf, "%i", Graphics.world.water.type);			w->objects["type"]->setText(0,buf);
	Graphics.WM.addwindow(w);
	return true;
}

MENUCOMMAND(cleantextures)
{
	Graphics.world.clean();
	std::vector<bool> used;
	int i;
	used.resize(Graphics.world.textures.size(), false);


	for(i = 0; i < (int)Graphics.world.tiles.size(); i++)
		used[Graphics.world.tiles[i].texture] = true;
	
	for(i = (int)used.size()-1; i >= 0; i--)
	{
		if (!used[i])
		{
			for(unsigned int ii = 0; ii < Graphics.world.tiles.size(); ii++)
			{
				if(Graphics.world.tiles[i].texture > i)
					Graphics.world.tiles[i].texture--;
			}
			TextureCache.unload(Graphics.world.textures[i]->texture);
			delete Graphics.world.textures[i];
			Graphics.world.textures.erase(Graphics.world.textures.begin() + i);
		}
	}
	Graphics.texturestart = 0;
	return true;
}

MENUCOMMAND(ambientlight)
{
	char buf[100];
	cWindow* w = new cAmbientLightWindow(Graphics.WM.texture, Graphics.WM.font, Graphics.WM.skin);
	sprintf(buf, "%i", Graphics.world.ambientLight.ambientr);		w->objects["ambientr"]->setText(0,buf);
	sprintf(buf, "%i", Graphics.world.ambientLight.ambientg);		w->objects["ambientg"]->setText(0,buf);
	sprintf(buf, "%i", Graphics.world.ambientLight.ambientb);		w->objects["ambientb"]->setText(0,buf);

	sprintf(buf, "%f", Graphics.world.ambientLight.diffuse.x);		w->objects["diffuser"]->setText(0,buf);
	sprintf(buf, "%f", Graphics.world.ambientLight.diffuse.y);		w->objects["diffuseg"]->setText(0,buf);
	sprintf(buf, "%f", Graphics.world.ambientLight.diffuse.z);		w->objects["diffuseb"]->setText(0,buf);

	sprintf(buf, "%f", Graphics.world.ambientLight.shadow.x);		w->objects["shadowr"]->setText(0,buf);
	sprintf(buf, "%f", Graphics.world.ambientLight.shadow.y);		w->objects["shadowg"]->setText(0,buf);
	sprintf(buf, "%f", Graphics.world.ambientLight.shadow.z);		w->objects["shadowb"]->setText(0,buf);
	
	sprintf(buf, "%f", Graphics.world.ambientLight.alpha);			w->objects["alpha"]->setText(0,buf);

	Graphics.WM.addwindow(w);
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
	Graphics.WM.addwindow(new cKeyBindWindow(Graphics.WM.texture, Graphics.WM.font, Graphics.WM.skin));
	return true;
}



MENUCOMMAND(fillarea)
{
	int x,y,i;
	std::map<int, bool, std::less<int> > used;

	cLightmap* map = new cLightmap();
	for(i = 0; i < 256; i++)
		map->buf[i] = i < 64 ? 255 : 0;
	Graphics.world.lightmaps.push_back(map);
	map = new cLightmap();
	for(i = 0; i < 256; i++)
		map->buf[i] = i < 64 ? 255 : 0;
	Graphics.world.lightmaps.push_back(map);


	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			if(Graphics.world.cubes[y][x].selected)
			{
				int xx = x % 4;
				int yy = y % 4;
				cTile t;
				t.lightmap = Graphics.world.textures.size()-1;
				t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
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
				Graphics.world.tiles.push_back(t);
				Graphics.world.cubes[y][x].tileUp = Graphics.world.tiles.size()-1;;
			}
		}
	}


	for(x = 0; x < Graphics.world.width; x++)
	{
		for(y = 0; y < Graphics.world.height; y++)
		{
			int tile = Graphics.world.cubes[y][x].tileUp;
			if(used.find(tile) != used.end())
			{
				cTile t = Graphics.world.tiles[tile];
				tile = Graphics.world.tiles.size();
				Graphics.world.tiles.push_back(t);
				Graphics.world.cubes[y][x].tileUp = tile;
			}
			used[tile] = 1;
///////////////////////////////////////
			tile = Graphics.world.cubes[y][x].tileSide;
			if (tile != -1)
			{
				if(used.find(tile) != used.end())
				{
					cTile t = Graphics.world.tiles[tile];
					tile = Graphics.world.tiles.size();
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileSide = tile;
				}
				used[tile] = 1;
			}
/////////////////////////////////////
			tile = Graphics.world.cubes[y][x].tileOtherSide;
			if (tile!= -1)
			{
				if(used.find(tile) != used.end())
				{
					cTile t = Graphics.world.tiles[tile];
					tile = Graphics.world.tiles.size();
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileOtherSide = tile;
				}
				used[tile] = 1;
			}
		}
	}


	

	return true;
}



MENUCOMMAND(rsmedit)
{
	Graphics.WM.addwindow(new cRSMEditWindow(Graphics.WM.texture, Graphics.WM.font, Graphics.WM.skin));
	return true;
}

MENUCOMMAND(favlights)
{
	Graphics.WM.addwindow(new cFavoriteLightsWindow(Graphics.WM.texture, Graphics.WM.font, Graphics.WM.skin));
	return true;
}


MENUCOMMAND(exportmapfiles)
{
#ifdef WIN32
	CreateDirectory(Graphics.world.fileName, NULL);
	CreateDirectory((Graphics.world.fileName + std::string("\\texture\\")).c_str(), NULL);

	int i;
	std::ofstream pFile((std::string(Graphics.world.fileName) + ".txt").c_str());
	for(i = 0; i < Graphics.world.textures.size(); i++)
	{
		cFile* pF = fs.open(rodir + "data\\texture\\" + Graphics.world.textures[i]->RoFilename);
		if(pF->location != -1)
		{
			pF->close();
			continue;
		}
		pF->close();


		CopyFile((rodir + "data\\texture\\" + Graphics.world.textures[i]->RoFilename).c_str(), (std::string(Graphics.world.fileName) + "\\texture\\" + Graphics.world.textures[i]->RoFilename2).c_str(), false);
		pFile.write("texture\\", 8);
		pFile.write(Graphics.world.textures[i]->RoFilename.c_str(), Graphics.world.textures[i]->RoFilename.length());
		pFile.put('\r');
		pFile.put('\n');
	}

	std::map<std::string, bool, std::less<std::string> > usedmodels;

	for(i = 0; i < Graphics.world.models.size(); i++)
	{
		if(usedmodels.find(Graphics.world.models[i]->rofilename) != usedmodels.end())
			continue;

		cFile* pF = fs.open(Graphics.world.models[i]->filename);
		if(pF->location != -1)
		{
			pF->close();
			continue;
		}
		pF->close();



		usedmodels[Graphics.world.models[i]->rofilename] = true;
		pFile.write("model\\", 6);
		pFile.write(Graphics.world.models[i]->rofilename.c_str(), Graphics.world.models[i]->rofilename.length());
		pFile.put('\r');
		pFile.put('\n');

		for(int ii = 0; ii < Graphics.world.models[i]->textures.size(); ii++)
		{
			std::string file = Graphics.world.models[i]->textures[ii]->getfilename();
			cFile* pF = fs.open(file);
			if(pF->location != -1)
			{
				pF->close();
				continue;
			}
			pF->close();

			file = file.substr(rodir.length()+5);
			pFile.write(file.c_str(), file.length());
			pFile.put('\r');
			pFile.put('\n');
		}
		
	}


	pFile.close();
	ShellExecute(NULL,"open",(std::string(Graphics.world.fileName) + ".txt").c_str(),NULL,"c:\\",SW_SHOW);
#endif
	return true;
}




MENUCOMMAND(random5)
{
	unsigned int i;
	int xx,yy;

	undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));
	float x,y;

	Graphics.world.tiles.clear();
	for(int tex = 0; tex < 5; tex++)
	{
		for(y = 0; y < 5; y++)
		{
			for(x = 0; x < 5; x++)
			{
				cTile t;
				t.lightmap = 1;
				t.texture = tex;
				t.u1 = x/5.0;
				t.v1 = y/5.0;
				t.u2 = (x+1)/5.0;
				t.v2 = (y)/5.0;
				t.u3 = (x)/5.0;
				t.v3 = (y+1)/5.0;
				t.u4 = (x+1)/5.0;
				t.v4 = (y+1)/5.0;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				Graphics.world.tiles.push_back(t);
			}
		}
	}
	
	
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[(int)y][(int)x].tileOtherSide = -1;
			Graphics.world.cubes[(int)y][(int)x].tileSide = -1;
			Graphics.world.cubes[(int)y][(int)x].tileUp = 0 + ((int)x%5) + 5*((int)y%5);
		}
	}


	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[(int)y][(int)x].cell1 = -32;
			Graphics.world.cubes[(int)y][(int)x].cell2 = -32;
			Graphics.world.cubes[(int)y][(int)x].cell3 = -32;
			Graphics.world.cubes[(int)y][(int)x].cell4 = -32;
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
			Graphics.draw();
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
			x = (Graphics.world.width-w)/2;
			y = (Graphics.world.height-h)/2;
		}

		if(!(x + w >= Graphics.world.width-1 || y+h >= Graphics.world.height-1 || x <= 1 || y <= 1))
		{
			int takencount = 0;
			for(xx = (int)x; xx < (int)x+w; xx++)
			{
				for(yy = (int)y; yy < (int)y+h; yy++)
				{
					if(Graphics.world.cubes[yy][xx].cell1 == 0)
						takencount++;
				}
			}
			if(takencount < 3)
			{
				for(xx = (int)x; xx < (int)x+w; xx++)
				{
					for(yy = (int)y; yy < (int)y+h; yy++)
					{
						Graphics.world.cubes[yy][xx].cell1 = 0;//rand()%25;
						Graphics.world.cubes[yy][xx].cell2 = 0;//rand()%25;
						Graphics.world.cubes[yy][xx].cell3 = 0;//rand()%25;
						Graphics.world.cubes[yy][xx].cell4 = 0;//rand()%25;
						Graphics.world.cubes[yy][xx].tileUp = 25 + (xx%5) + 5*(yy%5);
					}
				}
				if(island != -1)
					islands[island].connections.push_back(islands.size());

				islands.push_back(cIntQuad((int)x,(int)y,w,h));
				if(island != -1)
					islands[islands.size()-1].connections.push_back(island);
			}
			int count = 0;
			for(int yy = 0; yy < Graphics.world.height; yy++)
			{
				for(int xx = 0; xx < Graphics.world.width; xx++)
				{
					if(Graphics.world.cubes[yy][xx].cell1 == 0)
						count++;
				}
			}
			if(count > Graphics.world.height*Graphics.world.width / 2)
				filledenough = true;

		}
	}


	for(i = 0; i < islands.size(); i++)
	{
		Graphics.draw();
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
						Graphics.world.cubes[yy+yyy][xx+xxx].cell1 = 0;//rand()%25;
						Graphics.world.cubes[yy+yyy][xx+xxx].cell2 = 0;//rand()%25;
						Graphics.world.cubes[yy+yyy][xx+xxx].cell3 = 0;//rand()%25;
						Graphics.world.cubes[yy+yyy][xx+xxx].cell4 = 0;//rand()%25;
						Graphics.world.cubes[yy+yyy][xx+xxx].tileUp = 25 + ((xx+xxx)%5) + 5*((yy+yyy)%5);
						if(xx > x)
							xx--;
						if(xx < x)
							xx++;
						if(yy > y)
							yy--;
						if(yy < y)
							yy++;

						Graphics.world.cubes[yy+yyy][xx+xxx].cell1 = 0;//rand()%25;
						Graphics.world.cubes[yy+yyy][xx+xxx].cell2 = 0;//rand()%25;
						Graphics.world.cubes[yy+yyy][xx+xxx].cell3 = 0;//rand()%25;
						Graphics.world.cubes[yy+yyy][xx+xxx].cell4 = 0;//rand()%25;
						Graphics.world.cubes[yy+yyy][xx+xxx].tileUp = 25 + ((xx+xxx)%5) + 5*((yy+yyy)%5);
					}
				}
			}
		}
	}

	Graphics.selectionstart.y = 320;
	Graphics.texturestart = 1;
	MenuCommand_addwalls(src);


	MenuCommand_gatheight(src);
	MenuCommand_gatcollision(src);




/*	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			if((Graphics.world.cubes[y][x].cell1 <= -8 || Graphics.world.cubes[y][x].cell2 <= -8 || Graphics.world.cubes[y][x].cell3  <= -8|| Graphics.world.cubes[y][x].cell4 <= -8) && Graphics.world.cubes[y][x].cell1 > -63)
			{
				Graphics.world.cubes[y][x].tileUp= 50 + ((int)x%5) + 5*((int)y%5);
			}
		}
	}
*/
	Graphics.world.water.height = 8;


	return true;
}





MENUCOMMAND(99dun)
{
	srand(atoi(Graphics.WM.InputWindow("Random Seed: ").c_str()));

	for(int i = 0; i < 10; i++)
	{
		int mode = rand() % 3;

		if(mode == 0)
		{
			sprintf(Graphics.world.fileName, "%sdata\\random_pay", rodir.c_str());
			Graphics.world.load();
			MenuCommand_random2(src);
		}
		else if (mode == 1)
		{
			sprintf(Graphics.world.fileName, "%sdata\\random_cul", rodir.c_str());
			Graphics.world.load();
			MenuCommand_random3(src);
		}
		else if (mode == 2)
		{
			sprintf(Graphics.world.fileName, "%sdata\\random_ama", rodir.c_str());
			Graphics.world.load();
			MenuCommand_random5(src);
		}
		sprintf(Graphics.world.fileName, "%sdata\\ulti_dun%02i", rodir.c_str(), i);
		Graphics.world.save();
	}


	return true;
}


std::string scriptmap;
char dirmap[] = { 4,3,2,1,0,7,6,5 };

void readscript(std::string filename)
{
	Log(3,0,"Reading %s", filename.c_str());
	cFile* pFile = fs.open("C:\\Documents and Settings\\Borf\\Desktop\\eathena\\" + filename);
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
							if(y < (int)Graphics.world.gattiles.size())
							{
								if(x < (int)Graphics.world.gattiles[y].size())
								{
									s->pos.y = -Graphics.world.gattiles[y][x].cell1;
								}
							}
							if(direction < 0 || direction > 8)
								direction = 0;
							s->action = 0;
							s->direction = dirmap[(8-direction)%8];
							s->loadBody(rodir + "data\\sprite\\npc\\" + el->FirstChild()->Value());
							Graphics.world.sprites.push_back(s);
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
	for(unsigned int i = 0; i < Graphics.world.sprites.size(); i++)
		delete Graphics.world.sprites[i];

	Graphics.world.sprites.clear();
	scriptmap = Graphics.world.fileName;
	scriptmap = scriptmap.substr(scriptmap.rfind("\\")+1);
	if(!sprites.FirstChild())
		sprites = fs.getXml("sprites.xml");


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
	scriptmap = Graphics.world.fileName;
	scriptmap = scriptmap.substr(scriptmap.rfind("\\")+1);

	unsigned int i,ii;
	for(i = 0; i < Graphics.world.sprites.size(); i++)
	{
		Graphics.camerapointer.x = -5*Graphics.world.sprites[i]->pos.x;
		Graphics.camerapointer.y = -10*Graphics.world.height+5*Graphics.world.sprites[i]->pos.z;
		Graphics.camerarot = 0;



	    FILE *out;
		char filename[255];
		sprintf(filename, "npcs/%s_%i_%i.gif", scriptmap.c_str(), (int)Graphics.world.sprites[i]->pos.x, (int)Graphics.world.sprites[i]->pos.z);

	    out = fopen(filename, "wb");

		int size;
		gdImage* im = gdImageCreateTrueColor(320,240);
		void* ptr = gdImageGifAnimBeginPtr(im, &size, 1, 3);
		fwrite(ptr, size, 1, out); gdFree(ptr);

		gdImage* images[40];
		ZeroMemory(images, sizeof(images));

		for(ii = 0; ii < 16; ii++)
		{
			Graphics.draw(false);
			SDL_GL_SwapBuffers();
			Graphics.camerarot += 22.5f*((float)PI/180.0f);
			while(Graphics.camerarot > 360)
				Graphics.camerarot-=360;


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
#ifdef WIN32
	WIN32_FIND_DATA FileData;													// thingy for searching through a directory
	HANDLE hSearch;																// thingy for searching through a directory
	
	hSearch = FindFirstFile(std::string(rodir + "data/*.rsw").c_str(), &FileData);						// look for all files
	if (hSearch != INVALID_HANDLE_VALUE)										// if there are results...
	{
		while (true)														// loop through all the files
		{ 
			std::string filename = FileData.cFileName;
			if(filename != "." && filename != "..")
			{
				strcpy(Graphics.world.fileName, (rodir + "data\\" + filename.substr(0, filename.rfind("."))).c_str());
				Graphics.world.load();
				if(!sprites.FirstChild())
					sprites = fs.getXml("sprites.xml");


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
	l.pos = cVector3(mouseclickx/5, mouseclicky+atoi(n->FirstChildElement("height")->FirstChild()->Value()), mouseclickz/5);
	l.todo = std::string(buf, 40);
	l.todo2 = atoi(n->FirstChildElement("brightness")->FirstChild()->Value());
	l.maxLightIncrement = atoi(n->FirstChildElement("maxlight")->FirstChild()->Value());
	l.range = atoi(n->FirstChildElement("range")->FirstChild()->Value());
	l.lightFalloff = atof(n->FirstChildElement("lightFalloff")->FirstChild()->Value());

	Graphics.selectedObject = Graphics.world.lights.size();
	Graphics.world.lights.push_back(l);
	undostack.push(new cUndoNewLight());

	cWindow* w = Graphics.WM.getwindow(WT_LIGHTOVERVIEW);
	if(w != NULL)
	{
		w->userfunc(NULL);
		cLightOverViewWindow::cLightOverViewTree* tree = (cLightOverViewWindow::cLightOverViewTree*)w->objects["list"];
		Log(3,0,"Calling getobject for %i", tree);
		tree->getObject(Graphics.world.lights[Graphics.selectedObject]);
	}

	return true;
}


MENUCOMMAND(deselectlight)
{
	Graphics.selectedObject = -1;
	return true;
}


MENUCOMMAND(light_disableshadow)
{
	Graphics.world.lights[Graphics.selectedObject].givesShadow = !Graphics.world.lights[Graphics.selectedObject].givesShadow;
	return true;
}
MENUCOMMAND(light_snaptofloor)
{
	Graphics.world.lights[Graphics.selectedObject].pos.y = Graphics.world.cubes[(int)Graphics.world.lights[Graphics.selectedObject].pos.z/2][(int)Graphics.world.lights[Graphics.selectedObject].pos.x/2].cell1;
	return true;
}
MENUCOMMAND(light_setheight)
{
	delete popupmenu;
	popupmenu = NULL;


	MenuCommand_light_snaptofloor(src);
	Graphics.world.lights[Graphics.selectedObject].pos.y += atoi(Graphics.WM.InputWindow("Height:").c_str());


	return true;
}
















MENUCOMMAND(removefavlight)
{
	cFavoriteLightsWindow* w = (cFavoriteLightsWindow*)Graphics.WM.getwindow(WT_FAVLIGHTS);
	if(w != NULL)
	{
		cFavoriteLightsWindow::cFavoritesTree* tree = (cFavoriteLightsWindow::cFavoritesTree*)w->objects["list"];
		int i;
		int a = tree->selected;
		cWindowTree::cTreeNode* node;
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
	delete popupmenu;
	popupmenu = NULL;
	cFavoriteLightsWindow* w = (cFavoriteLightsWindow*)Graphics.WM.getwindow(WT_FAVLIGHTS);
	if(w != NULL)
	{
		cFavoriteLightsWindow::cFavoritesTree* tree = (cFavoriteLightsWindow::cFavoritesTree*)w->objects["list"];
		int i;
		int a = tree->selected;
		cWindowTree::cTreeNode* node;
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
			Graphics.WM.ShowMessage("You can't add a light to another light, you can only add lights to categories");
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

		std::string name = Graphics.WM.InputWindow("Light name:");
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
	delete popupmenu;
	popupmenu = NULL;
	cFavoriteLightsWindow* w = (cFavoriteLightsWindow*)Graphics.WM.getwindow(WT_FAVLIGHTS);
	if(w != NULL)
	{
		cFavoriteLightsWindow::cFavoritesTree* tree = (cFavoriteLightsWindow::cFavoritesTree*)w->objects["list"];
		int i;
		int a = tree->selected;
		cWindowTree::cTreeNode* node;
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
			Graphics.WM.ShowMessage("You can't add a category to a light, you can only add lights to categories");
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

		std::string catname = Graphics.WM.InputWindow("Category name:");
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
	std::string file = Graphics.WM.InputWindow("File to output:", "data/rotextures.txt");
	if(file == "")
		return false;
	std::ofstream pFile(file.c_str(), std::ios_base::binary | std::ios_base::out);
	unsigned int i;
	for(i = 0; i < fs.locations.size(); i++)
	{
		for(std::map<std::string, cFile*, std::less<std::string> >::iterator it = fs.locations[i]->files.begin(); it != fs.locations[i]->files.end(); it++)
		{
			if(it->first.substr(rodir.length(),13) != "data\\texture\\")
				continue;
			std::string ext = it->first.substr(it->first.length()-4);
			if(ext == ".jpg" || ext == ".bmp" || ext == ".tga")
			{
				std::string filename = it->first.substr(rodir.length()+13);
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
	std::string file = Graphics.WM.InputWindow("File to output:", "data/romodels.txt");
	if(file == "")
		return false;
	std::ofstream pFile(file.c_str(), std::ios_base::binary | std::ios_base::out);
	unsigned int i;
	for(i = 0; i < fs.locations.size(); i++)
	{
		for(std::map<std::string, cFile*, std::less<std::string> >::iterator it = fs.locations[i]->files.begin(); it != fs.locations[i]->files.end(); it++)
		{
			if(it->first.substr(rodir.length(),11) != "data\\model\\")
				continue;
			std::string ext = it->first.substr(it->first.length()-4);
			if(ext == ".rsm")
			{
				std::string filename = it->first.substr(rodir.length());
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
	std::string file = Graphics.WM.InputWindow("File to output:", "data/rosounds.txt");
	if(file == "")
		return false;
	std::ofstream pFile(file.c_str(), std::ios_base::binary | std::ios_base::out);
	unsigned int i;
	for(i = 0; i < fs.locations.size(); i++)
	{
		for(std::map<std::string, cFile*, std::less<std::string> >::iterator it = fs.locations[i]->files.begin(); it != fs.locations[i]->files.end(); it++)
		{
			if(it->first.substr(rodir.length(),9) != "data\\wav\\")
				continue;
			std::string ext = it->first.substr(it->first.length()-4);
			if(ext == ".wav")
			{
				std::string filename = it->first.substr(rodir.length());
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
	((cPluginBase*)src->pdata)->action(&Graphics.world);
	return true;
}


MENUCOMMAND(mapdatabase)
{
	Graphics.WM.addwindow(new cMapsWindow(Graphics.WM.texture, Graphics.WM.font, Graphics.WM.skin));
	return true;
}


MENUCOMMAND(saveOnline)
{
	if(!Graphics.WM.ConfirmWindow("This will save your map. Are you sure you want to save?"))
		return false;
	std::string mapname = Graphics.WM.InputWindow("Please enter the mapname", Graphics.world.fileName);
	std::string password = Graphics.WM.InputWindow("Please enter your browedit account password", "");
	std::map<std::string, bool, std::less<std::string> > textures;
	std::map<std::string, bool, std::less<std::string> > models;
	int i;
	for(i = 0; i < Graphics.world.textures.size(); i++)
		textures[Graphics.world.textures[i]->RoFilename] = true;
	for(i = 0; i < Graphics.world.models.size(); i++)
	{
		models[Graphics.world.models[i]->rofilename] = true;
		for(int ii = 0; ii < Graphics.world.models[i]->textures.size(); ii++)
		{
			std::string filename = Graphics.world.models[i]->textures[ii]->getfilename();
			filename = filename.substr(rodir.length() + 13);
			textures[filename] = true;
		}
	}

	std::string resources;

	resources = "mapname=" + mapname;
	resources+= "&uid=" + inttostring(userid);
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
				cProgressWindow* w = new cProgressWindow(Graphics.WM.texture, Graphics.WM.font, NULL, Graphics.WM.skin);
				Graphics.WM.addwindow(w);
				w->objects["progress"]->setInt(1,0);
				w->objects["progress"]->setInt(2,lines.size());
				renderMutex->unlock();

				for(int i = 0; i < lines.size(); i++)
				{
					renderMutex->lock();
					w->objects["progress"]->setInt(0,i);
					w->objects["lblStatus"]->setText(0,"Uploading " + lines[i]);
					renderMutex->unlock();
					if(lines[i].find("texture:") == 0)
					{
						std::string filename = lines[i].substr(8);
						cFile* pFile = fs.open(rodir + "data/texture/" + filename);
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
						cFile* pFile = fs.open(rodir + "data/model/" + filename);
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
						Graphics.WM.ShowMessage("Error: " + lines[i].substr(6));
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


	Graphics.world.save();			

	Graphics.WM.ShowMessage("Please move your viewpoint to show the map on the thumbnail");


	cFile* rsw = fs.open(std::string(Graphics.world.fileName) + ".rsw");
	cFile* gat = fs.open(std::string(Graphics.world.fileName) + ".gat");
	cFile* gnd = fs.open(std::string(Graphics.world.fileName) + ".gnd");

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
		CURLFORM_COPYCONTENTS, Graphics.world.fileName, 
		CURLFORM_END);
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME, "uid",
		CURLFORM_COPYCONTENTS, inttostring(userid).c_str(), 
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

inline void setLightIntensity( BYTE* buf, int yy, int xx, cVector3 worldpos )
{
	int from;
	unsigned int to = Graphics.world.lights.size();
	if(lightonly)
	{
		from = Graphics.selectedObject;
		to = from+1;
	}
	for(unsigned int i = from; i < to; i++)
	{
		if(buf[yy*8 + xx] == 255)
			break;
		
		cLight* l = &Graphics.world.lights[i];
		cVector3 lightpos = cVector3(l->pos.x*5, l->pos.y, l->pos.z*5);
		cVector3 diff = worldpos - lightpos;
		
		if(diff.y > 0)
			continue;
		
		float length = diff.Magnitude();
		if(length > l->range)
			continue;
		
		float obstructed = 1;
		
		if(l->givesShadow && !noshadow)
		{
			for(unsigned int ii = 0; ii < Graphics.world.models.size() && obstructed > 0; ii++)
			{
				if(Graphics.world.models[ii]->lightopacity != 0)
					if(Graphics.world.models[ii]->collides(worldpos, lightpos))
						obstructed -= Graphics.world.models[ii]->lightopacity;
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