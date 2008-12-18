#include <windows.h>
#include "clearmap.h"
#include "../base/interface.h"
#include <windows/xmlwindow.h>


cClearMapPlugin::cClearMapPlugin() : cPluginBase("Clear Map", "tools/CLEARMAP")
{
	
}

cClearMapPlugin* plugin;




void cClearMapPlugin::clickOk(cWindow* w)
{
	if(w->objects["chkTextures"]->getInt(0) != 0)
	{
		int x,y,i;
		std::map<int, bool, std::less<int> > used;

		while(browInterface->lightmapCount() > 0)
			browInterface->removeLightmap(0);
		while(browInterface->tileCount() > 0)
			browInterface->removeTile(0);


		cBrowInterface::cPluginLightmap* map = browInterface->newLightmap();
		for(i = 0; i < 256; i++)
			map->buf[i] = i < 64 ? 255 : 0;
		browInterface->addLightmap(map);

		map = browInterface->newLightmap();
		for(i = 0; i < 256; i++)
			map->buf[i] = i < 64 ? 255 : 0;
		browInterface->addLightmap(map);

		
		for(y = 0; y < 4; y++)
		{
			for(x = 0; x < 4; x++)
			{
				cBrowInterface::cPluginTile t;
				t.lightmap = 0;
				t.texture = browInterface->getSelectedTextureIndex();
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


		for(y = 0; y < browInterface->getWorldHeight(); y++)
		{
			for(x = 0; x < browInterface->getWorldWidth(); x++)
			{
				cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
				c->tileUp = (x%4) + 4*(y % 4);
				c->tileSide = -1;
				c->tileOtherSide = -1;
			}
		}
		browInterface->makeTilesUnique();
	}


	if(w->objects["chkHeight"]->getInt(0) != 0)
	{
		for(int y = 0; y < browInterface->getWorldHeight(); y++)
		{
			for(int x = 0; x < browInterface->getWorldWidth(); x++)
			{
				cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
				c->tileOtherSide = -1;
				c->tileSide = -1;
				c->cell1 = 0;
				c->cell2 = 0;
				c->cell3 = 0;
				c->cell4 = 0;
			}
		}
		browInterface->fixNormals();
	}

	if(w->objects["chkWalls"]->getInt(0) != 0)
	{
		for(int y = 0; y < browInterface->getWorldHeight(); y++)
		{
			for(int x = 0; x < browInterface->getWorldWidth(); x++)
			{
				cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
				c->tileOtherSide = -1;
				c->tileSide = -1;
			}
		}
	}
	if(w->objects["chkGat"]->getInt(0) != 0)
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
	}
	if(w->objects["chkLightmaps"]->getInt(0) != 0)
	{
		int i;
		while(browInterface->lightmapCount() > 0)
			browInterface->removeLightmap(0);		
		cBrowInterface::cPluginLightmap* map = browInterface->newLightmap();
		for(i = 0; i < 256; i++)
			map->buf[i] = i < 64 ? 255 : 0;
		browInterface->addLightmap(map);


		
		for(i = 0; i < browInterface->tileCount(); i++)
			browInterface->getTile(i)->lightmap = 0;
	}
	if(w->objects["chkObjects"]->getInt(0) != 0)
	{
		while(browInterface->modelCount() > 0)
			browInterface->removeModel(0);
	}
	if(w->objects["chkSprites"]->getInt(0) != 0)
	{
		while(browInterface->spriteCount() > 0)
			browInterface->removeSprite(0);
	}


	if(w->objects["chkLights"]->getInt(0) != 0)
	{
		while(browInterface->lightCount() > 0)
			browInterface->removeLight(0);
	}
	if(w->objects["chkSounds"]->getInt(0) != 0)
	{
		while(browInterface->soundCount() > 0)
			browInterface->removeSound(0);
	}
	if(w->objects["chkEffects"]->getInt(0) != 0)
	{
		while(browInterface->effectCount() > 0)
			browInterface->removeEffect(0);
	}

	w->close();
}



void handleevent(cWindow* w, std::string name, std::string event)
{
	if(name == "btnOk" && event == "click")
		plugin->clickOk(w);

	else if(name.substr(0,3) == "btn")
	{
		std::map<std::string, cWindowObject*, std::less<std::string> >::iterator it;
		for(it = w->objects.begin(); it != w->objects.end(); it++)
		{
			if(it->first == "chk" + name.substr(3))
			{
				it->second->setInt(0,1);
			}
			else if(it->first.substr(0,3) == "chk")
				it->second->setInt(0,0);

		}
	}
}

bool cClearMapPlugin::action()
{
	plugin = this;
	cXmlWindow* w = (cXmlWindow*)browInterface->addXmlWindow("plugins/clearmap.xml");
	w->eventhandler = handleevent;
	return true;
}
