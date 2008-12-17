#include <windows.h>
#include "setgatheight.h"
#include "../base/types.h"


cSetGatHeightPlugin::cSetGatHeightPlugin() : cPluginBase("Gat Height", "tools/gat/GATHEIGHT")
{
	
}

bool cSetGatHeightPlugin::action()
{
	for(int y = 0; y < browInterface->getWorldHeight(); y++)
	{
		for(int x = 0; x < browInterface->getWorldWidth(); x++)
		{
			cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
			
			cBrowInterface::cPluginGatCube* gc;
			
			gc = browInterface->getGatCube(2*x, 2*y);
			
			gc->cell1 = (c->cell1+c->cell1) / 2.0f;
			gc->cell2 = (c->cell1+c->cell2) / 2.0f;
			gc->cell3 = (c->cell1+c->cell3) / 2.0f;
			gc->cell4 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			gc->type = 0;
			
			gc = browInterface->getGatCube(2*x+1, 2*y);
			gc->cell1 = (c->cell1+c->cell2) / 2.0f;
			gc->cell2 = (c->cell2+c->cell2) / 2.0f;
			gc->cell3 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			gc->cell4 = (c->cell4+c->cell2) / 2.0f;
			gc->type = 0;
			
			gc = browInterface->getGatCube(2*x+1, 2*y+1);
			gc->cell1 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			gc->cell2 = (c->cell4 + c->cell2) / 2.0f;
			gc->cell3 = (c->cell4 + c->cell3) / 2.0f;
			gc->cell4 = (c->cell4 + c->cell4) / 2.0f;
			gc->type = 0;
			
			gc = browInterface->getGatCube(2*x, 2*y+1);
			gc->cell1 = (c->cell3 + c->cell1) / 2.0f;
			gc->cell2 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			gc->cell3 = (c->cell3 + c->cell3) / 2.0f;
			gc->cell4 = (c->cell3 + c->cell4) / 2.0f;
			gc->type = 0;
		}
		
	}
	return true;
}
