#ifndef __COMMON_H__
#define __COMMON_H__

#define GL_GLEXT_PROTOTYPES	
#include <SDL/SDL.h>
#include "svnver.h"

#ifdef WIN32
	#include <windows.h>
	#pragma warning( disable : 4786 )
	#pragma warning( disable : 4503 )
	#include <fstream>
	#define sleep(x) Sleep(1000*(x))
	#define DIR_SEPERATOR "\\"
	#define newline "\r\n"
	#define usleep(x) Sleep((x) / 1000);
	#define msgbox(x, y) MessageBox(NULL, x, y, MB_OK);
	#include <direct.h>

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;


#else
	#include <netdb.h>
	#include <netinet/in.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <fcntl.h>
	typedef int SOCKET;
	#define closesocket(x) close(x)
	#include <dirent.h>
	#define BYTE unsigned char
	#include <sys/stat.h>
	#define DIR_SEPERATOR "/"
	#define newline "\n"
	#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
	#define Sleep(x) usleep(x*1000)
#endif

#ifndef _MAIN_CPP_
	#include <vector>
	using namespace std;
	extern vector<char*> msgtable;
#endif

#include <vector>

#include "mymath.h"

#define SNAPPINGDIST 10
#define DRAGBORDER   6


#define BOLD 1				// options for logging
#define DEBUG_LEVEL 5


#ifndef WIN32
//#define min(x,y) (x) > (y) ? (y) : (x)
//#define max(x,y) (x) < (y) ? (y) : (x)
float max(float x, float y);
float min(float x, float y);
#endif

#include <string>
using namespace std;

// prototypes
void Log(int lvl, int options, char* fmt, ...);
void log_open(const char*, char*, int);
void log_close();

string lcase(string s);
string ucase(string s);
string replace(string s, string a, string b);
string ltrim(string s);
string rtrim(string s);
long tickcount();
void RotateShortestAngle(float &i, float angle);
void RotateShortestAngle(float &i, float angle, float mult);
long quad(long);
string getLongTimeString();
string removecolors(string s);
void drawlogo();
void changetoserverdir();
template <class T> void mergesort(vector<T> &tosort, bool compare(T,T))
{
	if (tosort.size() <= 1)
		return;

	vector<T>	ret;
	vector<T>	left;
	vector<T>	right;

	unsigned int pivot = (unsigned int)tosort.size()/2;

	left.reserve(pivot);
	right.reserve(tosort.size() - pivot);

	for(unsigned int i = 0; i < tosort.size(); i++)
	{
		if (i < pivot)
			left.push_back(tosort[i]);
		else
			right.push_back(tosort[i]);
	}
	
	mergesort(left, compare);
	mergesort(right, compare);

	unsigned int x = 0, y = 0;
	tosort.clear();
	while(x != left.size() || y != right.size())
	{
		if (x != left.size() && y != right.size())
		{
			if (compare(left[x], right[y]))
			{
				tosort.push_back(left[x]);
				x++;
			}
			else
			{
				tosort.push_back(right[y]);
				y++;
			}
		}
		else if (x != left.size() && y == right.size())
		{
			tosort.push_back(left[x]);
			x++;
		}
		else if (x == left.size() && y != right.size())
		{
			tosort.push_back(right[y]);
			y++;
		}
	}
}



enum eMode
{
	MODE_TEXTURE,
	MODE_HEIGHTDETAIL,
	MODE_HEIGHTGLOBAL,
	MODE_WALLS,
	MODE_OBJECTS,
	MODE_GAT,
	MODE_WATER,
	MODE_EFFECTS,
	MODE_SOUNDS,
	MODE_LIGHTS,
	MODE_OBJECTGROUP,
};




const float EPSILON = 0.001f;
struct tPlane
{
	cVector3 Normal;
	float D;
};

bool LineIntersectPolygon( cVector3 *,int,cVector3 &,cVector3 &, float &);

/*#ifdef WIN32
#ifndef _CONSOLE
#endif
#endif*/



enum eLanguage
{
	NOMSG,
	HOSTNOTFOUND,
	NOSOCKET,
	VERSIONERROR,
	LOADINGFILE,
	DONELOADINGFILE,
	COULDNOTOPENFILE,
	UNKNOWNCONFIG,
	DONEINIT,
	CALCMODEL,
	PERCENTAGE,
	//menu
	MENU_FILE,
	MENU_GENERATE,
	MENU_VIEW,
	MENU_EDITMODE,
	MENU_EDIT,
	MENU_MODELS,
	MENU_EFFECTS,
	MENU_WINDOWS,
	//file
	MENU_NEW,
	MENU_OPEN,
	MENU_SAVE,
	MENU_SAVEAS,
	MENU_IMPORTARCTURUS,
	MENU_EXPORTLIGHTMAPS,
	MENU_IMPORTLIGHTMAPS,
	MENU_EXIT,
	//generate
	MENU_RANDOM1,
	MENU_MAZESTUFF,
	MENU_QUADTREE,
	MENU_CALCULATELIGHTMAPS,
	MENU_CALCULATELIGHTMAPSLOCAL,
	MENU_CLEARMAP,
	//view
	MENU_GRID,
	MENU_OBJECTS,
	MENU_BOUNDINGBOXES,
	MENU_LIGHTMAPS,
	MENU_OGLLIGHTING,
	MENU_TILECOLORS,
	MENU_SHOWWATER,
	MENU_TOPCAMERA,
	MENU_INVISIBLETILES,
	MENU_SHOWAMBIENTLIGHTING,
	MENU_WATERANIMATION,
	MENU_GATTILES,
	//edit modes
	MENU_TEXTUREEDIT,
	MENU_GLOBALHEIGHTEDIT,
	MENU_DETAILTERRAINEDIT,
	MENU_WALLEDIT,
	MENU_OBJECTEDIT,
	MENU_GATEDIT,
	MENU_WATEREDIT,
	MENU_EFFECTSEDIT,
	MENU_SOUNDSEDIT,
	MENU_LIGHTSEDIT,
	MENU_OBJECTGROUPEDIT,
	//edit
	MENU_SPEED,
	MENU_SLOPING,
	MENU_SNAPOBJECTS,
	MENU_FLATTEN,
	MENU_FILL,
	MENU_GATHEIGHT,
	MENU_RESETCOLORS,
	MENU_CLEAROBJECTS,
	MENU_CLEAREFFECTS,
	MENU_CLEARLIGHTS,
	MENU_ADDWALLS,
	MENU_GATCOLLISION,
	MENU_CLEARLIGHTMAPS,
	MENU_CLEANLIGHTMAPS,
	MENU_REMOVETEXTURES,
//window
	MENU_AMBIENTLIGHTING,
	MENU_MODELWINDOW,
	MENU_TEXTURES,
	MENU_PROPERTIES,
	MENU_WATER,

	FS_GRFOPENERROR,
	FS_GRFOPENDONE,
	FS_FILEERROR,
	
	FONT_LOADING,
	FONT_DONELOADING,

	INIT_GRAPHICS,
	INIT_GRAPHICS_VIDEO_FAILED,
	INIT_GRAPHICS_QUERY_FAILED,
	INIT_GRAPHICS_MODE_FAILED,
	INIT_GRAPHICS_OPENGL_FAILED,
	INIT_GRAPHICS_TEXTURES,
	INIT_GRAPHICS_WM,

	TEXTURE_OPEN_FAILED,
	TEXTURECACHE_UNLOADFAIL,
	TEXTURECACHE_UNLOADALOT,
	TEXTURECACHE_STILLLOADED,

	WORLD_LOAD,
	WORLD_LOADFAIL,
	WORLD_LOADDONE,
	WORLD_MODELFAIL,
	WORLD_UNKNOWNOBJECT,
	WORLD_QUADTREECALC,
	WORLD_TOOMANYLIGHTMAPS,
	WORLD_MODELCOUNT,
	WORLD_TILESUSED,
	WORLD_TILESLEFT,

	WINDOW_OK,
	WINDOW_CANCEL,

	WINDOW_AMBIENT_TITLE,
	WINDOW_AMBIENT_AMBIENT,
	WINDOW_AMBIENT_DIFFUSE,
	WINDOW_AMBIENT_SHADOW,
	WINDOW_AMBIENT_ALPHA,

	WINDOW_CONFIRM_TITLE,

	WINDOW_EFFECT_TITLE,
	WINDOW_EFFECT_POS,
	WINDOW_EFFECT_SCALE,
	WINDOW_EFFECT_ROT,

	WINDOW_INPUT_TITLE,

	WINDOW_LIGHT_TITLE,
	WINDOW_LIGHT_POSITION,
	WINDOW_LIGHT_COLOR,
	WINDOW_LIGHT_INTENSITY,

	WINDOW_MESSAGE_TITLE,

	WINDOW_MODELS_TITLE,
	WINDOW_MODELS_DELETECONFIRM,
	WINDOW_MODELS_NODENAME,

	WINDOW_OBJECT_TITLE,
	WINDOW_OBJECT_OBJECT,
	WINDOW_OBJECT_POSITION,
	WINDOW_OBJECT_SCALE,
	WINDOW_OBJECT_ROTATION,

	WINDOW_TEXTURE_TITLE,

	WINDOW_WATER_TITLE,
	WINDOW_WATER_AMPLITUDE,
	WINDOW_WATER_HEIGHT,
	WINDOW_WATER_PHASE,
	WINDOW_WATER_SURFACECYCLE,
	WINDOW_WATER_TEXTURECYCLE,
	WINDOW_WATER_TYPE,


};



#endif
