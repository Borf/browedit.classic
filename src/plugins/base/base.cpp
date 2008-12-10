#include "base.h"
#include "interface.h"
#include <wm/wm.h>
#include <graphics.h>

cPluginBase::cPluginBase(char* n, char* m)
{
	ZeroMemory(name, 128);
	ZeroMemory(menu, 512);
	strcpy(name,n);
	strcpy(menu,m);
}

void cPluginBase::setInterface(cBrowInterface* pInterface)
{
	browInterface = pInterface;
}
