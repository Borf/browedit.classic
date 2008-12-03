#include <windows.h>
#include "culvertgenerator.h"
#include "../base/types.h"


cCulvertGenerator::cCulvertGenerator() : cPluginBase("Culvert Generator", "tools/generators/CULVERTGENERATOR")
{
	
}



bool cCulvertGenerator::action(cWorld* world)
{
	return true;
}


