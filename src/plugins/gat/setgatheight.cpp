#include <windows.h>
#include "setgatheight.h"
#include "../base/types.h"


cSetGatHeightPlugin::cSetGatHeightPlugin() : cPluginBase("Gat Height", "tools/gat/GATHEIGHT")
{
	
}

bool cSetGatHeightPlugin::action(cWorld* world)
{
	int x,y;
	for(y = 0; y < world->height; y++)
	{
		for(x = 0; x < world->width; x++)
		{
			cCube* c = &world->cubes[y][x];
			world->gattiles[2*y][2*x].cell1 = (c->cell1+c->cell1) / 2.0f;
			world->gattiles[2*y][2*x].cell2 = (c->cell1+c->cell2) / 2.0f;
			world->gattiles[2*y][2*x].cell3 = (c->cell1+c->cell3) / 2.0f;
			world->gattiles[2*y][2*x].cell4 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			
			world->gattiles[2*y][2*x+1].cell1 = (c->cell1+c->cell2) / 2.0f;
			world->gattiles[2*y][2*x+1].cell2 = (c->cell2+c->cell2) / 2.0f;
			world->gattiles[2*y][2*x+1].cell3 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			world->gattiles[2*y][2*x+1].cell4 = (c->cell4+c->cell2) / 2.0f;
			
			world->gattiles[2*y+1][2*x+1].cell1 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			world->gattiles[2*y+1][2*x+1].cell2 = (c->cell4 + c->cell2) / 2.0f;
			world->gattiles[2*y+1][2*x+1].cell3 = (c->cell4 + c->cell3) / 2.0f;
			world->gattiles[2*y+1][2*x+1].cell4 = (c->cell4 + c->cell4) / 2.0f;
			
			world->gattiles[2*y+1][2*x].cell1 = (c->cell3 + c->cell1) / 2.0f;
			world->gattiles[2*y+1][2*x].cell2 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
			world->gattiles[2*y+1][2*x].cell3 = (c->cell3 + c->cell3) / 2.0f;
			world->gattiles[2*y+1][2*x].cell4 = (c->cell3 + c->cell4) / 2.0f;
		}
		
	}
	return true;
}
