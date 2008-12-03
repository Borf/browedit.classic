#ifndef __MAZEGENERATOR_H__
#define __MAZEGENERATOR_H__

#include "../base/base.h"

class cMazeGenerator : public cPluginBase
{
public:
	cMazeGenerator();
	
	bool action(cWorld*);
	
};

#endif
