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
using namespace std;


cGraphics		Graphics;

float startmousex, startmousey;
float mousex, mousey;
float oldmousex, oldmousey;

float translatedstartmousex, translatedstartmousey;
float translatedmousex, translatedmousey;
float translatedoldmousex, translatedoldmousey;
int startmousepos;
bool dragged;
bool startvertex;
bool startobject;
map<long, string, less<long> >	idtomodel;

long lastlclick, lastrclick;

bool			lbuttondown, rbuttondown;


string message;
eState state = MAIN;
bool showmessage = false;


#endif


