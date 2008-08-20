enum eState
{
	MAIN,
	OBJECT,
	OBJECTSELECT,
	OBJECTPROPS,
};



#ifdef _MAIN_CPP_

#include <SDL/SDL.h>
#include "common.h"
#include "graphics.h"

#include <map>
//using namespace std;



long mouseX, mouseY;
float oldmousex, oldmousey;

float translatedstartmousex, translatedstartmousey;
float translatedmousex, translatedmousey;
float translatedoldmousex, translatedoldmousey;
int startmousepos;
bool dragged;
bool startvertex;
bool startobject;
std::map<long, std::string, std::less<long> >	idtomodel;

long lastlclick, lastrclick;

bool			lbuttondown, rbuttondown;


std::string message;
eState state = MAIN;
bool showmessage = false;


#endif


