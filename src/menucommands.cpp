#include "menucommands.h"
#include "graphics.h"
#include <SDL/SDL_syswm.h>
#include "undo.h"
#include "filesystem.h"
#include "wm/waterwindow.h"
#include "wm/ambientlightwindow.h"
#include "wm/keybindwindow.h"
#include "wm/rsmeditwindow.h"
#include "wm/favoritelights.h"
#include "wm/modeloverviewwindow.h"
#include "wm/lightoverviewwindow.h"
#include "wm/progresswindow.h"
#include "wm/filewindow.h"

extern cGraphics Graphics;
extern bool running;
extern cUndoStack undostack;
extern eMode editmode;
extern int brushsize;
extern double mouse3dx, mouse3dy, mouse3dz;
int process_events( );
extern vector<string> objectfiles;
extern cFileSystem fs;
extern string rodir;
extern cMenu* mode;
extern cMenu* editdetail;
extern cMenu* speed;
extern cMenu* models;
extern cMenu* currentobject;
extern float paintspeed;
extern TiXmlDocument sprites;
extern double mouseclickx, mouseclicky, mouseclickz;
extern string message;
extern bool showmessage;

void mainloop();


cMenuItem* selectedeffect = NULL;

MENUCOMMAND(new)
{
	Graphics.WM.MessageBox("This feature isn't working yet...");
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

	strcpy(Graphics.world.filename, replace(Graphics.world.filename, "/", "\\").c_str());
	ofn.lpstrFile = Graphics.world.filename;
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = "All\0*.*\0RO Maps\0*.rsw\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	if (GetOpenFileName(&ofn))
	{
		while(Graphics.world.filename[strlen(Graphics.world.filename)-1] != '.')
			Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
		Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';

		chdir(curdir);
		Graphics.world.load();
	}
#else


#endif
	return true;
}




void openfunc(string param)
{
	ZeroMemory(Graphics.world.filename, 128);
	memcpy(Graphics.world.filename, param.c_str(), param.length());
	Graphics.world.load();
}

MENUCOMMAND(opengrf)
{
	Graphics.WM.addwindow(new cFileWindow(Graphics.WM.texture, &Graphics.WM.font, openfunc,Graphics.WM.skin));
	return true;
}

MENUCOMMAND(save)
{
#ifdef WIN32
	if(Graphics.world.filename[0] == '\0')
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
		strcpy(Graphics.world.filename, replace(Graphics.world.filename, "/", "\\").c_str());
		ofn.lpstrFile = Graphics.world.filename;
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
		ofn.nFilterIndex = 2;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
		if (GetSaveFileName(&ofn))
		{
			while(Graphics.world.filename[strlen(Graphics.world.filename)-1] != '.')
				Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
			Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
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
	Graphics.world.quicksave = true;
#ifdef WIN32
	if(Graphics.world.filename[0] == '\0')
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
		strcpy(Graphics.world.filename, replace(Graphics.world.filename, "/", "\\").c_str());
		ofn.lpstrFile = Graphics.world.filename;
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
		ofn.nFilterIndex = 2;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
		if (GetSaveFileName(&ofn))
		{
			while(Graphics.world.filename[strlen(Graphics.world.filename)-1] != '.')
				Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
			Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
			Graphics.world.save();
		}
		chdir(curdir);
	}
	else
		Graphics.world.save();
#else

#endif

	Graphics.world.quicksave = false;

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
	strcpy(Graphics.world.filename, replace(Graphics.world.filename, "/", "\\").c_str());
	ofn.lpstrFile = Graphics.world.filename;
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
	if (!GetSaveFileName(&ofn))
	{
		Graphics.world.filename[0] = '\0';
		chdir(curdir);
	}
	else
	{
		if (strcmp(Graphics.world.filename+strlen(Graphics.world.filename)-4, ".rsw") == 0)
		{ 
			while(Graphics.world.filename[strlen(Graphics.world.filename)-1] != '.')
				Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
			Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
		}
		chdir(curdir);
		Graphics.world.save();

	}
#else
	string input = Graphics.WM.InputWindow("Filename (so not add .rsw): ", Graphics.world.filename);
	if(input != "")
	{
		sprintf(Graphics.world.filename, "%s", input.c_str());
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
		Graphics.WM.MessageBox("You must enter a height bigger then 0");
		return true;
	}

	undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));
	int x,y;
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[y][x].tileaside = -1;
			Graphics.world.cubes[y][x].tileside = -1;
		}
	}
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
		//	Graphics.world.cubes[2*y][2*x].tileup = 1;

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
			Graphics.world.cubes[(int)y][(int)x].tileaside = -1;
			Graphics.world.cubes[(int)y][(int)x].tileside = -1;
			Graphics.world.cubes[(int)y][(int)x].tileup = 0;
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

	vector<string>	randommodels;
	for(i = 0; i < objectfiles.size(); i++)
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
				Graphics.world.cubes[(int)y][(int)x].tileup= 50 + ((int)x%5) + 5*((int)y%5);
			else if(Graphics.world.cubes[(int)y][(int)x].cell1 >= -63)
				Graphics.world.cubes[(int)y][(int)x].tileup= 25 + ((int)x%5) + 5*((int)y%5);
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
			Graphics.world.cubes[(int)y][(int)x].maxh = -99999;
			Graphics.world.cubes[(int)y][(int)x].minh = 99999;
		}

	for(i = 0; i < Graphics.world.models.size(); i++)
		Graphics.world.models[i]->draw(false,false,true);


	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			if(Graphics.world.cubes[(int)y][(int)x].maxh != -99999)
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
	vector<int> connections;
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
			Graphics.world.cubes[(int)y][(int)x].tileaside = -1;
			Graphics.world.cubes[(int)y][(int)x].tileside = -1;
			Graphics.world.cubes[(int)y][(int)x].tileup = 75 + ((int)x%5) + 5*((int)y%5);
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

	
	vector<cIntQuad> islands;

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
						Graphics.world.cubes[yy][xx].tileup = 50 + (xx%5) + 5*(yy%5);
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
			Graphics.world.cubes[(int)y][(int)x].maxh = -99999;
			Graphics.world.cubes[(int)y][(int)x].minh = 99999;
		}

	for(i = 0; i < Graphics.world.models.size(); i++)
		Graphics.world.models[i]->draw(false,false,true);


	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			if(Graphics.world.cubes[(int)y][(int)x].maxh != -99999)
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
			Graphics.world.cubes[y][x].tileaside = -1;
			Graphics.world.cubes[y][x].tileside = -1;
			Graphics.world.cubes[y][x].tileup = 25 + ((int)x%5) + 5*((int)y%5);
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
				Graphics.world.cubes[y][x].tileup = 50 + ((int)x%5) + 5*((int)y%5);
		}
	}


	vector<string>	randommodels;
	for(i = 0; i < objectfiles.size(); i++)
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

			while(Graphics.world.cubes[(int)(model->pos.z/2)][(int)(model->pos.x/2)].tileup > 50)
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
	string title = src->title;
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
		Graphics.selectedobject = -1;
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

MENUCOMMAND(flatten)
{
	undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));
	for(int y = 0; y < Graphics.world.height; y++)
	{
		for(int x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[y][x].tileaside = -1;
			Graphics.world.cubes[y][x].tileside = -1;
			Graphics.world.cubes[y][x].cell1 = 0;
			Graphics.world.cubes[y][x].cell2 = 0;
			Graphics.world.cubes[y][x].cell3 = 0;
			Graphics.world.cubes[y][x].cell4 = 0;
			Graphics.world.cubes[y][x].calcnormal();
		}
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

MENUCOMMAND(fill)
{
	int x,y,i;
	map<int, bool, less<int> > used;

	Graphics.world.tiles.clear();
	Graphics.world.lightmaps.clear();
	cLightmap* map = new cLightmap();
	for(i = 0; i < 256; i++)
		map->buf[i] = i < 64 ? 255 : 0;
	Graphics.world.lightmaps.push_back(map);
	map = new cLightmap();
	for(i = 0; i < 256; i++)
		map->buf[i] = i < 64 ? 255 : 0;
	Graphics.world.lightmaps.push_back(map);
	for(y = 0; y < 4; y++)
	{
		for(x = 0; x < 4; x++)
		{
			cTile t;
			t.lightmap = 1;
			t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
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
			Graphics.world.tiles.push_back(t);
		}
	}


	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[y][x].tileup = (x%4) + 4*(y % 4);
			Graphics.world.cubes[y][x].tileside = -1;
			Graphics.world.cubes[y][x].tileaside = -1;
		}
	}


	for(x = 0; x < Graphics.world.width; x++)
	{
		for(y = 0; y < Graphics.world.height; y++)
		{
			int tile = Graphics.world.cubes[y][x].tileup;
			if(used.find(tile) != used.end())
			{
				cTile t = Graphics.world.tiles[tile];
				tile = Graphics.world.tiles.size();
				Graphics.world.tiles.push_back(t);
				Graphics.world.cubes[y][x].tileup = tile;
			}
			used[tile] = 1;
///////////////////////////////////////
			tile = Graphics.world.cubes[y][x].tileside;
			if (tile != -1)
			{
				if(used.find(tile) != used.end())
				{
					cTile t = Graphics.world.tiles[tile];
					tile = Graphics.world.tiles.size();
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileside = tile;
				}
				used[tile] = 1;
			}
/////////////////////////////////////
			tile = Graphics.world.cubes[y][x].tileaside;
			if (tile!= -1)
			{
				if(used.find(tile) != used.end())
				{
					cTile t = Graphics.world.tiles[tile];
					tile = Graphics.world.tiles.size();
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileaside = tile;
				}
				used[tile] = 1;
			}
		}
	}


	

	return true;
}

MENUCOMMAND(showobjects)
{
	src->ticked = !src->ticked;
	Graphics.showobjects = src->ticked;
	return true;
}


MENUCOMMAND(model)
{
	delete Graphics.previewmodel;
	Graphics.previewmodel = new cRSMModel();
	Graphics.previewmodel->load(rodir + src->data);
	Graphics.previewmodel->rot = cVector3(0,0,0);
	Graphics.previewmodel->scale = cVector3(4,4,4);

	Graphics.previewmodel->pos = cVector3(40,-40,-40);

	if (editmode != MODE_OBJECTS)
		Graphics.previewcolor = 200;
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
			Graphics.world.cubes[y][x].maxh = -99999;
			Graphics.world.cubes[y][x].minh = 99999;
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
			Graphics.world.cubes[y][x].maxh = -99999;
			Graphics.world.cubes[y][x].minh = 99999;
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
			if(Graphics.world.cubes[y/2][x/2].maxh == -99999 || Graphics.world.cubes[y/2][x/2].minh == 99999)
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
			Graphics.world.cubes[y][x].maxh = 0;
			Graphics.world.cubes[y][x].minh = 0;
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
		moveto(0,20);
		resizeto(100,20);
		text = "Preview";
	}
	void click()
	{
		Graphics.world.loaded = !Graphics.world.loaded;
	}
};


MENUCOMMAND(dolightmaps2)
{
	int x,y;
	unsigned int i;

	bool rendering = true;


	cProgressWindow* w = new cProgressWindow(Graphics.WM.texture, &Graphics.WM.font, &rendering, Graphics.WM.skin);
	Graphics.WM.addwindow(w);
	w->objects["progress"]->SetInt(1,0);
	w->objects["progress"]->SetInt(2,Graphics.world.height * Graphics.world.width);
	w->objects["toggle"] = new cWindowPreviewButton(w, Graphics.WM.skin);

	mainloop();

	Log(3,0,"Starting Lightmap Clearing");
	Graphics.world.makelightmapsunique();
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
//			Log(3,0,GetMsg("PERCENTAGE"), (y*Graphics.world.width+x) / (float)(Graphics.world.height * Graphics.world.width)*100); // %f %%
			w->objects["progress"]->SetInt(0, y*Graphics.world.width + x);

			
			if(c->tileup != -1)
			{
				BYTE* buf = (BYTE*)Graphics.world.lightmaps[Graphics.world.tiles[c->tileup].lightmap]->buf;

				for(int yy = 1; yy < 7; yy++)
				{
					for(int xx = 1; xx < 7; xx++)
					{
						float fx = (xx-1)/6.0f;
						float fy = (yy-1)/6.0f;

						cVector3 worldpos = cVector3(	10*x+(10/6.0)*(xx-1), 
														-((c->cell1*(1-fx)+c->cell2*(fx)) + (c->cell1*(fy)+c->cell3*(1-fy))-c->cell1),
														10*y+(10/6.0)*(yy-1));
						
						int from = 0;
						unsigned int to = Graphics.world.lights.size();
						if(lightonly)
						{
							from = Graphics.selectedobject;
							to = from+1;
						}
						for(i = from; i < to; i++)
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

							if(l->givesshadow && !noshadow)
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
								float intensity = (int)min((int)(l->maxlightincrement), (int)(pow(1-(length / l->range), l->lightfalloff) * l->todo2));
								intensity *= obstructed;

								buf[yy*8 + xx] = min(255, buf[yy*8 + xx] + max(0, (int)(intensity)));

								buf[64 + 3*(yy*8 + xx)+0] = min(255, buf[64 + 3*(yy*8 + xx)+0] + max(0, (int)(intensity*l->color.x)));
								buf[64 + 3*(yy*8 + xx)+1] = min(255, buf[64 + 3*(yy*8 + xx)+1] + max(0, (int)(intensity*l->color.y)));
								buf[64 + 3*(yy*8 + xx)+2] = min(255, buf[64 + 3*(yy*8 + xx)+2] + max(0, (int)(intensity*l->color.z)));
							}
						}
					}
				}
				Graphics.world.reallightmaps[y][x]->reset();
			}

			if(c->tileside != -1)
			{
				BYTE* buf = (BYTE*)Graphics.world.lightmaps[Graphics.world.tiles[c->tileside].lightmap]->buf;
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
						
						int from = 0;
						unsigned int to = Graphics.world.lights.size();
						if(lightonly)
						{
							from = Graphics.selectedobject;
							to = from+1;
						}
						for(i = from; i < to; i++)
						{
							if(buf[yy*8 + xx] == 255)
								break;

							cLight* l = &Graphics.world.lights[i];
							cVector3 lightpos = cVector3(l->pos.x*5, l->pos.y, l->pos.z*5);
							cVector3 diff = worldpos - lightpos;

							if(diff.z < 0 && c->cell3 > c2->cell1)
								continue;
							if(diff.z > 0 && c->cell3 < c2->cell1)
								continue;

							
							float length = diff.Magnitude();
							if(length > l->range)
								continue;

							float obstructed = 1;

							if(l->givesshadow && !noshadow)
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
								float intensity = (int)min((int)(l->maxlightincrement), (int)(pow(1-(length / l->range), l->lightfalloff) * l->todo2));
								intensity *= obstructed;

								buf[yy*8 + xx] = min(255, buf[yy*8 + xx] + max(0, (int)(intensity)));

								buf[64 + 3*(yy*8 + xx)+0] = min(255, buf[64 + 3*(yy*8 + xx)+0] + max(0, (int)(intensity*l->color.x)));
								buf[64 + 3*(yy*8 + xx)+1] = min(255, buf[64 + 3*(yy*8 + xx)+1] + max(0, (int)(intensity*l->color.y)));
								buf[64 + 3*(yy*8 + xx)+2] = min(255, buf[64 + 3*(yy*8 + xx)+2] + max(0, (int)(intensity*l->color.z)));
							}
						}
					}
				}
				Graphics.world.lightmaps[Graphics.world.tiles[c->tileside].lightmap]->del();
				Graphics.world.lightmaps[Graphics.world.tiles[c->tileside].lightmap]->del2();
			}

			if(c->tileaside != -1)
			{
				BYTE* buf = (BYTE*)Graphics.world.lightmaps[Graphics.world.tiles[c->tileaside].lightmap]->buf;
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
						
						int from = 0;
						unsigned int to = Graphics.world.lights.size();
						if(lightonly)
						{
							from = Graphics.selectedobject;
							to = from+1;
						}
						for(i = from; i < to; i++)
						{
							if(buf[yy*8 + xx] == 255)
								break;

							cLight* l = &Graphics.world.lights[i];
							cVector3 lightpos = cVector3(l->pos.x*5, l->pos.y, l->pos.z*5);
							cVector3 diff = worldpos - lightpos;

							if(diff.x < 0 && c->cell4 > c2->cell3)
								continue;
							if(diff.x > 0 && c->cell4 < c2->cell3)
								continue;

							float length = diff.Magnitude();
							if(length > l->range)
								continue;

							float obstructed = 1;

							if(l->givesshadow && !noshadow)
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
								float intensity = (int)min((int)(l->maxlightincrement), (int)(pow(1-(length / l->range), l->lightfalloff) * l->todo2));
								intensity *= obstructed;

								buf[yy*8 + xx] = min(255, buf[yy*8 + xx] + max(0, (int)(intensity)));

								buf[64 + 3*(yy*8 + xx)+0] = min(255, buf[64 + 3*(yy*8 + xx)+0] + max(0, (int)(intensity*l->color.x)));
								buf[64 + 3*(yy*8 + xx)+1] = min(255, buf[64 + 3*(yy*8 + xx)+1] + max(0, (int)(intensity*l->color.y)));
								buf[64 + 3*(yy*8 + xx)+2] = min(255, buf[64 + 3*(yy*8 + xx)+2] + max(0, (int)(intensity*l->color.z)));
							}
						}
					}
				}
				Graphics.world.lightmaps[Graphics.world.tiles[c->tileaside].lightmap]->del();
				Graphics.world.lightmaps[Graphics.world.tiles[c->tileaside].lightmap]->del2();
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

	Graphics.world.fixgridding();
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
			int tile = Graphics.world.cubes[y][x].tileup;
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

								if (c->tileaside != -1)
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
								if (c->tileside != -1 && y < Graphics.world.width - 1)
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
	if(c->tileup != -1)
	{
		cTile* t = &Graphics.world.tiles[c->tileup];
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
	string strFactor = Graphics.WM.InputWindow("Smoothing factor:", "1");
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
							total += factor * *getLightMap(x+xx,y+yy);
							count+=factor;
						}
						else
						{
							total += *getLightMap(x+xx,y+yy);
							count++;
						}
					}
				}
			}
			if(total/count != *getLightMap(x,y))
				Sleep(0);
			buf[x+Graphics.world.width*6*y] = min(max(total / count,0),255);
		}
	}

	for(x = 0; x < Graphics.world.width*6; x++)
	{
		for(y = 0; y < Graphics.world.height*6; y++)
		{
			*getLightMap(x,y) = buf[x+Graphics.world.width*6*y];
		}
	}
	for(x = 0; x < Graphics.world.width; x++)
		for(y = 0; y < Graphics.world.height; y++)
			Graphics.world.reallightmaps[y][x]->reset();
	Graphics.world.fixgridding();


	return true;
}

MENUCOMMAND(fixcolors)
{
	int x,y;
	for(x = 0; x < Graphics.world.width; x++)
		for(y = 0; y < Graphics.world.height; y++)
		{
			int tile = Graphics.world.cubes[y][x].tileup;
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

	Graphics.world.makelightmapsunique();
	Graphics.world.loadlightmap();
	Graphics.world.fixgridding();

	return true;
}


MENUCOMMAND(clearobjects)
{
	unsigned int i;
	vector<cUndoObjectsDelete::cObject> objectsdeleted;
	for(i = 0; i < Graphics.world.models.size(); i++)
	{
		cUndoObjectsDelete::cObject object;
		object.filename = Graphics.world.models[i]->filename;
		object.pos = Graphics.world.models[i]->pos;
		object.rot = Graphics.world.models[i]->rot;
		object.scale = Graphics.world.models[i]->scale;
		object.id = i;
		objectsdeleted.push_back(object);
	}
	undostack.push(new cUndoObjectsDelete(objectsdeleted));
	for(i = 0; i < Graphics.world.models.size(); i++)
		delete Graphics.world.models[i];
	Graphics.world.models.clear();
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
			if (c->tileaside == -1)
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
					Graphics.world.cubes[y][x].tileaside = Graphics.world.tiles.size()-1;
				}
			}
			if (c->tileside == -1)
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
					Graphics.world.cubes[y][x].tileside = Graphics.world.tiles.size()-1;
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

MENUCOMMAND(clearlightmaps)
{
	unsigned int i;
	for(i = 0; i < Graphics.world.lightmaps.size(); i++)
		delete 	Graphics.world.lightmaps[i];
	Graphics.world.lightmaps.clear();
	cLightmap* m = new cLightmap();
	for(i = 0; i < 256; i++)
		m->buf[i] = i < 64 ? 255 : 0;
	Graphics.world.lightmaps.push_back(m);

	for(i = 0; i < Graphics.world.tiles.size(); i++)
		Graphics.world.tiles[i].lightmap = 0;

	return true;
}


MENUCOMMAND(cleanuplightmaps)
{
	unsigned int i;
	vector<int> newvalue;
	map<int, bool, less<int> > used;
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
			if(Graphics.world.lightmaps[Graphics.world.tiles[c->tileup].lightmap]->buf[10] == 0)
			{
				c->cell1 = -20;
				c->cell2 = -20;
				c->cell3 = -20;
				c->cell4 = -20;

				c->tileup = Graphics.world.tiles.size()-2;
				c->tileside = -1;
				c->tileaside = -1;
			}
			else
			{
				c->tileside = -1;
				c->tileaside = -1;
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
	if (Graphics.texturepreview == NULL || Graphics.texturepreview->getfilename() != rodir + "data\\texture\\" + ((cMenuItem*)src)->data)
	{
		Graphics.texturepreview = TextureCache.load(rodir + "data\\texture\\" + ((cMenuItem*)src)->data);
		return false;
	}
	else
	{
		TextureCache.unload(Graphics.texturepreview);
		Graphics.texturepreview = NULL;
		return true;
	}
}
bool mouseouttexture(cMenu* src)
{
	if (Graphics.texturepreview != NULL)
	{
		TextureCache.unload(Graphics.texturepreview);
		Graphics.texturepreview = NULL;
	}
	return true;
}

MENUCOMMAND(clearstuff)
{
	MenuCommand_flatten(src);
	MenuCommand_clearobjects(src);
	MenuCommand_clearlightmaps(src);
	MenuCommand_fill(src);
	MenuCommand_fixcolors(src);
	Graphics.world.effects.clear();
	Graphics.world.lights.clear();
	Graphics.world.sounds.clear();
	return true;
}

MENUCOMMAND(effect)
{
	if (selectedeffect != NULL)
		selectedeffect->ticked = false;
	src->ticked = true;
	if (Graphics.selectedobject != -1)
	{
		Graphics.world.effects[Graphics.selectedobject].type = atoi(src->data.c_str());
		Graphics.world.effects[Graphics.selectedobject].readablename = src->title;
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
	cWindow* w = new cWaterWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin);
	sprintf(buf, "%f", Graphics.world.water.amplitude);		w->objects["amplitude"]->SetText(0,buf);
	sprintf(buf, "%f", Graphics.world.water.height);		w->objects["height"]->SetText(0,buf);
	sprintf(buf, "%f", Graphics.world.water.phase);			w->objects["phase"]->SetText(0,buf);
	sprintf(buf, "%f", Graphics.world.water.surfacecurve);	w->objects["surfacecurve"]->SetText(0,buf);
	sprintf(buf, "%i", Graphics.world.water.type);			w->objects["type"]->SetText(0,buf);
	Graphics.WM.addwindow(w);
	return true;
}

MENUCOMMAND(cleantextures)
{
	Graphics.world.clean();
	vector<bool> used;
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
	cWindow* w = new cAmbientLightWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin);
	sprintf(buf, "%i", Graphics.world.ambientlight.ambientr);		w->objects["ambientr"]->SetText(0,buf);
	sprintf(buf, "%i", Graphics.world.ambientlight.ambientg);		w->objects["ambientg"]->SetText(0,buf);
	sprintf(buf, "%i", Graphics.world.ambientlight.ambientb);		w->objects["ambientb"]->SetText(0,buf);

	sprintf(buf, "%f", Graphics.world.ambientlight.diffuse.x);		w->objects["diffuser"]->SetText(0,buf);
	sprintf(buf, "%f", Graphics.world.ambientlight.diffuse.y);		w->objects["diffuseg"]->SetText(0,buf);
	sprintf(buf, "%f", Graphics.world.ambientlight.diffuse.z);		w->objects["diffuseb"]->SetText(0,buf);

	sprintf(buf, "%f", Graphics.world.ambientlight.shadow.x);		w->objects["shadowr"]->SetText(0,buf);
	sprintf(buf, "%f", Graphics.world.ambientlight.shadow.y);		w->objects["shadowg"]->SetText(0,buf);
	sprintf(buf, "%f", Graphics.world.ambientlight.shadow.z);		w->objects["shadowb"]->SetText(0,buf);
	
	sprintf(buf, "%f", Graphics.world.ambientlight.alpha);			w->objects["alpha"]->SetText(0,buf);

	Graphics.WM.addwindow(w);
	return true;
}


MENUCOMMAND(cleareffects)
{
	unsigned int i;
	vector<int> objectsdeleted;
	for(i = 0; i < Graphics.world.effects.size(); i++)
		objectsdeleted.push_back(i);
	undostack.push(new cUndoEffectsDelete(objectsdeleted));

	Graphics.world.effects.clear();
	return true;
}

MENUCOMMAND(clearlights)
{
	Graphics.world.lights.clear();
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
	Graphics.WM.addwindow(new cKeyBindWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin));
	return true;
}



MENUCOMMAND(fillarea)
{
	int x,y,i;
	map<int, bool, less<int> > used;

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
				Graphics.world.cubes[y][x].tileup = Graphics.world.tiles.size()-1;;
			}
		}
	}


	for(x = 0; x < Graphics.world.width; x++)
	{
		for(y = 0; y < Graphics.world.height; y++)
		{
			int tile = Graphics.world.cubes[y][x].tileup;
			if(used.find(tile) != used.end())
			{
				cTile t = Graphics.world.tiles[tile];
				tile = Graphics.world.tiles.size();
				Graphics.world.tiles.push_back(t);
				Graphics.world.cubes[y][x].tileup = tile;
			}
			used[tile] = 1;
///////////////////////////////////////
			tile = Graphics.world.cubes[y][x].tileside;
			if (tile != -1)
			{
				if(used.find(tile) != used.end())
				{
					cTile t = Graphics.world.tiles[tile];
					tile = Graphics.world.tiles.size();
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileside = tile;
				}
				used[tile] = 1;
			}
/////////////////////////////////////
			tile = Graphics.world.cubes[y][x].tileaside;
			if (tile!= -1)
			{
				if(used.find(tile) != used.end())
				{
					cTile t = Graphics.world.tiles[tile];
					tile = Graphics.world.tiles.size();
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileaside = tile;
				}
				used[tile] = 1;
			}
		}
	}


	

	return true;
}



MENUCOMMAND(rsmedit)
{
	Graphics.WM.addwindow(new cRSMEditWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin));
	return true;
}

MENUCOMMAND(favlights)
{
	Graphics.WM.addwindow(new cFavoriteLightsWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin));
	return true;
}


MENUCOMMAND(exportmapfiles)
{
#ifdef WIN32
	CreateDirectory(Graphics.world.filename, NULL);
	CreateDirectory((Graphics.world.filename + string("\\texture\\")).c_str(), NULL);

	int i;
	ofstream pFile((string(Graphics.world.filename) + ".txt").c_str());
	for(i = 0; i < Graphics.world.textures.size(); i++)
	{
		cFile* pF = fs.open(rodir + "data\\texture\\" + Graphics.world.textures[i]->RoFilename);
		if(pF->location != -1)
		{
			pF->close();
			continue;
		}
		pF->close();


		CopyFile((rodir + "data\\texture\\" + Graphics.world.textures[i]->RoFilename).c_str(), (string(Graphics.world.filename) + "\\texture\\" + Graphics.world.textures[i]->RoFilename2).c_str(), false);
		pFile.write("texture\\", 8);
		pFile.write(Graphics.world.textures[i]->RoFilename.c_str(), Graphics.world.textures[i]->RoFilename.length());
		pFile.put('\r');
		pFile.put('\n');
	}

	map<string, bool, less<string> > usedmodels;

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
			string file = Graphics.world.models[i]->textures[ii]->getfilename();
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
	ShellExecute(NULL,"open",(string(Graphics.world.filename) + ".txt").c_str(),NULL,"c:\\",SW_SHOW);
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
			Graphics.world.cubes[(int)y][(int)x].tileaside = -1;
			Graphics.world.cubes[(int)y][(int)x].tileside = -1;
			Graphics.world.cubes[(int)y][(int)x].tileup = 0 + ((int)x%5) + 5*((int)y%5);
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

	
	vector<cIntQuad> islands;

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
						Graphics.world.cubes[yy][xx].tileup = 25 + (xx%5) + 5*(yy%5);
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
						Graphics.world.cubes[yy+yyy][xx+xxx].tileup = 25 + ((xx+xxx)%5) + 5*((yy+yyy)%5);
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
						Graphics.world.cubes[yy+yyy][xx+xxx].tileup = 25 + ((xx+xxx)%5) + 5*((yy+yyy)%5);
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
				Graphics.world.cubes[y][x].tileup= 50 + ((int)x%5) + 5*((int)y%5);
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
			sprintf(Graphics.world.filename, "%sdata\\random_pay", rodir.c_str());
			Graphics.world.load();
			MenuCommand_random2(src);
		}
		else if (mode == 1)
		{
			sprintf(Graphics.world.filename, "%sdata\\random_cul", rodir.c_str());
			Graphics.world.load();
			MenuCommand_random3(src);
		}
		else if (mode == 2)
		{
			sprintf(Graphics.world.filename, "%sdata\\random_ama", rodir.c_str());
			Graphics.world.load();
			MenuCommand_random5(src);
		}
		sprintf(Graphics.world.filename, "%sdata\\ulti_dun%02i", rodir.c_str(), i);
		Graphics.world.save();
	}


	return true;
}


string scriptmap;
char dirmap[] = { 4,3,2,1,0,7,6,5 };

void readscript(string filename)
{
	Log(3,0,"Reading %s", filename.c_str());
	cFile* pFile = fs.open("C:\\Documents and Settings\\Borf\\Desktop\\eathena\\" + filename);
	if(pFile == NULL)
		return;

	while(!pFile->eof())
	{
		string line = pFile->readline();
		if(ltrim(rtrim(line)).substr(0,4) == "npc:")
			readscript(ltrim(ltrim(rtrim(line)).substr(4)));
		else if(ltrim(rtrim(line)).substr(0,7) == "import:")
			readscript(ltrim(ltrim(rtrim(line)).substr(7)));
		else if(line.find("\tscript\t") != string::npos || line.find("\tduplicate") != string::npos)
		{
			char mapname[256];
			char npcname[256];
			ZeroMemory(mapname, 256);
			ZeroMemory(npcname, 256);
			int x,y,direction,spriteid;

			char buf[256];
			sprintf(buf, "%s", line.c_str());
			int ret = sscanf(buf, "%15[^,],%d,%d,%d\tscript\t%250[^\t]%d", mapname,&x,&y,&direction, npcname, &spriteid);
			if(line.find("\tduplicate") != string::npos)
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
						if(string(el->FirstChild()->Value()).find("gr2") == string::npos)
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
							s->loadbody(rodir + "data\\sprite\\npc\\" + el->FirstChild()->Value());
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
	scriptmap = Graphics.world.filename;
	scriptmap = scriptmap.substr(scriptmap.rfind("\\")+1);
	if(!sprites.FirstChild())
		sprites = fs.getxml("sprites.xml");


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
	scriptmap = Graphics.world.filename;
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
	
	hSearch = FindFirstFile(string(rodir + "data/*.rsw").c_str(), &FileData);						// look for all files
	if (hSearch != INVALID_HANDLE_VALUE)										// if there are results...
	{
		while (true)														// loop through all the files
		{ 
			string filename = FileData.cFileName;
			if(filename != "." && filename != "..")
			{
				strcpy(Graphics.world.filename, (rodir + "data\\" + filename.substr(0, filename.rfind("."))).c_str());
				Graphics.world.load();
				if(!sprites.FirstChild())
					sprites = fs.getxml("sprites.xml");


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
	vector<int> keys;
	string key = src->data;
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
	l.todo = string(buf, 40);
	l.todo2 = atoi(n->FirstChildElement("brightness")->FirstChild()->Value());
	l.maxlightincrement = atoi(n->FirstChildElement("maxlight")->FirstChild()->Value());
	l.range = atoi(n->FirstChildElement("range")->FirstChild()->Value());
	l.lightfalloff = atof(n->FirstChildElement("lightfalloff")->FirstChild()->Value());

	Graphics.selectedobject = Graphics.world.lights.size();
	Graphics.world.lights.push_back(l);
	undostack.push(new cUndoNewLight());

	cWindow* w = Graphics.WM.getwindow(WT_LIGHTOVERVIEW);
	if(w != NULL)
	{
		w->userfunc(NULL);
		cLightOverViewWindow::cLightOverViewTree* tree = (cLightOverViewWindow::cLightOverViewTree*)w->objects["list"];
		Log(3,0,"Calling getobject for %i", tree);
		tree->getobject(Graphics.world.lights[Graphics.selectedobject]);
	}

	return true;
}


MENUCOMMAND(deselectlight)
{
	Graphics.selectedobject = -1;
	return true;
}


MENUCOMMAND(light_disableshadow)
{
	Graphics.world.lights[Graphics.selectedobject].givesshadow = !Graphics.world.lights[Graphics.selectedobject].givesshadow;
	return true;
}
MENUCOMMAND(light_snaptofloor)
{
	Graphics.world.lights[Graphics.selectedobject].pos.y = Graphics.world.cubes[(int)Graphics.world.lights[Graphics.selectedobject].pos.z/2][(int)Graphics.world.lights[Graphics.selectedobject].pos.x/2].cell1;
	return true;
}
MENUCOMMAND(light_setheight)
{
	delete popupmenu;
	popupmenu = NULL;


	MenuCommand_light_snaptofloor(src);
	Graphics.world.lights[Graphics.selectedobject].pos.y += atoi(Graphics.WM.InputWindow("Height:").c_str());


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

		vector<int> keys = ((cFavoriteLightsWindow::cFavoriteTreeNode*)node)->keys;
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
		vector<cWindowTree::cTreeNode*> nodes;
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

		if(!((cFavoriteLightsWindow::cFavoriteTreeNode*)node)->iscat)
		{
			Graphics.WM.MessageBox("You can't add a light to another light, you can only add lights to categories");
			return false;
		}

		vector<int> keys = ((cFavoriteLightsWindow::cFavoriteTreeNode*)node)->keys;

		TiXmlNode* n = favoritelights.FirstChild();
		for(i = 0; i < keys[keys.size()-1]; i++)
			n = n->NextSibling();

		for(i = keys.size()-2; i > -1; i--)
		{
			n = n->FirstChild();
			for(int ii = 0; ii < keys[i]; ii++)
				n = n->NextSibling();
		}

		string name = Graphics.WM.InputWindow("Light name:");
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
		light.InsertEndChild(TiXmlElement("givesshadow"))->InsertEndChild(TiXmlText("1"));
		light.InsertEndChild(TiXmlElement("lightfalloff"))->InsertEndChild(TiXmlText("1"));
		light.InsertEndChild(TiXmlElement("height"))->InsertEndChild(TiXmlText("10"));

		n->InsertEndChild(light);


		
// rebuild tree with keys
		vector<cWindowTree::cTreeNode*> nodes;
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

		if(!((cFavoriteLightsWindow::cFavoriteTreeNode*)node)->iscat)
		{
			Graphics.WM.MessageBox("You can't add a category to a light, you can only add lights to categories");
			return false;
		}

		vector<int> keys = ((cFavoriteLightsWindow::cFavoriteTreeNode*)node)->keys;

		TiXmlNode* n = favoritelights.FirstChild();
		for(i = 0; i < keys[keys.size()-1]; i++)
			n = n->NextSibling();

		for(i = keys.size()-2; i > -1; i--)
		{
			n = n->FirstChild();
			for(int ii = 0; ii < keys[i]; ii++)
				n = n->NextSibling();
		}

		string catname = Graphics.WM.InputWindow("Category name:");
		if(catname == "")
			return false;

		TiXmlElement cat(catname.c_str());

		n->InsertEndChild(cat);


		
// rebuild tree with keys
		vector<cWindowTree::cTreeNode*> nodes;
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
	string file = Graphics.WM.InputWindow("File to output:", "data/rotextures.txt");
	if(file == "")
		return false;
	ofstream pFile(file.c_str(), ios_base::binary | ios_base::out);
	unsigned int i;
	for(i = 0; i < fs.locations.size(); i++)
	{
		for(map<string, cFile*, less<string> >::iterator it = fs.locations[i]->files.begin(); it != fs.locations[i]->files.end(); it++)
		{
			if(it->first.substr(rodir.length(),13) != "data\\texture\\")
				continue;
			string ext = it->first.substr(it->first.length()-4);
			if(ext == ".jpg" || ext == ".bmp" || ext == ".tga")
			{
				string filename = it->first.substr(rodir.length()+13);
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
	string file = Graphics.WM.InputWindow("File to output:", "data/romodels.txt");
	if(file == "")
		return false;
	ofstream pFile(file.c_str(), ios_base::binary | ios_base::out);
	unsigned int i;
	for(i = 0; i < fs.locations.size(); i++)
	{
		for(map<string, cFile*, less<string> >::iterator it = fs.locations[i]->files.begin(); it != fs.locations[i]->files.end(); it++)
		{
			if(it->first.substr(rodir.length(),11) != "data\\model\\")
				continue;
			string ext = it->first.substr(it->first.length()-4);
			if(ext == ".rsm")
			{
				string filename = it->first.substr(rodir.length());
				string shortname = filename.substr(11);
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
	string file = Graphics.WM.InputWindow("File to output:", "data/rosounds.txt");
	if(file == "")
		return false;
	ofstream pFile(file.c_str(), ios_base::binary | ios_base::out);
	unsigned int i;
	for(i = 0; i < fs.locations.size(); i++)
	{
		for(map<string, cFile*, less<string> >::iterator it = fs.locations[i]->files.begin(); it != fs.locations[i]->files.end(); it++)
		{
			if(it->first.substr(rodir.length(),9) != "data\\wav\\")
				continue;
			string ext = it->first.substr(it->first.length()-4);
			if(ext == ".wav")
			{
				string filename = it->first.substr(rodir.length());
				string shortname = filename.substr(9);
				filename = "RO/" + replace(shortname, "\\", "/").substr(0, shortname.length()-4) + "|" + filename.substr(9) + "\r\n";
				pFile.write(filename.c_str(), filename.length());
			}
		}
	}
	pFile.close();
	return true;
}

