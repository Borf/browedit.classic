#include <windows.h>
#include "clearmap.h"
#include "../base/types.h"
#include <windows/xmlwindow.h>


cClearMapPlugin::cClearMapPlugin() : cPluginBase("Clear Map", "tools/CLEARMAP")
{
	
}

cWorld* world;
cClearMapPlugin* plugin;




void clickOk(cWindow* w)
{
	if(w->objects["chkTextures"]->getInt(0) != 0)
	{
		int x,y,i;
		std::map<int, bool, std::less<int> > used;

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
				t.lightmap = 0;
				t.texture = plugin->browInterface->getSelectedTextureIndex();
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
				world->cubes[y][x].tileUp = (x%4) + 4*(y % 4);
				world->cubes[y][x].tileSide = -1;
				world->cubes[y][x].tileOtherSide = -1;
			}
		}


		for(x = 0; x < world->width; x++)
		{
			for(y = 0; y < world->height; y++)
			{
				int tile = world->cubes[y][x].tileUp;
				if(used.find(tile) != used.end())
				{
					cTile t = world->tiles[tile];
					tile = world->tiles.size();
					world->tiles.push_back(t);
					world->cubes[y][x].tileUp = tile;
				}
				used[tile] = 1;
	///////////////////////////////////////
				tile = world->cubes[y][x].tileSide;
				if (tile != -1)
				{
					if(used.find(tile) != used.end())
					{
						cTile t = world->tiles[tile];
						tile = world->tiles.size();
						world->tiles.push_back(t);
						world->cubes[y][x].tileSide = tile;
					}
					used[tile] = 1;
				}
	/////////////////////////////////////
				tile = world->cubes[y][x].tileOtherSide;
				if (tile!= -1)
				{
					if(used.find(tile) != used.end())
					{
						cTile t = world->tiles[tile];
						tile = world->tiles.size();
						world->tiles.push_back(t);
						world->cubes[y][x].tileOtherSide = tile;
					}
					used[tile] = 1;
				}
			}
		}		
	}


	if(w->objects["chkHeight"]->getInt(0) != 0)
	{
		for(int y = 0; y < world->height; y++)
		{
			for(int x = 0; x < world->width; x++)
			{
				world->cubes[y][x].tileOtherSide = -1;
				world->cubes[y][x].tileSide = -1;
				world->cubes[y][x].cell1 = 0;
				world->cubes[y][x].cell2 = 0;
				world->cubes[y][x].cell3 = 0;
				world->cubes[y][x].cell4 = 0;
			}
		}
		plugin->browInterface->fixNormals();
	}

	if(w->objects["chkWalls"]->getInt(0) != 0)
	{
		for(int y = 0; y < world->height; y++)
		{
			for(int x = 0; x < world->width; x++)
			{
				world->cubes[y][x].tileOtherSide = -1;
				world->cubes[y][x].tileSide = -1;
			}
		}
	}
	if(w->objects["chkGat"]->getInt(0) != 0)
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
	if(w->objects["chkLightmaps"]->getInt(0) != 0)
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
	if(w->objects["chkObjects"]->getInt(0) != 0)
	{
		for(unsigned int i = 0; i < world->models.size(); i++)
			world->plugin_api_deleteobjects.push_back(0);
	}
	if(w->objects["chkSprites"]->getInt(0) != 0)
	{
		for(unsigned int i = 0; i < world->sprites.size(); i++)
			world->plugin_api_deletesprites.push_back(0);
	}


	if(w->objects["chkLights"]->getInt(0) != 0)
		world->lights.clear();
	if(w->objects["chkSounds"]->getInt(0) != 0)
		world->sounds.clear();
	if(w->objects["chkEffects"]->getInt(0) != 0)
		world->effects.clear();

	w->close();
}



void handleevent(cWindow* w, std::string name, std::string event)
{
	if(name == "btnOk" && event == "click")
		clickOk(w);
}

bool cClearMapPlugin::action(cWorld* wrld)
{
	world = wrld;
	plugin = this;
	cXmlWindow* w = (cXmlWindow*)browInterface->addXmlWindow("plugins/clearmap.xml");
	w->eventhandler = handleevent;
	return true;
}
