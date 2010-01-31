#ifndef __MINGW32__
#pragma warning( disable : 4786 )
#endif

#include <windows.h>
#include "mazegenerator.h"
#include "../base/interface.h"
#include "types.h"
#include <list>


cMazeGenerator::cMazeGenerator() : cPluginBase("Maze Generator", "tools/generators/MAZEGENERATOR")
{
	
}




bool cMazeGenerator::action()
{
	if(browInterface->textureCount() < 3)
	{
		browInterface->messageWindow("You need at least 3 textures for this tool (floor, top and walls)");
		return false;
	}
	int x,y,i;
	int tileStart = browInterface->tileCount();

	for(int tex = 0; tex < 2; tex++)
	{
		for(y = 0; y < 4; y++)
		{
			for(x = 0; x < 4; x++)
			{
				cBrowInterface::cPluginTile t;
				t.lightmap = 0;
				t.texture = tex;
				t.u1 = x/4.0f;
				t.v1 = y/4.0f;
				t.u2 = (x+1)/4.0f;
				t.v2 = (y)/4.0f;
				t.u3 = (x)/4.0f;
				t.v3 = (y+1)/4.0f;
				t.u4 = (x+1)/4.0f;
				t.v4 = (y+1)/4.0f;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				browInterface->addTile(t);
			}
		}
	}
	for(x = 0; x < 4; x++)
	{
		cBrowInterface::cPluginTile t;
		t.lightmap = 0;
		t.texture = 2;
		t.u1 = (x+1)/4.0f;
		t.v1 = 0;
		t.u2 = (x)/4.0f;
		t.v2 = 0;
		t.u3 = (x+1)/4.0f;
		t.v3 = 1;
		t.u4 = (x)/4.0f;
		t.v4 = 1;
		t.color[0] = (char)255;
		t.color[1] = (char)255;
		t.color[2] = (char)255;
		t.color[3] = (char)255;
		browInterface->addTile(t);
	}

	for(y = 0; y < browInterface->getWorldHeight(); y++)
	{
		for(x = 0; x < browInterface->getWorldWidth(); x++)
		{
			cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
			c->tileUp = tileStart+16+((int)x%4) + 4*((int)y%4);
			c->tileSide = -1;
			c->tileOtherSide = -1;
			c->cell1 = -32;
			c->cell2 = -32;
			c->cell3 = -32;
			c->cell4 = -32;
		}
	}
	
	std::list<cPoint> positions;

	positions.push_back(cPoint(1,1));

	cPoint offsets[4] = { cPoint(-1,0), cPoint(1, 0), cPoint(0, 1), cPoint(0, -1) };

	std::vector<std::vector<bool> > tempMap;
	tempMap.resize(browInterface->getWorldHeight(), std::vector<bool>(browInterface->getWorldWidth(), true));

	int repeat = 0;
	while(positions.size() > 0)
	{
		cPoint p = positions.back();
		positions.pop_back();
		
		for(i = 0; i < 4; i++)
		{
			int rnd = rand()%4;
			cPoint a = offsets[i];
			offsets[i] = offsets[rnd];
			offsets[rnd] = a;
		}

		for(i = 0; i < 4; i++)
		{
			cPoint ppp = p;
			int rnd2 = rand()%10+2;
			for(; i < rnd2; i++)
			{
				cPoint pp = p + (offsets[i]*2);

				if(pp.x >= 0 && pp.y >= 0 && pp.x < browInterface->getWorldWidth()-1 && pp.y < browInterface->getWorldHeight()-1)
				{
					if(tempMap[pp.y][pp.x])
					{
						tempMap[pp.y][pp.x] = false;
						positions.push_front(pp);

						cBrowInterface::cPluginCube* c = browInterface->getCube(pp.x, pp.y);
						c->tileUp = tileStart+0+((int)pp.x%4) + 4*((int)pp.y%4);
						c->cell1 = 0;
						c->cell2 = 0;
						c->cell3 = 0;
						c->cell4 = 0;

						pp = p + offsets[i];
						c = browInterface->getCube(pp.x, pp.y);
						c->tileUp = tileStart+0+((int)pp.x%4) + 4*((int)pp.y%4);
						c->cell1 = 0;
						c->cell2 = 0;
						c->cell3 = 0;
						c->cell4 = 0;

						p = p + (offsets[i]*2);
					}
					else
						break;
				}
				else
					break;
			}
			p = ppp;
		}

		if((repeat++) % 10 == 0)
			browInterface->render();
	}


	for(x = 0; x < browInterface->getWorldWidth()-1; x++)
	{
		for(y = 0; y < browInterface->getWorldHeight()-1; y++)
		{
			cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
			if (c->tileOtherSide == -1)
			{
				if (c->cell4 != browInterface->getCube(x+1,y)->cell1 && c->cell2 != browInterface->getCube(x+1,y)->cell3)
				{
					c->tileOtherSide = tileStart+32+(y%4);
				}
			}
			if (c->tileSide == -1)
			{
				if (c->cell4 != browInterface->getCube(x,y+1)->cell1 && c->cell3 != browInterface->getCube(x,y+1)->cell2)
				{
					c->tileSide = tileStart+32+(x%4);
				}
			}
		}
		browInterface->render();
	}


	browInterface->fixNormals();


	cPluginBase* gatHeight = browInterface->getPlugin("Gat Height");
	if(gatHeight)
		gatHeight->action();

	return true;
}


