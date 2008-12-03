#ifndef __CLEARMAP_H__
#define __CLEARMAP_H__

#include "../base/base.h"

class cClearMapPlugin : public cPluginBase
{
public:
	cClearMapPlugin();
	bool action(cWorld*);
};

#endif
