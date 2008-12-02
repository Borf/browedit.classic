#include "base.h"
#include "interface.h"
#include <wm/wm.h>
#include <graphics.h>

cPluginBase::cPluginBase(std::string n, std::string m)
{
	name = n;
	menu = m;
}

void cPluginBase::setInterface(cBrowInterface* pInterface)
{
	browInterface = pInterface;
}
