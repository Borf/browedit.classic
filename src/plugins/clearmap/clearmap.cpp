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
		browInterface->clearLightmaps();
		browInterface->clearTiles();
		browInterface->clearTextures();


		cBrowInterface::cPluginLightmap* map = browInterface->newLightmap();
		for(i = 0; i < 256; i++)
			map->buf[i] = i < 64 ? 255 : 0;
		browInterface->addLightmap(map);

		for(y = 0; y < browInterface->getWorldHeight(); y++)
		{
			for(x = 0; x < browInterface->getWorldWidth(); x++)
			{
				cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
				c->tileUp = -1;
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
		browInterface->clearLightmaps();
		cBrowInterface::cPluginLightmap* map = browInterface->newLightmap();
		for(i = 0; i < 256; i++)
			map->buf[i] = i < 64 ? 255 : 0;
		browInterface->addLightmap(map);

		for(i = 0; i < browInterface->tileCount(); i++)
			browInterface->getTile(i)->lightmap = 0;

		browInterface->resetLightmaps();
	}
	if(w->objects["chkObjects"]->getInt(0) != 0)
		browInterface->clearModels();
	if(w->objects["chkSprites"]->getInt(0) != 0)
		browInterface->clearSprites();
	if(w->objects["chkLights"]->getInt(0) != 0)
		browInterface->clearLights();
	if(w->objects["chkSounds"]->getInt(0) != 0)
		browInterface->clearSounds();
	if(w->objects["chkEffects"]->getInt(0) != 0)
		browInterface->clearEffects();

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
	if(!w)
		browInterface->messageWindow("Error: could not open plugins/clearmap.xml");
	else
		w->eventhandler = handleevent;
	return true;
}
