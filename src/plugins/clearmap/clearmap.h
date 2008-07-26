
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PLUGIN_CLEARMAP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PLUGIN_CLEARMAP_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef PLUGIN_CLEARMAP_EXPORTS
#define PLUGIN_CLEARMAP_API __declspec(dllexport)
#else
#define PLUGIN_CLEARMAP_API __declspec(dllimport)
#endif

#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )
#include "../base/base.h"

extern "C"
{
	PLUGIN_CLEARMAP_API cPluginBase* _cdecl getInstance();
}

class cClearMapPlugin : public cPluginBase
{
public:
	cClearMapPlugin();

	bool action(cWorld*);

};