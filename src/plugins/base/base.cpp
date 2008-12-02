#include "base.h"
#include <wm/wm.h>
#include <graphics.h>

cPluginBase::cPluginBase(std::string n, std::string m)
{
	name = n;
	menu = m;
}

void cPluginBase::setInterface(cBrowInterface* pInterface)
{
	interface = pInterface;
}
