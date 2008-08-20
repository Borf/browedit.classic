#include "base.h"
#include <wm/wm.h>

cPluginBase::cPluginBase(std::string n, std::string m)
{
	name = n;
	menu = m;
}

void cPluginBase::SetFunctions(cWindow* (*func)(std::string))
{
	addxml = func;
}

cWindow* cPluginBase::addWindow(std::string s)
{
	return addxml(s);
}
