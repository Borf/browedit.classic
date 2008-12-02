#ifndef __COMMON_H__
#define __COMMON_H__

#define GL_GLEXT_PROTOTYPES	
#include <SDL/SDL.h>
#include "svnver.h"

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
#ifndef __MINGW32__
	#pragma warning( disable : 4786 )
	#pragma warning( disable : 4503 )
	#pragma warning( disable : 4244 ) //bad borf, implicit conversion with data loss (double -> float, int -> float)
	//#pragma warning( disable : 4018 ) //bad borf, signed/unsigned
#endif
	#include <fstream>
	#define sleep(x) Sleep(1000*(x))
	#define DIR_SEPERATOR "\\"
	#define newline "\r\n"
	#define usleep(x) Sleep((x) / 1000);
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


#include <vector>

#include "mymath.h"

#define SNAPPINGDIST 10
#define DRAGBORDER   6

#define MAX_MAPS 10


#define BOLD 1				// options for logging
#define DEBUG_LEVEL 5


//#ifndef WIN32
#ifndef min
//#define min(x,y) (x) > (y) ? (y) : (x)
//#define max(x,y) (x) < (y) ? (y) : (x)
float max(float x, float y);
float min(float x, float y);
#endif
//float max(float x, float y);
//float min(float x, float y);
//#endif

#include <string>
//using namespace std;

// prototypes
void Log(int lvl, int options, const char* fmt, ...);
void log_open(const char*, const char*, int);
void log_close();

std::vector<std::string> split(std::string, std::string);
std::string lcase(std::string s);
std::string ucase(std::string s);
std::string replace(std::string s, std::string a, std::string b);
std::string ltrim(std::string s);
std::string rtrim(std::string s);
std::string inttostring(int);
int hex2dec(std::string,int = 0);
cVector3 hex2floats(std::string);
void hex2floats(std::string, float*, int=4);

template <class T> inline bool inbetween(T val, T start, T end)
{
	return (start < end) ? (val >= start && val < end) : (val >= end && val < start);
}

int round(float f);
long tickcount();
void RotateShortestAngle(float &i, float angle);
void RotateShortestAngle(float &i, float angle, float mult);
long quad(long);
std::string getLongTimeString();
std::string removecolors(std::string s);
void drawlogo();
void changetoserverdir();
/*template <class T> void mergesort(std::vector<T> &tosort, bool compare(T,T))
{
	if (tosort.size() <= 1)
		return;

	std::vector<T>	ret;
	std::vector<T>	left;
	std::vector<T>	right;

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

template <class T> bool compare(T a,T b)
{
	return a < b;
}*/

template <class T> int linsearch(T needle, std::vector<T> &haystack)
{
	for(int i = 0; i < haystack.size(); i++)
		if(haystack[i] == needle)
			return i;
	return -1;
}


bool LineIntersectPolygon( cVector3 *,int,cVector3 &,cVector3 &, float &);

/*#ifdef WIN32
#ifndef _CONSOLE
#endif
#endif*/


const char* GetMsg(std::string s);

/**
 * Container for process handlers
 */
class cProcessManagement
{
public:
	static int main_process_events(SDL_Event &);

	static int textureedit_process_events(SDL_Event &);
	static int detailheightedit_process_events(SDL_Event &);
	static int globalheightedit_process_events(SDL_Event &);
	static int walledit_process_events(SDL_Event &);
	static int objectedit_process_events(SDL_Event &);
	static int gatedit_process_events(SDL_Event &);
	static int wateredit_process_events(SDL_Event &);
	static int effectedit_process_events(SDL_Event &);
	static int lightedit_process_events(SDL_Event &);
	static int soundedit_process_events(SDL_Event &);
	static int objectgroupedit_process_events(SDL_Event &);
	static int spriteedit_process_events(SDL_Event &);
	static int texturepaintedit_process_events(SDL_Event &);
};



#endif
