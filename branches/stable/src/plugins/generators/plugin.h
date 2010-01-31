#ifdef PLUGIN_EXPORTS
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API __declspec(dllimport)
#endif

#include "../base/base.h"

extern "C"
{
	PLUGIN_API cPluginBase** _cdecl	getInstances();
	extern PLUGIN_API int getInstanceCount;
}

