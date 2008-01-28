#ifndef __COMMON_H__
#define __COMMON_H__

#define GL_GLEXT_PROTOTYPES	
#include <SDL/SDL.h>
#include "svnver.h"
#include "tinyxml/tinyxml.h"

/* Some definitions common to C99, ported to ANSI C if not available
 * This include some handy types like uintptr_t (integer with size of a pointer)
 */

#ifndef __cplusplus
#if __STDC_VERSION__ >= 199901L
#include <stdint.h>
#ifndef __bool_true_false_are_defined
#include <stdbool.h>
#endif /* __bool_true_false_are_defined */
#endif /* __STDC_VERSION__ >= 199901L */
#endif /* !__cplusplus */

#ifndef _STDINT_H
#ifndef __cplusplus
#define true 1
#define false 0
#endif /* !__cplusplus */

/* get typedef for all required stuff */
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short int uint16_t;
typedef signed short int int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;

#if __WORDSIZE == 64
typedef uint64_t uintptr_t;
typedef int64_t intptr_t;
#else
typedef uint32_t uintptr_t;
typedef int32_t intptr_t;
#endif

#endif

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

	HWND GetConsoleHwnd();

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
	extern TiXmlDocument msgtable;
	extern int keymap[SDLK_LAST-SDLK_FIRST];
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

vector<string> split(string, string);
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
	MODE_SPRITE,
};



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


char* GetMsg(string s);

/**
 * Container for process handlers
 */
class cProcessManagement
{
public:
	int main_process_events(SDL_Event &);

	int textureedit_process_events(SDL_Event &);
	int detailheightedit_process_events(SDL_Event &);
	int globalheightedit_process_events(SDL_Event &);
	int walledit_process_events(SDL_Event &);
	int objectedit_process_events(SDL_Event &);
	int gatedit_process_events(SDL_Event &);
	int wateredit_process_events(SDL_Event &);
	int effectedit_process_events(SDL_Event &);
	int lightedit_process_events(SDL_Event &);
	int soundedit_process_events(SDL_Event &);
	int objectgroupedit_process_events(SDL_Event &);
	int spriteedit_process_events(SDL_Event &);
};



#endif
