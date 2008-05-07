#define _MAIN_CPP_
#include "tinyxml/tinyxml.h"
TiXmlDocument msgtable;
#include "common.h"
int keymap[SDLK_LAST-SDLK_FIRST];
#include "filesystem.h"
#include <math.h>
#include "main.h"
#include "menu.h"
#include <fstream>
#include <list>
#include "md5.h"
#include <time.h>
#include "wm/lightwindow.h"
#include "wm/texturewindow.h"
#include "wm/modelswindow.h"
#include "undo.h"
#include "menucommands.h"
#include "wm/modeloverviewwindow.h"
#include "wm/lightoverviewwindow.h"
#include "wm/minimapwindow.h"

#include "texturecache.h"
#ifdef WIN32
#include <windows.h>
#endif

#ifdef __MINGW32__
#include <GL/glext.h>											// We use a define from this file: GL_BGRA_EXT
#endif

cGraphics		Graphics;
cFileSystem fs;

bool IsLegal2 = true;
string inputboxresult;
bool IsInsideVPC();
bool IsInsideVMWare();
bool IsLegal = true;

long userid;
void MakeUndo();
void Undo();
int movement;
long dragoffsety, dragoffsetx;

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
extern cMenu* popupmenu;
TiXmlDocument favoritelights;
string skinFile;

unsigned int undosize = 50;
vector<string> texturefiles;
vector<string> objectfiles;

double mouse3dxstart, mouse3dystart, mouse3dzstart;
long mousestartx, mousestarty;
unsigned long keys[SDLK_LAST-SDLK_FIRST];
vector<pair<string, string> > translations;

bool mouseouttexture(cMenu*);
bool mouseovertexture(cMenu*);

cUndoStack undostack;

string rodir;


int brushsize = 1;

cTextureCache TextureCache;

cMenu*	menu;
cMenu* grid;
cMenu* showobjects;
cMenu* transparentobjects;
cMenu* currentobject;
cMenu* snaptofloor;
cMenu* lastmenu = NULL;

int cursorsize = 1;

cMenu* mode;
cMenu* editdetail;
cMenu* speed;
cMenu* models;

map<int, cMenu*, less<int> >	effects;
cMenu* effectsmenu;

vector<vector<vector<float> > > clipboard;
long lasttimer;



unsigned char * getPixelsBGR()
{
  int screenStats[4];
  glGetIntegerv(GL_VIEWPORT, screenStats);
  unsigned char *pixels;
  pixels = new unsigned char[screenStats[2]*screenStats[3]*3];
  glReadPixels(0, 0, screenStats[2], screenStats[3], GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
  return pixels;
}

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
	host = gethostbyname("206.222.12.202");
	if(host==NULL)
	{
		Log(1,0,GetMsg("net/HOSTNOTFOUND"), server.c_str());
		return 0;
	}
	addr.sin_family = host->h_addrtype;
	memcpy((char*) &addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	addr.sin_port = htons(80);
	memset(addr.sin_zero, 0, 8);

	if ((s = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		Log(1,0,GetMsg("net/NOSOCKET"));
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
	header = "";
	filesize = 0;
	string downloadbuffer = "";
	while((rc = recv(s, buffer, DOWNLOADBUFFERSIZE, 0)))
	{
		if (rc <= 0)
			break;

		buf += string(buffer, rc);

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

	static eMode lasteditmode = editmode;
	
	if(editmode != lasteditmode)
	{
		lasteditmode = editmode;
		cWindow* w = Graphics.WM.getwindow(WT_MODELOVERVIEW);
		if(editmode == MODE_OBJECTS)
		{
			if(w == NULL)
				Graphics.WM.addwindow(new cModelOverViewWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin));
			else
			{
				w->userfunc(NULL);
				w->show();
			}
		}
		else if(w != NULL)
			w->close();


		w = Graphics.WM.getwindow(WT_LIGHTOVERVIEW);
		if (editmode == MODE_LIGHTS)
		{
			if(w == NULL)
				Graphics.WM.addwindow(new cLightOverViewWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin));
			else
			{
				w->userfunc(NULL);
				w->show();
			}
		}
		else if(w != NULL)
			w->close();


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



bool translationcomp(pair<string, string> a, pair<string, string> b)
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
	msgtable = fs.getxml("data/" + language + ".txt");

	config = pFile->readline();
	config = config.substr(config.find("=")+1);


	if(IsInsideVMWare() || IsInsideVPC())
	{
		Log(3,0,"You're running browedit in a virtual PC");
		exit(0);
	}

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
		if(filesize > 534600)
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
		Log(3,0,"Checking for new version...");
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
			IsLegal = true;
		}
		else
		{
			ok = false;
			IsLegal = false;
		}
		if (res == updatebuf)
		{
			Log(3,0,"You do not have the latest version, please update");
			Log(3,0,GetMsg("net/VERSIONERROR")); // you do not have the latest version
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
	{
		IsLegal = false;
	}

#else
	if(!ok)
		Log(2,0,"Error: non-valid licence stuff");
#endif

	RegCloseKey( hKey );
#endif //win32
#endif //_nocheck_
	cMenu* mm;


	models = new cMenu();
//	models->parent = NULL;
//	models->title = msgtable[MENU_MODELS]; 
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
					else if(option == "skin")
						skinFile = value;
					else if (option == "model")
					{
						objectfiles.push_back(value);
						cFile* pFile2 = fs.open(value);
						if (pFile2 != NULL)
						{
							Log(3,0,GetMsg("file/LOADING"), value.c_str()); // Loading file
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
					}
					else if (option == "texture")
					{
						texturefiles.push_back(value);
					}
					else if (option == "undostack")
						undosize = atoi(value.c_str());
					else if (option == "bgcolor")
					{
						vector<string> splitted = split(value, ",");
						Graphics.backgroundcolor = cVector3(atoi(splitted[0].c_str())/255.0,atoi(splitted[1].c_str())/255.0,atoi(splitted[2].c_str())/255.0);
					}
					else if (option == "notilecolor")
					{
						vector<string> splitted = split(value, ",");
						Graphics.notilecolor = cVector3(atoi(splitted[0].c_str())/255.0,atoi(splitted[1].c_str())/255.0,atoi(splitted[2].c_str())/255.0);
					}
					else
						Log(2,0,GetMsg("UNKNOWNCONFIG"), option.c_str(), value.c_str()); // unknown config option

				}			

			}
		}
	}
	pFile->close();

	IsLegal2 = IsLegal;

	pFile = fs.open("data/korean2english.txt");
	while(!pFile->eof())
	{
		string a = pFile->readline();
		string b = pFile->readline();
		translations.push_back(pair<string, string>(a,b));
	}
	mergesort<pair<string, string> >(translations, translationcomp);
	pFile->close();


	itemsm.clear();
	levelm.clear();

	models->sort();
	
	favoritelights = fs.getxml("data/lights.txt");

	if (!Graphics.init())
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

	menu = new cMenu();
	menu->title = "root";
	menu->item = false;
	menu->drawstyle = 0;
	menu->opened = true;
	menu->x = 0;
	menu->y = 0;
	menu->w = Graphics.w();
	
	int posx = 0;
	ADDMENU2(file,		menu, GetMsg("menu/file/TITLE"),		posx); // File
	ADDMENU2(rnd,		menu, GetMsg("menu/generate/TITLE"),	posx); // Generate
	ADDMENU2(view,		menu, GetMsg("menu/view/TITLE"),		posx); // view
	ADDMENU2(mode,		menu, GetMsg("menu/editmode/TITLE"),	posx); // edit mode
	ADDMENU2(edit,		menu, GetMsg("menu/edit/TITLE"),		posx); // edit
	//ADDMENU2(models,		menu, msgtable[MENU_MODELS],	posx); // models
//	models->parent = menu;
//	menu->items.push_back(models);
//	models->x = posx;
//	models->w = Graphics.font->textlen(models->title)+10;
//	posx+=models->w;
	ADDMENU2(effectsmenu,menu, GetMsg("menu/effects/TITLE"),	posx); // effects
	ADDMENU2(windows,	menu, GetMsg("menu/windows/TITLE"),	posx); // windows

	ADDMENUITEM(mm,file,GetMsg("menu/file/NEW"),							&MenuCommand_new); //new
	ADDMENUITEM(mm,file,GetMsg("menu/file/OPEN"),							&MenuCommand_open); //open
	ADDMENUITEM(mm,file,GetMsg("menu/file/OPENGRF"),						&MenuCommand_opengrf); //open
	ADDMENUITEM(mm,file,GetMsg("menu/file/SAVE"),							&MenuCommand_save); //save
	ADDMENUITEM(mm,file,GetMsg("menu/file/SAVEAS"),							&MenuCommand_saveAs); //save as
	ADDMENUITEM(mm,file,GetMsg("menu/file/IMPORTARCTURUS"),					&MenuCommand_importalpha); // Import arcturus maps
	ADDMENUITEM(mm,file,GetMsg("menu/file/EXPORTLIGHTMAPS"),				&MenuCommand_savelightmaps); // export lightmaps
	ADDMENUITEM(mm,file,GetMsg("menu/file/IMPORTLIGHTMAPS"),				&MenuCommand_loadlightmaps); // import lightmaps
	ADDMENUITEM(mm,file,GetMsg("menu/file/EXPORTMAPFILES"),					&MenuCommand_exportmapfiles);
	ADDMENUITEM(mm,file,GetMsg("menu/file/REBUILDTEXTUREFILE"),			&MenuCommand_rebuildtexturefile);
	ADDMENUITEM(mm,file,GetMsg("menu/file/REBUILDMODELFILE"),				&MenuCommand_rebuildmodelfile);
	ADDMENUITEM(mm,file,GetMsg("menu/file/EXIT"),							&MenuCommand_exit); // exit
	
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/RANDOM1"),					&MenuCommand_random1); // random1 Hills
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/RANDOM2"),					&MenuCommand_random2); // random2 Valleys 
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/RANDOM3"),					&MenuCommand_random3); // random3 Culverts
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/RANDOM4"),					&MenuCommand_random4); // random4 Mountains 
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/RANDOM5"),					&MenuCommand_random5); // random4 Mountains 
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/MAZESTUFF"),					&MenuCommand_tempfunc); // Maze stuff
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/QUADTREE"),					&MenuCommand_quadtree); // Quadtree
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/CALCULATELIGHTMAPS"),			&MenuCommand_dolightmapsall); // Lightmaps
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/CALCULATELIGHTMAPSLOCAL"),	&MenuCommand_dolightmaps); // Selected lightmaps
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/CALCULATELIGHTMAPSLIGHT"),	&MenuCommand_dolightmapslights); // Selected light
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/CALCULATELIGHTMAPSNOSHADOW"),	&MenuCommand_dolightmapsnoshadow); // 
	ADDMENUITEM(mm,rnd,	GetMsg("menu/generate/CLEARMAP"),					&MenuCommand_clearstuff); // clear map
	ADDMENUITEM(mm,rnd, GetMsg("menu/generate/DUN99"),						&MenuCommand_99dun); // 99 level dungeon
	ADDMENUITEM(mm,rnd, "eAthena Script",						&MenuCommand_eascript);
	ADDMENUITEM(mm,rnd, "NPC stuff",							&MenuCommand_npcscreenies);

	ADDMENUITEM(grid,view,GetMsg("menu/view/GRID"),							&MenuCommand_grid); //grid
	grid->ticked = true;
	ADDMENUITEM(showobjects,view,GetMsg("menu/view/OBJECTS"),				&MenuCommand_showobjects);
	ADDMENUITEMDATAP(transparentobjects,view,GetMsg("menu/view/TRANSPARENTOBJECTS"),	&MenuCommand_toggle, (void*)&Graphics.transparentobjects);
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/BOUNDINGBOXES"),				&MenuCommand_toggle, (void*)&Graphics.showboundingboxes);
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/LIGHTMAPS"),					&MenuCommand_toggle, (void*)&Graphics.showlightmaps);
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/OGLLIGHTING"),				&MenuCommand_toggle, (void*)&Graphics.showoglighting);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/TILECOLORS"),				&MenuCommand_toggle, (void*)&Graphics.showtilecolors);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/SHOWWATER"),					&MenuCommand_toggle, (void*)&Graphics.showwater);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/TOPCAMERA"),					&MenuCommand_toggle, (void*)&Graphics.topcamera);
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/INVISIBLETILES"),			&MenuCommand_toggle, (void*)&Graphics.shownotiles);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/SHOWAMBIENTLIGHTING"),		&MenuCommand_toggle, (void*)&Graphics.showambientlighting);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/WATERANIMATION"),			&MenuCommand_toggle, (void*)&Graphics.animatewater);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/GATTILES"),					&MenuCommand_toggle, (void*)&Graphics.showgat);
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/SHOWDOT"),					&MenuCommand_toggle, (void*)&Graphics.showdot);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,GetMsg("menu/view/SHOWSPRITES"),				&MenuCommand_toggle, (void*)&Graphics.showsprites);
	mm->ticked = true;
	ADDMENUITEMDATAP(mm,view,"Show all light spheres",						&MenuCommand_toggle, (void*)&Graphics.showalllights);


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


	ADDMENU(speed,edit, GetMsg("menu/edit/SPEED"),						480, 100);
	ADDMENUITEM(mm,speed,"5",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"10",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"25",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"50",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"100",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"250",												&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"500",												&MenuCommand_speed);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/SLOPING"),						&MenuCommand_slope);
	ADDMENUITEM(snaptofloor,edit,GetMsg("menu/edit/SNAPOBJECTS"),			&MenuCommand_snaptofloor);
	snaptofloor->ticked = true;

	ADDMENUITEM(mm,edit,GetMsg("menu/edit/FLATTEN"),						&MenuCommand_flatten);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/FILL"),							&MenuCommand_fill);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/FILLAREA"),						&MenuCommand_fillarea);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/GATHEIGHT"),						&MenuCommand_gatheight);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/RESETCOLORS"),					&MenuCommand_fixcolors);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/CLEAROBJECTS"),					&MenuCommand_clearobjects);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/CLEAREFFECTS"),					&MenuCommand_cleareffects);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/CLEARLIGHTS"),					&MenuCommand_clearlights);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/ADDWALLS"),						&MenuCommand_addwalls);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/GATCOLLISION"),					&MenuCommand_gatcollision);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/GATCOLLISION")+string("2"),		&MenuCommand_gatcollision2);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/CLEARLIGHTMAPS"),					&MenuCommand_clearlightmaps);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/CLEANLIGHTMAPS"),					&MenuCommand_cleanuplightmaps);
	ADDMENUITEM(mm,edit,GetMsg("menu/edit/REMOVETEXTURES"),					&MenuCommand_cleantextures);

	ADDMENUITEM(mm,windows,GetMsg("menu/windows/AMBIENTLIGHTING"),			&MenuCommand_ambientlight);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/MODELWINDOW"),				&MenuCommand_modelwindow);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/TEXTURES"),					&MenuCommand_texturewindow);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/PROPERTIES"),				&MenuCommand_properties);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/WATER"),					&MenuCommand_water);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/PREFERENCES"),				&MenuCommand_preferences);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/RSMEDITOR"),				&MenuCommand_rsmedit);
	ADDMENUITEM(mm,windows,GetMsg("menu/windows/FAVLIGHTS"),				&MenuCommand_favlights);



	lastlclick = 0;
	lastrclick = 0;

	Log(3,0,GetMsg("file/LOADING"), "keymap.txt");
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
	Log(3,0,GetMsg("file/DONELOADING"), "keymap.txt");


	Log(3,0,GetMsg("file/LOADING"), "effects.txt");
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
	Log(3,0,GetMsg("file/DONELOADING"), "effects.txt");




	Log(3,0,GetMsg("DONEINIT"));
	Graphics.world.newworld();
	strcpy(Graphics.world.filename, string(rodir + "data\\lighttest").c_str());
	if(argc > 1)
	{
		strcpy(Graphics.world.filename, string(rodir + "data\\" + argv[1]).c_str());
		Graphics.world.load();
	}
#ifndef WIN32
//	Graphics.world.load();
#endif

#ifdef _DEBUG
	if(argc == 1)
		Graphics.world.load();
//	Graphics.world.importalpha();
#endif

	for(i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
		keys[i] = 0;


	if(!IsLegal2)
		Graphics.WM.MessageBox("This version of browedit is not activated. Please post on the access reset topic to get it activated");


	
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

cProcessManagement processManagement;

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
			if(Graphics.WM.ConfirmWindow("Are you sure you want to quit?"))
				running = false;
			break;
		case SDL_KEYUP:
			keys[event.key.keysym.sym-SDLK_FIRST] = 0;
			if (Graphics.WM.onkeyup(event.key.keysym.sym, (event.key.keysym.mod&KMOD_SHIFT) != 0))
				return 0;
#ifdef _DEBUG
			if(keymap[event.key.keysym.sym] == SDLK_ESCAPE)
				running = false;
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
			if(Graphics.WM.onkeydown(event.key.keysym.sym, (event.key.keysym.mod&KMOD_SHIFT) != 0))
				return 0;
			if (strlen(SDL_GetKeyName(event.key.keysym.sym)) == 1 || event.key.keysym.sym == SDLK_SPACE)
			{
				if (event.key.keysym.unicode > 0 && event.key.keysym.unicode < 128)
					if (Graphics.WM.onchar((char)event.key.keysym.unicode, (event.key.keysym.mod&KMOD_SHIFT) != 0))
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
	
		int go = processManagement.main_process_events(event);
		if(go == 0)
		{
			switch(editmode)
			{
			case MODE_TEXTURE:			processManagement.textureedit_process_events(event);		break;
			case MODE_HEIGHTDETAIL:		processManagement.detailheightedit_process_events(event);	break;
			case MODE_HEIGHTGLOBAL:		processManagement.globalheightedit_process_events(event);	break;
			case MODE_WALLS:			processManagement.walledit_process_events(event);			break;
			case MODE_OBJECTS:			processManagement.objectedit_process_events(event);			break;
			case MODE_GAT:				processManagement.gatedit_process_events(event);			break;
			case MODE_WATER:			processManagement.wateredit_process_events(event);			break;
			case MODE_EFFECTS:			processManagement.effectedit_process_events(event);			break;
			case MODE_SOUNDS:			processManagement.soundedit_process_events(event);			break;
			case MODE_LIGHTS:			processManagement.lightedit_process_events(event);			break;
			case MODE_OBJECTGROUP:		processManagement.objectgroupedit_process_events(event);	break;
			case MODE_SPRITE:			processManagement.spriteedit_process_events(event);			break;
			}
		}

		if(event.type == SDL_MOUSEMOTION)
		{
			dragged = true;
			oldmousex = mousex;
			oldmousey = mousey;
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
				return 1;

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

			if(menu->inwindow((int)mousestartx, Graphics.h()-(int)mousestarty) != NULL)
				return 1;

			if (rbuttondown && !lbuttondown)
			{
				if(SDL_GetModState() & KMOD_SHIFT)
				{
					if (SDL_GetModState() & KMOD_CTRL)
					{
						Graphics.cameraangle += (oldmousey - mousey) / 10.0f;
						Graphics.cameraangle = max(min(Graphics.cameraangle, (float)20), (float)-10);
						Graphics.camerarot += (oldmousex - mousex) / 100.0f;
						while(Graphics.camerarot < 0)
							Graphics.camerarot+=2*(float)PI;
						while(Graphics.camerarot > 2*PI)
							Graphics.camerarot-=2*(float)PI;
					}
					else
					{
						Graphics.cameraheight += (oldmousey - mousey) / 2.0f;
						Graphics.cameraheight = max(min(Graphics.cameraheight, (float)15000), (float)-5);
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
				if(mousestartx > Graphics.w()-256)
				{
					Graphics.selectionstart.x = floor(mousestartx / 32)*32;
					Graphics.selectionstart.y = floor(mousestarty / 32)*32;
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
					return 1;
				}
			}
			return 0;
		} 
		case SDL_MOUSEBUTTONDOWN:
			{
			movement = 0;
			mousestartx = mousex = event.motion.x;
			mousestarty = mousey = event.motion.y;

			mouse3dxstart = mouse3dx;
			mouse3dystart = mouse3dy;
			mouse3dzstart = mouse3dz;

			tilex = (int)mouse3dx / 10;
			tiley = (int)mouse3dz / 10;

			if(event.button.button == 4)
			{ // scroll up
				cWindow* w = Graphics.WM.inwindow();
				if(w != NULL)
					w->scrollup();
				else
				{
					Graphics.cameraheight*=1.1f;
					Graphics.cameraheight = max(min(Graphics.cameraheight, (float)15000), (float)-5);
				}
				return 1;
			}

			if(event.button.button == 5)
			{ // scroll down
				cWindow* w = Graphics.WM.inwindow();
				if(w != NULL)
					w->scrolldown();
				else
				{
					Graphics.cameraheight/=1.1f;
					Graphics.cameraheight = max(min(Graphics.cameraheight, (float)15000), (float)-5);
				}
				return 1;
			}


			dragged = false;
			doubleclick = false;
			if (SDL_GetTicks() - lastlclick < 250)
				doubleclick = true;

		
			cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
		
			cMenu* pm = NULL;
			if(popupmenu != NULL)
			{
				pm = popupmenu->inwindow((int)mousex, Graphics.h()-(int)mousey);
				return 1;
			}

			if(doubleclick && m == NULL && pm == NULL && event.button.button == SDL_BUTTON_LEFT)
			{
				return 1;
			}
			if (!dragged && !doubleclick && m == NULL && pm == NULL && event.button.button == SDL_BUTTON_LEFT)
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
						if(mousestartx < draggingwindow->px()+draggingwindow->pw() && mousestartx > draggingwindow->px()+draggingwindow->pw() - DRAGBORDER)
							draggingwindow->startresisingxy();
						if((Graphics.h()-mousestarty) > draggingwindow->py() && (Graphics.h()-mousestarty) < draggingwindow->py() + DRAGBORDER)
							draggingwindow->startresizingyx();
						if(mousestartx > draggingwindow->px() && mousestartx < draggingwindow->px() + DRAGBORDER)
							draggingwindow->startresisingx();
						if((Graphics.h()-mousestarty) < draggingwindow->py()+draggingwindow->ph() && (Graphics.h()-mousestarty) > draggingwindow->py()+draggingwindow->ph() - DRAGBORDER)
							draggingwindow->startresizingy();
						return 1;
					}
					else
					{ // drag this component
						Graphics.WM.click(false);
						draggingobject = w->inobject();
						dragoffsetx = mousex - w->px() - w->inobject()->realx();
						dragoffsety = (Graphics.h()-mousey) - w->py() - w->inobject()->realy();
					}
					return 1;
				}
				else
				{
					Graphics.WM.defocus();
				}
			}			
			
			if(event.button.button == SDL_BUTTON_LEFT)
				lbuttondown = true;
			else // rbutton
				rbuttondown = true;
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
				cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
				cMenu* pm = NULL;
				if(popupmenu != NULL)
					pm = popupmenu->inwindow((int)mousex, Graphics.h()-(int)mousey);
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
				if (movement <= 1 && m == NULL && popupmenu == NULL)
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
				if(pm != NULL)
					pm->unmouseover();
				if(pm == NULL && popupmenu != NULL)
				{
					delete popupmenu;
					popupmenu = NULL;
				}
				if (m == NULL)
				{
					menu->closemenu();
					menu->opened = true;
				}
				if (m != NULL && m->opened)
				{
					m->click((int)mousex, Graphics.h()-(int)mousey);
					return 1;
				}
				else if (pm != NULL && pm->opened)
				{
					pm->click((int)mousex, Graphics.h()-(int)mousey);
					if(!pm->opened)
					{
						delete popupmenu;
						popupmenu = NULL;
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
				cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
				cMenu* pm = NULL;
				if(popupmenu != NULL)
					pm = popupmenu->inwindow((int)mousex, Graphics.h()-(int)mousey);
				menu->unmouseover();
				if(pm != NULL)
					pm->unmouseover();
				if (m == NULL)
				{
					menu->closemenu();
					menu->opened = true;
				}
				if(pm == NULL && popupmenu != NULL)
				{
					delete popupmenu;
					popupmenu = NULL;
					return 1;
				}

				rbuttondown = false;
				doubleclick = false;
				if (movement < 2)
				{
					if(Graphics.WM.inwindow() != NULL)
					{
						Graphics.WM.rightclick();
						return 1;
					}
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

				if(movement < 3 && (editmode == MODE_OBJECTS || editmode == MODE_EFFECTS))
				{
					Graphics.selectedobject = -1;
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
					if (keymap[i] == event.key.keysym.sym+SDLK_FIRST)
					{
						event.key.keysym.sym = (SDLKey)(i+SDLK_FIRST);
						found = true;
						break;
					}
				}
				if(!found)
					return 1;
			}

			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE:
				if (Graphics.previewcolor > 20)
					Graphics.previewcolor = 20;
				break;
			case SDLK_g:
				MenuCommand_grid((cMenuItem*)grid);
				break;
			case SDLK_l:
				MenuCommand_toggle((cMenuItem*)menu->find("Lightmaps"));
				break;
			case SDLK_w:
				if(SDL_GetModState() & KMOD_META)
				{
					MenuCommand_toggle((cMenuItem*)menu->find("Water"));
					return 1;
				}
				break;
			case SDLK_o:
			{
				if((event.key.keysym.mod&KMOD_SHIFT) == 0)
					MenuCommand_showobjects((cMenuItem*)showobjects);
				else
					MenuCommand_toggle((cMenuItem*)transparentobjects);
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
			case SDLK_F1:
				if((event.key.keysym.mod&KMOD_SHIFT) == 0)
				{
					editmode = MODE_TEXTURE;
					if (Graphics.texturestart >= (int)Graphics.world.textures.size())
						Graphics.texturestart = 0;
				}
				else
				{
					editmode = MODE_SPRITE;
				}
				break;
			case SDLK_F2:
				editmode = MODE_HEIGHTGLOBAL;
				if (Graphics.texturestart >= (int)Graphics.world.textures.size())
					Graphics.texturestart = 0;
				break;
			case SDLK_F3:
				editmode = MODE_HEIGHTDETAIL;
				if (Graphics.texturestart >= (int)Graphics.world.textures.size())
					Graphics.texturestart = 0;
				break;
			case SDLK_F4:
				editmode = MODE_WALLS;
				break;
			case SDLK_F5:
				editmode = MODE_OBJECTS;
				if (Graphics.texturestart >= (int)Graphics.world.textures.size())
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
					undostack.undo();
					break;
			case SDLK_t:
				{
					cWindow* w = Graphics.WM.getwindow(WT_TEXTURE);
					if (w == NULL)
						Graphics.WM.addwindow(new cTextureWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin));
					else
						Graphics.WM.togglewindow(WT_TEXTURE);
					break;
				}
				break;
			case SDLK_m:
				{
					cWindow* w = Graphics.WM.getwindow(WT_MODELS);
					if (w == NULL)
						Graphics.WM.addwindow(new cModelsWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin)	);
					else
						Graphics.WM.togglewindow(WT_MODELS);
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
						Graphics.camerarot += 10.0f*((float)PI/180.0f);
					}
					return true;
				}
				break;
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			if(event.key.keysym.sym == SDLK_TAB && event.key.keysym.mod& KMOD_CTRL)
			{
				cWindow* w = Graphics.WM.getwindow(WT_MINIMAP);
				if (w == NULL)
					Graphics.WM.addwindow(new cMiniMapWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin)	);
				else
					Graphics.WM.togglewindow(WT_MINIMAP);
			}

			if(event.key.keysym.sym == SDLK_PRINT || event.key.keysym.sym == SDLK_SYSREQ)
			{
				if((event.key.keysym.mod&KMOD_SHIFT) != 0)
				{
				if (!Graphics.draw(false))
					running = false;
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
						if (string(FileData.cFileName) != "." && string(FileData.cFileName) != ".." && string(FileData.cFileName) != "CVS")	// if this is a real folder, not . or .. or CVS
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
						xto = Graphics.w() - 256;
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
