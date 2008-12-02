
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PLUGIN_MAZEGENERATOR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PLUGIN_MAZEGENERATOR_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef PLUGIN_MAZEGENERATOR_EXPORTS
#define PLUGIN_MAZEGENERATOR_API __declspec(dllexport)
#else
#define PLUGIN_MAZEGENERATOR_API __declspec(dllimport)
#endif

#ifndef __MINGW32__
#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )
#endif

#include "../base/base.h"

extern "C"
{
	PLUGIN_MAZEGENERATOR_API cPluginBase* _cdecl getInstance();
}

class cMazeGenerator : public cPluginBase
{
public:
	cMazeGenerator();

	bool action(cWorld*);

};
