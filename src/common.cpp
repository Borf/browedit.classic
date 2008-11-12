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
#include <stdio.h>
#include <stdlib.h>
#include "tinyxml/tinyxml.h"
extern TiXmlDocument msgtable;


const float EPSILON = 0.001f;


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
std::string logfilename;
int loglines = 0;
char logname[32];

void log_open(const char* filename, const char* name, int color)
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

void Log(int lvl, int options, const char* fmt, ...)
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


std::vector<std::string> split(std::string tosplit, std::string seperator)
{
	std::vector<std::string> res;
	while(tosplit.find(seperator) != std::string::npos)
	{
		res.push_back(tosplit.substr(0, tosplit.find(seperator)));
		tosplit = tosplit.substr(tosplit.find(seperator)+seperator.length());
	}
	res.push_back(tosplit);
	return res;

}

std::string lcase(std::string s)
{
	int i;
	int siz = s.size();
	for(i = 0; i < siz; i++)
		if (s[i] > (char) 64 && s[i] < (char) 91) s[i] = (char)((int)s[i] +32);
	return s;
}

std::string ucase(std::string s)
{
	int i;
	int siz = s.size();
	for(i = 0; i < siz; i++)
		if (s[i] > (char) 96 && s[i] < (char) 123) s[i] = (char)((int)s[i] -32);
	return s;
}

std::string replace(std::string s, std::string a, std::string b)
{
	std::string s2;
	while(s.find(a) != std::string::npos)
	{
		s2 = s2 + s.substr(0, s.find(a)) + b;
		s = s.substr(s.find(a) + a.length(), s.length() - s.find(a) - a.length());
	}
	s2 = s2 + s;
	return s2;
}
std::string ltrim(std::string s)
{
	while((s[0] == '\t' || s[0] == ' ') && s.length() > 0)
		s = s.substr(1);
	return s;
}
std::string rtrim(std::string s)
{
	while((s[s.length()-1] == '\t' || s[s.length()-1] == ' ') && s.length() > 0)
		s = s.substr(0, s.length()-1);
	return s;
}


std::string inttostring(int i)
{
	char buf[10];
	//itoa(i, buf, 10);
	sprintf(buf,"%i",i);
	return buf;
}


int round(float f)
{
	return floor(f+0.5f);
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
	return SDL_GetTicks();
#endif
}
std::string removecolors(std::string s)
{
	std::string::iterator i;
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
std::string getLongTimeString()
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
	Log(3,0,"//////////////////////////////////////////////////////////////");
	Log(3,0,"//     ____                    ______    _ _ _              //");
	Log(3,0,"//    |  _ \\                  |  ____|  | (_) |             //");
	Log(3,0,"//    | |_) |_ __ _____      _| |__   __| |_| |_            //");
	Log(3,0,"//    |  _ <| '__/ _ \\ \\ /\\ / /  __| / _` | | __|           //");
	Log(3,0,"//    | |_) | | | (_) \\ V  V /| |___| (_| | | |_            //");
	Log(3,0,"//    |____/|_|  \\___/ \\_/\\_/ |______\\__,_|_|\\__|	     //");
	Log(3,0,"//                                                          //");
	Log(3,0,"//                   Coded by:      Borf                    //");
	Log(3,0,"//                     Libraries used:                      //");
	Log(3,0,"//            - OpenGL                                      //");
	Log(3,0,"//            - SDL                                         //");
	Log(3,0,"//            - zlib                                        //");
	Log(3,0,"//            - gd2                                         //");
	Log(3,0,"//            - (win)sockets                                //");
	Log(3,0,"//                                                          //");
	Log(3,0,"//////////////////////////////////////////////////////////////");
	Log(3,0,"");
	Log(3,0,"");
	Log(3,0,"       Public Beta Release, don't nag about any bugs please");
	Log(3,0,"       Version %i", getversion());
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



#ifndef min

float max(float x, float y)
{
    return x < y ? y : x;
}
float min(float x, float y)
{
	return x > y ? y : x;
}
	
#endif




struct tPlane
{
	cVector3 Normal;
	float D;
};

bool LinePlaneIntersection(tPlane &Plane, 
			   cVector3 &StartLine,
			   cVector3 &EndLine,
			   float &t)
{
	cVector3 LineDir = EndLine - StartLine;			
	float Denominator = LineDir.dot(Plane.Normal);
	if (fabs(Denominator) <= EPSILON) // Parallel to the plane
	{		
		return false;
	}		
	float Numerator = StartLine.dot(Plane.Normal) + Plane.D;
	t = - Numerator / Denominator;			  
	if (t < 0.0f || t > 1.0f) // The intersection point is not on the line
	{
		return false;
	}
	return true;
}

cVector3 Normal(cVector3* vertices)
{
	cVector3 b1, b2, normal;
	b1 = vertices[0] - vertices[1];
	b2 = vertices[2] - vertices[1];

	normal.x = b1.y * b2.z - b1.z * b2.y;
	normal.y = b1.z * b2.x - b1.x * b2.z;
	normal.z = b1.x * b2.y - b1.y * b2.x;

	normal.normalize();
	return normal;
	
}


bool LineIntersectPolygon( cVector3 *Vertices, 
			   int NumVertices,
			   cVector3 &StartLine,
			   cVector3 &EndLine,
			  float &t)
{
	tPlane Plane;
	Plane.Normal = Normal(Vertices);
	Plane.D = - Plane.Normal.dot(Vertices[0]);
	float tt;
	
	if (!LinePlaneIntersection(Plane, StartLine, EndLine, tt))
		return false;	
	
	cVector3 Intersection = StartLine + (EndLine - StartLine) * tt;			
	
/*	if (Intersection == EndLine)
		return false;*/
	int Vertex;
	for (Vertex = 0; Vertex < NumVertices; Vertex ++)
	{
		tPlane EdgePlane;		
		int NextVertex = (Vertex + 1) % NumVertices;
		cVector3 EdgeVector = Vertices[NextVertex] - Vertices[Vertex];			
		EdgePlane.Normal = EdgeVector.cross(Plane.Normal);
		EdgePlane.Normal.normalize();
		EdgePlane.D = - EdgePlane.Normal.dot(Vertices[Vertex]);
											
		if (EdgePlane.Normal.dot(Intersection) + EdgePlane.D < 0.0f)
			return false;
	}

	t = tt;	
	return true;
}
#ifdef WIN32
HWND GetConsoleHwnd()
{
	HWND hwndFound;
	char TempWindowTitle[1024];
	char WindowTitle[1024];
	GetConsoleTitle(WindowTitle, 1024);
	wsprintf(TempWindowTitle,"%d/%d", GetTickCount(), GetCurrentProcessId());
	SetConsoleTitle(TempWindowTitle);
	Sleep(40);
	hwndFound=FindWindow(NULL, TempWindowTitle);
	SetConsoleTitle(WindowTitle);
	return(hwndFound);
}
#endif




const char* GetMsg(std::string s)
{
	std::string olds = s;
	TiXmlNode* n = msgtable.FirstChildElement("language");

	while(s.find("/") != std::string::npos)
	{
		std::string a = s.substr(0, s.find("/"));
		n = n->FirstChildElement(a.c_str());
		if(n == NULL)
		{
			Log(1,0,"Could not find translation for %s", olds.c_str());
			return "NULL";
		}
		s = s.substr(s.find("/")+1);
	}
	if(n == NULL)
	{
		Log(1,0,"Could not find translation for %s", olds.c_str());
		return "NULL";
	}
	n = n->FirstChildElement(s.c_str());
	if(n == NULL)
	{
		Log(1,0,"Could not find translation for %s", olds.c_str());
		return "NULL";
	}
	return (char*)n->FirstChild()->Value();
}


int hex2dec(std::string s, int mult)
{
	static std::string lookup = "0123456789ABCDEF";
	int val = lookup.find(s[s.length()-1]);
	if(s.length() == 1)
		return pow(16.0,mult) * val;
	else
		return pow(16.0,mult) * val  + hex2dec(s.substr(0,s.length()-1), mult+1);
}

cVector3 hex2floats(std::string c)
{
	return cVector3(hex2dec(c.substr(0,2))/256.0f, hex2dec(c.substr(2,2))/256.0f, hex2dec(c.substr(4,2))/256.0f);
}

void hex2floats( std::string c, float* floats, int count)
{
	for(int i = 0; i < count; i++)
		floats[i] = hex2dec(c.substr(2*i, 2)) / 255.0f;
}