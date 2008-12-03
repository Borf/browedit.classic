#ifndef __SETGATHEIGHT_H__
#define __SETGATHEIGHT_H__

#include "../base/base.h"

class cSetGatHeightPlugin : public cPluginBase
{
public:
	cSetGatHeightPlugin();
	bool action(cWorld*);
};

#endif
