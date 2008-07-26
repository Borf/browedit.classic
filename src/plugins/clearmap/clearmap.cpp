// plugin_clearmap.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include "clearmap.h"
#include "../base/types.h"
#include <wm/xmlwindow.h>

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported function.
PLUGIN_CLEARMAP_API cPluginBase* _cdecl getInstance()
{
	return new cClearMapPlugin();
}



cClearMapPlugin::cClearMapPlugin() : cPluginBase("Clear Map", "tools/CLEARMAP")
{
	
}

cWorld* world;

void handleevent(cWindow* w, string name, string event)
{
	if(w->objects["chkTextures"]->GetInt(0) != 0)
	{
		int x,y,i;
		map<int, bool, less<int> > used;

		world->tiles.clear();
		world->lightmaps.clear();
		cLightmap* map = new cLightmap();
		for(i = 0; i < 256; i++)
			map->buf[i] = i < 64 ? 255 : 0;
		world->lightmaps.push_back(map);
		map = new cLightmap();
		for(i = 0; i < 256; i++)
			map->buf[i] = i < 64 ? 255 : 0;
		world->lightmaps.push_back(map);
		for(y = 0; y < 4; y++)
		{
			for(x = 0; x < 4; x++)
			{
				cTile t;
				t.lightmap = 1;
				t.texture = 0;
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


		for(y = 0; y < world->height; y++)
		{
			for(x = 0; x < world->width; x++)
			{
				world->cubes[y][x].tileup = (x%4) + 4*(y % 4);
				world->cubes[y][x].tileside = -1;
				world->cubes[y][x].tileaside = -1;
			}
		}


		for(x = 0; x < world->width; x++)
		{
			for(y = 0; y < world->height; y++)
			{
				int tile = world->cubes[y][x].tileup;
				if(used.find(tile) != used.end())
				{
					cTile t = world->tiles[tile];
					tile = world->tiles.size();
					world->tiles.push_back(t);
					world->cubes[y][x].tileup = tile;
				}
				used[tile] = 1;
	///////////////////////////////////////
				tile = world->cubes[y][x].tileside;
				if (tile != -1)
				{
					if(used.find(tile) != used.end())
					{
						cTile t = world->tiles[tile];
						tile = world->tiles.size();
						world->tiles.push_back(t);
						world->cubes[y][x].tileside = tile;
					}
					used[tile] = 1;
				}
	/////////////////////////////////////
				tile = world->cubes[y][x].tileaside;
				if (tile!= -1)
				{
					if(used.find(tile) != used.end())
					{
						cTile t = world->tiles[tile];
						tile = world->tiles.size();
						world->tiles.push_back(t);
						world->cubes[y][x].tileaside = tile;
					}
					used[tile] = 1;
				}
			}
		}		
	}


	if(w->objects["chkHeight"]->GetInt(0) != 0)
	{
		for(int y = 0; y < world->height; y++)
		{
			for(int x = 0; x < world->width; x++)
			{
				world->cubes[y][x].tileaside = -1;
				world->cubes[y][x].tileside = -1;
				world->cubes[y][x].cell1 = 0;
				world->cubes[y][x].cell2 = 0;
				world->cubes[y][x].cell3 = 0;
				world->cubes[y][x].cell4 = 0;
				world->cubes[y][x].calcnormal();
			}
		}
	}

	if(w->objects["chkWalls"]->GetInt(0) != 0)
	{
		for(int y = 0; y < world->height; y++)
		{
			for(int x = 0; x < world->width; x++)
			{
				world->cubes[y][x].tileaside = -1;
				world->cubes[y][x].tileside = -1;
			}
		}
	}
	if(w->objects["chkGat"]->GetInt(0) != 0)
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
				world->gattiles[2*y][2*x].type = 0;

				world->gattiles[2*y][2*x+1].cell1 = (c->cell1+c->cell2) / 2.0f;
				world->gattiles[2*y][2*x+1].cell2 = (c->cell2+c->cell2) / 2.0f;
				world->gattiles[2*y][2*x+1].cell3 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
				world->gattiles[2*y][2*x+1].cell4 = (c->cell4+c->cell2) / 2.0f;
				world->gattiles[2*y][2*x+1].type = 0;

				world->gattiles[2*y+1][2*x+1].cell1 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
				world->gattiles[2*y+1][2*x+1].cell2 = (c->cell4 + c->cell2) / 2.0f;
				world->gattiles[2*y+1][2*x+1].cell3 = (c->cell4 + c->cell3) / 2.0f;
				world->gattiles[2*y+1][2*x+1].cell4 = (c->cell4 + c->cell4) / 2.0f;
				world->gattiles[2*y+1][2*x+1].type = 0;

				world->gattiles[2*y+1][2*x].cell1 = (c->cell3 + c->cell1) / 2.0f;
				world->gattiles[2*y+1][2*x].cell2 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
				world->gattiles[2*y+1][2*x].cell3 = (c->cell3 + c->cell3) / 2.0f;
				world->gattiles[2*y+1][2*x].cell4 = (c->cell3 + c->cell4) / 2.0f;
				world->gattiles[2*y+1][2*x].type = 0;
			}

		}
	}
	if(w->objects["chkLightmaps"]->GetInt(0) != 0)
	{
		unsigned int i;
		for(i = 0; i < world->lightmaps.size(); i++)
			delete 	world->lightmaps[i];
		world->lightmaps.clear();
		cLightmap* m = new cLightmap();
		for(i = 0; i < 256; i++)
			m->buf[i] = i < 64 ? 255 : 0;
		world->lightmaps.push_back(m);

		for(i = 0; i < world->tiles.size(); i++)
			world->tiles[i].lightmap = 0;
	}
	if(w->objects["chkObjects"]->GetInt(0) != 0)
	{
		for(unsigned int i = 0; i < world->models.size(); i++)
			world->plugin_api_deleteobjects.push_back(0);
	}
	if(w->objects["chkSprites"]->GetInt(0) != 0)
	{
		for(unsigned int i = 0; i < world->sprites.size(); i++)
			world->plugin_api_deletesprites.push_back(0);
	}


	if(w->objects["chkLights"]->GetInt(0) != 0)
		world->lights.clear();
	if(w->objects["chkSounds"]->GetInt(0) != 0)
		world->sounds.clear();
	if(w->objects["chkEffects"]->GetInt(0) != 0)
		world->effects.clear();

	w->close();
}

bool cClearMapPlugin::action(cWorld* wrld)
{
	world = wrld;
	cXmlWindow* w = (cXmlWindow*)addWindow("plugins/clearmap.xml");
	w->eventhandler = handleevent;
	return true;
}