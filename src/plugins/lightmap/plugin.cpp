#include <windows.h>
#include "plugin.h"
#include "lightmapimport.h"
#include "lightmapexport.h"

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

PLUGIN_API int getInstanceCount=2;

PLUGIN_API cPluginBase** _cdecl getInstances()
{
	cPluginBase** plugins = new cPluginBase*[getInstanceCount];
	plugins[0] = new cLightMapImportPlugin();
	plugins[1] = new cLightMapExportPlugin();
	return plugins;
}


