#define _MAIN_CPP_
#include "common.h"
#include "filesystem.h"
#include <math.h>
#include "main.h"
#include "menu.h"
#include <fstream>
#include <list>
#include "md5.h"
#include <time.h>

#include "texturecache.h"
#ifdef WIN32
#include <windows.h>
#include <SDL/SDL_syswm.h>
#endif

cFileSystem fs;

#define VERTEXDIST 10

string inputboxresult;


long userid;
list<cGraphics> undos;
void MakeUndo();
void Undo();
int movement;

void ChangeGrid();
void UpdateTriangleMenu();
void CleanSurfaces();
int process_events();
bool running = true;
eMode editmode = MODE_TEXTURE;
float paintspeed = 100;
string config;
extern double mouse3dx, mouse3dy, mouse3dz;

bool mouseouttexture(cMenu*);
bool mouseovertexture(cMenu*);

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
MENUCOMMAND(steepness);
MENUCOMMAND(random1);
MENUCOMMAND(shading);
MENUCOMMAND(exportheight);
MENUCOMMAND(textures);
MENUCOMMAND(mode);
MENUCOMMAND(flatten);
MENUCOMMAND(editrandom);
MENUCOMMAND(grid);
MENUCOMMAND(mode_detail);
MENUCOMMAND(speed);
MENUCOMMAND(fill);
MENUCOMMAND(showobjects);
MENUCOMMAND(showoglighting);
MENUCOMMAND(model);
MENUCOMMAND(slope);
MENUCOMMAND(picktexture);
MENUCOMMAND(quadtree);
MENUCOMMAND(boundingboxes);
MENUCOMMAND(gatheight);
MENUCOMMAND(lightmaps);
MENUCOMMAND(tilecolors);
MENUCOMMAND(water);
MENUCOMMAND(dolightmaps);
MENUCOMMAND(fixcolors);
MENUCOMMAND(clearobjects);
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

cMenu*	menu;
cMenu* grid;
cMenu* showobjects;
cMenu* showwater;
cMenu* currentobject;
cMenu* lightmaps;
cMenu* showoglighting;
cMenu* snaptofloor;

int cursorsize = 1;

#define ADDMENUITEM(m, p, t, pr) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawstyle = 1; ((cMenuItem*)m)->proc = pr; p->items.push_back(m);
#define ADDMENUITEMDATA(m, p, t, pr,d) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawstyle = 1; ((cMenuItem*)m)->proc = pr; ((cMenuItem*)m)->data = d; p->items.push_back(m);
#define ADDMENUITEMDATA2(m, p, t, pr,d,d2) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawstyle = 1; ((cMenuItem*)m)->proc = pr; ((cMenuItem*)m)->data = d; ((cMenuItem*)m)->data2 = d2; p->items.push_back(m);
#define ADDMENU(m,p,t,xpos,width) m = new cMenu(); m->parent = p; m->title = t; m->item = false; m->drawstyle = 1; m->y = 20; m->x = xpos; m->w = width; p->items.push_back(m);
cMenu* mode;
cMenu* editdetail;
cMenu* speed;
cMenu* models;

map<int, cMenu*, less<int> >	effects;
cMenu* effectsmenu;

vector<vector<vector<float> > > clipboard;
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
		Log(1,0,"Could not look up host '%s', are you connected to the internet?", server.c_str());
		return 0;
	}
	addr.sin_family = host->h_addrtype;
	memcpy((char*) &addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	addr.sin_port = htons(80);
	memset(addr.sin_zero, 0, 8);

	if ((s = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		Log(1,0,"Cannot create socket, try a reboot");
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


int main(int argc, char *argv[])
{
	int i;
	log_open("log_worldeditor.txt","worldedit",2);
#ifdef WIN32
	char fileBuffer[1024];
	GetModuleFileName(NULL, fileBuffer, 1024);
	WIN32_FIND_DATA FileData;													// thingy for searching through a directory
	HANDLE hSearch;																// thingy for searching through a directory
	bool fFinished = false;														// not finished with looking yet....
	long filesize;
	hSearch = FindFirstFile(fileBuffer, &FileData);						// look for all files
	if (hSearch != INVALID_HANDLE_VALUE)										// if there are results...
	{
		filesize = FileData.nFileSizeLow;
#ifndef _DEBUG
		if(filesize > 100000)
			return 0;
#endif
	}
	else
		return 0;
 	FindClose(hSearch);															// Close the search handle. 

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
			Log(3,0,"You do not have the latest version of browedit");
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

	cMenu* file;
	cMenu* mm;
	cMenu* steepness;
	cMenu* rnd;
	cMenu* view;
	cMenu* edit;
	cMenu* textures;

	menu = new cMenu();
	menu->title = "root";
	menu->item = false;
	menu->drawstyle = 0;
	menu->opened = true;
	menu->x = 0;
	menu->y = 0;
	menu->w = Graphics.w();
	
	
	ADDMENU(file,		menu, "File",				0,100);
	ADDMENU(rnd,		menu, "Generate",			100,100);
	ADDMENU(view,		menu, "View",				200,50);
	ADDMENU(mode,		menu, "Edit Mode",			250,100);
	ADDMENU(edit,		menu, "Edit",				350,100);
	ADDMENU(models,		menu, "Models",				450,100);
	ADDMENU(textures,	menu, "Textures",			550,100);
	ADDMENU(effectsmenu,menu, "Effects",			650,100);
	
	ADDMENU(steepness,	edit, "Steepness...",		480,100);

	ADDMENUITEM(mm,file,"New"	,				&MenuCommand_new);
	ADDMENUITEM(mm,file,"Open",					&MenuCommand_open);
	ADDMENUITEM(mm,file,"Save",					&MenuCommand_save);
	ADDMENUITEM(mm,file,"Save As",				&MenuCommand_saveAs);
	ADDMENUITEM(mm,file,"Import RO Alpha maps",	&MenuCommand_importalpha);
	ADDMENUITEM(mm,file,"Export Lightmaps",	&MenuCommand_savelightmaps);
	ADDMENUITEM(mm,file,"Import Lightmaps",	&MenuCommand_loadlightmaps);
	ADDMENUITEM(mm,file,"Exit",					&MenuCommand_exit);

	ADDMENUITEM(mm,steepness, "0.5", &MenuCommand_steepness);
	ADDMENUITEM(mm,steepness, "1.0", &MenuCommand_steepness);
	ADDMENUITEM(mm,steepness, "1.5", &MenuCommand_steepness);
	ADDMENUITEM(mm,steepness, "2.0", &MenuCommand_steepness);
	ADDMENUITEM(mm,steepness, "4.0", &MenuCommand_steepness);
	ADDMENUITEM(mm,steepness, "8.0", &MenuCommand_steepness);
	
	ADDMENUITEM(mm,rnd, "Random 1", &MenuCommand_random1);
	ADDMENUITEM(mm,rnd, "Maze stuff", &MenuCommand_tempfunc);
	ADDMENUITEM(mm,rnd,"Quadtree",				&MenuCommand_quadtree);
	ADDMENUITEM(mm,rnd,"Calculate Lightmaps",		&MenuCommand_dolightmaps);
	ADDMENUITEM(mm,rnd,"Clear Map",		&MenuCommand_clearstuff);

	ADDMENUITEM(grid,view,"Grid",&MenuCommand_grid);
	grid->ticked = true;
	ADDMENUITEM(showobjects,view,"Objects",&MenuCommand_showobjects);
	ADDMENUITEM(mm,view,"Boundingboxes",&MenuCommand_boundingboxes);
	ADDMENUITEM(lightmaps,view,"Lightmaps",&MenuCommand_lightmaps);
	ADDMENUITEM(showoglighting,view,"Show OGL Lighting", &MenuCommand_showoglighting);
	ADDMENUITEM(mm,view,"Tilecolors",&MenuCommand_tilecolors);
	mm->ticked = true;
	showoglighting->ticked = true;
	ADDMENUITEM(showwater,view,"Water",&MenuCommand_water);
	showwater->ticked = true;

	ADDMENUITEM(mm,mode,"Texture Edit",			&MenuCommand_mode);
	mm->ticked = true;
	ADDMENUITEM(mm,mode,"Global Heightmap Edit",	&MenuCommand_mode);
	ADDMENU(editdetail,mode,"Detail Terrain Edit...",400,100);


	ADDMENUITEM(mm, editdetail, "1", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "2", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "4", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "8", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "16", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "32", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "64", &MenuCommand_mode_detail);

	ADDMENUITEM(mm,mode,"Wall Edit",	&MenuCommand_mode);
	ADDMENUITEM(mm,mode,"Object Edit",	&MenuCommand_mode);
	ADDMENUITEM(mm,mode,"GAT Edit",	&MenuCommand_mode);
	ADDMENUITEM(mm,mode,"Water Edit",	&MenuCommand_mode);
	ADDMENUITEM(mm,mode,"Effects Edit",	&MenuCommand_mode);
	ADDMENUITEM(mm,mode,"Sounds Edit",	&MenuCommand_mode);
	ADDMENUITEM(mm,mode,"Lights Edit",	&MenuCommand_mode);


	ADDMENUITEM(mm,edit,"Flatten map",			&MenuCommand_flatten);
	ADDMENUITEM(mm,edit,"Round Flatten",		&MenuCommand_flatten);
	ADDMENUITEM(mm,edit,"Randomize a bit",		&MenuCommand_editrandom);
	ADDMENUITEM(mm,edit,"Fill",					&MenuCommand_fill);
	ADDMENUITEM(mm,edit,"Sloping",				&MenuCommand_slope);
	ADDMENUITEM(mm,edit,"Set GAT height",		&MenuCommand_gatheight);
	ADDMENU(speed,edit, "Speed", 480, 100);
	ADDMENUITEM(mm,speed,"5",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"10",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"25",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"50",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"100",&MenuCommand_speed);
	mm->ticked = true;
	ADDMENUITEM(mm,speed,"250",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"500",&MenuCommand_speed);
	ADDMENUITEM(mm,edit,"Reset Colors",		&MenuCommand_fixcolors);
	ADDMENUITEM(mm,edit,"Clear Objects",		&MenuCommand_clearobjects);
	ADDMENUITEM(mm,edit,"Add Walls",		&MenuCommand_addwalls);
	ADDMENUITEM(mm,edit,"Set gat collision",		&MenuCommand_gatcollision);
	ADDMENUITEM(mm,edit,"Clear Lightmaps",		&MenuCommand_clearlightmaps);
	ADDMENUITEM(mm,edit,"Clean up Lightmaps",		&MenuCommand_cleanuplightmaps);
	ADDMENUITEM(snaptofloor,edit,"Snap objects to floor",		&MenuCommand_snaptofloor);
	snaptofloor->ticked = true;


	fs.LoadFile("data.dat");
	
	cFile* pFile = fs.open("config.txt");
	if (pFile == NULL)
	{
		Log(1,0,"Error opening configfile");
	}
	config = pFile->readline();
	config = config.substr(config.find("=")+1);

	
	map<string, cMenu*, less<string> > itemsm;
	map<cMenu*, int, less<cMenu*> > levelm;
	map<string, cMenu*, less<string> > itemst;
	map<cMenu*, int, less<cMenu*> > levelt;
	levelm[models] = 0;
	levelt[textures] = 0;

	while(!pFile->eof())
	{
		string line = pFile->readline();
		if (line == "[" + config + "]")
		{
			line = "";
			while(!pFile->eof() && line[0] != '[')
			{
				line = pFile->readline();
				if(line.find("=") != string::npos)
				{
					string option = line.substr(0, line.find("="));
					string value = line.substr(line.find("=")+1);

					if(option == "datadir")
						rodir = value;
					else if(option == "resx")
						Graphics.width = atoi(value.c_str());
					else if(option == "resy")
						Graphics.height = atoi(value.c_str());
					else if(option == "bpp")
						Graphics.bits = atoi(value.c_str());
					else if(option == "fullscreen")
						Graphics.fullscreen = value == "1";
					else if (option == "model")
					{
						cFile* pFile2 = fs.open(value);
						if (pFile2 != NULL)
						{
							Log(3,0,"Loading %s", value.c_str());
							while(!pFile2->eof())
							{
								string line = pFile2->readline();
								string pre = line.substr(0, line.find("|"));
								string filename = line.substr(line.find("|")+1);

								string cat = pre.substr(0, pre.rfind("/"));
								string menuname = pre.substr(pre.rfind("/")+1);

								if (cat != "" && itemsm.find(cat) == itemsm.end())
								{
									cMenu* root = models;
									string catname = cat;
									if(cat.find("/") != string::npos)
									{
										root = itemsm[cat.substr(0, cat.rfind("/"))];
										catname = cat.substr(cat.rfind("/")+1);
									}
									
									cMenu* submenu;
									ADDMENU(submenu,		root, catname + "...",				450 + 100*(levelm[root]+1),100);
									itemsm[cat] = submenu;
									levelm[submenu] = levelm[root] + 1;
								}
								char* f = (char*)filename.c_str();
								if(filename != "")
								{
									ADDMENUITEMDATA2(mm,itemsm[cat],menuname, &MenuCommand_model, filename, pre);
								}
								
							}
							Log(3,0,"done loading %s", value.c_str());
							pFile2->close();
						}
						else
							Log(1,0,"Couldn't open %s", value.c_str());
					}
					else if (option == "texture")
					{
						Log(3,0,"Loading %s", value.c_str());
						cFile* pFile2 = fs.open(value);
						if (pFile2 != NULL)
						{
							while(!pFile2->eof())
							{
								string line = pFile2->readline();
								string pre = line.substr(0, line.find("|"));
								string filename = line.substr(line.find("|")+1);

								string cat = pre.substr(0, pre.rfind("/"));
								string menuname = pre.substr(pre.rfind("/")+1);

								if (cat != "" && itemst.find(cat) == itemst.end())
								{
									cMenu* root = textures;
									string catname = cat;
									if(cat.find("/") != string::npos)
									{
										root = itemst[cat.substr(0, cat.rfind("/"))];
										catname = cat.substr(cat.rfind("/")+1);
									}
									
									cMenu* submenu;
									ADDMENU(submenu,		root, catname + "...",				550 + 100*(levelt[root]+1),100);
									itemst[cat] = submenu;
									levelt[submenu] = levelt[root] + 1;
								}
								char* f = (char*)filename.c_str();
								if(filename != "")
								{
									ADDMENUITEMDATA(mm,itemst[cat],menuname, &MenuCommand_picktexture, filename);
									mm->mouseoverproc = mouseovertexture;
									mm->mouseoutproc = mouseouttexture;
								}
									
							}
							pFile2->close();
							Log(3,0,"done loading %s", value.c_str());
						}
						else
							Log(1,0,"Couldn't open %s", value.c_str());
					}
					else
						Log(2,0,"Unknown option: %s=%s", option.c_str(), value.c_str());

				}			

			}
		}
	}
	pFile->close();

	itemsm.clear();
	levelm.clear();
	itemst.clear();
	levelt.clear();

	models->sort();
	textures->sort();


	lastlclick = 0;
	lastrclick = 0;

	vector<cMenu*> effectssubmenu;

	pFile = fs.open("effects.txt");
	i = 0;
	while(pFile && !pFile->eof())
	{
		string line = pFile->readline();
		if(line.find("|") != string::npos)
		{
			if (effectssubmenu.size() <= floor(i/30))
			{
				effectssubmenu.resize(effectssubmenu.size()+1);
				char buf[100];
				sprintf(buf, "%i - %i", (int) (floor(i/30)*30), (int)((floor(i/30)+1)*30));
				ADDMENU(effectssubmenu.back(),		effectsmenu, buf,				0,100);
			}

			int id = atoi(line.substr(0,line.find("|")).c_str());
			string val = line.substr(line.find("|")+1);

			char buf[255];
			sprintf(buf, "%i. %s", id, val.c_str());

			ADDMENUITEMDATA(mm,effectssubmenu[floor(i/30)],buf, &MenuCommand_effect, line.substr(0,line.find("|")));

			i++;
		
		}
	}

	pFile->close();



	
	if (!Graphics.init())
		return 1;

	Log(3,0,"Done initializing..");
	Graphics.world.newworld();
	strcpy(Graphics.world.filename, string(rodir + "prontera").c_str());
#ifdef _DEBUG
	Graphics.world.load();
//	Graphics.world.importalpha();
#endif
	lasttimer = SDL_GetTicks();
	while( running ) {
		if(lasttimer + paintspeed < SDL_GetTicks())
		{
			if(editmode == MODE_HEIGHTDETAIL && menu->inwindow(mousex, Graphics.h()-mousey) == NULL)
			{
				if (lbuttondown || rbuttondown)
				{
									int posx = mouse3dx / 10;
									int posy = mouse3dz / 10;

									if (posx >= floor(brushsize/2.0f) && posx <= Graphics.world.width-ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy<= Graphics.world.height-ceil(brushsize/2.0f))
									{
										for(int x = posx-floor(brushsize/2.0f); x < posx+ceil(brushsize/2.0f); x++)
										{
											for(int y = posy-floor(brushsize/2.0f); y < posy+ceil(brushsize/2.0f); y++)
											{
												cCube* c = &Graphics.world.cubes[y][x];
												if(lbuttondown && !rbuttondown)
												{
													if (!Graphics.slope || (x > posx-floor(brushsize/2.0f)) && y > posy-floor(brushsize/2.0f))
														c->cell1-=1;
													if (!Graphics.slope || (x < posx+ceil(brushsize/2.0f)-1) && y > posy-floor(brushsize/2.0f))
														c->cell2-=1;
													if (!Graphics.slope || (x > posx-floor(brushsize/2.0f)) && y < posy+ceil(brushsize/2.0f)-1)
														c->cell3-=1;
													if (!Graphics.slope || (x < posx+ceil(brushsize/2.0f)-1) && y < posy+ceil(brushsize/2.0f)-1)
														c->cell4-=1;
												}
												if(lbuttondown && rbuttondown)
												{
													if (!Graphics.slope || (x > posx-floor(brushsize/2.0f)) && y > posy-floor(brushsize/2.0f))
														c->cell1+=1;
													if (!Graphics.slope || (x < posx+ceil(brushsize/2.0f)-1) && y > posy-floor(brushsize/2.0f))
														c->cell2+=1;
													if (!Graphics.slope || (x > posx-floor(brushsize/2.0f)) && y < posy+ceil(brushsize/2.0f)-1)
														c->cell3+=1;
													if (!Graphics.slope || (x < posx+ceil(brushsize/2.0f)-1) && y < posy+ceil(brushsize/2.0f)-1)
														c->cell4+=1;
												}
												c->calcnormal();
											}
										}
									}
					lasttimer = SDL_GetTicks();
				}
			}
		}
			
		process_events( );														// process keypresses
		if (!Graphics.draw())												// Active?  Was There A Quit Received?
			break;
		SDL_GL_SwapBuffers();
		Sleep(1);																// god save the CPU
	}


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
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_F4:
				if(SDL_GetModState() & KMOD_ALT)
					running = false;
				break;
			case SDLK_ESCAPE:
				running = false;
				break;
			default:
				break;
			}
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


float steepness = 3;
void checkheight(int x, int y)
{
}

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
			mousex = event.motion.x;
			mousey = event.motion.y;
			cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
			if(m != NULL)
				break;

			movement++;

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
					cVector2 v = cVector2((oldmousex - mousex),  (oldmousey - mousey)) / 1.0f;
		
					v.rotate(-Graphics.camerarot / PI * 180.0f);

					Graphics.camerapointer = Graphics.camerapointer - v;
					long size = Graphics.world.size;
/*					while (Graphics.camerapointer.x+(size/2) > size)
						Graphics.camerapointer.x -= size;
					while (Graphics.camerapointer.y+(size/2) > size)
						Graphics.camerapointer.y -= size;

					while (Graphics.camerapointer.x+(size/2) < 0)
						Graphics.camerapointer.x += size;
					while (Graphics.camerapointer.y+(size/2) < 0)
						Graphics.camerapointer.y += size;*/

				}
			}
			else if (lbuttondown && !rbuttondown)
			{
				if(startmousex > Graphics.w()-256)
				{
					Graphics.selectionstart.x = floor(startmousex / 32)*32;
					Graphics.selectionstart.y = floor(startmousey / 32)*32;
					Graphics.selectionend.x = ceil(mousex / 32)*32;
					Graphics.selectionend.y = ceil(mousey / 32)*32;
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
				if(editmode == MODE_OBJECTS)
				{
					int minobj = 0;
					float mindist = 999999;
					if(Graphics.objectstartdrag)
					{
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


			}
			dragged = true;
			oldmousex = mousex;
			oldmousey = mousey;
			break;
		} 
		case SDL_MOUSEBUTTONDOWN:
			movement = 0;
			startmousex = mousex = event.motion.x;
			startmousey = mousey = event.motion.y;
			dragged = false;
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				lbuttondown = true;
				cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
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

							int posx = mouse3dx / 10;
							int posy = mouse3dz / 10;

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
									t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
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
						int posx = mouse3dx / 10;
						int posy = mouse3dz / 10;

						if (posx >= floor(brushsize/2.0f) && posx <= Graphics.world.width-ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy<= Graphics.world.height-ceil(brushsize/2.0f))
						{
							glColor4f(1,0,0,1);
							glDisable(GL_TEXTURE_2D);
							glDisable(GL_BLEND);
							for(int x = posx-floor(brushsize/2.0f); x < posx+ceil(brushsize/2.0f); x++)
							{
								for(int y = posy-floor(brushsize/2.0f); y < posy+ceil(brushsize/2.0f); y++)
								{
									cCube* c = &Graphics.world.cubes[y][x];
									if(lbuttondown && !rbuttondown)
									{
										if (!Graphics.slope || (x > posx-floor(brushsize/2.0f)) && y > posy-floor(brushsize/2.0f))
											c->cell1-=1;
										if (!Graphics.slope || (x < posx+ceil(brushsize/2.0f)-1) && y > posy-floor(brushsize/2.0f))
											c->cell2-=1;
										if (!Graphics.slope || (x > posx-floor(brushsize/2.0f)) && y < posy+ceil(brushsize/2.0f)-1)
											c->cell3-=1;
										if (!Graphics.slope || (x < posx+ceil(brushsize/2.0f)-1) && y < posy+ceil(brushsize/2.0f)-1)
											c->cell4-=1;
									}
									if(lbuttondown && rbuttondown)
									{
										if (!Graphics.slope || (x > posx-floor(brushsize/2.0f)) && y > posy-floor(brushsize/2.0f))
											c->cell1+=1;
										if (!Graphics.slope || (x < posx+ceil(brushsize/2.0f)-1) && y > posy-floor(brushsize/2.0f))
											c->cell2+=1;
										if (!Graphics.slope || (x > posx-floor(brushsize/2.0f)) && y < posy+ceil(brushsize/2.0f)-1)
											c->cell3+=1;
										if (!Graphics.slope || (x < posx+ceil(brushsize/2.0f)-1) && y < posy+ceil(brushsize/2.0f)-1)
											c->cell4+=1;
									}
								}
							}
							lasttimer = SDL_GetTicks()+500;
						}
					}
					else if (editmode == MODE_GAT)
					{
						int posx = mouse3dx / 5;
						int posy = mouse3dz / 5;

						int f = ceil(Graphics.brushsize);

					//	if (posx >= floor(f/2.0f) && posx < 2*Graphics.world.width-ceil(f/2.0f) && posy >= floor(f/2.0f) && posy< 2*Graphics.world.height-ceil(f/2.0f))
						{
							glColor4f(1,0,0,1);
							glDisable(GL_TEXTURE_2D);
							glDisable(GL_BLEND);
							for(int x = posx-floor(f/2.0f); x < posx+ceil(f/2.0f); x++)
							{
								for(int y = posy-floor(f/2.0f); y < posy+ceil(f/2.0f); y++)
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
				}
			}
			else // rbutton
			{
				rbuttondown = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				lbuttondown = false;
				bool doubleclick = false;
				long l = SDL_GetTicks();
				if (l - lastlclick < 250)
				{
					Log(3,0,"Doubleclick");
					doubleclick = true;
					lastlclick = SDL_GetTicks();
				}
				else
					lastlclick = SDL_GetTicks();
				lbuttondown = false;
				mousex = event.motion.x;
				mousey = event.motion.y;
				menu->unmouseover();
				cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
				if (m == NULL)
				{
					menu->closemenu();
					menu->opened = true;
				}
				if (m != NULL && m->opened)
				{
					m->click((int)mousex, Graphics.h()-(int)mousey);
				}
				else // no menu
				{
					if(editmode == MODE_OBJECTS && movement < 3)
					{
						if (SDL_GetModState() & KMOD_CTRL && Graphics.previewmodel != NULL)
						{
							cRSMModel* model = new cRSMModel();
							model->load(Graphics.previewmodel->filename);
							model->pos = cVector3(mouse3dx/5, -mouse3dy, mouse3dz/5);
							model->scale = cVector3(1,1,1);
							model->rot = cVector3(0,0,0);
							model->id = Graphics.world.models.size();
							Graphics.world.models.push_back(model);
							Graphics.selectedobject = Graphics.world.models.size()-1;
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
						if (m!=NULL)
							Log(3,0,"Selected effect %s", m->title.c_str());
						Log(3,0,"Looping: %i, ", Graphics.world.effects[Graphics.selectedobject].loop);
					}
					else if(editmode == MODE_WALLS && movement < 3)
					{
						int x = mouse3dx / 10;
						int y = mouse3dz / 10;
						if (y < 0 || y > Graphics.world.height-1)
							break;
						if (x < 0 || x > Graphics.world.width-1)
							break;
						if (Graphics.wallheightmin == cVector2(x,y))
							Graphics.wallheightmin = cVector2(-1,-1);
						else
							Graphics.wallheightmin = cVector2(x,y);
					}

				}
				
			}
			else // right button
			{
				rbuttondown = false;
				bool doubleclick = false;
				long l = SDL_GetTicks();
				if (l - lastrclick < 250)
				{
					Log(3,0,"Doubleclick");
					doubleclick = true;
					lastrclick = 0;
				}
				else
					lastrclick = SDL_GetTicks();
				
				lastrclick = SDL_GetTicks();
				rbuttondown = false;
				mousex = event.motion.x;
				mousey = event.motion.y;

				if(movement < 3 && editmode == MODE_OBJECTS)
				{
					Graphics.selectedobject = -1;
				}
				else if(editmode == MODE_WALLS && movement < 3)
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
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
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				if (editmode == MODE_OBJECTS)
				{
					if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
					{
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
				else if (editmode == MODE_HEIGHTDETAIL)
				{
					brushsize/=2;
				}
				else
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
				if (editmode == MODE_OBJECTS)
				{
					currentobject = currentobject->parent->getprev(currentobject);
					MenuCommand_model((cMenuItem*)currentobject);
				}
				else if (editmode == MODE_WATER)
				{
					Graphics.world.water.type = max(0, Graphics.world.water.type - 1);
				}
				else
				{
					Graphics.texturestart--;
					if (Graphics.texturestart < 0)
						Graphics.texturestart = 0;
				}
				break;
			case SDLK_RIGHTBRACKET:
				if (editmode == MODE_OBJECTS)
				{
					currentobject = currentobject->parent->getnext(currentobject);
					MenuCommand_model((cMenuItem*)currentobject);
				}
				else if (editmode == MODE_WATER)
				{
					Graphics.world.water.type = min(5, Graphics.world.water.type + 1);
				}
				else
				{
					Graphics.texturestart++;
					if (Graphics.texturestart > (editmode == MODE_GAT ? 8 : Graphics.world.textures.size()) - (Graphics.h() / 288))
						Graphics.texturestart--;
				}
				break;
			case SDLK_SPACE:
				if (Graphics.previewcolor > 20)
					Graphics.previewcolor = 20;
				Graphics.texturerot = (Graphics.texturerot + 1) % 4;
				break;
			case SDLK_h:
				if (editmode == MODE_TEXTURE)
					Graphics.fliph = !Graphics.fliph;
				if (editmode == MODE_OBJECTS && Graphics.selectedobject != -1)
				{
					if(SDL_GetModState() & KMOD_ALT)
						Graphics.world.models[Graphics.selectedobject]->scale.y = -	Graphics.world.models[Graphics.selectedobject]->scale.y;
					else
						Graphics.world.models[Graphics.selectedobject]->scale.x = -	Graphics.world.models[Graphics.selectedobject]->scale.x;
				}
				if(editmode == MODE_WALLS)
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
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
							float f;
							f = t->u1;
							t->u1 = t->u2;
							t->u2 = f;

							f = t->u3;
							t->u3 = t->u4;
							t->u4 = f;
						}
					}
				}
				break;
			case SDLK_v:
				if (editmode == MODE_TEXTURE)
					Graphics.flipv = !Graphics.flipv;
				if (editmode == MODE_OBJECTS && Graphics.selectedobject != -1)
					Graphics.world.models[Graphics.selectedobject]->scale.z = -	Graphics.world.models[Graphics.selectedobject]->scale.z;
				if(editmode == MODE_WALLS)
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
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
							float f;
							f = t->v1;
							t->v1 = t->v3;
							t->v3 = f;

							f = t->v2;
							t->v2 = t->v4;
							t->v4 = f;
						}
					}
				}
				break;
			case SDLK_g:
				MenuCommand_grid((cMenuItem*)grid);
				break;
			case SDLK_l:
				MenuCommand_lightmaps((cMenuItem*)lightmaps);
				break;
			case SDLK_d:
				{
					int posx = mouse3dx / 10;
					int posy = mouse3dz / 10;

					if (posx >= brushsize && posx < Graphics.world.width-brushsize && posy >= brushsize && posy< Graphics.world.height-brushsize)
					{
						Log(3,0,"Pos: %i,%i", posx, posy);
						Log(3,0,"Cube: tileup: %i, tileside: %i, tileaside: %i, v1: %f, v2: %f, v3: %f, v4: %f", Graphics.world.cubes[posy][posx].tileup, Graphics.world.cubes[posy][posx].tileside, Graphics.world.cubes[posy][posx].tileaside, Graphics.world.cubes[posy][posx].cell1, Graphics.world.cubes[posy][posx].cell2, Graphics.world.cubes[posy][posx].cell3, Graphics.world.cubes[posy][posx].cell4);
						Log(3,0,"Tileup: texture: %i, v1: (%f,%f), v2: (%f,%f), v3: (%f,%f), v4: (%f,%f)", Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].texture, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u1, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v1, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u2, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v2, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u3, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v3, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u4, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v4);
					}
				}
				break;
			case SDLK_COMMA:
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if (y < 0 || y > Graphics.world.cubes.size()-1)
						break;
					if (x < 0 || x > Graphics.world.cubes[0].size()-1)
						break;

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
					t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
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
					Graphics.world	.cubes[y][x].tileside = Graphics.world.tiles.size()-1;

					break;
				}
			case SDLK_PERIOD:
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if (y < 0 || y > Graphics.world.cubes.size()-1)
						break;
					if (x < 0 || x > Graphics.world.cubes[0].size()-1)
						break;

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
					t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
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
						int x = mouse3dx / 10;
						int y = mouse3dz / 10;
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
								Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v3+=0.03125;
								Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v4+=0.03125;
							}
						}
						else
						{
								Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u1+=0.03125;
								Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u3+=0.03125;
						}
					}
					if (editmode == MODE_OBJECTS)
					{
						if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
						{
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
						int posx = mouse3dx / 5;
						int posy = mouse3dz / 5;

						float f = ceil(Graphics.brushsize);

						if (posx >= floor(f/2.0f) && posx < 2*Graphics.world.width-ceil(f/2.0f) && posy >= floor(f/2.0f) && posy< 2*Graphics.world.height-ceil(f/2.0f))
						{
							glColor4f(1,0,0,1);
							glDisable(GL_TEXTURE_2D);
							glDisable(GL_BLEND);
							for(int x = posx-floor(f/2.0f); x < posx+ceil(f/2.0f); x++)
							{
								for(int y = posy-floor(f/2.0f); y < posy+ceil(f/2.0f); y++)
								{
									cGatTile* c = &Graphics.world.gattiles[y][x];
									if (!Graphics.slope || (x > posx-floor(f/2.0f)) && y > posy-floor(f/2.0f))
										c->cell1+=1;
									if (!Graphics.slope || (x < posx+ceil(f/2.0f)-1) && y > posy-floor(f/2.0f))
										c->cell2+=1;
									if (!Graphics.slope || (x > posx-floor(f/2.0f)) && y < posy+ceil(f/2.0f)-1)
										c->cell3+=1;
									if (!Graphics.slope || (x < posx+ceil(f/2.0f)-1) && y < posy+ceil(f/2.0f)-1)
										c->cell4+=1;
								}
							}
						}
					}
					if (editmode == MODE_WATER)
					{
						Graphics.world.water.height++;
					}
					if (editmode == MODE_HEIGHTGLOBAL)
					{
						for(int x = 0; x < Graphics.world.width; x++)
						{
							for(int y = 0; y < Graphics.world.height; y++)
							{
								Graphics.world.cubes[y][x].cell1 += 10;
								Graphics.world.cubes[y][x].cell2 += 10;
								Graphics.world.cubes[y][x].cell3 += 10;
								Graphics.world.cubes[y][x].cell4 += 10;
							}
						}
						for(int y = 0; y < Graphics.world.gattiles.size(); y++)
						{
							for(int x = 0; x < Graphics.world.gattiles[y].size(); x++)
							{
								Graphics.world.gattiles[y][x].cell1 += 10;
								Graphics.world.gattiles[y][x].cell2 += 10;
								Graphics.world.gattiles[y][x].cell3 += 10;
								Graphics.world.gattiles[y][x].cell4 += 10;
							}
						}
						for(int i = 0; i < Graphics.world.models.size(); i++)
							Graphics.world.models[i]->pos.y+=10;
					}
					break;
				}
			case SDLK_PAGEUP:
				{
					if(editmode == MODE_WALLS)
					{
						int x = mouse3dx / 10;
						int y = mouse3dz / 10;
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
								Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v3-=0.03125;
								Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v4-=0.03125;
							}
						}
						else
						{
							Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u1-=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u3-=0.03125;
						}
					}
					if (editmode == MODE_OBJECTS)
					{
						if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
						{
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
						int posx = mouse3dx / 5;
						int posy = mouse3dz / 5;

						float f = ceil(Graphics.brushsize);

						if (posx >= floor(f/2.0f) && posx < 2*Graphics.world.width-ceil(f/2.0f) && posy >= floor(f/2.0f) && posy< 2*Graphics.world.height-ceil(f/2.0f))
						{
							glColor4f(1,0,0,1);
							glDisable(GL_TEXTURE_2D);
							glDisable(GL_BLEND);
							for(int x = posx-floor(f/2.0f); x < posx+ceil(f/2.0f); x++)
							{
								for(int y = posy-floor(f/2.0f); y < posy+ceil(f/2.0f); y++)
								{
									cGatTile* c = &Graphics.world.gattiles[y][x];
									if (!Graphics.slope || (x > posx-floor(f/2.0f)) && y > posy-floor(f/2.0f))
										c->cell1-=1;
									if (!Graphics.slope || (x < posx+ceil(f/2.0f)-1) && y > posy-floor(f/2.0f))
										c->cell2-=1;
									if (!Graphics.slope || (x > posx-floor(f/2.0f)) && y < posy+ceil(f/2.0f)-1)
										c->cell3-=1;
									if (!Graphics.slope || (x < posx+ceil(f/2.0f)-1) && y < posy+ceil(f/2.0f)-1)
										c->cell4-=1;
								}
							}
						}
					}
					if (editmode == MODE_WATER)
					{
						Graphics.world.water.height--;
					}
					if (editmode == MODE_HEIGHTGLOBAL)
					{
						float avg = 0;
						float mmin = 999999;
						float mmax = -999999;
						for(int x = 0; x < Graphics.world.width; x++)
						{
							for(int y = 0; y < Graphics.world.height; y++)
							{
								Graphics.world.cubes[y][x].cell1 -= 10;
								Graphics.world.cubes[y][x].cell2 -= 10;
								Graphics.world.cubes[y][x].cell3 -= 10;
								Graphics.world.cubes[y][x].cell4 -= 10;
								avg = (avg+Graphics.world.cubes[y][x].cell1+Graphics.world.cubes[y][x].cell2+Graphics.world.cubes[y][x].cell3+Graphics.world.cubes[y][x].cell4)/5.0f;
								mmin = min(min(min(min(mmin,Graphics.world.cubes[y][x].cell1),Graphics.world.cubes[y][x].cell2),Graphics.world.cubes[y][x].cell3),Graphics.world.cubes[y][x].cell4);
								mmax = max(max(max(max(mmax,Graphics.world.cubes[y][x].cell1),Graphics.world.cubes[y][x].cell2),Graphics.world.cubes[y][x].cell3),Graphics.world.cubes[y][x].cell4);
							}
						}
						for(int y = 0; y < Graphics.world.gattiles.size(); y++)
						{
							for(int x = 0; x < Graphics.world.gattiles[y].size(); x++)
							{
								Graphics.world.gattiles[y][x].cell1 -= 10;
								Graphics.world.gattiles[y][x].cell2 -= 10;
								Graphics.world.gattiles[y][x].cell3 -= 10;
								Graphics.world.gattiles[y][x].cell4 -= 10;
							}
						}
						for(int i = 0; i < Graphics.world.models.size(); i++)
							Graphics.world.models[i]->pos.y-=10;
						Log(3,0,"Avg: %f, Min: %f, Max: %f", avg, mmin, mmax);
					}
					break;
				}
			case SDLK_HOME:
				if(editmode == MODE_WALLS)
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
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
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2+=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4+=0.03125;
					}
					break;
				}
			case SDLK_END:
				if(editmode == MODE_WALLS)
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
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
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2+=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4+=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2-=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4-=0.03125;
					}
					break;
				}
			case SDLK_w:
				if(editmode == MODE_WATER)
				{
					char buf[100];
					scanf("%s", buf);
					Graphics.world.water.height = atof(buf);
				}
				else
				{
					if(SDL_GetModState() & KMOD_META)
					{
						MenuCommand_water((cMenuItem*)showwater);
						break;
					}
					bool wrap = true;
					if (SDL_GetModState() & KMOD_SHIFT)
						wrap = false;


					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if (y < 0 || y > Graphics.world.height - 1)
						break;
					if (x < 0 || x > Graphics.world.width - 1)
						break;

					float selstartx = (((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
					float selstarty = (((int)(Graphics.selectionstart.y - 32) % 288) / 32) / 8.0f;
					float selendx = (((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
					float selendy = (((int)(Graphics.selectionend.y - 32) % 288) / 32) / 8.0f;
					float selheight = selendy - selstarty;

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
							vmin = min(vmin, Graphics.world.cubes[Graphics.wallheightmin.y][Graphics.wallheightmin.x].cell4/32.0f);
							vmin = min(vmin, Graphics.world.cubes[Graphics.wallheightmin.y][Graphics.wallheightmin.x].cell2/32.0f);
							vmin = min(vmin, Graphics.world.cubes[Graphics.wallheightmin.y][Graphics.wallheightmin.x].cell3/32.0f);
							vmin = min(vmin, Graphics.world.cubes[Graphics.wallheightmin.y][Graphics.wallheightmin.x].cell1/32.0f);
						}
						if (!(Graphics.wallheightmax == cVector2(-1,-1)))
						{
							vmax = max(vmax, Graphics.world.cubes[Graphics.wallheightmax.y][Graphics.wallheightmax.x].cell4/32.0f);
							vmax = max(vmax, Graphics.world.cubes[Graphics.wallheightmax.y][Graphics.wallheightmax.x].cell2/32.0f);
							vmax = max(vmax, Graphics.world.cubes[Graphics.wallheightmax.y][Graphics.wallheightmax.x].cell3/32.0f);
							vmax = max(vmax, Graphics.world.cubes[Graphics.wallheightmax.y][Graphics.wallheightmax.x].cell1/32.0f);
						}



						for(yy = ymin; yy < ymax; yy++)
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
							t.lightmap = 0;
							if(wrap)
							{
								t.u1 = (yy%4+1) *  (1.0/(float)ydiff);
								t.u2 = (yy%4) *  (1.0/(float)ydiff);
								t.u3 = (yy%4+1) *  (1.0/(float)ydiff);
								t.u4 = (yy%4) *  (1.0/(float)ydiff);


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
							Graphics.world.cubes[yy][x].tileaside = Graphics.world.tiles.size()-1;
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
							vmin = min(vmin, Graphics.world.cubes[Graphics.wallheightmin.y][Graphics.wallheightmin.x].cell4/32.0f);
							vmin = min(vmin, Graphics.world.cubes[Graphics.wallheightmin.y][Graphics.wallheightmin.x].cell2/32.0f);
							vmin = min(vmin, Graphics.world.cubes[Graphics.wallheightmin.y][Graphics.wallheightmin.x].cell3/32.0f);
							vmin = min(vmin, Graphics.world.cubes[Graphics.wallheightmin.y][Graphics.wallheightmin.x].cell1/32.0f);
						}
						if (!(Graphics.wallheightmax == cVector2(-1,-1)))
						{
							vmax = max(vmax, Graphics.world.cubes[Graphics.wallheightmax.y][Graphics.wallheightmax.x].cell4/32.0f);
							vmax = max(vmax, Graphics.world.cubes[Graphics.wallheightmax.y][Graphics.wallheightmax.x].cell2/32.0f);
							vmax = max(vmax, Graphics.world.cubes[Graphics.wallheightmax.y][Graphics.wallheightmax.x].cell3/32.0f);
							vmax = max(vmax, Graphics.world.cubes[Graphics.wallheightmax.y][Graphics.wallheightmax.x].cell1/32.0f);
						}


						for(xx = xmin; xx < xmax; xx++)
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
							t.lightmap = 0;


							if (wrap)
							{
								t.u2 = (xx%4+1) *  (1.0/(float)xdiff);
								t.u1 = (xx%4) *  (1.0/(float)xdiff);
								t.u3 = (xx%4) *  (1.0/(float)xdiff);
								t.u4 = (xx%4+1) *  (1.0/(float)xdiff);


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
							Graphics.world.cubes[y][xx].tileside = Graphics.world.tiles.size()-1;
						}
					}
					break;
				}
			case SDLK_c:
				{
					if (editmode == MODE_HEIGHTDETAIL)
					{
						int posx = mouse3dx / 10;
						int posy = mouse3dz / 10;

						if (posx >= floor(brushsize/2.0f) && posx <= Graphics.world.width-ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy<= Graphics.world.height-ceil(brushsize/2.0f))
						{
							clipboard.clear();
							for(int y = posy-floor(brushsize/2.0f); y < posy+ceil(brushsize/2.0f); y++)
							{
								vector<vector<float> > row;
								for(int x = posx-floor(brushsize/2.0f); x < posx+ceil(brushsize/2.0f); x++)
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

							Log(3,0,"Copied %s", Graphics.clipboardfile.c_str());

							currentobject = models->finddata("model\\" + Graphics.world.models[Graphics.selectedobject]->rofilename);
							if(currentobject != NULL)
								MenuCommand_model((cMenuItem*)currentobject);
						}
					}
					if (editmode == MODE_GAT)
					{
						int posx = mouse3dx / 5;
						int posy = mouse3dz / 5;

						int f = ceil(Graphics.brushsize);

						if (posx >= floor(f/2.0f) && posx < 2*Graphics.world.width-ceil(f/2.0f) && posy >= f && posy< 2*Graphics.world.height-f)
						{
							clipboard.clear();
							for(int y = posy-floor(f/2.0f); y < posy+ceil(f/2.0f); y++)
							{
								vector<vector<float> > row;
								for(int x = posx-floor(f/2.0f); x < posx+ceil(f/2.0f); x++)
								{
									vector<float> c;
									c.push_back(Graphics.world.gattiles[y][x].cell1);
									c.push_back(Graphics.world.gattiles[y][x].cell2);
									c.push_back(Graphics.world.gattiles[y][x].cell3);
									c.push_back(Graphics.world.gattiles[y][x].cell4);
									row.push_back(c);
								}
								clipboard.push_back(row);
							}
						}
					}
					break;
				}
			case SDLK_p:
				{
					if (editmode == MODE_HEIGHTDETAIL)
					{
						int posx = mouse3dx / 10;
						int posy = mouse3dz / 10;
						if (clipboard.size() != brushsize)
							break;

//						if (posx >= floor(brushsize/2.0f) && posx <= Graphics.world.width-ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy <= Graphics.world.height-ceil(brushsize/2.0f))
						{
							int yy = 0;
							for(int y = posy-floor(brushsize/2.0f); y < posy+ceil(brushsize/2.0f); y++)
							{
								vector<vector<float> > row;
								int xx = 0;
								for(int x = posx-floor(brushsize/2.0f); x < posx+ceil(brushsize/2.0f); x++)
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
							model->id = Graphics.world.models.size();
							Graphics.world.models.push_back(model);
							Graphics.selectedobject = Graphics.world.models.size()-1;
						}
					}
					if (editmode == MODE_GAT)
					{
						int posx = mouse3dx / 5;
						int posy = mouse3dz / 5;

						int f = ceil(Graphics.brushsize);

						if (clipboard.size() != f)
							break;

						if (posx >= floor(f/2.0f) && posx < 2*Graphics.world.width-ceil(f/2.0f) && posy >= f && posy< 2*Graphics.world.height-f)
						{
							int yy = 0;
							for(int y = posy-floor(f/2.0f); y < posy+ceil(f/2.0f); y++)
							{
								int xx = 0;
								for(int x = posx-floor(f/2.0f); x < posx+ceil(f/2.0f); x++)
								{
									Graphics.world.gattiles[y][x].cell1 = clipboard[yy][xx][0];
									Graphics.world.gattiles[y][x].cell2 = clipboard[yy][xx][1];
									Graphics.world.gattiles[y][x].cell3 = clipboard[yy][xx][2];
									Graphics.world.gattiles[y][x].cell4 = clipboard[yy][xx][3];
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
				if (editmode == MODE_OBJECTS)
				{
					if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
					{
						delete Graphics.world.models[Graphics.selectedobject];
						Graphics.world.models.erase(Graphics.world.models.begin() + Graphics.selectedobject);
						Graphics.selectedobject = -1;
					}
				}
				break;
			}
			case SDLK_f:
			{
				if (editmode == MODE_HEIGHTDETAIL)
				{
					int posx = mouse3dx / 10;
					int posy = mouse3dz / 10;

					for(int x = posx-floor(brushsize/2.0f); x < posx+ceil(brushsize/2.0f)-1; x++)
					{
						for(int y = posy-floor(brushsize/2.0f)+1; y < posy+ceil(brushsize/2.0f); y++)
						{
							if (x >= 0 && x < Graphics.world.width-ceil(brushsize/2.0f) && y > 0 && y <= Graphics.world.height-ceil(brushsize/2.0f))
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
				break;
			}
			case SDLK_i:
				{
					int posx = mouse3dx / 10;
					int posy = mouse3dz / 10;
					if(posx > -1 && posy > -1 && posx < Graphics.world.width && posy < Graphics.world.height)
					{
						Log(3,0,"Cube (%i,%i): %f,%f,%f,%f", posx, posy, Graphics.world.cubes[posy][posx].cell1, Graphics.world.cubes[posy][posx].cell2, Graphics.world.cubes[posy][posx].cell3, Graphics.world.cubes[posy][posx].cell4);
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
				break;
			case SDLK_F9:
				editmode = MODE_SOUNDS;
				break;
			case SDLK_F10:
				editmode = MODE_LIGHTS;
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
			case SDLK_s:
				{
					if (editmode == MODE_HEIGHTDETAIL)
					{
						int x,y;
						int posx = mouse3dx / 10;
						int posy = mouse3dz / 10;
						for(x = posx-floor(brushsize/2.0f); x < posx+ceil(brushsize/2.0f)-1; x++)
						{
							for(y = posy-floor(brushsize/2.0f)+1; y < posy+ceil(brushsize/2.0f); y++)
							{
								if (x >= 0 && x < Graphics.world.width-ceil(brushsize/2.0f) && y > 0 && y <= Graphics.world.height-ceil(brushsize/2.0f))
								{
									float to = Graphics.world.cubes[y][x].cell2;
									Graphics.world.cubes[y][x].cell2 = to;
									Graphics.world.cubes[y][x+1].cell1 = to;
									Graphics.world.cubes[y-1][x+1].cell3 = to;
									Graphics.world.cubes[y-1][x].cell4 = to;
								}
							}
						}
						
						for(x = posx-floor(brushsize/2.0f); x < posx+ceil(brushsize/2.0f)-1; x++)
						{
							for(y = posy-floor(brushsize/2.0f)+1; y < posy+ceil(brushsize/2.0f); y++)
							{
								if (x > 0 && x < Graphics.world.width-ceil(brushsize/2.0f)-1 && y > 0 && y < Graphics.world.height-ceil(brushsize/2.0f)-1)
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





MENUCOMMAND(lightmapsreset)
{

	return true;
}


MENUCOMMAND(steepness)
{
	steepness = atof(src->title.c_str());
	return true;
}


MENUCOMMAND(random1)
{
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

			Graphics.world.cubes[2*y][2*x].cell1 = -rand()%10;
			Graphics.world.cubes[2*y][2*x].cell2 = -rand()%10;
			Graphics.world.cubes[2*y][2*x].cell3 = -rand()%10;
			Graphics.world.cubes[2*y][2*x].cell4 = -rand()%10;

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


MENUCOMMAND(exportheight)
{
	return true;
}
MENUCOMMAND(mode)
{
	string title = src->title;
	for(int i = 0; i < mode->items.size(); i++)
		mode->items[i]->ticked = false;
	src->ticked = true;

	if(title == "Global Heightmap Edit")
	{
		editmode = MODE_HEIGHTGLOBAL;
		if (Graphics.texturestart >= Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == "Detail Terrain Edit...")
	{
		editmode = MODE_HEIGHTDETAIL;
		if (Graphics.texturestart >= Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == "Texture Edit")
	{
		editmode = MODE_TEXTURE;
		if (Graphics.texturestart >= Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == "Wall Edit")
	{
		editmode = MODE_WALLS;
	}
	else if (title == "Object Edit")
	{
		editmode = MODE_OBJECTS;
		if (Graphics.texturestart >= Graphics.world.textures.size())
			Graphics.texturestart = 0;
	}
	else if (title == "GAT Edit")
	{
		editmode = MODE_GAT;
		if (Graphics.texturestart >= 6)
			Graphics.texturestart = 0;
	}
	else if (title == "Water Edit")
	{
		editmode = MODE_WATER;
		Graphics.texturestart = Graphics.world.water.type;
	}
	else if (title == "Effects Edit")
	{
		editmode = MODE_EFFECTS;
	}
	else if (title == "Sounds Edit")
	{
		editmode = MODE_SOUNDS;
	}
	else if (title == "Lights Edit")
	{
		editmode = MODE_LIGHTS;
	}
	return true;
}
MENUCOMMAND(textures)
{
	Graphics.world.showtextures = !Graphics.world.showtextures;
	src->ticked = !src->ticked;
	return true;
}

MENUCOMMAND(flatten)
{
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

MENUCOMMAND(editrandom)
{
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
	int x,y;


	Graphics.world.tiles.clear();
	Graphics.world.lightmaps.clear();
	cLightmap* map = new cLightmap();
	for(int i = 0; i < 256; i++)
		map->buf[i] = i < 64 ? 255 : 0;
	Graphics.world.lightmaps.push_back(map);

	for(y = 0; y < 4; y++)
	{
		for(x = 0; x < 4; x++)
		{
			cTile t;
			t.lightmap = 0;
			t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
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
	Log(3,0,"Meshcount: %i", Graphics.previewmodel->meshes.size());
	return true;
}


MENUCOMMAND(slope)
{
	src->ticked = !src->ticked;
	Graphics.slope = src->ticked;
	return true;
}


MENUCOMMAND(picktexture)
{
	if(SDL_GetModState() & KMOD_SHIFT)
	{
		int id = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
		TextureCache.unload(Graphics.world.textures[id]->texture);
		delete Graphics.world.textures[id];

		string data = src->data;
		cTextureContainer* t = new cTextureContainer();
		t->RoFilename = src->data;
		char buf[40];
		ZeroMemory(buf, 40);
		sprintf(buf, "%i%i", rand(), rand());
		t->RoFilename2 = string(buf,40);
		t->texture = TextureCache.load(rodir + "texture/" + src->data);
		Graphics.world.textures[id] = t;

	}
	else
	{
		string data = src->data;
		cTextureContainer* t = new cTextureContainer();
		t->RoFilename = src->data;
		char buf[40];
		ZeroMemory(buf, 40);
		sprintf(buf, "%i%i", rand(), rand());
		t->RoFilename2 = string(buf,40);
		t->texture = TextureCache.load(rodir + "texture/" + src->data);
		Graphics.world.textures.push_back(t);
		Graphics.texturestart = Graphics.world.textures.size() - 2;
	}
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
		Log(3,0,"Doing model %i out of %i (%.2f%%)", i, Graphics.world.models.size(), (i/(float)Graphics.world.models.size())*100);
		Graphics.world.models[i]->draw(false,false,true);
	}


	Graphics.world.root->recalculate();
	return true;
}

MENUCOMMAND(boundingboxes)
{
	src->ticked = !src->ticked;
	Graphics.showboundingboxes = src->ticked;
	return true;
}

MENUCOMMAND(gatheight)
{
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


MENUCOMMAND(lightmaps)
{
	src->ticked = !src->ticked;
	Graphics.showlightmaps = src->ticked;
	return true;
}

MENUCOMMAND(tilecolors)
{
	src->ticked = !src->ticked;
	Graphics.showtilecolors = src->ticked;
	return true;
}

MENUCOMMAND(water)
{
	src->ticked = !src->ticked;
	Graphics.showwater = src->ticked;
	return true;
}



cVector3 lightpos = cVector3(-20000,20000,-20000);

MENUCOMMAND(dolightmaps)
{
	int x,y,i;

	map<int, bool, less<int> > used;

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
			if(tile != -1)
			{
				cLightmap* map = new cLightmap();
				for(int i = 0; i < 256; i++)
					map->buf[i] = i < 64 ? 255 : 0;
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



	for(i = 0; i < Graphics.world.models.size(); i++)
	{
		Log(3,0,"Doing model %i out of %i (%.2f%%)", i, Graphics.world.models.size(), (i/(float)Graphics.world.models.size())*100);
		Graphics.world.models[i]->draw(false,false,false, true);
	}

	float t;
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
								if(*lightmappos == 128)
									break;
			//					if (xxx == x && yyy == y)
			//						continue;
								cCube* c = &Graphics.world.cubes[yyy][xxx];
								cVector3 triangle[6];
								triangle[2] = cVector3(xxx*10+10, -c->cell2, yyy*10);
								triangle[1] = cVector3(xxx*10, -c->cell3, yyy*10+10);
								triangle[0] = cVector3(xxx*10+10, -c->cell4, yyy*10+10);

								triangle[5] = cVector3(xxx*10, -c->cell4, yyy*10+10);
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


	for(x = 1; x < Graphics.world.width-1; x++)
	{
		for(y = 1; y < Graphics.world.height-1; y++)
		{
			int tile = Graphics.world.cubes[y][x].tileup;
			int tileleft = Graphics.world.cubes[y][x-1].tileup;
			int tiletop = Graphics.world.cubes[y-1][x].tileup;
			int tileright = Graphics.world.cubes[y][x+1].tileup;
			int tilebottom = Graphics.world.cubes[y+1][x].tileup;
			if (tile != -1 && tileleft != -1 && tiletop != -1)
			{
				int lightmap = Graphics.world.tiles[tile].lightmap;
				int lightmapleft = Graphics.world.tiles[tileleft].lightmap;
				int lightmaptop = Graphics.world.tiles[tiletop].lightmap;
				int lightmapright = Graphics.world.tiles[tileright].lightmap;
				int lightmapbottom = Graphics.world.tiles[tilebottom].lightmap;

				if (lightmap < Graphics.world.lightmaps.size() && lightmapleft < Graphics.world.lightmaps.size() && lightmaptop < Graphics.world.lightmaps.size() &&
					lightmap > -1 && lightmapleft > -1 && lightmaptop > -1)
				{
					cLightmap* map = Graphics.world.lightmaps[lightmap];
					cLightmap* mapleft = Graphics.world.lightmaps[lightmapleft];
					cLightmap* maptop = Graphics.world.lightmaps[lightmaptop];
					cLightmap* mapright = Graphics.world.lightmaps[lightmapright];
					cLightmap* mapbottom = Graphics.world.lightmaps[lightmapbottom];

					for(i = 0; i < 8; i++)
					{
						mapleft->buf[8*i+7] = map->buf[8*i+1];
						maptop->buf[7*8+i] = map->buf[i+8];
						mapright->buf[8*i] = map->buf[8*i+6];
						mapbottom->buf[i] = map->buf[6*8+i];
					}
				}
				else
				{
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
	int x,y;

	map<int, bool, less<int> > used;
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
	for(x = 1; x < Graphics.world.width-1; x++)
	{
		for(y = 1; y < Graphics.world.height-1; y++)
		{
			int tile = Graphics.world.cubes[y][x].tileup;
			int tileleft = Graphics.world.cubes[y][x-1].tileup;
			int tiletop = Graphics.world.cubes[y-1][x].tileup;
			int tileright = Graphics.world.cubes[y][x+1].tileup;
			int tilebottom = Graphics.world.cubes[y+1][x].tileup;
			if (tile != -1 && tileleft != -1 && tiletop != -1)
			{
				int lightmap = Graphics.world.tiles[tile].lightmap;
				int lightmapleft = Graphics.world.tiles[tileleft].lightmap;
				int lightmaptop = Graphics.world.tiles[tiletop].lightmap;
				int lightmapright = Graphics.world.tiles[tileright].lightmap;
				int lightmapbottom = Graphics.world.tiles[tilebottom].lightmap;

				if (lightmap < Graphics.world.lightmaps.size() && lightmapleft < Graphics.world.lightmaps.size() && lightmaptop < Graphics.world.lightmaps.size() &&
					lightmap > -1 && lightmapleft > -1 && lightmaptop > -1)
				{
					cLightmap* map = Graphics.world.lightmaps[lightmap];
					cLightmap* mapleft = Graphics.world.lightmaps[lightmapleft];
					cLightmap* maptop = Graphics.world.lightmaps[lightmaptop];
					cLightmap* mapright = Graphics.world.lightmaps[lightmapright];
					cLightmap* mapbottom = Graphics.world.lightmaps[lightmapbottom];

					for(int i = 0; i < 8; i++)
					{
						mapleft->buf[8*i+7] = map->buf[8*i+1];
						maptop->buf[7*8+i] = map->buf[i+8];
						mapright->buf[8*i] = map->buf[8*i+6];
						mapbottom->buf[i] = map->buf[6*8+i];

						mapleft->buf[64+3*(8*i+7)] = map->buf[64+3*(8*i+1)];
						mapleft->buf[64+3*(8*i+7)+1] = map->buf[64+3*(8*i+1)+1];
						mapleft->buf[64+3*(8*i+7)+2] = map->buf[64+3*(8*i+1)+2];
						maptop->buf[64+3*(7*8+i)] = map->buf[64+3*(i+8)];
						maptop->buf[64+3*(7*8+i)+1] = map->buf[64+3*(i+8)+1];
						maptop->buf[64+3*(7*8+i)+2] = map->buf[64+3*(i+8)+2];
						mapright->buf[64+3*(8*i)] = map->buf[64+3*(8*i+6)];
						mapright->buf[64+3*(8*i)+1] = map->buf[64+3*(8*i+6)+1];
						mapright->buf[64+3*(8*i)+2] = map->buf[64+3*(8*i+6)+2];
						mapbottom->buf[64+3*(i)] = map->buf[64+3*(6*8+i)];
						mapbottom->buf[64+3*(i)+1] = map->buf[64+3*(6*8+i)+1];
						mapbottom->buf[64+3*(i)+2] = map->buf[64+3*(6*8+i)+2];
					
					}
				}
				else
				{
				}
			}
				
		}
	}
	return true;
}



MENUCOMMAND(clearobjects)
{
	for(int i = 0; i < Graphics.world.models.size(); i++)
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
					t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
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
					t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
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

MENUCOMMAND(showoglighting)
{
	src->ticked = !src->ticked;
	Graphics.showoglighting = src->ticked;
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
	t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
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
	if (Graphics.texturepreview == NULL || Graphics.texturepreview->getfilename() != rodir + "texture/" + ((cMenuItem*)src)->data)
	{
		Graphics.texturepreview = new cTexture();
		Graphics.texturepreview->Load(rodir + "texture/" + ((cMenuItem*)src)->data);
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
	return true;
}