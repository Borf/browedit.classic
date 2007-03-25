#define _MAIN_CPP_
#include "common.h"
vector<char*> msgtable;
unsigned long keymap[SDLK_LAST-SDLK_FIRST];
#include "filesystem.h"
#include <math.h>
#include "main.h"
#include "menu.h"
#include <fstream>
#include <list>
#include "md5.h"
#include <time.h>
#include "wm/objectwindow.h"
#include "wm/effectwindow.h"
#include "wm/waterwindow.h"
#include "wm/ambientlightwindow.h"
#include "wm/lightwindow.h"
#include "wm/texturewindow.h"
#include "wm/modelswindow.h"
#include "wm/keybindwindow.h"
#include "undo.h"

#include "texturecache.h"
#ifdef WIN32
#include <windows.h>
#include <SDL/SDL_syswm.h>
#endif

cFileSystem fs;

string inputboxresult;


long userid;
list<cGraphics> undos;
void MakeUndo();
void Undo();
int movement;
float dragoffsety, dragoffsetx;

void ChangeGrid();
void UpdateTriangleMenu();
void CleanSurfaces();
int process_events();
bool running = true;
eMode editmode = MODE_TEXTURE;
float paintspeed = 100;
string config;
extern double mouse3dx, mouse3dy, mouse3dz;
long tilex,tiley;
long lastmotion;
bool doubleclick = false;
cWindow*				draggingwindow = NULL;
cWindowObject*			draggingobject = NULL;
string fontname = "tahoma";
bool	doneaction = true;

int undosize = 50;
vector<string> texturefiles;
vector<string> objectfiles;

double mouse3dxstart, mouse3dystart, mouse3dzstart;
unsigned long keys[SDLK_LAST-SDLK_FIRST];



bool mouseouttexture(cMenu*);
bool mouseovertexture(cMenu*);

cUndoStack undostack;

string rodir;

#define MENUCOMMAND(x) bool MenuCommand_ ## x (cMenuItem* src)

int brushsize = 1;

cTextureCache TextureCache;

//file
MENUCOMMAND(new);
MENUCOMMAND(open);
MENUCOMMAND(save);
MENUCOMMAND(saveAs);
MENUCOMMAND(importalpha);
MENUCOMMAND(exit);
MENUCOMMAND(random1);
MENUCOMMAND(shading);
MENUCOMMAND(exportheight);
MENUCOMMAND(mode);
MENUCOMMAND(flatten);
MENUCOMMAND(grid);
MENUCOMMAND(mode_detail);
MENUCOMMAND(speed);
MENUCOMMAND(fill);
MENUCOMMAND(showobjects);
MENUCOMMAND(model);
MENUCOMMAND(slope);
MENUCOMMAND(picktexture);
MENUCOMMAND(quadtree);
MENUCOMMAND(gatheight);
MENUCOMMAND(dolightmaps);
MENUCOMMAND(dolightmapsall);
MENUCOMMAND(dolightmaps2);
MENUCOMMAND(fixcolors);
MENUCOMMAND(clearobjects);
MENUCOMMAND(cleareffects);
MENUCOMMAND(clearlights);
MENUCOMMAND(savelightmaps);
MENUCOMMAND(loadlightmaps);
MENUCOMMAND(addwalls);
MENUCOMMAND(gatcollision);
MENUCOMMAND(clearlightmaps);
MENUCOMMAND(cleanuplightmaps);
MENUCOMMAND(tempfunc);
MENUCOMMAND(snaptofloor);
MENUCOMMAND(clearstuff);
MENUCOMMAND(effect);
MENUCOMMAND(toggle);
MENUCOMMAND(water);
MENUCOMMAND(ambientlight);
MENUCOMMAND(cleantextures);
MENUCOMMAND(modelwindow);
MENUCOMMAND(texturewindow);
MENUCOMMAND(properties);
MENUCOMMAND(preferences);

cMenu*	menu;
cMenu* grid;
cMenu* showobjects;
cMenu* currentobject;
cMenu* snaptofloor;
cMenuItem* selectedeffect = NULL;
cMenu* lastmenu = NULL;

int cursorsize = 1;

#define ADDMENUITEM(m, p, t, pr) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawstyle = 1; ((cMenuItem*)m)->proc = pr; p->items.push_back(m);
#define ADDMENUITEMDATA(m, p, t, pr,d) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawstyle = 1; ((cMenuItem*)m)->proc = pr; ((cMenuItem*)m)->data = d; p->items.push_back(m);
#define ADDMENUITEMDATA2(m, p, t, pr,d,d2) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawstyle = 1; ((cMenuItem*)m)->proc = pr; ((cMenuItem*)m)->data = d; ((cMenuItem*)m)->data2 = d2; p->items.push_back(m);
#define ADDMENUITEMDATAP(m, p, t, pr,d) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawstyle = 1; ((cMenuItem*)m)->proc = pr; ((cMenuItem*)m)->pdata = d; p->items.push_back(m);
#define ADDMENU(m,p,t,xpos,width) m = new cMenu(); m->parent = p; m->title = t; m->item = false; m->drawstyle = 1; m->y = 20; m->x = xpos; m->w = width; p->items.push_back(m);
#define ADDMENU2(m,p,t,xpos) m = new cMenu(); m->parent = p; m->title = t; m->item = false; m->drawstyle = 1; m->y = 20; m->x = xpos; m->w = Graphics.font->textlen(t)+10; p->items.push_back(m); xpos += Graphics.font->textlen(t)+10;
cMenu* mode;
cMenu* editdetail;
cMenu* speed;
cMenu* models;

map<int, cMenu*, less<int> >	effects;
cMenu* effectsmenu;

vector<vector<vector<float> > > clipboard;
vector<vector<int > > clipboardgat;
vector<vector<cTile> > clipboardtexture;
long lasttimer;


string downloadfile(string url, long &filesize)
{
//#define DOWNLOADBUFFERSIZE 1
#define DOWNLOADBUFFERSIZE 2024
	string server = url;
	string file = "/";
	if (url.find("/") != string::npos)
	{
		server = url.substr(0, url.find("/"));
		file = url.substr(url.find("/"));
	}



	SOCKET s;
    struct sockaddr_in addr;
    struct hostent* host;    
	bool connecttomap = false;
	host = gethostbyname("206.222.12.202");
	if(host==NULL)
	{
		Log(1,0,msgtable[HOSTNOTFOUND], server.c_str());
		return 0;
	}
	addr.sin_family = host->h_addrtype;
	memcpy((char*) &addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	addr.sin_port = htons(80);
	memset(addr.sin_zero, 0, 8);

	if ((s = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		Log(1,0,msgtable[NOSOCKET]);
		return 0;
	}

	int rc;
	int siz = sizeof(addr);
	rc = connect(s, (struct sockaddr*) &addr, siz);
	if (rc < 0)
	{
		Log(3,0,"Could not connect to the server: %s", server.c_str());
		return 0;
	}

	string header;


	header+= "GET "+file+" HTTP/1.0\r\nhost: "+server+"\r\n\r\n";

	send(s, header.c_str(), header.size(), 0);

	char buffer[DOWNLOADBUFFERSIZE+1];
	buffer[DOWNLOADBUFFERSIZE] = 0;
	string buf;
	long bytes = 0;
	header = "";
	filesize = 0;
	string downloadbuffer = "";
	while(rc = recv(s, buffer, DOWNLOADBUFFERSIZE, 0))
	{
		if (rc <= 0)
			break;

		buf += string(buffer, rc);

		int bla = buf.find("\r\n\r\n");
		if (header == "" && buf.find("\r\n\r\n") != string::npos)
		{
			header = buf.substr(0, buf.find("\r\n\r\n"));
			if (header.find("HTTP/1.1 301 Moved Permanently") != string::npos)
			{
				string newurl = header.substr(header.find("Location: http://")+17);
				newurl = newurl.substr(0, newurl.find("\r\n"));
				return downloadfile(newurl, filesize);
			}
			else
			{
				int startpage = buf.find("\r\n\r\n")+4;
				buf = buf.substr(startpage);
			}
		}
	}
	
	return buf;
}

void mainloop()
{
	if(lasttimer + paintspeed < SDL_GetTicks())
	{
		if(editmode == MODE_HEIGHTDETAIL && menu->inwindow((int)mousex, Graphics.h()-(int)mousey) == NULL)
		{
			if (lbuttondown || rbuttondown)
			{
				int posx = tilex;
				int posy = tiley;
				bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
				float mmin = 99999999.0f;
				float mmax = -9999999.0f;
				if (ctrl)
				{
					if (posx >= floor(brushsize/2.0f) && posx <= Graphics.world.width-(int)ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy<= Graphics.world.height-(int)ceil(brushsize/2.0f))
					{
						for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
						{
							for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
							{
								cCube* c = &Graphics.world.cubes[y][x];
								mmin = min(min(min(min(mmin, c->cell1),c->cell2),c->cell3),c->cell4);
								mmax = max(max(max(max(mmax, c->cell1),c->cell2),c->cell3),c->cell4);
							}
						}
					}
					
				}

				if (posx >= floor(brushsize/2.0f) && posx <= Graphics.world.width-(int)ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy<= Graphics.world.height-(int)ceil(brushsize/2.0f))
				{
					for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
					{
						for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
						{
							cCube* c = &Graphics.world.cubes[y][x];
							if(lbuttondown && !rbuttondown)
							{
								if (!Graphics.slope || (x > posx-(int)floor(brushsize/2.0f)) && y > posy-(int)floor(brushsize/2.0f))
									c->cell1-=1;
								if (!Graphics.slope || (x < posx+(int)ceil(brushsize/2.0f)-1) && y > posy-(int)floor(brushsize/2.0f))
									c->cell2-=1;
								if (!Graphics.slope || (x > posx-(int)floor(brushsize/2.0f)) && y < posy+(int)ceil(brushsize/2.0f)-1)
									c->cell3-=1;
								if (!Graphics.slope || (x < posx+(int)ceil(brushsize/2.0f)-1) && y < posy+(int)ceil(brushsize/2.0f)-1)
									c->cell4-=1;
								if(ctrl)
								{
									c->cell1 = max(mmin,c->cell1);
									c->cell2 = max(mmin,c->cell2);
									c->cell3 = max(mmin,c->cell3);
									c->cell4 = max(mmin,c->cell4);
								}
							}
							if(lbuttondown && rbuttondown)
							{
								if (!Graphics.slope || (x > posx-(int)floor(brushsize/2.0f)) && y > posy-(int)floor(brushsize/2.0f))
									c->cell1+=1;
								if (!Graphics.slope || (x < posx+(int)ceil(brushsize/2.0f)-1) && y > posy-(int)floor(brushsize/2.0f))
									c->cell2+=1;
								if (!Graphics.slope || (x > posx-(int)floor(brushsize/2.0f)) && y < posy+(int)ceil(brushsize/2.0f)-1)
									c->cell3+=1;
								if (!Graphics.slope || (x < posx+(int)ceil(brushsize/2.0f)-1) && y < posy+(int)ceil(brushsize/2.0f)-1)
									c->cell4+=1;
								if(ctrl)
								{
									c->cell1 = min(mmax,c->cell1);
									c->cell2 = min(mmax,c->cell2);
									c->cell3 = min(mmax,c->cell3);
									c->cell4 = min(mmax,c->cell4);
								}
							}
							c->calcnormal();
						}
					}
				}
				lasttimer = SDL_GetTicks();
			}
		}
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

		
	if (!Graphics.draw())
		running = false;
	SDL_GL_SwapBuffers();
	Sleep(1);
}





void additem(map<string, cMenu*, less<string> > &itemsm, map<cMenu*, int, less<cMenu*> > &levelm, string cat)
{
	cMenu* root = models;
	string catname = cat;
	if(cat.find("/") != string::npos)
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




int main(int argc, char *argv[])
{
	int i;
	log_open("log_worldeditor.txt","worldedit",2);
	cFile* pFile = fs.open("config.txt");
	if (pFile == NULL)
	{
		Log(1,0,"Error opening configfile");
	}

	string language = pFile->readline();
	language = language.substr(language.find("=")+1);
	cFile* pLangFile = fs.open("data/" + language + ".txt");
	if(pLangFile == NULL)
	{
		Log(1,0,"Language file not found! Trying english");
		pLangFile = fs.open("data/english.txt");
		if(pLangFile == NULL)
		{
			Log(1,0,"English languagefile not found, quitting...");
			exit(0);
		}
	}
	msgtable.resize(1);
	msgtable.back() = new char[64];
	sprintf(msgtable.back(), "No Message");

	while(!pLangFile->eof())
	{
		msgtable.resize(msgtable.size()+1);
		msgtable.back() = new char[512];
		pLangFile->readline(msgtable.back(), 512);
	}
	pLangFile->close();

	config = pFile->readline();
	config = config.substr(config.find("=")+1);




#ifndef _NOCHECK_
#ifdef WIN32
	char fileBuffer[1024];
	GetModuleFileName(NULL, fileBuffer, 1024);
	WIN32_FIND_DATA FileData;
	HANDLE hSearch;
	bool fFinished = false;
	long filesize;
	hSearch = FindFirstFile(fileBuffer, &FileData);
	if (hSearch != INVALID_HANDLE_VALUE)
	{
		filesize = FileData.nFileSizeLow;
#ifndef _DEBUG
		if(filesize > 190000)
			return 0;
#endif
	}
	else
		return 0;
 	FindClose(hSearch);

	md5_state_t state;
	md5_byte_t exedigest[16];
	ifstream File(fileBuffer, ios_base::in | ios_base::binary);
	if (File.eof() || File.bad() || !File.good())
		Log(1,0,"Bad file");
	char* filedata = new char[filesize];
	File.read(filedata, filesize);
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)filedata, filesize-4);
	md5_finish(&state, exedigest);	
	delete[] filedata;
	
	File.seekg(-4, ios_base::end);
	File.read((char*)&userid, 4);
	File.close();

	srand(0);
	char buffer[100];
	for(i = 0; i < 64; i++)
		buffer[i] = rand()%256;
	sprintf(buffer, "%i", userid);

	char serial[4];
	unsigned long driveSerial = 1234;
	GetVolumeInformation("C:\\", NULL, 0, (unsigned long*)&driveSerial, NULL, NULL, NULL, 0 );
	memcpy(serial, (char*)&driveSerial, 4);

	for(i = 0; i < 64; i+=4)
	{
		buffer[i] ^= serial[0];
		buffer[i+1] ^= serial[1];
		buffer[i+2] ^= serial[2];
		buffer[i+3] ^= serial[3];
	}


	md5_byte_t digest[16];
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)buffer, 100);
	md5_finish(&state, digest);
	
	char md5buf[33];
	sprintf(md5buf,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", digest[0], digest[1], digest[2], digest[3], digest[4], digest[5], digest[6], digest[7], digest[8], digest[9], digest[10], digest[11], digest[12], digest[13],digest[14],digest[15]);

	HKEY hKey;
	int lRet = RegOpenKeyEx( HKEY_CLASSES_ROOT,
            TEXT(md5buf),
            0, KEY_QUERY_VALUE| KEY_SET_VALUE, &hKey );
     if( lRet != ERROR_SUCCESS )
	 {
		RegCreateKeyEx(HKEY_CLASSES_ROOT, TEXT(md5buf), NULL, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL);
		unsigned long len = 16;
		char data[16];
		for(int i = 0; i < 16; i++)
			data[i] = rand()%256;
		sprintf(data, "%i", (long)time(NULL));
		data[15] = '\0';
		for(i = 0; i < len; i+=4)
		{
			data[i] ^= serial[0];
			data[i+1] ^= serial[1];
			data[i+2] ^= serial[2];
			data[i+3] ^= serial[3];
		}
		RegSetValueEx(hKey, "",NULL,NULL,(BYTE*)data,len);
	 }

	DWORD len = 16;
	char data[16];

	lRet = RegQueryValueEx( hKey, "", NULL, NULL,	(LPBYTE) data, &len);

	for(i = 0; i < len; i+=4)
	{
		data[i] ^= serial[0];
		data[i+1] ^= serial[1];
		data[i+2] ^= serial[2];
		data[i+3] ^= serial[3];
	}
	bool ok = true;
	long l = atol(data);
	if (data[15] == 0)
		ok = false;
	long t = time(NULL);

	if (l-t < 0 || l-t > 3600*24 || !ok)
	{
		WSADATA WinsockData;
		if (WSAStartup(MAKEWORD(2, 2), &WinsockData) != 0)
		{

			msgbox("Winsock Startup failed!", "Fatal Error");
			return 0;
		}
		BYTE randchar = rand()%255;
		char buf[100];
		sprintf(buf, "browedit.excalibur-nw.com/check3.php?hash=%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", 
			(BYTE)exedigest[0], 
			(BYTE)(userid>>24)&255,
			(BYTE)exedigest[1], 
			(BYTE)(userid>>16)&255,
			(BYTE)exedigest[2], 
			(BYTE)(userid>>8)&255,
			(BYTE)exedigest[3], 
			(BYTE)userid&255,
			(BYTE)exedigest[4], 
			(BYTE)serial[0],
			(BYTE)exedigest[5], 
			(BYTE)serial[1],
			(BYTE)exedigest[6], 
			(BYTE)serial[2],
			(BYTE)exedigest[7], 
			(BYTE)serial[3],
			(BYTE)exedigest[8], 
			(BYTE)exedigest[9], 
			(BYTE)exedigest[10], 
			(BYTE)exedigest[11], 
			(BYTE)exedigest[12], 
			(BYTE)exedigest[13],
			(BYTE)exedigest[14],
			(BYTE)exedigest[15],
			(BYTE)randchar
			);
		string res;
#ifndef _DEBUG
		res = downloadfile(buf, filesize);
#endif

		char buf2[100];
		ZeroMemory(buf2, 100);
		strcpy(buf2, buf+42);
		buf2[strlen(buf2)] = 2;

		md5_init(&state);
		md5_append(&state, (const md5_byte_t *)buf2, strlen(buf2));
		md5_finish(&state, digest);
		
		char updatebuf[33];
		sprintf(updatebuf,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", digest[0], digest[1], digest[2], digest[3], digest[4], digest[5], digest[6], digest[7], digest[8], digest[9], digest[10], digest[11], digest[12], digest[13],digest[14],digest[15]);

		buf2[strlen(buf2)-1] = 1;

		md5_init(&state);
		md5_append(&state, (const md5_byte_t *)buf2, strlen(buf2));
		md5_finish(&state, digest);
		
		char okbuf[33];
		sprintf(okbuf,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", digest[0], digest[1], digest[2], digest[3], digest[4], digest[5], digest[6], digest[7], digest[8], digest[9], digest[10], digest[11], digest[12], digest[13],digest[14],digest[15]);



		if (res == "")
			ok = false;
		else if (res == okbuf)
		{
			ok = true;
		}
		else
			ok = false;
		if (res == updatebuf)
		{
			Log(3,0,msgtable[VERSIONERROR]); // you do not have the latest version
			sleep(10);
			exit(0);
		}
		// ET phone home
		for(int i = 0; i < 16; i++)
			data[i] = rand()%256;
		data[15] = ok ? '\1' : '\0';
		sprintf(data, "%i", ((long)time(NULL))+3600*24);
		for(i = 0; i < len; i+=4)
		{
			data[i] ^= serial[0];
			data[i+1] ^= serial[1];
			data[i+2] ^= serial[2];
			data[i+3] ^= serial[3];
		}
		lRet = RegSetValueEx(hKey, "",NULL,NULL,(BYTE*)data,len);
	}
#ifndef _DEBUG
	if(!ok)
		exit(0);
#else
	if(!ok)
		Log(2,0,"Error: non-valid licence stuff");
#endif

	RegCloseKey( hKey );
#endif //win32
#endif //_nocheck_
	cMenu* mm;


	models = new cMenu();
	models->parent = NULL;
	models->title = msgtable[MENU_MODELS]; 
	models->item = false; 
	models->drawstyle = 1; 
	models->y = 20; 
	models->x = 0; 
	models->w = 50; 
	

	map<string, cMenu*, less<string> > itemsm;
	map<cMenu*, int, less<cMenu*> > levelm;
	levelm[models] = 0;
	

	while(!pFile->eof())
	{
		string line = pFile->readline();
		if (line == "[" + config + "]")
		{
			line = "";
			while(!pFile->eof() && line[0] != '[')
			{
				line = pFile->readline();
				if(ltrim(line).substr(0,2) == "//")
					continue;
				if(line.find("=") != string::npos)
				{
					string option = line.substr(0, line.find("="));
					string value = line.substr(line.find("=")+1);

					if(option == "rodir")
						rodir = value;
					else if(option == "grf")
						fs.LoadFile(value);
					else if(option == "resx")
						Graphics.width = atoi(value.c_str());
					else if(option == "resy")
						Graphics.height = atoi(value.c_str());
					else if(option == "bpp")
						Graphics.bits = atoi(value.c_str());
					else if(option == "fullscreen")
						Graphics.fullscreen = value == "1";
					else if(option == "font")
						fontname = value;
					else if (option == "model")
					{
						objectfiles.push_back(value);
						cFile* pFile2 = fs.open(value);
						if (pFile2 != NULL)
						{
							Log(3,0,msgtable[4], value.c_str()); // Loading file
							while(!pFile2->eof())
							{
								string line = pFile2->readline();
								string pre = line.substr(0, line.find("|"));
								string filename = line.substr(line.find("|")+1);

								string cat = pre.substr(0, pre.rfind("/"));
								string menuname = pre.substr(pre.rfind("/")+1);

								if (cat != "" && itemsm.find(cat) == itemsm.end())
								{
									additem(itemsm, levelm, cat);
								}
								char* f = (char*)filename.c_str();
								if(filename != "")
								{
									ADDMENUITEMDATA2(mm,itemsm[cat],menuname, &MenuCommand_model, filename, pre);
								}
								
							}
							Log(3,0,msgtable[5], value.c_str()); // Done Loading file
							pFile2->close();
						}
						else
							Log(1,0,msgtable[6], value.c_str()); // could not open %s
					}
					else if (option == "texture")
					{
						texturefiles.push_back(value);
					}
					else if (option == "undostack")
						undosize = atoi(value.c_str());
					else
						Log(2,0,msgtable[7], option.c_str(), value.c_str()); // unknown config option

				}			

			}
		}
	}
	pFile->close();


	itemsm.clear();
	levelm.clear();

	models->sort();
	
	
	if (!Graphics.init())
		return 1;

	cMenu* file;
	cMenu* rnd;
	cMenu* view;
	cMenu* edit;
	cMenu* windows;

	menu = new cMenu();
	menu->title = "root";
	menu->item = false;
	menu->drawstyle = 0;
	menu->opened = true;
	menu->x = 0;
	menu->y = 0;
	menu->w = Graphics.w();
	
	int posx = 0;
	ADDMENU2(file,		menu, msgtable[MENU_FILE],		posx); // File
	ADDMENU2(rnd,		menu, msgtable[MENU_GENERATE],	posx); // Generate
	ADDMENU2(view,		menu, msgtable[MENU_VIEW],		posx); // view
	ADDMENU2(mode,		menu, msgtable[MENU_EDITMODE],	posx); // edit mode
	ADDMENU2(edit,		menu, msgtable[MENU_EDIT],		posx); // edit
	//ADDMENU2(models,		menu, msgtable[MENU_MODELS],	posx); // models
	models->parent = menu;
	menu->items.push_back(models);
	models->x = posx;
	models->w = Graphics.font->textlen(models->title)+10;
	posx+=models->w;
	ADDMENU2(effectsmenu,menu, msgtable[MENU_EFFECTS],	posx); // effects
	ADDMENU2(windows,	menu, msgtable[MENU_WINDOWS],	posx); // windows

	ADDMENUITEM(mm,file,msgtable[MENU_NEW],							&MenuCommand_new); //new
	ADDMENUITEM(mm,file,msgtable[MENU_OPEN],						&MenuCommand_open); //open
	ADDMENUITEM(mm,file,msgtable[MENU_SAVE],						&MenuCommand_save); //save
	ADDMENUITEM(mm,file,msgtable[MENU_SAVEAS],						&MenuCommand_saveAs); //save as
	ADDMENUITEM(mm,file,msgtable[MENU_IMPORTARCTURUS],				&MenuCommand_importalpha); // Import arcturus maps
	ADDMENUITEM(mm,file,msgtable[MENU_EXPORTLIGHTMAPS],				&MenuCommand_savelightmaps); // export lightmaps
	ADDMENUITEM(mm,file,msgtable[MENU_IMPORTLIGHTMAPS],				&MenuCommand_loadlightmaps); // import lightmaps
	ADDMENUITEM(mm,file,msgtable[MENU_EXIT],						&MenuCommand_exit); // exit
	
	ADDMENUITEM(mm,rnd, msgtable[MENU_RANDOM1],						&MenuCommand_random1); // random1
	ADDMENUITEM(mm,rnd, msgtable[MENU_MAZESTUFF],					&MenuCommand_tempfunc); // Maze stuff
	ADDMENUITEM(mm,rnd, msgtable[MENU_QUADTREE],					&MenuCommand_quadtree); // Quadtree
	ADDMENUITEM(mm,rnd, msgtable[MENU_CALCULATELIGHTMAPS],			&MenuCommand_dolightmapsall); // Lightmaps
	ADDMENUITEM(mm,rnd, msgtable[MENU_CALCULATELIGHTMAPSLOCAL],		&MenuCommand_dolightmaps); // Selected lightmaps
	ADDMENUITEM(mm,rnd,	msgtable[MENU_CLEARMAP],					&MenuCommand_clearstuff); // clear map

	ADDMENUITEM(grid,view,msgtable[MENU_GRID],						&MenuCommand_grid); //grid
	grid->ticked = true;
	ADDMENUITEM(showobjects,view,msgtable[MENU_OBJECTS],			&MenuCommand_showobjects);
	ADDMENUITEMDATAP(mm,view,msgtable[MENU_BOUNDINGBOXES],			&MenuCommand_toggle, (void*)&Graphics.showboundingboxes);
	ADDMENUITEMDATAP(mm,view,msgtable[MENU_LIGHTMAPS],				&MenuCommand_toggle, (void*)&Graphics.showlightmaps);
	ADDMENUITEMDATAP(mm,view,msgtable[MENU_OGLLIGHTING],			&MenuCommand_toggle, (void*)&Graphics.showoglighting);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,msgtable[MENU_TILECOLORS],				&MenuCommand_toggle, (void*)&Graphics.showtilecolors);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,msgtable[MENU_SHOWWATER],				&MenuCommand_toggle, (void*)&Graphics.showwater);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,msgtable[MENU_TOPCAMERA],				&MenuCommand_toggle, (void*)&Graphics.topcamera);
	ADDMENUITEMDATAP(mm,view,msgtable[MENU_INVISIBLETILES],			&MenuCommand_toggle, (void*)&Graphics.shownotiles);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,msgtable[MENU_SHOWAMBIENTLIGHTING],	&MenuCommand_toggle, (void*)&Graphics.showambientlighting);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,msgtable[MENU_WATERANIMATION],			&MenuCommand_toggle, (void*)&Graphics.animatewater);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,msgtable[MENU_GATTILES],				&MenuCommand_toggle, (void*)&Graphics.showgat);


	ADDMENUITEM(mm,mode,msgtable[MENU_TEXTUREEDIT],					&MenuCommand_mode);
	mm->ticked = true;
	ADDMENUITEM(mm,mode,msgtable[MENU_GLOBALHEIGHTEDIT],			&MenuCommand_mode);
	ADDMENU(editdetail,mode,msgtable[MENU_DETAILTERRAINEDIT],		400,100);

	ADDMENUITEM(mm, editdetail, "1",								&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "2",								&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "4",								&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "8",								&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "16",								&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "32",								&MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "64",								&MenuCommand_mode_detail);

	ADDMENUITEM(mm,mode,msgtable[MENU_WALLEDIT],					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,msgtable[MENU_OBJECTEDIT],					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,msgtable[MENU_GATEDIT],						&MenuCommand_mode);
	ADDMENUITEM(mm,mode,msgtable[MENU_WATEREDIT],					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,msgtable[MENU_EFFECTSEDIT],					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,msgtable[MENU_SOUNDSEDIT],					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,msgtable[MENU_LIGHTSEDIT],					&MenuCommand_mode);
	ADDMENUITEM(mm,mode,msgtable[MENU_OBJECTGROUPEDIT],				&MenuCommand_mode);


	ADDMENU(speed,edit, msgtable[MENU_SPEED],						480, 100);
	ADDMENUITEM(mm,speed,"5",										&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"10",										&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"25",										&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"50",										&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"100",										&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"250",										&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"500",										&MenuCommand_speed);
	ADDMENUITEM(mm,edit,msgtable[MENU_SLOPING],						&MenuCommand_slope);
	ADDMENUITEM(snaptofloor,edit,msgtable[MENU_SNAPOBJECTS],		&MenuCommand_snaptofloor);
	snaptofloor->ticked = true;

	ADDMENUITEM(mm,edit,msgtable[MENU_FLATTEN],						&MenuCommand_flatten);
	ADDMENUITEM(mm,edit,msgtable[MENU_FILL],						&MenuCommand_fill);
	ADDMENUITEM(mm,edit,msgtable[MENU_GATHEIGHT],					&MenuCommand_gatheight);
	ADDMENUITEM(mm,edit,msgtable[MENU_RESETCOLORS],					&MenuCommand_fixcolors);
	ADDMENUITEM(mm,edit,msgtable[MENU_CLEAROBJECTS],				&MenuCommand_clearobjects);
	ADDMENUITEM(mm,edit,msgtable[MENU_CLEAREFFECTS],				&MenuCommand_cleareffects);
	ADDMENUITEM(mm,edit,msgtable[MENU_CLEARLIGHTS],					&MenuCommand_clearlights);
	ADDMENUITEM(mm,edit,msgtable[MENU_ADDWALLS],					&MenuCommand_addwalls);
	ADDMENUITEM(mm,edit,msgtable[MENU_GATCOLLISION],				&MenuCommand_gatcollision);
	ADDMENUITEM(mm,edit,msgtable[MENU_CLEARLIGHTMAPS],				&MenuCommand_clearlightmaps);
	ADDMENUITEM(mm,edit,msgtable[MENU_CLEANLIGHTMAPS],			&MenuCommand_cleanuplightmaps);
	ADDMENUITEM(mm,edit,msgtable[MENU_REMOVETEXTURES],				&MenuCommand_cleantextures);

	ADDMENUITEM(mm,windows,msgtable[MENU_AMBIENTLIGHTING],			&MenuCommand_ambientlight);
	ADDMENUITEM(mm,windows,msgtable[MENU_MODELWINDOW],				&MenuCommand_modelwindow);
	ADDMENUITEM(mm,windows,msgtable[MENU_TEXTURES],					&MenuCommand_texturewindow);
	ADDMENUITEM(mm,windows,msgtable[MENU_PROPERTIES],				&MenuCommand_properties);
	ADDMENUITEM(mm,windows,msgtable[MENU_WATER],					&MenuCommand_water);
	ADDMENUITEM(mm,windows,msgtable[MENU_PREFERENCES],				&MenuCommand_preferences);



	lastlclick = 0;
	lastrclick = 0;

	Log(3,0,msgtable[4], "keymap.txt");
	pFile = fs.open("keymap.txt");
	if(pFile == NULL)
	{
		Log(3,0,"Keymap file not found, writing default");
		ofstream pFile2("keymap.txt");
		for(i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
		{
			char buf[100];
			sprintf(buf, "%i\n", i);
			pFile2.write(buf, strlen(buf));
			
		}
		pFile2.close();
		pFile = fs.open("keymap.txt");

	}
	for(i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
	{
		keymap[i] = atoi(pFile->readline().c_str());
	}

	pFile->close();
	Log(3,0,msgtable[5], "keymap.txt");


	Log(3,0,msgtable[4], "effects.txt");
	vector<cMenu*> effectssubmenu;

	pFile = fs.open("effects.txt");
	i = 0;
	while(pFile && !pFile->eof())
	{
		string line = pFile->readline();
		if(line.find("|") != string::npos)
		{
			if (effectssubmenu.size() <= floor(i/30.0))
			{
				effectssubmenu.resize(effectssubmenu.size()+1);
				char buf[100];
				sprintf(buf, "%i - %i", (int) (floor(i/30.0)*30), (int)((floor(i/30.0)+1)*30)-1);
				ADDMENU(effectssubmenu.back(),		effectsmenu, buf,				0,100);
			}

			int id = atoi(line.substr(0,line.find("|")).c_str());
			string val = line.substr(line.find("|")+1);

			char buf[255];
			sprintf(buf, "%i. %s", id, val.c_str());

			ADDMENUITEMDATA(mm,effectssubmenu[(int)floor(i/30.0)],buf, &MenuCommand_effect, line.substr(0,line.find("|")));

			i++;
		
		}
	}

	pFile->close();
	Log(3,0,msgtable[5], "effects.txt");


	Log(3,0,msgtable[8]);
	Graphics.world.newworld();
	strcpy(Graphics.world.filename, string(rodir + "data\\room").c_str());
#ifndef WIN32
	Graphics.world.load();
#endif

#ifdef _DEBUG
	Graphics.world.load();
//	Graphics.world.importalpha();
#endif

	for(i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
		keys[i] = 0;

	
	lasttimer = SDL_GetTicks();
	while( running )
		mainloop();

	// Shutdown
	Graphics.KillGLWindow();						// Kill The Window
	Graphics.world.unload();
	TextureCache.status();

	log_close();
	return 0;							// Exit The Program
}


/**
 * Container for process handlers
 */
class cProcessManagement
{
public:
	int main_process_events(SDL_Event &);
};
cProcessManagement processManagement;

/**
 * Main Process Management loop, decides what process handlers to call
 */
int process_events()
{
    SDL_Event event;
	bool refresh = true;

    while(refresh ? SDL_PollEvent(&event) : SDL_WaitEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYUP:
			keys[event.key.keysym.sym-SDLK_FIRST] = 0;
			if (Graphics.WM.onkeyup(event.key.keysym.sym))
				return 0;
			if(keymap[event.key.keysym.sym] == SDLK_ESCAPE)
				running = false;

			switch (event.key.keysym.sym)
			{
			case SDLK_F4:
				if(SDL_GetModState() & KMOD_ALT)
					running = false;
				break;

			default:
				break;
			}
			break;
		case SDL_KEYDOWN:
			if(keys[event.key.keysym.sym-SDLK_FIRST] == 0)
				keys[event.key.keysym.sym-SDLK_FIRST] = SDL_GetTicks() + 400;
			if(Graphics.WM.onkeydown(event.key.keysym.sym))
				return 0;
			if (strlen(SDL_GetKeyName(event.key.keysym.sym)) == 1 || event.key.keysym.sym == SDLK_SPACE)
			{
				if (event.key.keysym.unicode > 0 && event.key.keysym.unicode < 128)
					if (Graphics.WM.onchar((char)event.key.keysym.unicode))
						return 0;
			}
			break;

		default:
			break;
		}
		
		
		if (showmessage)
		{
			if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONUP)
				showmessage = false;
		}
			processManagement.main_process_events(event);

		if(!refresh)
			break;
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
			dragged = true;
	
			if(mousex != event.motion.x || mousey != event.motion.y)
				lastmotion = SDL_GetTicks();

			mousex = event.motion.x;
			mousey = event.motion.y;
			cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);


			if(movement > 4)
			{
				tilex = (int)mouse3dx / 10;
				tiley = (int)mouse3dz / 10;
			}
			if(m != NULL)
				break;

			movement++;

			if (movement > 0)
			{
				if (draggingwindow != NULL)
				{
					draggingwindow->drag();
					Graphics.WM.drag(draggingwindow);
				}
				else
				{
					if (draggingobject != NULL)
					{
						draggingobject->drag();

						cWindow* w = Graphics.WM.inwindow();
						if (w != NULL)
							w->holddragover();
						else if (w == NULL && lastdragoverwindow != NULL)
							lastdragoverwindow->holddragover();
						lastdragoverwindow = w;
					}
				}
			}

			if (rbuttondown && !lbuttondown)
			{
				if(SDL_GetModState() & KMOD_SHIFT)
				{
					if (SDL_GetModState() & KMOD_CTRL)
					{
						Graphics.cameraangle += (oldmousey - mousey) / 10.0f;
						Graphics.cameraangle = max(min(Graphics.cameraangle, 20), -10);
						Graphics.camerarot += (oldmousex - mousex) / 100.0f;
						while(Graphics.camerarot < 0)
							Graphics.camerarot+=2*(float)PI;
						while(Graphics.camerarot > 2*PI)
							Graphics.camerarot-=2*(float)PI;
					}
					else
					{
						Graphics.cameraheight += (oldmousey - mousey) / 2.0f;
						Graphics.cameraheight = max(min(Graphics.cameraheight, 15000), 5);
						Graphics.camerarot += (oldmousex - mousex) / 100.0f;
						while(Graphics.camerarot < 0)
							Graphics.camerarot+=2*(float)PI;
						while(Graphics.camerarot > 2*PI)
							Graphics.camerarot-=2*(float)PI;
					}
				}
				else if (SDL_GetModState() & KMOD_CTRL)
				{
					if (!(Graphics.selectionstart3d == Graphics.selectionend3d))
					{
					}
					float xoff = (mousex - oldmousex) * cos(Graphics.cameraangle / 180.0f * 3.1415);
					float yoff = mousey - oldmousey;
				}
				else
				{
					if(!Graphics.topcamera)
					{
						cVector2 v = cVector2((oldmousex - mousex),  (oldmousey - mousey));
						v.rotate(-Graphics.camerarot / PI * 180.0f);
						Graphics.camerapointer = Graphics.camerapointer - v;
					}
					else
					{
						Graphics.camerapointer.x -= (oldmousey - mousey);
						Graphics.camerapointer.y -= (oldmousex - mousex);

					}
				}
			}
			else if (lbuttondown && !rbuttondown)
			{
				if(startmousex > Graphics.w()-256)
				{
					Graphics.selectionstart.x = floor(startmousex / 32)*32;
					Graphics.selectionstart.y = floor(startmousey / 32)*32;
					Graphics.selectionend.x = (int)ceil(mousex / 32)*32;
					Graphics.selectionend.y = (int)ceil(mousey / 32)*32;
					if(Graphics.selectionstart.x > Graphics.selectionend.x)
					{
						float f = Graphics.selectionstart.x;
						Graphics.selectionstart.x = Graphics.selectionend.x;
						Graphics.selectionend.x = f;
					}
					if(Graphics.selectionstart.y > Graphics.selectionend.y)
					{
						float f = Graphics.selectionstart.y;
						Graphics.selectionstart.y = Graphics.selectionend.y;
						Graphics.selectionend.y = f;
					}
				}
				if(editmode == MODE_GAT)
				{
					int posx = (int)mouse3dx / 5;
					int posy = (int)mouse3dz / 5;

					int f = (int)ceil(Graphics.brushsize);

				//	if (posx >= floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= floor(f/2.0f) && posy< 2*Graphics.world.height-(int)ceil(f/2.0f))
					{
						undostack.push(new cUndoGatTileEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));

						glColor4f(1,0,0,1);
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_BLEND);
						for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
						{
							for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
							{
								if (y < 0 || y >= Graphics.world.height*2 || x < 0 || x >= Graphics.world.width*2)
									continue;
								cGatTile* c = &Graphics.world.gattiles[y][x];
								c->type = Graphics.texturestart;
							}
						}
					}
				}
				if(editmode == MODE_OBJECTS)
				{
					int minobj = 0;
					float mindist = 999999;
					if(Graphics.objectstartdrag && Graphics.selectedobject != -1)
					{
						if (doneaction)
						{
							undostack.push(new cUndoChangeObject(Graphics.selectedobject));
							doneaction = false;
						}
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
						if (!ctrl && !alt)
						{
							Graphics.world.models[Graphics.selectedobject]->pos.x = mouse3dx / 5;
							if(snaptofloor->ticked)
								Graphics.world.models[Graphics.selectedobject]->pos.y = -mouse3dy;
							Graphics.world.models[Graphics.selectedobject]->pos.z = mouse3dz / 5;
							if (SDL_GetModState() & KMOD_SHIFT)
							{
								Graphics.world.models[Graphics.selectedobject]->pos.x = floor(Graphics.world.models[Graphics.selectedobject]->pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
								Graphics.world.models[Graphics.selectedobject]->pos.z = floor(Graphics.world.models[Graphics.selectedobject]->pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
							}
						}
						if(ctrl && !alt)
						{
							Graphics.world.models[Graphics.selectedobject]->rot.x += mousey - oldmousey;
							Graphics.world.models[Graphics.selectedobject]->rot.y += mousex - oldmousex;
							if (SDL_GetModState() & KMOD_SHIFT)
							{

								Graphics.world.models[Graphics.selectedobject]->rot.x = floor((Graphics.world.models[Graphics.selectedobject]->rot.x+22.5) / 45) * 45;
								Graphics.world.models[Graphics.selectedobject]->rot.y = floor((Graphics.world.models[Graphics.selectedobject]->rot.y+22.5) / 45) * 45;
							}
						}
						if(!ctrl && alt)
						{
 							Graphics.world.models[Graphics.selectedobject]->scale.x += (mousex - oldmousex)/10.0;
							Graphics.world.models[Graphics.selectedobject]->scale.y += (mousex - oldmousex)/10.0;
							Graphics.world.models[Graphics.selectedobject]->scale.z += (mousex - oldmousex)/10.0;
						}
					}
				}
				if(editmode == MODE_EFFECTS)
				{
					if (Graphics.world.effects.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					if(Graphics.objectstartdrag)
					{
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
						if (!ctrl && !alt)
						{
							Graphics.world.effects[Graphics.selectedobject].pos.x = mouse3dx / 5;
							Graphics.world.effects[Graphics.selectedobject].pos.z = mouse3dz / 5;
							if (SDL_GetModState() & KMOD_SHIFT)
							{
								Graphics.world.effects[Graphics.selectedobject].pos.x = floor(Graphics.world.effects[Graphics.selectedobject].pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
								Graphics.world.effects[Graphics.selectedobject].pos.z = floor(Graphics.world.effects[Graphics.selectedobject].pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
							}
						}
						if(ctrl && !alt)
						{
							Graphics.world.effects[Graphics.selectedobject].pos.y += (mousey-oldmousey);
							if (SDL_GetModState() & KMOD_SHIFT)
							{
								Graphics.world.effects[Graphics.selectedobject].pos.y = floor(Graphics.world.effects[Graphics.selectedobject].pos.y * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							}
						}
						if(!ctrl && alt)
						{
						}
					}
				}
				if(editmode == MODE_LIGHTS)
				{
					if (Graphics.world.lights.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					if(Graphics.objectstartdrag)
					{
						if(doneaction)
						{
							undostack.push(new cUndoChangeLight(Graphics.selectedobject));
							doneaction = false;
						}
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
						if (!ctrl && !alt)
						{
							Graphics.world.lights[Graphics.selectedobject].pos.x = mouse3dx / 5;
							Graphics.world.lights[Graphics.selectedobject].pos.z = mouse3dz / 5;
							if (SDL_GetModState() & KMOD_SHIFT)
							{
								Graphics.world.lights[Graphics.selectedobject].pos.x = floor(Graphics.world.lights[Graphics.selectedobject].pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
								Graphics.world.lights[Graphics.selectedobject].pos.z = floor(Graphics.world.lights[Graphics.selectedobject].pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
							}
						}
						if(ctrl && !alt)
						{
							Graphics.world.lights[Graphics.selectedobject].pos.y += (mousey-oldmousey);
							if (SDL_GetModState() & KMOD_SHIFT)
							{
								Graphics.world.lights[Graphics.selectedobject].pos.y = floor(Graphics.world.lights[Graphics.selectedobject].pos.y * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							}
						}
						if(!ctrl && alt)
						{
						}
					}
				}
				if (editmode == MODE_OBJECTGROUP && Graphics.groupeditmode)
				{
					int i;
					if(doneaction)
					{
						vector<int> objectsselected;
						for(i = 0; i < Graphics.world.models.size(); i++)
							if(Graphics.world.models[i]->selected)
								objectsselected.push_back(i);
						if (objectsselected.size() > 0)
							undostack.push(new cUndoChangeObjects(objectsselected));
						doneaction = false;
					}
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					for(i = 0; i < Graphics.world.models.size(); i++)
					{
						if (!Graphics.world.models[i]->selected)
							continue;

						if (!ctrl && !alt)
						{
							cVector3 diff = Graphics.world.models[i]->pos - Graphics.selectioncenter;

							Graphics.world.models[i]->pos.x = (mouse3dx/5) + diff.x;
							Graphics.world.models[i]->pos.z = (mouse3dz/5) + diff.z;
							if(snaptofloor->ticked)
								Graphics.world.models[i]->pos.y = -mouse3dy + diff.y;
							
						}
						if(ctrl && !alt)
						{
							cVector2 diff = cVector2(Graphics.world.models[i]->pos.x - Graphics.selectioncenter.x, Graphics.world.models[i]->pos.z - Graphics.selectioncenter.z);
							diff.rotate((mousex-oldmousex)/10.0f);
							Graphics.world.models[i]->pos.x = Graphics.selectioncenter.x + diff.x;
							Graphics.world.models[i]->pos.z = Graphics.selectioncenter.z + diff.y;

							Graphics.world.models[i]->rot.y -= (mousex - oldmousex)/10.0f;
						}
						if(alt && !ctrl)
						{
							cVector2 diff = cVector2(Graphics.world.models[i]->pos.x - Graphics.selectioncenter.x, Graphics.world.models[i]->pos.z - Graphics.selectioncenter.z);
							diff = diff * (1 + ((mousex - oldmousex) / 10.0f));
							Graphics.world.models[i]->pos.x = Graphics.selectioncenter.x + diff.x;
							Graphics.world.models[i]->pos.z = Graphics.selectioncenter.z + diff.y;

							Graphics.world.models[i]->scale = Graphics.world.models[i]->scale * (1+((mousex - oldmousex) / 10.0f));
						}
					}
					if(!ctrl && !alt)
					{
						int count = 0;
						Graphics.selectioncenter = cVector3(0,0,0);
						for(int i = 0; i < Graphics.world.models.size(); i++)
						{
							if (Graphics.world.models[i]->selected)
							{
								count++;
								Graphics.selectioncenter+=Graphics.world.models[i]->pos;
							}
						}
						Graphics.selectioncenter = Graphics.selectioncenter / count;
					}
				}


			}
			dragged = true;
			oldmousex = mousex;
			oldmousey = mousey;
			break;
		} 
		case SDL_MOUSEBUTTONDOWN:
			{
			movement = 0;
			startmousex = mousex = event.motion.x;
			startmousey = mousey = event.motion.y;

			mouse3dxstart = mouse3dx;
			mouse3dystart = mouse3dy;
			mouse3dzstart = mouse3dz;

			tilex = (int)mouse3dx / 10;
			tiley = (int)mouse3dz / 10;

			dragged = false;
			doubleclick = false;
			if (SDL_GetTicks() - lastlclick < 250)
				doubleclick = true;

			cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
		

			if (!dragged && !doubleclick && m == NULL && event.button.button == SDL_BUTTON_LEFT)
			{
				draggingobject = NULL;
				draggingwindow = NULL;
				if (Graphics.WM.inwindow() != NULL)
				{
					cWindow* w = Graphics.WM.inwindow();
					if (!w->inobject())
					{ // drag this window
						dragoffsetx = mousex - w->px();
						dragoffsety = (Graphics.h()-mousey) - w->py2();
						Graphics.WM.click(false);
						draggingwindow = Graphics.WM.inwindow();
						if(startmousex < draggingwindow->px()+draggingwindow->pw() && startmousex > draggingwindow->px()+draggingwindow->pw() - DRAGBORDER)
							draggingwindow->startresisingxy();
						if((Graphics.h()-startmousey) > draggingwindow->py() && (Graphics.h()-startmousey) < draggingwindow->py() + DRAGBORDER)
							draggingwindow->startresizingyx();
						if(startmousex > draggingwindow->px() && startmousex < draggingwindow->px() + DRAGBORDER)
							draggingwindow->startresisingx();
						if((Graphics.h()-startmousey) < draggingwindow->py()+draggingwindow->ph() && (Graphics.h()-startmousey) > draggingwindow->py()+draggingwindow->ph() - DRAGBORDER)
							draggingwindow->startresizingy();
						return 0;
					}
					else
					{ // drag this component
						Graphics.WM.click(false);
						draggingobject = w->inobject();
						dragoffsetx = mousex - w->px() - w->inobject()->realx();
						dragoffsety = (Graphics.h()-mousey) - w->py() - w->inobject()->realy();
					}
					return 0;
				}
				else
				{
					Graphics.WM.defocus();
				}
			}			
			
			
			
			
			
			
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				lbuttondown = true;
				if(m == NULL)
				{
					if (editmode == MODE_TEXTURE)
					{
						if(startmousex < Graphics.w()-256)
						{
							float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
							float selsizey = (fabs(Graphics.selectionstart.y - Graphics.selectionend.y) / 32);

							selsizex = floor(selsizex*Graphics.brushsize);
							selsizey = floor(selsizey*Graphics.brushsize);

							int posx = (int)mouse3dx / 10;
							int posy = (int)mouse3dz / 10;

							float selstartx = ((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f);
							float selstarty = ((int)(Graphics.selectionstart.y - 32) % 288) / 32;
							float selendx = ((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f);
							float selendy = ((int)(Graphics.selectionend.y - 32) % 288) / 32;
							glColor4f(1,1,1,0.7f);
							glEnable(GL_BLEND);

							if (Graphics.texturerot % 2 == 1)
							{
								float a = selsizex;
								selsizex = selsizey;
								selsizey = a;

								a = selstartx;
								selstartx = selstarty;
								selstarty = a;

								a = selendx;
								selendx = selendy;
								selendy = a;

							}

							undostack.push(new cUndoTexture(posx-(int)selsizex+1, posy-(int)selsizey+1, posx+1, posy+1));


							for(int x = posx; x > posx-selsizex; x--)
							{
								for(int y = posy; y > posy-selsizey; y--)
								{
									int xx = posx - x;
									int yy = posy - y;
									if (y < 0)
										continue;
									if (x < 0)
										continue;
									cTile t;
									t.color[0] = (char)255;
									t.color[1] = (char)255;
									t.color[2] = (char)255;
									t.color[3] = (char)255;
									t.texture = Graphics.texturestart + (int)(Graphics.selectionstart.y - 32) / 288;
									t.lightmap = 0;
									if (Graphics.texturerot == 0)
									{
										t.u1 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
										t.v1 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));

										t.u2 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
										t.v2 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));
										
										t.u3 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
										t.v3 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
										
										t.u4 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
										t.v4 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
									}
									if (Graphics.texturerot == 1)
									{
										t.v1 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
										t.u1 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));

										t.v2 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
										t.u2 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));
										
										t.v3 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
										t.u3 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
										
										t.v4 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
										t.u4 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
									}
									if(Graphics.texturerot == 2)
									{
										t.u1 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
										t.v1 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));

										t.u2 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
										t.v2 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));
										
										t.u3 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
										t.v3 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
										
										t.u4 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
										t.v4 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
									}
									if (Graphics.texturerot == 3)
									{
										t.v1 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
										t.u1 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));

										t.v2 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
										t.u2 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));
										
										t.v3 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
										t.u3 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
										
										t.v4 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
										t.u4 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
									}
									

									if(Graphics.fliph)
									{
										t.u1 = ((selendx+selstartx)/8.0)-t.u1;
										t.u2 = ((selendx+selstartx)/8.0)-t.u2;
										t.u3 = ((selendx+selstartx)/8.0)-t.u3;
										t.u4 = ((selendx+selstartx)/8.0)-t.u4;
									}
									if(Graphics.flipv)
									{
										t.v1 = ((selendy+selstarty)/8.0)-t.v1;
										t.v2 = ((selendy+selstarty)/8.0)-t.v2;
										t.v3 = ((selendy+selstarty)/8.0)-t.v3;
										t.v4 = ((selendy+selstarty)/8.0)-t.v4;
									}

//									cTile* tt = &Graphics.world.tiles[Graphics.world.cubes[y][x].tileup];
									Graphics.world.tiles.push_back(t);
									Graphics.world.cubes[y][x].tileup = Graphics.world.tiles.size()-1;
								}
							}
						
						
						}
					}
					else if (editmode == MODE_HEIGHTDETAIL)
					{
						int posx = tilex;
						int posy = tiley;
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						float mmin = 99999999.0f;
						float mmax = -9999999.0f;
						if (ctrl)
						{
							if (posx >= floor(brushsize/2.0f) && posx <= Graphics.world.width-(int)ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy<= Graphics.world.height-(int)ceil(brushsize/2.0f))
							{
								for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
								{
									for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
									{
										cCube* c = &Graphics.world.cubes[y][x];
										mmin = min(min(min(min(mmin, c->cell1),c->cell2),c->cell3),c->cell4);
										mmax = max(max(max(max(mmax, c->cell1),c->cell2),c->cell3),c->cell4);
									}
								}
							}
							
						}

						undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));

						if (posx >= floor(brushsize/2.0f) && posx <= Graphics.world.width-(int)ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy<= Graphics.world.height-(int)ceil(brushsize/2.0f))
						{
							for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
							{
								for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
								{
									cCube* c = &Graphics.world.cubes[y][x];
									if(lbuttondown && !rbuttondown)
									{
										if (!Graphics.slope || (x > posx-(int)floor(brushsize/2.0f)) && y > posy-(int)floor(brushsize/2.0f))
											c->cell1-=1;
										if (!Graphics.slope || (x < posx+(int)ceil(brushsize/2.0f)-1) && y > posy-(int)floor(brushsize/2.0f))
											c->cell2-=1;
										if (!Graphics.slope || (x > posx-(int)floor(brushsize/2.0f)) && y < posy+(int)ceil(brushsize/2.0f)-1)
											c->cell3-=1;
										if (!Graphics.slope || (x < posx+(int)ceil(brushsize/2.0f)-1) && y < posy+(int)ceil(brushsize/2.0f)-1)
											c->cell4-=1;
										if(ctrl)
										{
											c->cell1 = max(mmin,c->cell1);
											c->cell2 = max(mmin,c->cell2);
											c->cell3 = max(mmin,c->cell3);
											c->cell4 = max(mmin,c->cell4);
										}
									}
									if(lbuttondown && rbuttondown)
									{
										if (!Graphics.slope || (x > posx-(int)floor(brushsize/2.0f)) && y > posy-(int)floor(brushsize/2.0f))
											c->cell1+=1;
										if (!Graphics.slope || (x < posx+(int)ceil(brushsize/2.0f)-1) && y > posy-(int)floor(brushsize/2.0f))
											c->cell2+=1;
										if (!Graphics.slope || (x > posx-(int)floor(brushsize/2.0f)) && y < posy+(int)ceil(brushsize/2.0f)-1)
											c->cell3+=1;
										if (!Graphics.slope || (x < posx+(int)ceil(brushsize/2.0f)-1) && y < posy+(int)ceil(brushsize/2.0f)-1)
											c->cell4+=1;
										if(ctrl)
										{
											c->cell1 = min(mmax,c->cell1);
											c->cell2 = min(mmax,c->cell2);
											c->cell3 = min(mmax,c->cell3);
											c->cell4 = min(mmax,c->cell4);
										}
									}
									c->calcnormal();
								}
							}
							lasttimer = SDL_GetTicks()+500;
						}
					}
					else if (editmode == MODE_GAT)
					{
						int posx = (int)mouse3dx / 5;
						int posy = (int)mouse3dz / 5;

						int f = (int)ceil(Graphics.brushsize);

					//	if (posx >= floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= floor(f/2.0f) && posy< 2*Graphics.world.height-(int)ceil(f/2.0f))
						{
							undostack.push(new cUndoGatTileEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));

							glColor4f(1,0,0,1);
							glDisable(GL_TEXTURE_2D);
							glDisable(GL_BLEND);
							for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
							{
								for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
								{
									if (y < 0 || y >= Graphics.world.height*2 || x < 0 || x >= Graphics.world.width*2)
										continue;
									cGatTile* c = &Graphics.world.gattiles[y][x];
									c->type = Graphics.texturestart;
								}
							}
						}
					}
					if(editmode == MODE_OBJECTS)
					{
						int minobj = 0;
						float mindist = 999999;
						for(int i = 0; i < Graphics.world.models.size(); i++)
						{
							cVector3 d = Graphics.world.models[i]->pos;
							d.x = d.x;
							
							d.x -= mouse3dx/5;
							d.z -= mouse3dz/5;
							d.y = 0;

							if(mindist > d.Magnitude())
							{
								mindist = d.Magnitude();
								minobj = i;
							}
						}
						Graphics.objectstartdrag = Graphics.selectedobject == minobj;
					}
					if(editmode == MODE_EFFECTS)
					{
						if(Graphics.world.effects.size() == 0)
							break;
						int minobj = 0;
						float mindist = 999999;
						for(int i = 0; i < Graphics.world.effects.size(); i++)
						{
							cVector3 d = Graphics.world.effects[i].pos;
							d.x = d.x;
							
							d.x -= mouse3dx/5;
							d.z -= mouse3dz/5;
							d.y = 0;

							if(mindist > d.Magnitude())
							{
								mindist = d.Magnitude();
								minobj = i;
							}
						}
						Graphics.objectstartdrag = Graphics.selectedobject == minobj;
					}
					if(editmode == MODE_LIGHTS)
					{
						if(Graphics.world.lights.size() == 0)
							break;
						int minobj = 0;
						float mindist = 999999;
						for(int i = 0; i < Graphics.world.lights.size(); i++)
						{
							cVector3 d = Graphics.world.lights[i].pos;
							d.x = d.x;
							
							d.x -= mouse3dx/5;
							d.z -= mouse3dz/5;
							d.y = 0;

							if(mindist > d.Magnitude())
							{
								mindist = d.Magnitude();
								minobj = i;
							}
						}
						Graphics.objectstartdrag = Graphics.selectedobject == minobj;
					}
				}
			}
			else // rbutton
			{
				rbuttondown = true;
			}
			break;
			}
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
				doneaction = true;
				lbuttondown = false;
				mousex = event.motion.x;
				mousey = event.motion.y;
				cWindow* w = Graphics.WM.inwindow();
				if (draggingwindow != NULL && m == NULL)
				{
					draggingwindow->stopresizing();
				}
				draggingwindow = NULL;
				if (movement <= 1 && m == NULL)
					Graphics.WM.click(true);
				if (draggingobject != NULL && m == NULL)
				{
					if(Graphics.WM.inwindow() != NULL)
						Graphics.WM.inwindow()->dragover();
					if(draggingobject != NULL)
						draggingobject->parent->stopdrag();
					draggingobject = NULL;
				}

				lbuttondown = false;
				if (SDL_GetTicks() - lastlclick < 250)
				{
					doubleclick = true;
					lastlclick = SDL_GetTicks();
					if(m == NULL)
						Graphics.WM.doubleclick();
				}
				else
					lastlclick = SDL_GetTicks();
				menu->unmouseover();
				if (m == NULL)
				{
					menu->closemenu();
					menu->opened = true;
				}
				if (m != NULL && m->opened)
				{
					m->click((int)mousex, Graphics.h()-(int)mousey);
					break;
				}
				else // no menu
				{
					if(w != NULL)
						return 0;
					if(editmode == MODE_OBJECTS && movement < 3)
					{
						if (SDL_GetModState() & KMOD_CTRL && Graphics.previewmodel != NULL)
						{
							cRSMModel* model = new cRSMModel();
							model->load(Graphics.previewmodel->filename);
							model->pos = cVector3(mouse3dx/5, -mouse3dy, mouse3dz/5);
							model->scale = cVector3(1,1,1);
							model->rot = cVector3(0,0,0);
							Graphics.world.models.push_back(model);
							Graphics.selectedobject = Graphics.world.models.size()-1;
							undostack.push(new cUndoNewObject());
						}
						else
						{
							int minobj = 0;
							float mindist = 999999;
							for(int i = 0; i < Graphics.world.models.size(); i++)
							{
								cVector3 d = Graphics.world.models[i]->pos;
								d.x = d.x;
								
								d.x -= mouse3dx/5;
								d.z -= mouse3dz/5;
								d.y = 0;

								if(mindist > d.Magnitude())
								{
									mindist = d.Magnitude();
									minobj = i;
								}
							}
							Graphics.selectedobject = minobj;
						}
					}
					if(editmode == MODE_EFFECTS && movement < 3)
					{

						if (SDL_GetModState() & KMOD_CTRL)
						{
							if (selectedeffect == NULL)
								break;
							cEffect e;
							char buf[100];
							sprintf(buf, "obj%i", rand());
							e.category = "\0\0\0\0";
							e.loop = 40;
							e.pos = cVector3(mouse3dx/5, mouse3dy/5, mouse3dz/5);
							e.name = buf;
							e.readablename = selectedeffect->title;
							e.type = atoi(selectedeffect->data.c_str());
							e.todo1 = 1;
							e.todo2 = 1;
							e.todo3 = 1;
							e.rotation = cVector3(0,0,0);
							e.scale = cVector3(1,1,1);
							e.todo10 = 1; // seems to be linked to 11
							e.todo11 = 1; // seems to be linked to 10
							e.todo12 = 0; // seems to be always 0
							e.todo13 = 0; // seems to be always 0

							Graphics.world.effects.push_back(e);
						}
						else
						{
							if (Graphics.world.effects.size() == 0)
								break;
							int minobj = 0;
							float mindist = 999999;
							for(int i = 0; i < Graphics.world.effects.size(); i++)
							{
								cVector3 d = Graphics.world.effects[i].pos;
								d.x = d.x;
								
								d.x -= mouse3dx/5;
								d.z -= mouse3dz/5;
								d.y = 0;

								if(mindist > d.Magnitude())
								{
									mindist = d.Magnitude();
									minobj = i;
								}
							}
							Graphics.selectedobject = minobj;
							char buf[100];
							sprintf(buf, "%i", Graphics.world.effects[Graphics.selectedobject].type);
							cMenu* m = effectsmenu->finddata(buf);
						}
					}
					if(editmode == MODE_LIGHTS && movement < 3)
					{

						if (SDL_GetModState() & KMOD_CTRL)
						{
							cLight l;
							char buf[100];
							sprintf(buf, "obj%i", rand());
							l.name = buf;
							l.color.x = 0;
							l.color.y = 0;
							l.color.z = 0;
							l.pos = cVector3(mouse3dx/5, mouse3dy/5, mouse3dz/5);
							l.todo = string(buf, 40);
							l.todo2 = 300;

							Graphics.world.lights.push_back(l);
							undostack.push(new cUndoNewLight());
						}
						else
						{
							if (Graphics.world.lights.size() == 0)
								break;
							int minobj = 0;
							float mindist = 999999;
							for(int i = 0; i < Graphics.world.lights.size(); i++)
							{
								cVector3 d = Graphics.world.lights[i].pos;
								d.x = d.x;
								
								d.x -= mouse3dx/5;
								d.z -= mouse3dz/5;
								d.y = 0;

								if(mindist > d.Magnitude())
								{
									mindist = d.Magnitude();
									minobj = i;
								}
							}
							Graphics.selectedobject = minobj;
						}
					}
					else if(editmode == MODE_WALLS && movement < 3)
					{
						int x = (int)mouse3dx / 10;
						int y = (int)mouse3dz / 10;
						if (y < 0 || y > Graphics.world.height-1)
							break;
						if (x < 0 || x > Graphics.world.width-1)
							break;
						if (Graphics.wallheightmin == cVector2(x,y))
							Graphics.wallheightmin = cVector2(-1,-1);
						else
							Graphics.wallheightmin = cVector2(x,y);
					}
					else if (editmode == MODE_HEIGHTGLOBAL)
					{
						if (mouse3dxstart > mouse3dx)
						{
							double d = mouse3dx;
							mouse3dx = mouse3dxstart;
							mouse3dxstart = d;
						}
						if (mouse3dzstart > mouse3dz)
						{
							double d = mouse3dz;
							mouse3dz = mouse3dzstart;
							mouse3dzstart = d;
						}
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						bool alt = (SDL_GetModState() & KMOD_ALT) != 0;

						
						int x,y;
						if(!ctrl && !alt)
						{
							for(x = 0; x < Graphics.world.width; x++)
							{
								for(y = 0; y < Graphics.world.height; y++)
									Graphics.world.cubes[y][x].selected = false;
							}
						}

						for(x = (int)floor(mouse3dxstart/10); x < floor(mouse3dx/10); x++)
						{
							for(y = (int)floor(mouse3dzstart/10); y < (int)floor(mouse3dz/10); y++)
							{
								if (x >= 0 && x < Graphics.world.width && y >= 0 && y < Graphics.world.height)
								{
									Graphics.world.cubes[y][x].selected = !alt;
								}
							}
						}

					}
					else if (editmode == MODE_OBJECTGROUP && !Graphics.groupeditmode)
					{
						if (mouse3dxstart > mouse3dx)
						{
							double d = mouse3dx;
							mouse3dx = mouse3dxstart;
							mouse3dxstart = d;
						}
						if (mouse3dzstart > mouse3dz)
						{
							double d = mouse3dz;
							mouse3dz = mouse3dzstart;
							mouse3dzstart = d;
						}
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						bool alt = (SDL_GetModState() & KMOD_ALT) != 0;

						
						int i;
						if(!ctrl && !alt)
						{
							for(i = 0; i < Graphics.world.models.size(); i++)
								Graphics.world.models[i]->selected = false;
						}


						for(i = 0; i < Graphics.world.models.size(); i++)
						{
							cVector3* pos = &Graphics.world.models[i]->pos;
							if (pos->x*5 > mouse3dxstart && pos->x*5 < mouse3dx && pos->z*5 > mouse3dzstart && pos->z*5 < mouse3dz)
							{
								Graphics.world.models[i]->selected = !alt;
							}
						}

					}

				}
				
			}
			else // right button
			{
				rbuttondown = false;
				doubleclick = false;
				if (movement < 2)
				{
					if(Graphics.WM.inwindow() != NULL)
						Graphics.WM.rightclick();
				}
				long l = SDL_GetTicks();
				if (l - lastrclick < 250)
				{
					doubleclick = true;
					lastrclick = 0;
				}
				else
					lastrclick = SDL_GetTicks();
				if(doubleclick && movement < 3)
				{
					Graphics.camerarot = 0;
				}
				lastrclick = SDL_GetTicks();
				rbuttondown = false;
				mousex = event.motion.x;
				mousey = event.motion.y;

				if(movement < 3 && (editmode == MODE_OBJECTS || editmode == MODE_EFFECTS || editmode == MODE_LIGHTS))
				{
					Graphics.selectedobject = -1;
				}
				else if(editmode == MODE_WALLS && movement < 3)
				{
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0 || y > Graphics.world.height-1)
						break;
					if (x < 0 || x > Graphics.world.width-1)
						break;
					if (Graphics.wallheightmax == cVector2(x,y))
						Graphics.wallheightmax = cVector2(-1,-1);
					else
						Graphics.wallheightmax = cVector2(x,y);

				}
				
			}
			break;
		case SDL_KEYDOWN:
			{
				bool found = false;
				for(int i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
				{
					if (keymap[i] == event.key.keysym.sym+SDLK_FIRST)
					{
						event.key.keysym.sym = (SDLKey)(i+SDLK_FIRST);
						found = true;
						break;
					}
				}
				if(!found)
					break;
			}


			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				if (editmode == MODE_OBJECTS)
				{
					if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
					{
						undostack.push(new cUndoChangeObject(Graphics.selectedobject));
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
						bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
						if (!ctrl && !alt)
							Graphics.world.models[Graphics.selectedobject]->pos.z+=0.1 + (shift ? 0.4 : 0);
						if (ctrl && !alt)
							Graphics.world.models[Graphics.selectedobject]->rot.z+=1 + (shift ? 44 : 0);
						if (!ctrl && alt)
							Graphics.world.models[Graphics.selectedobject]->scale.z+=0.1 + shift ? 0.1 : 0;
					}						
				}
				break;
			case SDLK_DOWN:
				if (editmode == MODE_OBJECTS)
				{
					if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
					{
						undostack.push(new cUndoChangeObject(Graphics.selectedobject));
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
						bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
						if (!ctrl && !alt)
							Graphics.world.models[Graphics.selectedobject]->pos.z-=0.1 + (shift ? 0.4 : 0);
						if (ctrl && !alt)
							Graphics.world.models[Graphics.selectedobject]->rot.z-=1 + (shift ? 44 : 0);
						if (!ctrl && alt)
							Graphics.world.models[Graphics.selectedobject]->scale.z-=0.1 + shift ? 0.1 : 0;
					}						
				}
				break;
			case SDLK_RIGHT:
				if (editmode == MODE_OBJECTS)
				{
					if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
					{
						undostack.push(new cUndoChangeObject(Graphics.selectedobject));
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
						bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
						if (!ctrl && !alt)
							Graphics.world.models[Graphics.selectedobject]->pos.x+=0.1 + (shift ? 0.4 : 0);
						if (ctrl && !alt)
							Graphics.world.models[Graphics.selectedobject]->rot.x+=1 + (shift ? 44 : 0);
						if (!ctrl && alt)
							Graphics.world.models[Graphics.selectedobject]->scale.x+=0.1 + shift ? 0.1 : 0;
					}						
				}
				break;
			case SDLK_LEFT:
				if (editmode == MODE_OBJECTS)
				{
					if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
					{
						undostack.push(new cUndoChangeObject(Graphics.selectedobject));
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
						bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
						if (!ctrl && !alt)
							Graphics.world.models[Graphics.selectedobject]->pos.x-=0.1 + (shift ? 0.4 : 0);
						if (ctrl && !alt)
							Graphics.world.models[Graphics.selectedobject]->rot.x-=1 + (shift ? 44 : 0);
						if (!ctrl && alt)
							Graphics.world.models[Graphics.selectedobject]->scale.x-=0.1 + shift ? 0.1 : 0;
					}						
				}
				break;
			case SDLK_MINUS:
				if (editmode == MODE_OBJECTS)
				{
					if (Graphics.previewmodel != NULL)
						Graphics.previewmodel->scale = Graphics.previewmodel->scale * 0.9f;
				}
				else if (editmode == MODE_HEIGHTDETAIL && brushsize > 1)
				{
					brushsize/=2;
				}
				else if (editmode != MODE_GAT || Graphics.brushsize > 1)
					Graphics.brushsize/=2;
					
				break;
			case SDLK_EQUALS:
				if (editmode == MODE_OBJECTS)
				{
					if (Graphics.previewmodel != NULL)
						Graphics.previewmodel->scale = Graphics.previewmodel->scale * 1.1f;
				}
				else if (editmode == MODE_HEIGHTDETAIL)
				{
					brushsize*=2;
				}
				else
					Graphics.brushsize*=2;
				break;
			case SDLK_LEFTBRACKET:
				if (editmode == MODE_OBJECTS && currentobject != NULL)
				{
					currentobject = currentobject->parent->getprev(currentobject);
					MenuCommand_model((cMenuItem*)currentobject);
				}
				else if (editmode == MODE_WATER)
				{
					Graphics.world.water.type = max(0, Graphics.world.water.type - 1);
				}
				else if (editmode == MODE_OBJECTGROUP)
				{
					Graphics.groupeditmode = !Graphics.groupeditmode;
					int count = 0;
					Graphics.selectioncenter = cVector3(0,0,0);
					for(int i = 0; i < Graphics.world.models.size(); i++)
					{
						if (Graphics.world.models[i]->selected)
						{
							count++;
							Graphics.selectioncenter+=Graphics.world.models[i]->pos;
						}
					}
					Graphics.selectioncenter = Graphics.selectioncenter / count;
				}
				else
				{
					Graphics.texturestart--;
					if (Graphics.texturestart < 0)
						Graphics.texturestart = 0;
				}
				break;
			case SDLK_RIGHTBRACKET:
				if (editmode == MODE_OBJECTS && currentobject != NULL)
				{
					currentobject = currentobject->parent->getnext(currentobject);
					MenuCommand_model((cMenuItem*)currentobject);
				}
				else if (editmode == MODE_WATER)
				{
					Graphics.world.water.type = min(5, Graphics.world.water.type + 1);
				}
				else if (editmode == MODE_OBJECTGROUP)
				{
					Graphics.groupeditmode = !Graphics.groupeditmode;
					int count = 0;
					Graphics.selectioncenter = cVector3(0,0,0);
					for(int i = 0; i < Graphics.world.models.size(); i++)
					{
						if (Graphics.world.models[i]->selected)
						{
							count++;
							Graphics.selectioncenter+=Graphics.world.models[i]->pos;
						}
					}
					Graphics.selectioncenter = Graphics.selectioncenter / count;
				}
				else
				{
					Graphics.texturestart++;
					if (Graphics.texturestart > (editmode == MODE_GAT ? 8 : (int)Graphics.world.textures.size()) - (Graphics.h() / 288))
						Graphics.texturestart--;
				}
				break;
			case SDLK_SPACE:
				if (Graphics.previewcolor > 20)
					Graphics.previewcolor = 20;
				if(editmode == MODE_TEXTURE)
					Graphics.texturerot = (Graphics.texturerot + 1) % 4;
				else if (editmode == MODE_GAT)
				{
					int posx = (int)mouse3dx / 5;
					int posy = (int)mouse3dz / 5;

					float f = (int)ceil(Graphics.brushsize);


					if (posx >= (int)floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= (int)floor(f/2.0f) && posy< 2*Graphics.world.height-(int)ceil(f/2.0f))
					{
						undostack.push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
						for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
						{
							for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
							{
								cCube* c = &Graphics.world.cubes[y/2][x/2];

								if (y%2 == 0 && x%2 == 0)
								{
									Graphics.world.gattiles[y][x].cell1 = (c->cell1+c->cell1) / 2.0f;
									Graphics.world.gattiles[y][x].cell2 = (c->cell1+c->cell2) / 2.0f;
									Graphics.world.gattiles[y][x].cell3 = (c->cell1+c->cell3) / 2.0f;
									Graphics.world.gattiles[y][x].cell4 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
								}
								if (y%2 == 0 && x%2 == 1)
								{
									Graphics.world.gattiles[y][x].cell1 = (c->cell1+c->cell2) / 2.0f;
									Graphics.world.gattiles[y][x].cell2 = (c->cell2+c->cell2) / 2.0f;
									Graphics.world.gattiles[y][x].cell3 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
									Graphics.world.gattiles[y][x].cell4 = (c->cell4+c->cell2) / 2.0f;
								}
								if (y%2 == 1 && x%2 == 0)
								{
									Graphics.world.gattiles[y][x].cell1 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
									Graphics.world.gattiles[y][x].cell2 = (c->cell4 + c->cell2) / 2.0f;
									Graphics.world.gattiles[y][x].cell3 = (c->cell4 + c->cell3) / 2.0f;
									Graphics.world.gattiles[y][x].cell4 = (c->cell4 + c->cell4) / 2.0f;
								}
								if (y%2 == 1 && x%2 == 1)
								{
									Graphics.world.gattiles[y][x].cell1 = (c->cell3 + c->cell1) / 2.0f;
									Graphics.world.gattiles[y][x].cell2 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
									Graphics.world.gattiles[y][x].cell3 = (c->cell3 + c->cell3) / 2.0f;
									Graphics.world.gattiles[y][x].cell4 = (c->cell3 + c->cell4) / 2.0f;
								}
							
							}
						}
					}
				}
				break;
			case SDLK_h:
				if (editmode == MODE_TEXTURE)
					Graphics.fliph = !Graphics.fliph;
				if (editmode == MODE_OBJECTS && Graphics.selectedobject != -1)
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					if(SDL_GetModState() & KMOD_ALT)
						Graphics.world.models[Graphics.selectedobject]->scale.y = -	Graphics.world.models[Graphics.selectedobject]->scale.y;
					else
						Graphics.world.models[Graphics.selectedobject]->scale.x = -	Graphics.world.models[Graphics.selectedobject]->scale.x;
				}
				if(editmode == MODE_WALLS)
				{
					vector<pair<int, cTile> > tilesedited;

					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;

					if(SDL_GetModState() & KMOD_ALT)
					{
						if(Graphics.world.cubes[y][x].tileaside == -1)
							break;

						int yy = y;
						while(Graphics.world.cubes[yy][x].tileaside != -1)
							yy++;
						int ymax = yy;
						yy = y;
						while(Graphics.world.cubes[yy][x].tileaside != -1)
							yy--;
						int ymin = yy+1;
						int ydiff = 4;

						for(yy = ymin; yy < ymax; yy++)
						{
							cTile* t = &Graphics.world.tiles[Graphics.world.cubes[yy][x].tileaside];
							tilesedited.push_back(pair<int, cTile>(Graphics.world.cubes[yy][x].tileaside, *t));
							float f;
							f = t->u1;
							t->u1 = t->u2;
							t->u2 = f;

							f = t->u3;
							t->u3 = t->u4;
							t->u4 = f;
						}
					}
					else
					{
						if(Graphics.world.cubes[y][x].tileside == -1)
							break;

						int xx = x;
						while(Graphics.world.cubes[y][xx].tileside != -1)
							xx++;
						int xmax = xx;
						xx = x;
						while(Graphics.world.cubes[y][xx].tileside != -1)
							xx--;
						int xmin = xx+1;
						int xdiff = 4;

						for(xx = xmin; xx < xmax; xx++)
						{
							cTile* t = &Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside];
							tilesedited.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, *t));
							float f;
							f = t->u1;
							t->u1 = t->u2;
							t->u2 = f;

							f = t->u3;
							t->u3 = t->u4;
							t->u4 = f;
						}
					}
					if (tilesedited.size() > 0)
						undostack.push(new cUndoTileEdit(tilesedited));
				}
				break;
			case SDLK_v:
				if (editmode == MODE_TEXTURE)
					Graphics.flipv = !Graphics.flipv;
				if (editmode == MODE_OBJECTS && Graphics.selectedobject != -1)
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					Graphics.world.models[Graphics.selectedobject]->scale.z = -	Graphics.world.models[Graphics.selectedobject]->scale.z;
				}
				if(editmode == MODE_WALLS)
				{
					vector<pair<int, cTile> > tilesedited;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;

					if(SDL_GetModState() & KMOD_ALT)
					{
						if(Graphics.world.cubes[y][x].tileaside == -1)
							break;

						int yy = y;
						int ymax = yy+1;
						int ymin = yy;
						int ydiff = 2;

						if (SDL_GetModState() & KMOD_SHIFT)
						{
							yy = y;
							while(Graphics.world.cubes[yy][x].tileaside != -1)
								yy++;
							ymax = yy;
							yy = y;
							while(Graphics.world.cubes[yy][x].tileaside != -1)
								yy--;
							ymin = yy+1;
							ydiff = 4;
						}

						for(yy = ymin; yy < ymax; yy++)
						{
							cTile* t = &Graphics.world.tiles[Graphics.world.cubes[yy][x].tileaside];
							tilesedited.push_back(pair<int, cTile>(Graphics.world.cubes[yy][x].tileaside, *t));
							float f;
							f = t->v1;
							t->v1 = t->v3;
							t->v3 = f;

							f = t->v2;
							t->v2 = t->v4;
							t->v4 = f;
						}
					}
					else
					{
						if(Graphics.world.cubes[y][x].tileside == -1)
							break;

						int xx = x;
						int xmax = xx+1;
						int xmin = xx;
						int xdiff = 4;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							xx = x;
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx++;
							xmax = xx;
							xx = x;
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx--;
							xmin = xx+1;
							xdiff = 4;
						}

						for(xx = xmin; xx < xmax; xx++)
						{
							cTile* t = &Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside];
							tilesedited.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, *t));
							float f;
							f = t->v1;
							t->v1 = t->v3;
							t->v3 = f;

							f = t->v2;
							t->v2 = t->v4;
							t->v4 = f;
						}
					}
					if (tilesedited.size() > 0)
						undostack.push(new cUndoTileEdit(tilesedited));

				}
				break;
			case SDLK_g:
				MenuCommand_grid((cMenuItem*)grid);
				break;
			case SDLK_l:
				MenuCommand_toggle((cMenuItem*)menu->find("Lightmaps"));
				break;
			case SDLK_d:
				{
					Graphics.WM.printdebug();
					if (editmode == MODE_OBJECTGROUP)
					{
						undostack.push(new cUndoNewObjects(Graphics.world.models.size()));
						int start = Graphics.world.models.size();
						int i;
						for(i = 0; i < start; i++)
						{
							if (Graphics.world.models[i]->selected)
							{
								Graphics.world.models[i]->selected = false;
								cRSMModel* model = new cRSMModel();
								model->load(Graphics.world.models[i]->filename);
								model->pos = Graphics.world.models[i]->pos + cVector3(4,0,4);
								model->scale = Graphics.world.models[i]->scale;
								model->rot = Graphics.world.models[i]->rot;
								model->selected = true;
								Graphics.world.models.push_back(model);
							}
						}

					}
					else
					{
						int posx = (int)mouse3dx / 10;
						int posy = (int)mouse3dz / 10;

						if (posx >= brushsize && posx < Graphics.world.width-brushsize && posy >= brushsize && posy< Graphics.world.height-brushsize)
						{
							Log(3,0,"Pos: %i,%i", posx, posy);
							Log(3,0,"Cube: tileup: %i, tileside: %i, tileaside: %i, v1: %f, v2: %f, v3: %f, v4: %f", Graphics.world.cubes[posy][posx].tileup, Graphics.world.cubes[posy][posx].tileside, Graphics.world.cubes[posy][posx].tileaside, Graphics.world.cubes[posy][posx].cell1, Graphics.world.cubes[posy][posx].cell2, Graphics.world.cubes[posy][posx].cell3, Graphics.world.cubes[posy][posx].cell4);
							Log(3,0,"Tileup: texture: %i, v1: (%f,%f), v2: (%f,%f), v3: (%f,%f), v4: (%f,%f)", Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].texture, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u1, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v1, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u2, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v2, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u3, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v3, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u4, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v4);
						}
					}
				}
				break;
			case SDLK_COMMA:
				{
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0 || y > Graphics.world.cubes.size()-1)
						break;
					if (x < 0 || x > Graphics.world.cubes[0].size()-1)
						break;

					undostack.push(new cUndoChangeWall(0,x,y, Graphics.world.cubes[y][x].tileside));
					if(Graphics.world.cubes[y][x].tileside != -1)
					{
						Graphics.world.cubes[y][x].tileside = -1;
						break;
					}

					cTile t;
					t.color[0] = (char)255;
					t.color[1] = (char)255;
					t.color[2] = (char)255;
					t.color[3] = (char)255;
					t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
					t.lightmap = 0;
					float selstartx = (((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
					float selstarty = (((int)(Graphics.selectionstart.y - 32) % 288) / 32) / 8.0f;
					float selendx = (((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
					float selendy = (((int)(Graphics.selectionend.y - 32) % 288) / 32) / 8.0f;

					
					t.u1 = selstartx;
					t.v1 = selstarty;

					t.u2 = selendx;
					t.v2 = selstarty;
					
					t.u3 = selstartx;
					t.v3 = selendy;
					
					t.u4 = selendx;
					t.v4 = selendy;
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileside = Graphics.world.tiles.size()-1;

					break;
				}
			case SDLK_PERIOD:
				{
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0 || y > Graphics.world.cubes.size()-1)
						break;
					if (x < 0 || x > Graphics.world.cubes[0].size()-1)
						break;

					undostack.push(new cUndoChangeWall(1,x,y, Graphics.world.cubes[y][x].tileaside));
					if(Graphics.world.cubes[y][x].tileaside != -1)
					{
						Graphics.world.cubes[y][x].tileaside = -1;
						break;
					}

					cTile t;
					t.color[0] = (char)255;
					t.color[1] = (char)255;
					t.color[2] = (char)255;
					t.color[3] = (char)255;
					t.texture = Graphics.texturestart + (int)(Graphics.selectionstart.y - 32) / 288;
					t.lightmap = 0;
					float selstartx = (((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
					float selstarty = (((int)(Graphics.selectionstart.y - 32) % 288) / 32) / 8.0f;
					float selendx = (((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
					float selendy = (((int)(Graphics.selectionend.y - 32) % 288) / 32) / 8.0f;

					
					t.u1 = selstartx;
					t.v1 = selstarty;

					t.u2 = selendx;
					t.v2 = selstarty;
					
					t.u3 = selstartx;
					t.v3 = selendy;
					
					t.u4 = selendx;
					t.v4 = selendy;
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileaside = Graphics.world.tiles.size()-1;

					break;
				}
			case SDLK_PAGEDOWN:
				{
					if(editmode == MODE_WALLS)
					{
						vector<pair<int, cTile> > tileschanged;
						int x = (int)mouse3dx / 10;
						int y = (int)mouse3dz / 10;
						if(SDL_GetModState() & KMOD_SHIFT)
						{
							int xx = x;
							int xmax;
							if (SDL_GetModState() & KMOD_CTRL)
							{
								while(Graphics.world.cubes[y][xx].tileside != -1)
									xx++;
								xmax = xx;
							}
							else
								xmax = xx+1;
							xx = x;
							int xmin;
							if (SDL_GetModState() & KMOD_CTRL)
							{
								while(Graphics.world.cubes[y][xx].tileside != -1)
									xx--;
								xmin = xx+1;
							}
							else
								xmin = xx;
							 
							int xdiff = 4;

							for(xx = xmin; xx < xmax; xx++)
							{
								tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside]));
								Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v3+=0.03125;
								Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v4+=0.03125;
							}
						}
						else
						{
								tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][x].tileside, Graphics.world.tiles[Graphics.world.cubes[y][x].tileside]));
								Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u1+=0.03125;
								Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u3+=0.03125;
						}
						if(tileschanged.size() > 0)
							undostack.push(new cUndoTileEdit(tileschanged));
					}
					if (editmode == MODE_OBJECTS)
					{
						if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
						{
							undostack.push(new cUndoChangeObject(Graphics.selectedobject));
							bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
							bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
							bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
							if (!ctrl && !alt)
								Graphics.world.models[Graphics.selectedobject]->pos.y+=0.1 + (shift ? 0.4 : 0);
							if (ctrl && !alt)
								Graphics.world.models[Graphics.selectedobject]->rot.y+=1 + (shift ? 44 : 0);
							if (!ctrl && alt)
								Graphics.world.models[Graphics.selectedobject]->scale.y+=0.1 + shift ? 0.1 : 0;
						}						
					}
					if(editmode == MODE_GAT)
					{
						int posx = (int)mouse3dx / 5;
						int posy = (int)mouse3dz / 5;

						float f = (int)ceil(Graphics.brushsize);


						if (posx >= (int)floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= (int)floor(f/2.0f) && posy< 2*Graphics.world.height-(int)ceil(f/2.0f))
						{
							undostack.push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
							for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
							{
								for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
								{
									cGatTile* c = &Graphics.world.gattiles[y][x];
									if (!Graphics.slope || (x > posx-(int)floor(f/2.0f)) && y > posy-(int)floor(f/2.0f))
										c->cell1+=1;
									if (!Graphics.slope || (x < posx+(int)ceil(f/2.0f)-1) && y > posy-(int)floor(f/2.0f))
										c->cell2+=1;
									if (!Graphics.slope || (x > posx-(int)floor(f/2.0f)) && y < posy+(int)ceil(f/2.0f)-1)
										c->cell3+=1;
									if (!Graphics.slope || (x < posx+(int)ceil(f/2.0f)-1) && y < posy+(int)ceil(f/2.0f)-1)
										c->cell4+=1;
								}
							}
						}
					}
					if (editmode == MODE_WATER)
					{
						undostack.push(new cUndoChangeWater(Graphics.world.water));
						Graphics.world.water.height++;
					}
					if (editmode == MODE_HEIGHTGLOBAL)
					{
						undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));
						for(int x = 0; x < Graphics.world.width; x++)
						{
							for(int y = 0; y < Graphics.world.height; y++)
							{
								if(!Graphics.world.cubes[y][x].selected)
									continue;
								Graphics.world.cubes[y][x].cell1 += 1;
								Graphics.world.cubes[y][x].cell2 += 1;
								Graphics.world.cubes[y][x].cell3 += 1;
								Graphics.world.cubes[y][x].cell4 += 1;
							}
						}
						for(int y = 0; y < Graphics.world.gattiles.size(); y++)
						{
							for(int x = 0; x < Graphics.world.gattiles[y].size(); x++)
							{
								if(!Graphics.world.cubes[y/2][x/2].selected)
									continue;
								Graphics.world.gattiles[y][x].cell1 += 1;
								Graphics.world.gattiles[y][x].cell2 += 1;
								Graphics.world.gattiles[y][x].cell3 += 1;
								Graphics.world.gattiles[y][x].cell4 += 1;
							}
						}
					}
					if (editmode == MODE_EFFECTS)
					{
						if (Graphics.selectedobject != -1)
						{
							undostack.push(new cUndoChangeEffect(Graphics.selectedobject));
							Graphics.world.effects[Graphics.selectedobject].loop--;
						}
					}
					if(editmode == MODE_OBJECTGROUP)
					{
						for(int i = 0; i < Graphics.world.models.size(); i++)
						{
							if(Graphics.world.models[i]->selected)
							{
								Graphics.world.models[i]->pos.y++;
							}
						}
					}
					break;
				}
			case SDLK_PAGEUP:
				{
					if(editmode == MODE_WALLS)
					{
						vector<pair<int, cTile> > tileschanged;
						int x = (int)mouse3dx / 10;
						int y = (int)mouse3dz / 10;
						if(SDL_GetModState() & KMOD_SHIFT)
						{
							int xx = x;
							int xmax;
							if (SDL_GetModState() & KMOD_CTRL)
							{
								while(Graphics.world.cubes[y][xx].tileside != -1)
									xx++;
								xmax = xx;
							}
							else
								xmax = xx+1;
							xx = x;
							int xmin;
							if (SDL_GetModState() & KMOD_CTRL)
							{
								while(Graphics.world.cubes[y][xx].tileside != -1)
									xx--;
								xmin = xx+1;
							}
							else
								xmin = xx;
							 
							int xdiff = 4;

							for(xx = xmin; xx < xmax; xx++)
							{
								tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside]));
								Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v3-=0.03125;
								Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v4-=0.03125;
							}
						}
						else
						{
							tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][x].tileside, Graphics.world.tiles[Graphics.world.cubes[y][x].tileside]));
							Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u1-=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u3-=0.03125;
						}
						if(tileschanged.size() > 0)
							undostack.push(new cUndoTileEdit(tileschanged));
					}
					if (editmode == MODE_OBJECTS)
					{
						if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
						{
							undostack.push(new cUndoChangeObject(Graphics.selectedobject));
							bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
							bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
							bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
							if (!ctrl && !alt)
								Graphics.world.models[Graphics.selectedobject]->pos.y-=0.1 + (shift ? 0.4 : 0);
							if (ctrl && !alt)
								Graphics.world.models[Graphics.selectedobject]->rot.y-=1 + (shift ? 44 : 0);
							if (!ctrl && alt)
								Graphics.world.models[Graphics.selectedobject]->scale.y-=0.1 + shift ? 0.1 : 0;
						}						
					}
					if(editmode == MODE_GAT)
					{
						int posx = (int)mouse3dx / 5;
						int posy = (int)mouse3dz / 5;

						float f = (int)ceil(Graphics.brushsize);

						if (posx >= (int)floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= (int)floor(f/2.0f) && posy< 2*Graphics.world.height-(int)ceil(f/2.0f))
						{
							undostack.push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
							glColor4f(1,0,0,1);
							glDisable(GL_TEXTURE_2D);
							glDisable(GL_BLEND);
							for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
							{
								for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
								{
									cGatTile* c = &Graphics.world.gattiles[y][x];
									if (!Graphics.slope || (x > posx-(int)floor(f/2.0f)) && y > posy-(int)floor(f/2.0f))
										c->cell1-=1;
									if (!Graphics.slope || (x < posx+(int)ceil(f/2.0f)-1) && y > posy-(int)floor(f/2.0f))
										c->cell2-=1;
									if (!Graphics.slope || (x > posx-(int)floor(f/2.0f)) && y < posy+(int)ceil(f/2.0f)-1)
										c->cell3-=1;
									if (!Graphics.slope || (x < posx+(int)ceil(f/2.0f)-1) && y < posy+(int)ceil(f/2.0f)-1)
										c->cell4-=1;
								}
							}
						}
					}
					if (editmode == MODE_WATER)
					{
						undostack.push(new cUndoChangeWater(Graphics.world.water));
						Graphics.world.water.height--;
					}
					if (editmode == MODE_HEIGHTGLOBAL)
					{
						undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));
						for(int x = 0; x < Graphics.world.width; x++)
						{
							for(int y = 0; y < Graphics.world.height; y++)
							{
								if(!Graphics.world.cubes[y][x].selected)
									continue;
								Graphics.world.cubes[y][x].cell1 -= 1;
								Graphics.world.cubes[y][x].cell2 -= 1;
								Graphics.world.cubes[y][x].cell3 -= 1;
								Graphics.world.cubes[y][x].cell4 -= 1;
							}
						}
						for(int y = 0; y < Graphics.world.gattiles.size(); y++)
						{
							for(int x = 0; x < Graphics.world.gattiles[y].size(); x++)
							{
								if(!Graphics.world.cubes[y/2][x/2].selected)
									continue;
								Graphics.world.gattiles[y][x].cell1 -= 1;
								Graphics.world.gattiles[y][x].cell2 -= 1;
								Graphics.world.gattiles[y][x].cell3 -= 1;
								Graphics.world.gattiles[y][x].cell4 -= 1;
							}
						}
					}
					if (editmode == MODE_EFFECTS)
					{
						if (Graphics.selectedobject != -1)
						{
							undostack.push(new cUndoChangeEffect(Graphics.selectedobject));
							Graphics.world.effects[Graphics.selectedobject].loop++;
						}
					}
					if(editmode == MODE_OBJECTGROUP)
					{
						for(int i = 0; i < Graphics.world.models.size(); i++)
						{
							if(Graphics.world.models[i]->selected)
							{
								Graphics.world.models[i]->pos.y--;
							}
						}
					}
					break;
				}
			case SDLK_HOME:
				if(editmode == MODE_WALLS)
				{
					vector<pair<int, cTile> > tileschanged;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if(SDL_GetModState() & KMOD_SHIFT)
					{
						int xx = x;
						int xmax;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx++;
							xmax = xx;
						}
						else
							xmax = xx+1;
						xx = x;
						int xmin;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx--;
							xmin = xx+1;
						}
						else
							xmin = xx;
						 
						int xdiff = 4;

						for(xx = xmin; xx < xmax; xx++)
						{
							if (y < 0 || y > Graphics.world.height)
								continue;
							if (xx < 0 || xx > Graphics.world.width)
								continue;

							tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside]));
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v1+=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v2+=0.03125;
						}
					}
					else
					{
						if (y < 0 || y > Graphics.world.height-1)
							break;
						if (x < 0 || x > Graphics.world.width-1)
							break;
						tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][x].tileside, Graphics.world.tiles[Graphics.world.cubes[y][x].tileside]));
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2+=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4+=0.03125;
					}
					if(tileschanged.size() > 0)
						undostack.push(new cUndoTileEdit(tileschanged));
					break;
				}
			case SDLK_END:
				if(editmode == MODE_WALLS)
				{
					vector<pair<int, cTile> > tileschanged;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if(SDL_GetModState() & KMOD_SHIFT)
					{
						int xx = x;
						int xmax;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx++;
							xmax = xx;
						}
						else
							xmax = xx+1;
						xx = x;
						int xmin;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx--;
							xmin = xx+1;
						}
						else
							xmin = xx;
						 
						int xdiff = 4;

						for(xx = xmin; xx < xmax; xx++)
						{
							tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside]));
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v1-=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v2-=0.03125;
						}
					}
					else
					{
						if (y < 0 || y > Graphics.world.height)
							break;
						if (x < 0 || x > Graphics.world.width)
							break;
						tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][x].tileside, Graphics.world.tiles[Graphics.world.cubes[y][x].tileside]));
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2+=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4+=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2-=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4-=0.03125;
					}
					if(tileschanged.size() > 0)
						undostack.push(new cUndoTileEdit(tileschanged));
					break;
				}
			case SDLK_w:
				if(SDL_GetModState() & KMOD_META)
				{
					MenuCommand_toggle((cMenuItem*)menu->find("Water"));
					break;
				}
				else if (editmode == MODE_WALLS)
				{
					bool wrap = true;
					if (SDL_GetModState() & KMOD_SHIFT)
						wrap = false;


					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0 || y > Graphics.world.height - 1)
						break;
					if (x < 0 || x > Graphics.world.width - 1)
						break;

					float selstartx = (((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
					float selstarty = (((int)(Graphics.selectionstart.y - 32) % 288) / 32) / 8.0f;
					float selendx = (((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
					float selendy = (((int)(Graphics.selectionend.y - 32) % 288) / 32) / 8.0f;
					float selheight = selendy - selstarty;
					float selwidth = selendx - selstartx;

					vector<pair<pair<int,int>, int> > wallschanged;
					if(SDL_GetModState() & KMOD_ALT)
					{
						if(Graphics.world.cubes[y][x].tileaside == -1)
							break;

						int yy = y;
						while(Graphics.world.cubes[yy][x].tileaside != -1)
							yy++;
						int ymax = yy;
						yy = y;
						while(Graphics.world.cubes[yy][x].tileaside != -1)
							yy--;
						int ymin = yy+1;

					
						float vmin = 99999, vmax = -99999;
						for(yy = ymin; yy < ymax; yy++)
						{
							vmin = min(vmin, Graphics.world.cubes[yy][x].cell4/32.0f);
							vmax = max(vmax, Graphics.world.cubes[yy][x].cell4/32.0f);
							vmin = min(vmin, Graphics.world.cubes[yy][x].cell2/32.0f);
							vmax = max(vmax, Graphics.world.cubes[yy][x].cell2/32.0f);
							vmin = min(vmin, Graphics.world.cubes[yy][x+1].cell3/32.0f);
							vmax = max(vmax, Graphics.world.cubes[yy][x+1].cell3/32.0f);
							vmin = min(vmin, Graphics.world.cubes[yy][x+1].cell1/32.0f);
							vmax = max(vmax, Graphics.world.cubes[yy][x+1].cell1/32.0f);
						}
						if (!(Graphics.wallheightmin == cVector2(-1,-1)))
						{
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell4/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell2/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell3/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell1/32.0f);
						}
						if (!(Graphics.wallheightmax == cVector2(-1,-1)))
						{
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell4/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell2/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell3/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell1/32.0f);
						}



						for(yy = ymin; yy < ymax; yy++)
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
							t.lightmap = 0;
							if(wrap)
							{

								int gridsize = (int)(Graphics.gridsize * 4);
								int gridoff = (int)(Graphics.gridoffsetx * gridsize);
								t.u1 = selstartx + ((yy+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);
								t.u2 = selstartx + ((yy+gridoff)%gridsize) *  ((float)selwidth/gridsize);
								t.u3 = selstartx + ((yy+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);
								t.u4 = selstartx + ((yy+gridoff)%gridsize) *  ((float)selwidth/gridsize);


								if (Graphics.world.cubes[yy][x].cell4 > Graphics.world.cubes[yy][x+1].cell3)
								{
									t.v3 = selstarty+(Graphics.world.cubes[yy][x+1].cell3/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v4 = selstarty+(Graphics.world.cubes[yy][x+1].cell1/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v1 = selendy+(Graphics.world.cubes[yy][x].cell4/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v2 = selendy+(Graphics.world.cubes[yy][x].cell2/32.0f-vmax)/(vmax-vmin)*selheight;
								}
								else
								{
									t.v1 = selstarty + (Graphics.world.cubes[yy][x].cell4/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v2 = selstarty + (Graphics.world.cubes[yy][x].cell2/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v3 = selendy + (Graphics.world.cubes[yy][x+1].cell3/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v4 = selendy + (Graphics.world.cubes[yy][x+1].cell1/32.0f-vmax)/(vmax-vmin)*selheight;
								}
							}
							else
							{
								int index = yy-ymin;
								t.v1 = selstarty;
								t.v2 = selstarty;
								t.v3 = selendy;
								t.v4 = selendy;

								t.u1 = selstartx + ((index+1)/(float)(ymax-ymin)) * (selendx-selstartx);
								t.u2 = selstartx + (index/(float)(ymax-ymin)) * (selendx-selstartx);
								t.u3 = selstartx + ((index+1)/(float)(ymax-ymin)) * (selendx-selstartx);
								t.u4 = selstartx + (index/(float)(ymax-ymin)) * (selendx-selstartx);
							}
							Graphics.world.tiles.push_back(t);
							wallschanged.push_back(pair<pair<int,int>,int>(pair<int,int>(x,yy),Graphics.world.cubes[yy][x].tileaside));
							Graphics.world.cubes[yy][x].tileaside = Graphics.world.tiles.size()-1;
						}
						if(wallschanged.size() > 0)
							undostack.push(new cUndoChangeWalls(1, wallschanged));
					}
					else
					{
						if(Graphics.world.cubes[y][x].tileside == -1)
							break;

						int xx = x;
						while(Graphics.world.cubes[y][xx].tileside != -1)
							xx++;
						int xmax = xx;
						xx = x;
						while(Graphics.world.cubes[y][xx].tileside != -1)
							xx--;
						int xmin = xx+1;
						int xdiff = 4;

						float vmin = 99999, vmax = -99999;
						for(xx = xmin; xx < xmax; xx++)
						{
							vmin = min(vmin, Graphics.world.cubes[y][xx].cell3/32.0f);
							vmax = max(vmax, Graphics.world.cubes[y][xx].cell3/32.0f);
							vmin = min(vmin, Graphics.world.cubes[y][xx].cell4/32.0f);
							vmax = max(vmax, Graphics.world.cubes[y][xx].cell4/32.0f);
							vmin = min(vmin, Graphics.world.cubes[y+1][xx].cell1/32.0f);
							vmax = max(vmax, Graphics.world.cubes[y+1][xx].cell1/32.0f);
							vmin = min(vmin, Graphics.world.cubes[y+1][xx].cell2/32.0f);
							vmax = max(vmax, Graphics.world.cubes[y+1][xx].cell2/32.0f);
						}
						if (!(Graphics.wallheightmin == cVector2(-1,-1)))
						{
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell4/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell2/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell3/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell1/32.0f);
						}
						if (!(Graphics.wallheightmax == cVector2(-1,-1)))
						{
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell4/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell2/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell3/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell1/32.0f);
						}


						for(xx = xmin; xx < xmax; xx++)
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
							t.lightmap = 0;


							if (wrap)
							{
								int gridsize = (int)(Graphics.gridsize * 4);
								int gridoff = (int)(Graphics.gridoffsetx * gridsize);

								t.u1 = selstartx + ((xx+gridoff)%gridsize) *  ((float)selwidth/gridsize);
								t.u2 = selstartx + ((xx+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);
								t.u3 = selstartx + ((xx+gridoff)%gridsize) *  ((float)selwidth/gridsize);
								t.u4 = selstartx + ((xx+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);


								if (Graphics.world.cubes[y][xx].cell3 > Graphics.world.cubes[y+1][xx].cell1)
								{
									t.v1 = selendy + (Graphics.world.cubes[y][xx].cell3/32.0f-vmax)/(vmax-vmin)*selheight;
							 		t.v2 = selendy + (Graphics.world.cubes[y][xx].cell4/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v3 = selstarty + (Graphics.world.cubes[y+1][xx].cell1/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v4 = selstarty + (Graphics.world.cubes[y+1][xx].cell2/32.0f-vmin)/(vmax-vmin)*selheight;
								}
								else
								{
									t.v1 = selstarty + (Graphics.world.cubes[y][xx].cell3/32.0f-vmin)/(vmax-vmin)*selheight;
							 		t.v2 = selstarty + (Graphics.world.cubes[y][xx].cell4/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v3 = selendy + (Graphics.world.cubes[y+1][xx].cell1/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v4 = selendy + (Graphics.world.cubes[y+1][xx].cell2/32.0f-vmax)/(vmax-vmin)*selheight;
								}


							}
							else
							{
								int index = xx-xmin;
								t.v1 = selstarty;
								t.v2 = selstarty;
								t.v3 = selendy;
								t.v4 = selendy;

								t.u1 = selstartx + (index/(float)(xmax-xmin)) * (selendx-selstartx);
								t.u2 = selstartx + ((index+1)/(float)(xmax-xmin)) * (selendx-selstartx);
								t.u3 = selstartx + (index/(float)(xmax-xmin)) * (selendx-selstartx);
								t.u4 = selstartx + ((index+1)/(float)(xmax-xmin)) * (selendx-selstartx);
							}

							Graphics.world.tiles.push_back(t);
							wallschanged.push_back(pair<pair<int,int>,int>(pair<int,int>(xx,y),Graphics.world.cubes[y][xx].tileside));
							Graphics.world.cubes[y][xx].tileside = Graphics.world.tiles.size()-1;
						}
						if(wallschanged.size() > 0)
							undostack.push(new cUndoChangeWalls(0, wallschanged));
					}
					break;
				}
			case SDLK_c:
				{
					if (editmode == MODE_TEXTURE)
					{
						if(startmousex < Graphics.w()-256)
						{
							float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
							float selsizey = (fabs(Graphics.selectionstart.y - Graphics.selectionend.y) / 32);

							selsizex = floor(selsizex*Graphics.brushsize);
							selsizey = floor(selsizey*Graphics.brushsize);

							int posx = (int)mouse3dx / 10;
							int posy = (int)mouse3dz / 10;

							clipboardtexture.clear();
							clipboardgat.clear();

							for(int x = posx; x > posx-selsizex; x--)
							{
								vector<cTile> row;
								vector<int> row2;
								for(int y = posy; y > posy-selsizey; y--)
								{
									if(x >= 0 && x < Graphics.world.width && y >= 0 && y < Graphics.world.height)
									{
										if(Graphics.world.cubes[y][x].tileup != -1)
										{
											row.push_back(Graphics.world.tiles[Graphics.world.cubes[y][x].tileup]);
											row2.push_back(1);
										}
										else
										{
											row.push_back(cTile());
											row2.push_back(2);
										}
									}
									else
									{
										row.push_back(cTile());
										row2.push_back(0);
									}

								}
								clipboardtexture.push_back(row);
								clipboardgat.push_back(row2);
							}						
						}
					}
					if (editmode == MODE_HEIGHTDETAIL)
					{
						int posx = (int)mouse3dx / 10;
						int posy = (int)mouse3dz / 10;

						if (posx >= (int)floor(brushsize/2.0f) && posx <= Graphics.world.width-(int)ceil(brushsize/2.0f) && posy >= (int)floor(brushsize/2.0f) && posy<= Graphics.world.height-(int)ceil(brushsize/2.0f))
						{
							clipboard.clear();
							for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
							{
								vector<vector<float> > row;
								for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
								{
									vector<float> c;
									c.push_back(Graphics.world.cubes[y][x].cell1);
									c.push_back(Graphics.world.cubes[y][x].cell2);
									c.push_back(Graphics.world.cubes[y][x].cell3);
									c.push_back(Graphics.world.cubes[y][x].cell4);
									row.push_back(c);
								}
								clipboard.push_back(row);
							}
						}
					}
					else if (editmode == MODE_OBJECTS)
					{
						if (Graphics.selectedobject != -1)
						{
							Graphics.clipboardrot = Graphics.world.models[Graphics.selectedobject]->rot;
							Graphics.clipboardscale = Graphics.world.models[Graphics.selectedobject]->scale;
							Graphics.clipboardfile = Graphics.world.models[Graphics.selectedobject]->filename;
							Graphics.clipboardy = Graphics.world.models[Graphics.selectedobject]->pos.y;
							currentobject = models->finddata("data\\model\\" + Graphics.world.models[Graphics.selectedobject]->rofilename);
							if(currentobject != NULL)
								MenuCommand_model((cMenuItem*)currentobject);
						}
					}
					if (editmode == MODE_GAT)
					{
						int posx = (int)mouse3dx / 5;
						int posy = (int)mouse3dz / 5;

						int f = (int)ceil(Graphics.brushsize);

						if (posx >= (int)floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= f && posy< 2*Graphics.world.height-f)
						{
							clipboard.clear();
							for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
							{
								vector<vector<float> > row;
								vector<int > row2;
								for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
								{
									vector<float> c;
									c.push_back(Graphics.world.gattiles[y][x].cell1);
									c.push_back(Graphics.world.gattiles[y][x].cell2);
									c.push_back(Graphics.world.gattiles[y][x].cell3);
									c.push_back(Graphics.world.gattiles[y][x].cell4);
									row.push_back(c);
									row2.push_back(Graphics.world.gattiles[y][x].type);
								}
								clipboard.push_back(row);
								clipboardgat.push_back(row2);
							}
						}
					}
					break;
				}
			case SDLK_p:
				{
					if (editmode == MODE_TEXTURE)
					{
						if(startmousex < Graphics.w()-256)
						{
							float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
							float selsizey = (fabs(Graphics.selectionstart.y - Graphics.selectionend.y) / 32);

							selsizex = floor(selsizex*Graphics.brushsize);
							selsizey = floor(selsizey*Graphics.brushsize);

							int posx = (int)mouse3dx / 10;
							int posy = (int)mouse3dz / 10;

							undostack.push(new cUndoTexture(posx-(int)selsizex+1, posy-(int)selsizey+1, posx+1, posy+1));

							for(int x = posx; x > posx-selsizex; x--)
							{
								for(int y = posy; y > posy-selsizey; y--)
								{
									int xx = posx - x;
									int yy = posy - y;
									if(clipboardgat[xx][yy] == 2)
									{
										Graphics.world.cubes[y][x].tileup = -1;
									}
									if(clipboardgat[xx][yy] == 1)
									{
										Graphics.world.tiles.push_back(clipboardtexture[xx][yy]);
										Graphics.world.cubes[y][x].tileup = Graphics.world.tiles.size()-1;
									}
								}
							}						
						}
					}
					if (editmode == MODE_HEIGHTDETAIL)
					{
						int posx = (int)mouse3dx / 10;
						int posy = (int)mouse3dz / 10;
						if (clipboard.size() != brushsize)
							break;

						undostack.push(new cUndoHeightEdit(posx-(int)floor(brushsize/2.0f), posy-(int)floor(brushsize/2.0f), posx+(int)ceil(brushsize/2.0f), posy+(int)ceil(brushsize/2.0f)));
//						if (posx >= (int)floor(brushsize/2.0f) && posx <= Graphics.world.width-(int)ceil(brushsize/2.0f) && posy >= (int)floor(brushsize/2.0f) && posy <= Graphics.world.height-(int)ceil(brushsize/2.0f))
						{
							int yy = 0;
							for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
							{
								vector<vector<float> > row;
								int xx = 0;
								for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
								{
									if (x >= Graphics.world.width || x < 0 || y < 0 || y >= Graphics.world.height)
										continue;
									Graphics.world.cubes[y][x].cell1 = clipboard[yy][xx][0];
									Graphics.world.cubes[y][x].cell2 = clipboard[yy][xx][1];
									Graphics.world.cubes[y][x].cell3 = clipboard[yy][xx][2];
									Graphics.world.cubes[y][x].cell4 = clipboard[yy][xx][3];
									Graphics.world.cubes[y][x].calcnormal();
									xx++;
								}
								yy++;
							}
						}
					}
					else if (editmode == MODE_OBJECTS && Graphics.clipboardfile != "")
					{
						if (SDL_GetModState() & KMOD_CTRL)
						{
							Graphics.world.models[Graphics.selectedobject]->pos.y = Graphics.clipboardy;
						}
						else
						{
							cRSMModel* model = new cRSMModel();
							model->load(Graphics.clipboardfile);
							model->pos = cVector3(mouse3dx/5, -mouse3dy, mouse3dz/5);
							if (SDL_GetModState() & KMOD_SHIFT)
								model->pos.y = Graphics.clipboardy;
							model->scale = Graphics.clipboardscale;
							model->rot = Graphics.clipboardrot;
							Graphics.world.models.push_back(model);
							Graphics.selectedobject = Graphics.world.models.size()-1;
							undostack.push(new cUndoNewObject());
						}
					}
					if (editmode == MODE_GAT)
					{
						int posx = (int)mouse3dx / 5;
						int posy = (int)mouse3dz / 5;

						int f = (int)ceil(Graphics.brushsize);

						if (clipboard.size() != f)
							break;

						undostack.push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
						undostack.push(new cUndoGatTileEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));

						if (posx >= (int)floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= f && posy< 2*Graphics.world.height-f)
						{
							int yy = 0;
							for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
							{
								int xx = 0;
								for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
								{
									Graphics.world.gattiles[y][x].cell1 = clipboard[yy][xx][0];
									Graphics.world.gattiles[y][x].cell2 = clipboard[yy][xx][1];
									Graphics.world.gattiles[y][x].cell3 = clipboard[yy][xx][2];
									Graphics.world.gattiles[y][x].cell4 = clipboard[yy][xx][3];
									Graphics.world.gattiles[y][x].type = clipboardgat[yy][xx];
									xx++;
								}
								yy++;
							}
						}
					}
					break;
				}
			case SDLK_o:
			{
				MenuCommand_showobjects((cMenuItem*)showobjects);
				break;
			}
			case SDLK_BACKSPACE:
			{
				if (editmode == MODE_TEXTURE)
				{
					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;
					float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
					float selsizey = (fabs((int)Graphics.selectionstart.y - Graphics.selectionend.y) / 32);
					selsizex = (int)floor(selsizex*Graphics.brushsize);
					selsizey = (int)floor(selsizey*Graphics.brushsize);

					undostack.push(new cUndoTexture(posx-(int)selsizex+1, posy-(int)selsizey+1, posx+1, posy+1));
					for(int x = posx; x > posx-selsizex; x--)
					{
						for(int y = posy; y > posy-selsizey; y--)
						{
							int xx = posx - x;
							int yy = posy - y;
							if (y < 0 || yy >= Graphics.world.height)
								continue;
							if (x < 0 || xx >= Graphics.world.width)
								continue;
							Graphics.world.cubes[y][x].tileup = -1;
						}
					}
				}
				if (editmode == MODE_OBJECTS)
				{
					if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
					{
						undostack.push(new cUndoObjectDelete(Graphics.selectedobject));
						delete Graphics.world.models[Graphics.selectedobject];
						Graphics.world.models.erase(Graphics.world.models.begin() + Graphics.selectedobject);
						Graphics.selectedobject = -1;
					}
				}
				if (editmode == MODE_EFFECTS)
				{
					if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.effects.size())
					{
						undostack.push(new cUndoEffectDelete(Graphics.selectedobject));
						Graphics.world.effects.erase(Graphics.world.effects.begin() + Graphics.selectedobject);
						Graphics.selectedobject = -1;
					}
				}
				if (editmode == MODE_LIGHTS)
				{
					if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.lights.size())
					{
						undostack.push(new cUndoLightDelete(Graphics.selectedobject));
						Graphics.world.lights.erase(Graphics.world.lights.begin() + Graphics.selectedobject);
						Graphics.selectedobject = -1;
					}
				}
				if (editmode == MODE_OBJECTGROUP)
				{
					vector<cUndoObjectsDelete::cObject> objectsdeleted;
					int idoff = 0;
					for(int i = 0; i < Graphics.world.models.size(); i++)
					{
						if (Graphics.world.models[i]->selected)
						{
							cUndoObjectsDelete::cObject object;
							object.filename = Graphics.world.models[i]->filename;
							object.pos = Graphics.world.models[i]->pos;
							object.rot = Graphics.world.models[i]->rot;
							object.scale = Graphics.world.models[i]->scale;
							object.id = i-idoff;
							objectsdeleted.push_back(object);
							delete Graphics.world.models[i];
							Graphics.world.models.erase(Graphics.world.models.begin() + i);
							i--;
							idoff--;
						}
					}
					Graphics.selectedobject = -1;
					if (objectsdeleted.size() > 0)
						undostack.push(new cUndoObjectsDelete(objectsdeleted));
				}

				break;
			}
			case SDLK_f:
			{
				if (editmode == MODE_HEIGHTDETAIL)
				{
					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;

					undostack.push(new cUndoHeightEdit(posx-(int)floor(brushsize/2.0f), posy-(int)floor(brushsize/2.0f), posx+(int)ceil(brushsize/2.0f), posy+(int)ceil(brushsize/2.0f)));
					for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f)-1; x++)
					{
						for(int y = posy-(int)floor(brushsize/2.0f)+1; y < posy+(int)ceil(brushsize/2.0f); y++)
						{
							if (x >= 0 && x < Graphics.world.width-(int)ceil(brushsize/2.0f) && y > 0 && y <= Graphics.world.height-(int)ceil(brushsize/2.0f))
							{
								float to = Graphics.world.cubes[y][x].cell2;
								Graphics.world.cubes[y][x].cell2 = to;
								Graphics.world.cubes[y][x+1].cell1 = to;
								Graphics.world.cubes[y-1][x+1].cell3 = to;
								Graphics.world.cubes[y-1][x].cell4 = to;

								Graphics.world.cubes[y][x].calcnormal();
								Graphics.world.cubes[y][x+1].calcnormal();
								Graphics.world.cubes[y-1][x+1].calcnormal();
								Graphics.world.cubes[y-1][x].calcnormal();
							}
						}
					}

				}
				else if (editmode == MODE_GAT)
				{
					int posx = (int)mouse3dx / 5;
					int posy = (int)mouse3dz / 5;

					float f = (int)ceil(Graphics.brushsize);


					undostack.push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
					for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
					{
						for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
						{
							if (x >= 0 && x < Graphics.world.width*2 && y > 0 && y <= Graphics.world.height*2)
							{
								float to = Graphics.world.gattiles[y][x].cell2;
								Graphics.world.gattiles[y][x].cell2 = to;
								Graphics.world.gattiles[y][x+1].cell1 = to;
								Graphics.world.gattiles[y-1][x+1].cell3 = to;
								Graphics.world.gattiles[y-1][x].cell4 = to;
							}
						}
					}

				}
				break;
			}
			case SDLK_i:
				{
					if (editmode == MODE_TEXTURE)
					{
						int posx = (int)mouse3dx / 10;
						int posy = (int)mouse3dz / 10;
						if(posx > -1 && posy > -1 && posx < Graphics.world.width && posy < Graphics.world.height)
						{
							Log(3,0,"Cube (%i,%i): %f,%f,%f,%f", posx, posy, Graphics.world.cubes[posy][posx].cell1, Graphics.world.cubes[posy][posx].cell2, Graphics.world.cubes[posy][posx].cell3, Graphics.world.cubes[posy][posx].cell4);
						}
					}
					else if (editmode == MODE_OBJECTS)
					{
						if (Graphics.selectedobject != -1)
						{
							cRSMModel* m = Graphics.world.models[Graphics.selectedobject];
							Log(3,0,"Objects: %i", Graphics.selectedobject);
							Log(3,0,"Pos: %f,%f,%f", m->pos.x, m->pos.y, m->pos.z);
							Log(3,0,"scale: %f,%f,%f", m->scale.x, m->scale.y, m->scale.z);
							Log(3,0,"rot: %f,%f,%f", m->rot.x, m->rot.y, m->rot.z);
							Log(3,0,"nr of submeshes: %i", m->meshes.size());
						}
					}
					break;
				}
			case SDLK_INSERT:
				{
					Graphics.quadtreeview++;
					if (Graphics.quadtreeview > 5)
						Graphics.quadtreeview = 5;
				}
				break;
			case SDLK_DELETE:
				{
					Graphics.quadtreeview--;
					if (Graphics.quadtreeview < -1)
						Graphics.quadtreeview = -1;
				}
				break;
			case SDLK_r:
				{
					if (Graphics.selectedobject != -1)
					{
						undostack.push(new cUndoChangeObject(Graphics.selectedobject));
						Graphics.world.models[Graphics.selectedobject]->rot = cVector3(0,0,0);
					}
					break;
				}
			case SDLK_F1:
				editmode = MODE_TEXTURE;
				if (Graphics.texturestart >= Graphics.world.textures.size())
					Graphics.texturestart = 0;
				break;
			case SDLK_F2:
				editmode = MODE_HEIGHTGLOBAL;
				if (Graphics.texturestart >= Graphics.world.textures.size())
					Graphics.texturestart = 0;
				break;
			case SDLK_F3:
				editmode = MODE_HEIGHTDETAIL;
				if (Graphics.texturestart >= Graphics.world.textures.size())
					Graphics.texturestart = 0;
				break;
			case SDLK_F4:
				editmode = MODE_WALLS;
				break;
			case SDLK_F5:
				editmode = MODE_OBJECTS;
				if (Graphics.texturestart >= Graphics.world.textures.size())
					Graphics.texturestart = 0;
				break;
			case SDLK_F6:
				editmode = MODE_GAT;
				if (Graphics.texturestart >= 6)
					Graphics.texturestart = 0;
				break;
			case SDLK_F7:
				editmode = MODE_WATER;
				Graphics.texturestart = Graphics.world.water.type;
				break;
			case SDLK_F8:
				editmode = MODE_EFFECTS;
				Graphics.selectedobject = -1;
				break;
			case SDLK_F9:
				editmode = MODE_SOUNDS;
				break;
			case SDLK_F10:
				editmode = MODE_LIGHTS;
				break;
			case SDLK_F11:
				editmode = MODE_OBJECTGROUP;
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
					Graphics.gridoffsetx = (event.key.keysym.sym - SDLK_0) / 10.0f;
				else if(SDL_GetModState() & KMOD_CTRL)
					Graphics.gridoffsety = (event.key.keysym.sym - SDLK_0) / 10.0f;
				else
					Graphics.gridsize = (event.key.keysym.sym - SDLK_0) / 4.0f;
				break;
			case SDLK_0:
				if(SDL_GetModState() & KMOD_SHIFT)
					Graphics.gridoffsetx = 0;
				else if(SDL_GetModState() & KMOD_CTRL)
					Graphics.gridoffsety = 0;
				else
					Graphics.gridsize = 16 / 4.0f;
				break;
			case SDLK_u:
				{
					undostack.undo();
					break;
				}
			case SDLK_t:
				{
					cWindow* w = Graphics.WM.getwindow(WT_TEXTURE);
					if (w == NULL)
					{
						w = new cTextureWindow();
						w->init(&Graphics.WM.texture, &Graphics.WM.font);
						Graphics.WM.addwindow(w);
					}
					else
					{
						w->togglevis();
					}
					break;
				}
			case SDLK_RETURN:
				{
					if (editmode == MODE_OBJECTS)
					{
						if (Graphics.selectedobject != -1)
						{
							cRSMModel* o = Graphics.world.models[Graphics.selectedobject];
							cMenuItem* menuitem = (cMenuItem*)models->finddata("model\\" + o->rofilename);

							cWindow* w = new cObjectWindow();
							w->init(&Graphics.WM.texture, &Graphics.WM.font);
							if (menuitem != NULL)
								w->objects["objectmenu"]->SetText(0,menuitem->data2);
							w->objects["posx"]->SetInt(3,(int)&o->pos.x);
							w->objects["posy"]->SetInt(3,(int)&o->pos.y);
							w->objects["posz"]->SetInt(3,(int)&o->pos.z);
							w->objects["rotx"]->SetInt(3,(int)&o->rot.x);
							w->objects["roty"]->SetInt(3,(int)&o->rot.y);
							w->objects["rotz"]->SetInt(3,(int)&o->rot.z);
							w->objects["scalex"]->SetInt(3,(int)&o->scale.x);
							w->objects["scaley"]->SetInt(3,(int)&o->scale.y);
							w->objects["scalez"]->SetInt(3,(int)&o->scale.z);
							w->objects["objectname"]->SetText(0, o->rofilename);
							((cObjectWindow*)w)->undo = new cUndoChangeObject(Graphics.selectedobject);

							Graphics.WM.addwindow(w);
						}
					}
					else if (editmode == MODE_EFFECTS)
					{
						if (Graphics.selectedobject != -1)
						{
							cEffect* o = &Graphics.world.effects[Graphics.selectedobject];

							cWindow* w = new cEffectWindow();
							w->init(&Graphics.WM.texture, &Graphics.WM.font);
							w->objects["posx"]->SetInt(3,(int)&o->pos.x);
							w->objects["posy"]->SetInt(3,(int)&o->pos.y);
							w->objects["posz"]->SetInt(3,(int)&o->pos.z);
							w->objects["rotx"]->SetInt(3,(int)&o->rotation.x);
							w->objects["roty"]->SetInt(3,(int)&o->rotation.y);
							w->objects["rotz"]->SetInt(3,(int)&o->rotation.z);
							w->objects["scalex"]->SetInt(3,(int)&o->scale.x);
							w->objects["scaley"]->SetInt(3,(int)&o->scale.y);
							w->objects["scalez"]->SetInt(3,(int)&o->scale.z);
							w->objects["looptime"]->SetInt(3,(int)&o->loop);
							w->objects["objectname"]->SetText(0, o->readablename);
							((cEffectWindow*)w)->undo = new cUndoChangeEffect(Graphics.selectedobject);
							Graphics.WM.addwindow(w);
						}
					}
					else if (editmode == MODE_LIGHTS)
					{
						if (Graphics.selectedobject != -1)
						{
							cLight* l = &Graphics.world.lights[Graphics.selectedobject];

							cWindow* w = new cLightWindow();
							w->init(&Graphics.WM.texture, &Graphics.WM.font);
							w->objects["posx"]->SetInt(3,(int)&l->pos.x);
							w->objects["posy"]->SetInt(3,(int)&l->pos.y);
							w->objects["posz"]->SetInt(3,(int)&l->pos.z);
							w->objects["colorr"]->SetInt(3,(int)&l->color.x);
							w->objects["colorg"]->SetInt(3,(int)&l->color.y);
							w->objects["colorb"]->SetInt(3,(int)&l->color.z);
							w->objects["intensity"]->SetInt(3,(int)&l->todo2);
							//((cEffectWindow*)w)->undo = new cUndoChangeEffect(Graphics.selectedobject);
							Graphics.WM.addwindow(w);
						}
					}
					break;
				}
			case SDLK_s:
				{
					if (editmode == MODE_HEIGHTDETAIL)
					{
						int x,y;
						int posx = (int)mouse3dx / 10;
						int posy = (int)mouse3dz / 10;
						undostack.push(new cUndoHeightEdit(posx-(int)floor(brushsize/2.0f), posy-(int)floor(brushsize/2.0f), posx+(int)ceil(brushsize/2.0f), posy+(int)ceil(brushsize/2.0f)));
						for(x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f)-1; x++)
						{
							for(y = posy-(int)floor(brushsize/2.0f)+1; y < posy+(int)ceil(brushsize/2.0f); y++)
							{
								if (x >= 0 && x < Graphics.world.width && y >= 0 && y < Graphics.world.height)
								{
									float to = Graphics.world.cubes[y][x].cell2;
									Graphics.world.cubes[y][x].cell2 = to;
									Graphics.world.cubes[y][x+1].cell1 = to;
									if (y > 0)	
									{
										Graphics.world.cubes[y-1][x+1].cell3 = to;
										Graphics.world.cubes[y-1][x].cell4 = to;
									}
								}
							}
						}
						
						for(x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f)-1; x++)
						{
							for(y = posy-(int)floor(brushsize/2.0f)+1; y < posy+(int)ceil(brushsize/2.0f); y++)
							{
								if (x > 1 && x < Graphics.world.width-1 && y > 1 && y < Graphics.world.height-1)
								{
									float to = (Graphics.world.cubes[y+1][x-1].cell2 + Graphics.world.cubes[y+1][x].cell2 + Graphics.world.cubes[y+1][x+1].cell2 + Graphics.world.cubes[y][x-1].cell2 + Graphics.world.cubes[y][x].cell2 + Graphics.world.cubes[y][x+1].cell2 + Graphics.world.cubes[y-1][x-1].cell2 + Graphics.world.cubes[y-1][x].cell2 + Graphics.world.cubes[y-1][x+1].cell2) / 9.0f;
									Graphics.world.cubes[y][x].cell2 = to;
									Graphics.world.cubes[y][x+1].cell1 = to;
									Graphics.world.cubes[y-1][x+1].cell3 = to;
									Graphics.world.cubes[y-1][x].cell4 = to;
									Graphics.world.cubes[y][x].calcnormal();
									Graphics.world.cubes[y][x+1].calcnormal();
									Graphics.world.cubes[y-1][x+1].calcnormal();
									Graphics.world.cubes[y-1][x].calcnormal();
								}
							}
						}
						

					}
					break;

				}
				break;
			case SDLK_m:
				{
					cWindow* w = Graphics.WM.getwindow(WT_MODELS);
					if (w == NULL)
					{
						w = new cModelsWindow();
						w->init(&Graphics.WM.texture, &Graphics.WM.font);
						Graphics.WM.addwindow(w);
					}
					else
					{
						w->togglevis();
					}
				}

			default:
				break;
		}

	}
	return true;
}



void MakeUndo()
{
	undos.push_back(Graphics);
}

void Undo()
{
	if(undos.size() > 0)
	{
		Graphics = undos.back();
		undos.pop_back();
	}
}



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


MENUCOMMAND(importalpha)
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
	ofn.lpstrFilter = "All\0*.*\0RO ALPHA Maps\0*.rsw\0";
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
		Graphics.world.importalpha();
	}
#else


#endif
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
	message = "Sorry, you can't save-as in linux, since I don't know how to make a save-as box yet";
	showmessage = true;
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
	for(y = 2; y < Graphics.world.height/2-1; y++)
	{
		for(x = 2; x < Graphics.world.width/2-1; x++)
		{
		//	Graphics.world.cubes[2*y][2*x].tileup = 1;

			Graphics.world.cubes[2*y][2*x].cell1 = 500-rand()%1000;
			Graphics.world.cubes[2*y][2*x].cell2 = 500-rand()%1000;
			Graphics.world.cubes[2*y][2*x].cell3 = 500-rand()%1000;
			Graphics.world.cubes[2*y][2*x].cell4 = 500-rand()%1000;

			Graphics.world.cubes[2*y][2*x-1].cell2 =	Graphics.world.cubes[2*y][2*x].cell1;
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
		
		}
	}




	return true;
}


MENUCOMMAND(mode)
{
	string title = src->title;
	for(int i = 0; i < mode->items.size(); i++)
		mode->items[i]->ticked = false;
	src->ticked = true;

	if(title == msgtable[MENU_GLOBALHEIGHTEDIT])
	{
		editmode = MODE_HEIGHTGLOBAL;
		if (Graphics.texturestart >= Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == msgtable[MENU_DETAILTERRAINEDIT])
	{
		editmode = MODE_HEIGHTDETAIL;
		if (Graphics.texturestart >= Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == msgtable[MENU_TEXTUREEDIT])
	{
		editmode = MODE_TEXTURE;
		if (Graphics.texturestart >= Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == msgtable[MENU_WALLEDIT])
	{
		editmode = MODE_WALLS;
	}
	else if (title == msgtable[MENU_OBJECTEDIT])
	{
		editmode = MODE_OBJECTS;
		if (Graphics.texturestart >= Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == msgtable[MENU_GATEDIT])
	{
		editmode = MODE_GAT;
		if (Graphics.texturestart >= 6)
			Graphics.texturestart = 0;
	}
	else if (title == msgtable[MENU_WATEREDIT])
	{
		editmode = MODE_WATER;
		Graphics.texturestart = Graphics.world.water.type;
	}
	else if (title == msgtable[MENU_EFFECTSEDIT])
	{
		editmode = MODE_EFFECTS;
		Graphics.selectedobject = -1;
	}
	else if (title == msgtable[MENU_SOUNDSEDIT])
	{
		editmode = MODE_SOUNDS;
	}
	else if (title == msgtable[MENU_LIGHTSEDIT])
	{
		editmode = MODE_LIGHTS;
	}
	else if (title == msgtable[MENU_OBJECTGROUPEDIT])
	{
		editmode = MODE_OBJECTGROUP;
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
	int i;
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
	for(int i =0 ; i < speed->items.size(); i++)
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

	for(int i = 0; i < Graphics.world.models.size(); i++)
	{
		Log(3,0,msgtable[9], i, Graphics.world.models.size(), (i/(float)Graphics.world.models.size())*100);
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




cVector3 lightpos = cVector3(-20000,20000,-20000);
bool selectonly;

MENUCOMMAND(dolightmaps)
{
	selectonly = true;
	return MenuCommand_dolightmaps2(src);
}
MENUCOMMAND(dolightmapsall)
{
	selectonly = false;
	return MenuCommand_dolightmaps2(src);
}


MENUCOMMAND(dolightmaps2)
{
	int x,y,i;

	map<int, bool, less<int> > used;

	for(x = 0; x < Graphics.world.width; x++)
	{
		for(y = 0; y < Graphics.world.height; y++)
		{
			if(selectonly && !Graphics.world.cubes[y][x].selected)
				continue;
			int tile = Graphics.world.cubes[y][x].tileup;
			if(used.find(tile) != used.end())
			{
				cTile t = Graphics.world.tiles[tile];
				tile = Graphics.world.tiles.size();
				Graphics.world.tiles.push_back(t);
				Graphics.world.cubes[y][x].tileup = tile;
			}
			used[tile] = 1;
			if(tile != -1)
			{
				cLightmap* map = new cLightmap();
				for(int i = 0; i < 256; i++)
					map->buf[i] = i < 64 ? 0 : 0;
				Graphics.world.tiles[tile].lightmap = Graphics.world.lightmaps.size();
				Graphics.world.lightmaps.push_back(map);
			}
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
				if(tile != -1)
				{
					cLightmap* map = new cLightmap();
					for(int i = 0; i < 256; i++)
						map->buf[i] = i < 64 ? 255 : 0;
					Graphics.world.tiles[tile].lightmap = Graphics.world.lightmaps.size();
					Graphics.world.lightmaps.push_back(map);
				}
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
				if(tile != -1)
				{
					cLightmap* map = new cLightmap();
					for(int i = 0; i < 256; i++)
						map->buf[i] = i < 64 ? 255 : 0;
					Graphics.world.tiles[tile].lightmap = Graphics.world.lightmaps.size();
					Graphics.world.lightmaps.push_back(map);
				}
			}
		}
	}

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
	gluPerspective(45.0f,(GLfloat)Graphics.w()/(GLfloat)Graphics.h(),10.0f,10000.0f);
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

	for(y = 0; y < Graphics.world.height; y++)
//	for(y = 40; y < 60; y++)
	{
		if(y == 99)
			Sleep(0);
		for(x = 0; x < Graphics.world.width; x++)
//		for(x = 40; x < 60; x++)
		{
			cCube* c = &Graphics.world.cubes[y][x];
			if(selectonly && !c->selected)
				continue;
			Log(3,0,msgtable[10], (y*Graphics.world.width+x) / (float)(Graphics.world.height * Graphics.world.width)*100); // %f %%
			if(Graphics.world.cubes[y][x].tileup == -1)
				continue;
			Graphics.world.reallightmaps[y][x]->reset();
			BYTE* buf = (BYTE*)Graphics.world.lightmaps[Graphics.world.tiles[Graphics.world.cubes[y][x].tileup].lightmap]->buf;

			for(int yy = 0; yy < 6; yy++)
			{
				for(int xx = 0; xx < 6; xx++)
				{
					cVector3 worldpos = cVector3(	10*x+(10/6.0)*xx, 
													-((Graphics.world.cubes[y][x].cell1+Graphics.world.cubes[y][x].cell2+Graphics.world.cubes[y][x].cell3+Graphics.world.cubes[y][x].cell4)/4),
													10*y+(10/6.0)*yy);
					for(i = 0; i < Graphics.world.lights.size(); i++)
					{
						cLight* l = &Graphics.world.lights[i];
						cVector3 diff = worldpos - cVector3(l->pos.x*5, l->pos.y, l->pos.z*5);
						float bla = diff.Magnitude();
						bool obstructed = false;
						if(l->todo2-bla < 0)
							continue;
						if(buf[yy*8 + xx + 9] == 255)
							continue;

						for(int ii = 0; ii < Graphics.world.models.size() && !obstructed; ii++)
						{
							if(Graphics.world.models[ii]->collides(worldpos, cVector3(l->pos.x*5, l->pos.y, l->pos.z*5)))
								obstructed = true;
						}

						if(!obstructed)
						{
							buf[yy*8 + xx + 9] = min(255, buf[yy*8 + xx + 9] + max(0, (int)(l->todo2 - bla)));

							buf[64 + 3*(yy*8 + xx + 9)+0] = min(255, buf[64 + 3*(yy*8 + xx + 9)+0] + max(0, (int)ceil((l->todo2 - bla)*l->color.x)));
							buf[64 + 3*(yy*8 + xx + 9)+1] = min(255, buf[64 + 3*(yy*8 + xx + 9)+1] + max(0, (int)ceil((l->todo2 - bla)*l->color.y)));
							buf[64 + 3*(yy*8 + xx + 9)+2] = min(255, buf[64 + 3*(yy*8 + xx + 9)+2] + max(0, (int)ceil((l->todo2 - bla)*l->color.z)));
						}
						else
						{
						}
					}
				}
			}
		}
	}



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

	for(x = 1; x < (Graphics.world.width*6)-1; x++)
	{
		for(y = 1; y < (Graphics.world.height*6)-1; y++)
		{

		}
	}

	int lightmap,lightmapleft,lightmaptop,lightmapright,lightmapbottom;
	cLightmap* map;
	cLightmap* mapleft;
	cLightmap* maptop;
	cLightmap* mapright;
	cLightmap* mapbottom;


	for(x = 1; x < Graphics.world.width-1; x++)
	{
		for(y = 1; y < Graphics.world.height-1; y++)
		{
			int tile = Graphics.world.cubes[y][x].tileup;
			int tileleft = Graphics.world.cubes[y][x-1].tileup;
			int tiletop = Graphics.world.cubes[y-1][x].tileup;
			int tileright = Graphics.world.cubes[y][x+1].tileup;
			int tilebottom = Graphics.world.cubes[y+1][x].tileup;
			if (tile != -1)
			{
				if(tile != -1)
					lightmap = Graphics.world.tiles[tile].lightmap;
				if(tileleft != -1)
					lightmapleft = Graphics.world.tiles[tileleft].lightmap;
				if(tiletop != -1)
					lightmaptop = Graphics.world.tiles[tiletop].lightmap;
				if(tileright != -1)
					lightmapright = Graphics.world.tiles[tileright].lightmap;
				if(tilebottom != -1)
					lightmapbottom = Graphics.world.tiles[tilebottom].lightmap;

				if(tile != -1)
					map = Graphics.world.lightmaps[lightmap];
				if(tileleft != -1)
					mapleft = Graphics.world.lightmaps[lightmapleft];
				if(tiletop != -1)
					maptop = Graphics.world.lightmaps[lightmaptop];
				if(tileright != -1)
					mapright = Graphics.world.lightmaps[lightmapright];
				if(tilebottom != -1)
					mapbottom = Graphics.world.lightmaps[lightmapbottom];

				for(i = 0; i < 8; i++)
				{
					if(tileleft != -1)
						mapleft->buf[8*i+7] = map->buf[8*i+1];
					if(tiletop != -1)
						maptop->buf[7*8+i] = map->buf[i+8];
					if(tileright != -1)
						mapright->buf[8*i] = map->buf[8*i+6];
					if(tilebottom != -1)
						mapbottom->buf[i] = map->buf[6*8+i];
				}
			}
				
		}
	}
		

	


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
	int x,y,i;

	map<int, bool, less<int> > used;
	for(x = 0; x < Graphics.world.width; x++)
	{
		for(y = 0; y < Graphics.world.height; y++)
		{
			Graphics.world.reallightmaps[y][x]->reset();
			int tile = Graphics.world.cubes[y][x].tileup;
			if(used.find(tile) != used.end())
			{
				cTile t = Graphics.world.tiles[tile];
				tile = Graphics.world.tiles.size();
				Graphics.world.tiles.push_back(t);
				Graphics.world.cubes[y][x].tileup = tile;
			}
			used[tile] = 1;
			if(tile != -1)
			{
				cLightmap* map = new cLightmap();
				for(int i = 0; i < 256; i++)
					map->buf[i] = i < 64 ? 255 : 0;
				Graphics.world.tiles[tile].lightmap = Graphics.world.lightmaps.size();
				Graphics.world.lightmaps.push_back(map);
			}
//////////////////////////////////
			tile = Graphics.world.cubes[y][x].tileside;
			if(used.find(tile) != used.end() && tile != -1)
			{
				cTile t = Graphics.world.tiles[tile];
				tile = Graphics.world.tiles.size();
				Graphics.world.tiles.push_back(t);
				Graphics.world.cubes[y][x].tileside = tile;
			}
			used[tile] = 1;
			if(tile != -1)
			{
				cLightmap* map = new cLightmap();
				for(int i = 0; i < 256; i++)
					map->buf[i] = i < 64 ? 255 : 0;
				Graphics.world.tiles[tile].lightmap = Graphics.world.lightmaps.size();
				Graphics.world.lightmaps.push_back(map);
			}
////////////////////////////////
			tile = Graphics.world.cubes[y][x].tileaside;
			if(used.find(tile) != used.end() && tile != -1)
			{
				cTile t = Graphics.world.tiles[tile];
				tile = Graphics.world.tiles.size();
				Graphics.world.tiles.push_back(t);
				Graphics.world.cubes[y][x].tileaside = tile;
			}
			used[tile] = 1;
			if(tile != -1)
			{
				cLightmap* map = new cLightmap();
				for(int i = 0; i < 256; i++)
					map->buf[i] = i < 64 ? 255 : 0;
				Graphics.world.tiles[tile].lightmap = Graphics.world.lightmaps.size();
				Graphics.world.lightmaps.push_back(map);
			}
		}
	}


	Graphics.world.loadlightmap();

		int lightmap,lightmapleft,lightmaptop,lightmapright,lightmapbottom;
	cLightmap* map;
	cLightmap* mapleft;
	cLightmap* maptop;
	cLightmap* mapright;
	cLightmap* mapbottom;


	for(x = 1; x < Graphics.world.width-1; x++)
	{
		for(y = 1; y < Graphics.world.height-1; y++)
		{
			int tile = Graphics.world.cubes[y][x].tileup;
			int tileleft = Graphics.world.cubes[y][x-1].tileup;
			int tiletop = Graphics.world.cubes[y-1][x].tileup;
			int tileright = Graphics.world.cubes[y][x+1].tileup;
			int tilebottom = Graphics.world.cubes[y+1][x].tileup;
			if (tile != -1)
			{
				if(tile != -1)
					lightmap = Graphics.world.tiles[tile].lightmap;
				if(tileleft != -1)
					lightmapleft = Graphics.world.tiles[tileleft].lightmap;
				if(tiletop != -1)
					lightmaptop = Graphics.world.tiles[tiletop].lightmap;
				if(tileright != -1)
					lightmapright = Graphics.world.tiles[tileright].lightmap;
				if(tilebottom != -1)
					lightmapbottom = Graphics.world.tiles[tilebottom].lightmap;

				if(tile != -1)
					map = Graphics.world.lightmaps[lightmap];
				if(tileleft != -1)
					mapleft = Graphics.world.lightmaps[lightmapleft];
				if(tiletop != -1)
					maptop = Graphics.world.lightmaps[lightmaptop];
				if(tileright != -1)
					mapright = Graphics.world.lightmaps[lightmapright];
				if(tilebottom != -1)
					mapbottom = Graphics.world.lightmaps[lightmapbottom];

				for(i = 0; i < 8; i++)
				{
					if(tileleft != -1)
						mapleft->buf[8*i+7] = map->buf[8*i+1];
					if(tiletop != -1)
						maptop->buf[7*8+i] = map->buf[i+8];
					if(tileright != -1)
						mapright->buf[8*i] = map->buf[8*i+6];
					if(tilebottom != -1)
						mapbottom->buf[i] = map->buf[6*8+i];
				}
			}
				
		}
	}

	return true;
}


MENUCOMMAND(clearobjects)
{
	int i;
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
		for(int ii = 0; ii < i; ii++)
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
		Graphics.texturepreview = new cTexture();
		Graphics.texturepreview->Load(rodir + "data\\texture\\" + ((cMenuItem*)src)->data);
		return false;
	}
	else
	{
		Graphics.texturepreview->unLoad();
		delete Graphics.texturepreview;
		Graphics.texturepreview = NULL;
		return true;
	}
}
bool mouseouttexture(cMenu* src)
{
	if (Graphics.texturepreview != NULL)
	{
		Graphics.texturepreview->unLoad();
		delete Graphics.texturepreview;
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
	cWindow* w = new cWaterWindow();
	w->init(&Graphics.WM.texture, &Graphics.WM.font);
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
	for(i = 0; i < Graphics.world.tiles.size(); i++)
	{
		used[Graphics.world.tiles[i].texture] = true;
	}
	
	for(i = used.size()-1; i > -1; i--)
	{
		if (!used[i])
		{
			for(int ii = 0; ii < Graphics.world.tiles.size(); ii++)
			{
				if(Graphics.world.tiles[i].texture > i)
					Graphics.world.tiles[i].texture--;
			}
			TextureCache.unload(Graphics.world.textures[i]->texture);
			delete Graphics.world.textures[i]->texture;
			delete Graphics.world.textures[i];
			Graphics.world.textures.erase(Graphics.world.textures.begin() + i);
		}
	}

	return true;
}

MENUCOMMAND(ambientlight)
{
	char buf[100];
	cWindow* w = new cAmbientLightWindow();
	w->init(&Graphics.WM.texture, &Graphics.WM.font);
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
	int i;
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
	cWindow* w = new cKeyBindWindow();
	w->init(&Graphics.WM.texture, &Graphics.WM.font);
	Graphics.WM.addwindow(w);
	return true;
}

