#include "base.h"
#include <wm/wm.h>

cPluginBase::cPluginBase(string n, string m)
{
	name = n;
	menu = m;
}

void cPluginBase::SetFunctions(cWindow* (*func)(string))
{
	addxml = func;
}

cWindow* cPluginBase::addWindow(string s)
{
	return addxml(s);
}
