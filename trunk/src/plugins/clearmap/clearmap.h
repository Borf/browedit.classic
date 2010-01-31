#ifndef __CLEARMAP_H__
#define __CLEARMAP_H__

#include "../base/base.h"
#include <windows/jsonwindow.h>

class cClearMapPlugin : public cPluginBase
{
public:
	cClearMapPlugin();
	bool action();

	void clickOk(cWindow* w);
};

#endif
