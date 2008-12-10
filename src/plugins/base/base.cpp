#include "base.h"
#include "interface.h"
#include <wm/wm.h>
#include <graphics.h>

cPluginBase::cPluginBase(std::string n, std::string m)
{
	ZeroMemory(name, 128);
	ZeroMemory(menu, 512);
	strcpy(name,n.c_str());
	strcpy(menu,m.c_str());
}

void cPluginBase::setInterface(cBrowInterface* pInterface)
{
	browInterface = pInterface;
}
