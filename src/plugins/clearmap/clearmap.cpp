// plugin_clearmap.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include "clearmap.h"
#include "../base/types.h"
#include "clearwindow.h"

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported function.
PLUGIN_CLEARMAP_API cPluginBase* _cdecl getInstance()
{
	return new cClearMapPlugin();
}



cClearMapPlugin::cClearMapPlugin() : cPluginBase("Clear Map", "generate/CLEARMAP")
{


}

bool cClearMapPlugin::action(cWorld* world)
{
	addWindow("plugins/clearmap.window");
	return true;
}