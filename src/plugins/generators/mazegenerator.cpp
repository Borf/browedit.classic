#include <windows.h>
#include "mazegenerator.h"
#include "../base/types.h"



cMazeGenerator::cMazeGenerator() : cPluginBase("Maze Generator", "tools/generators/MAZEGENERATOR")
{
	
}


class cPoint
{
public:
	cPoint(int xx,int yy)
	{
		x = xx;
		y = yy;
	}
	cPoint operator+(cPoint b)
	{
		return cPoint(x+b.x, y+b.y);
	}
	cPoint operator*(int b)
	{
		return cPoint(x*b, y*b);
	}

	int x,y;
};


bool cMazeGenerator::action(cWorld* world)
{
	if(world->textures.size() < 3)
	{
		browInterface->messageWindow("You need at least 3 textures for this tool (floor, top and walls)");
		return false;
	}
	int x,y,i;
	int tileStart = world->tiles.size();

	for(int tex = 0; tex < 2; tex++)
	{
		for(y = 0; y < 4; y++)
		{
			for(x = 0; x < 4; x++)
			{
				cTile t;
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
				world->tiles.push_back(t);
			}
		}
	}
	for(x = 0; x < 4; x++)
	{
		cTile t;
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
		world->tiles.push_back(t);
	}

	for(y = 0; y < world->height; y++)
	{
		for(x = 0; x < world->width; x++)
		{
			world->cubes[y][x].tileUp = tileStart+16+((int)x%4) + 4*((int)y%4);
			world->cubes[y][x].tileSide = -1;
			world->cubes[y][x].tileOtherSide = -1;
			world->cubes[y][x].cell1 = -32;
			world->cubes[y][x].cell2 = -32;
			world->cubes[y][x].cell3 = -32;
			world->cubes[y][x].cell4 = -32;
		}
	}
	
	std::list<cPoint> positions;

	positions.push_back(cPoint(1,1));

	cPoint offsets[4] = { cPoint(-1,0), cPoint(1, 0), cPoint(0, 1), cPoint(0, -1) };

	std::vector<std::vector<bool> > tempMap;
	tempMap.resize(world->height, std::vector<bool>(world->width, true));

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

				if(pp.x >= 0 && pp.y >= 0 && pp.x < world->width-1 && pp.y < world->height-1)
				{
					if(tempMap[pp.y][pp.x])
					{
						tempMap[pp.y][pp.x] = false;
						positions.push_front(pp);
						world->cubes[pp.y][pp.x].tileUp = tileStart+0+((int)pp.x%4) + 4*((int)pp.y%4);
						world->cubes[pp.y][pp.x].cell1 = 0;
						world->cubes[pp.y][pp.x].cell2 = 0;
						world->cubes[pp.y][pp.x].cell3 = 0;
						world->cubes[pp.y][pp.x].cell4 = 0;

						pp = p + offsets[i];
						world->cubes[pp.y][pp.x].tileUp = tileStart+0+((int)pp.x%4) + 4*((int)pp.y%4);
						world->cubes[pp.y][pp.x].cell1 = 0;
						world->cubes[pp.y][pp.x].cell2 = 0;
						world->cubes[pp.y][pp.x].cell3 = 0;
						world->cubes[pp.y][pp.x].cell4 = 0;

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


	for(x = 0; x < world->width-1; x++)
	{
		for(y = 0; y < world->height-1; y++)
		{
			cCube* c = &world->cubes[y][x];
			if (c->tileOtherSide == -1)
			{
				if (c->cell4 != (c+1)->cell1 && c->cell2 != (c+1)->cell3)
				{
					world->cubes[y][x].tileOtherSide = tileStart+32+(y%4);
				}
			}
			if (c->tileSide == -1)
			{
				if (c->cell4 != world->cubes[y+1][x].cell1 && c->cell3 != world->cubes[y+1][x].cell2)
				{
					world->cubes[y][x].tileSide = tileStart+32+(x%4);
				}
			}
		}
		browInterface->render();
	}


	browInterface->fixNormals();

	return true;
}


