#include "common.h"

#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#else
#include <windows.h>
#include <time.h>
#include <direct.h>
#endif
#include <memory.h>
#include "mymath.h"

float DistLineToPoint(cVector2 LineStart, cVector2 LineEnd, cVector2 point)
{
    float LineMag;
    float U;
    cVector2 Intersection;
 
	cVector2 diff = (LineEnd-LineStart);

    LineMag = diff.Magnitude();
 
    U = ( ( ( point.x - LineStart.x ) * ( LineEnd.x - LineStart.x ) ) +
        ( ( point.y - LineStart.y ) * ( LineEnd.y - LineStart.y ) ) ) /
        ( LineMag * LineMag );
 
    if( U < 0.0f)
        return (point-LineStart).Magnitude();
	if (U > 1.0f )
        return (point-LineEnd).Magnitude();
 
    Intersection.x = LineStart.x + U * diff.x;
    Intersection.y = LineStart.y + U * diff.y;
 
    return (point-Intersection).Magnitude();
}


FILE* pFile = NULL;
string logfilename;
int loglines = 0;
char logname[32];

void log_open(const char* filename, char* name, int color)
{
	logfilename = filename;
	pFile = fopen(filename, "w");
	if (pFile == NULL)
	{
		fprintf(stderr, "Error opening logfile!");
		exit(0);
	}
#ifndef WIN32
	sprintf(logname, "\033[22;%im%s\033[22;32m", 33+color, name);
#else
	memset(logname, '\0', sizeof(logname));
#endif	
	drawlogo();
}

void log_close()
{
	fclose(pFile);
}

void Log(int lvl, int options, char* fmt, ...)
{
	char text[10240];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap,fmt);
	vsprintf(text,fmt,ap);
	va_end(ap);
	
	char buf[10240];
	strcpy(buf, text);
	sprintf(text, "%s %s", getLongTimeString().c_str(), buf);


	if ((options & BOLD) != 0)
	{
		char buf[10240];
		memcpy(buf, text, 10240);
#ifndef WIN32
		sprintf(text, "\033[1m%s", buf);
#else
		sprintf(text, buf);
#endif
	}
	if (lvl > 2)
	{
#ifndef WIN32
		fprintf(stdout, "\033[22;32mInfo (%s): \033[39m%s\n", logname, text);
#else
		fprintf(stdout, "Info: %s\n", text);
#endif
		fputs("Info: ", pFile);
		fputs(text, pFile);
		fputc('\n', pFile);
	}
	else
	{
#ifndef WIN32
		fprintf(stderr, "\033[22;31mError (%s): \033[1;39m%s\n", logname, text);
#else
		fprintf(stderr, "Error: %s\n", text);
#endif
		fputs("Error: ", pFile);
		fputs(text, pFile);
		fputc('\n', pFile);
	}
	loglines++;
//	if (loglines%10 == 0)
	{
		fclose(pFile);
		pFile = fopen(logfilename.c_str(), "a");
		if (pFile == NULL)
		{
			fprintf(stderr, "Error opening logfile!");
			exit(0);
		}
	}

#ifndef _NOSLEEP_
	if(lvl < 2)
		Sleep(2000);
#endif
	if(lvl == 0)
		exit(0);
}




string lcase(string s)
{
	int i;
	int siz = s.size();
	for(i = 0; i < siz; i++)
		if (s[i] > (char) 64 && s[i] < (char) 91) s[i] = (char)((int)s[i] +32);
	return s;
}

string ucase(string s)
{
	int i;
	int siz = s.size();
	for(i = 0; i < siz; i++)
		if (s[i] > (char) 96 && s[i] < (char) 123) s[i] = (char)((int)s[i] -32);
	return s;
}

string replace(string s, string a, string b)
{
	string s2;
	while(s.find(a) != string::npos)
	{
		s2 = s2 + s.substr(0, s.find(a)) + b;
		s = s.substr(s.find(a) + a.length(), s.length() - s.find(a) - a.length());
	}
	s2 = s2 + s;
	return s2;
}
string ltrim(string s)
{
	while((s[0] == '\t' || s[0] == ' ') && s.length() > 0)
		s = s.substr(1);
	return s;
}
string rtrim(string s)
{
	while((s[s.length()-1] == '\t' || s[s.length()-1] == ' ') && s.length() > 0)
		s = s.substr(0, s.length()-1);
	return s;
}

// rotates i to angle over the shortest angle
void RotateShortestAngle(float &i, float angle)	
{
	if (fabs(i - (angle + 360.0f)) < fabs(i - angle)) angle+=360.0f;					// do stuff to angle ^^;;
	if (fabs(i - (angle - 360.0f)) < fabs(i - angle)) angle-=360.0f;
	if (i < angle) i+=5.0f;
	if (i > angle) i-=5.0f;
	if (i < 0.0) i = i + 360.0f;
	if (i > 360.0) i = i - 360.0f;
}
// rotates i to angle over the shortest angle, with multiplying factor mult
void RotateShortestAngle(float &i, float angle, float mult)
{
	if (fabs(i - (angle + 360.0f)) < fabs(i - angle)) angle+=360.0f;					// do stuff to angle ^^;;
	if (fabs(i - (angle - 360.0f)) < fabs(i - angle)) angle-=360.0f;
	if (i < angle) i+=mult*5.0f;
	if (i > angle) i-=mult*5.0f;
	
	if (fabs(i - angle) < mult*5.0f) i = angle;

	if (i < 0.0) i = i + 360.0f;
	if (i > 360.0) i = i - 360.0f;
}



long tickcount()
{
#ifndef WIN32
	struct timeval tval;
	gettimeofday(&tval,NULL);
	return tval.tv_sec * 1000 + tval.tv_usec/1000;
#else
	return GetTickCount();
#endif
}
string removecolors(string s)
{
	string::iterator i;
	int index = 0;
	for(i = s.begin(); i != s.end(); i++)
	{
		if ((*i) == 1)
		{
			s.erase(i, i+10);
			i = s.begin() + index;
		}
		index++;
	}
	return s;
}
string getLongTimeString()
{
	char buf[12];
	time_t _tt = time(NULL);
	tm* _tm = localtime(&_tt);
	if(_tm == NULL) {
		strcpy(buf, "xx:xx");
	} else {
		strftime(buf, 12, "[%H:%M:%S]", _tm);
	}
	return buf;
}


long quad(long d)
{
	return d*d;
}



void drawlogo()
{
	Log(3,0,"//////////////////////////////////////////////////////////////////////");
	Log(3,0,"//          ___  ____   __    ___  ____    __    __                 //");
	Log(3,0,"//         / __)(  _ \\ /__\\  / __)(_  _)  /__\\  (  )                //");
	Log(3,0,"//         \\__ \\ )___//(__)\\( (__  _)(_  /(__)\\  )(__               //");
	Log(3,0,"//         (___/(__) (__)(__)\\___)(____)(__)(__)(____)              //");
	Log(3,0,"//                                                                  //");
	Log(3,0,"//                   Coded by:      Borf                            //");
	Log(3,0,"//                     Libraries used:                              //");
	Log(3,0,"//            - OpenGL                                              //");
	Log(3,0,"//            - pthread-win32                                       //");
	Log(3,0,"//            - Cal3d 0.10                                          //");
	Log(3,0,"//            - zlib                                                //");
	Log(3,0,"//            - (win)sockets                                        //");
	Log(3,0,"//            - mysql                                               //");
	Log(3,0,"//                                                                  //");
	Log(3,0,"//////////////////////////////////////////////////////////////////////");
	Log(3,0,"");
	Log(3,0,"");
}


void changetoserverdir()
{
	char buf[1025];
	ZeroMemory(buf, 1024);
	getcwd(buf, 1024);
	printf("CWD: %s\n", buf);

	for(int i = 0; i < 4; i++)
	{
		FILE* pFile = fopen("data.dat", "r");
		if (pFile == NULL)
		{
			pFile = fopen("data\\id2model.txt", "r");
			if (pFile == NULL)
			{
				chdir("..");
			}
			else
			{
				fclose(pFile);
				return;
			}
		}
		else
		{
			fclose(pFile);
			return;
		}
	}
	chdir(buf);
}



#ifndef WIN32
float max(float x, float y)
{
    return x < y ? y : x;
}
float min(float x, float y)
{
	return x > y ? y : x;
}
	
#endif