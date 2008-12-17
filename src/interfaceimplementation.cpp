#include "interfaceimplementation.h"
#include <graphics.h>
#include <windows/xmlwindow.h>
#include <SDL/SDL.h>
#include <settings.h>

void cBrowInterfaceImplementation::fixNormals()
{
	cGraphics::world->calcVertexNormals();
}

cWindow* cBrowInterfaceImplementation::addXmlWindow(const char* fileName)
{
	TiXmlDocument windowLayout = cFileSystem::getXml(fileName);
	cXmlWindow* w = new cXmlWindow(windowLayout);
	cWM::addWindow(w);
	return w;
}

int cBrowInterfaceImplementation::getSelectedTextureIndex()
{
	return cGraphics::worldContainer->settings.texturestart + ((int)cGraphics::worldContainer->settings.selectionstart.y - 32) / 288;	
}


void cBrowInterfaceImplementation::render()
{
	cGraphics::draw();
	SDL_GL_SwapBuffers();
}

const char* cBrowInterfaceImplementation::inputWindow( const char* msg, const char* defaultText)
{
	return cWM::inputWindow(msg, defaultText).c_str();
}

void cBrowInterfaceImplementation::messageWindow( const char* msg)
{
	cWM::showMessage(msg);
}

cBrowInterface::cPluginRSMModel* cBrowInterfaceImplementation::loadModel( const char* )
{
	cRSMModel* model = new cRSMModel();
	model->load(cSettings::roDir +  "data\\model\\郴何家前\\枚促府.rsm");
	return model;	
}

void cBrowInterfaceImplementation::removeModel(int id)
{
	delete cGraphics::world->models[id];
	cGraphics::world->models.erase(cGraphics::world->models.begin() + id);
}

void cBrowInterfaceImplementation::removeSprite(int id)
{
	delete cGraphics::world->sprites[id];
	cGraphics::world->sprites.erase(cGraphics::world->sprites.begin() + id);
}

void cBrowInterfaceImplementation::addModel(cPluginRSMModel* model)
{
	cGraphics::world->models.push_back((cRSMModel*)model);
}

cBrowInterface::cPluginLightmap* cBrowInterfaceImplementation::getLightmap(int i)
{
	return cGraphics::world->lightmaps[i];
}

cBrowInterface::cPluginCube* cBrowInterfaceImplementation::getCube(int x, int y)
{
	return &cGraphics::world->cubes[y][x];
}

cBrowInterface::cPluginTexture* cBrowInterfaceImplementation::getTexture(int i)
{
	return cGraphics::world->textures[i];
}

cBrowInterface::cPluginTile* cBrowInterfaceImplementation::getTile(int i)
{
	return &cGraphics::world->tiles[i];
}


int cBrowInterfaceImplementation::tileCount()
{
	return cGraphics::world->tiles.size();
}
int cBrowInterfaceImplementation::effectCount()
{
	return cGraphics::world->effects.size();
}
int cBrowInterfaceImplementation::lightmapCount()
{
	return cGraphics::world->lightmaps.size();
}
int cBrowInterfaceImplementation::soundCount()
{
	return cGraphics::world->sounds.size();
}
int cBrowInterfaceImplementation::textureCount()
{
	return cGraphics::world->textures.size();
}

void cBrowInterfaceImplementation::addTile(cBrowInterface::cPluginTile tile)
{
	cGraphics::world->tiles.push_back(tile);
}
void cBrowInterfaceImplementation::addEffect(cBrowInterface::cPluginEffect effect)
{
	cGraphics::world->effects.push_back(effect);
}
void cBrowInterfaceImplementation::addTexture(cBrowInterface::cPluginTexture* pTexture)
{
	cGraphics::world->textures.push_back((cTextureContainer*)pTexture);
}
void cBrowInterfaceImplementation::addSound(cBrowInterface::cPluginSound sound)
{
	cGraphics::world->sounds.push_back(sound);
}
void cBrowInterfaceImplementation::addLight(cBrowInterface::cPluginLight light)
{
	cGraphics::world->lights.push_back(light);
}

void cBrowInterfaceImplementation::removeTile(int i)
{
	cGraphics::world->tiles.erase(cGraphics::world->tiles.begin()+i);
}
void cBrowInterfaceImplementation::removeEffect(int i)
{
	cGraphics::world->effects.erase(cGraphics::world->effects.begin()+i);
}
void cBrowInterfaceImplementation::removeTexture(int i)
{
	cTextureCache::unload(cGraphics::world->textures[i]->texture);
	cGraphics::world->textures.erase(cGraphics::world->textures.begin() + i);
}
void cBrowInterfaceImplementation::removeSound(int i)
{
	cGraphics::world->sounds.erase(cGraphics::world->sounds.begin()+i);
}
void cBrowInterfaceImplementation::removeLight(int i)
{
	cGraphics::world->lights.erase(cGraphics::world->lights.begin()+i);
}
void cBrowInterfaceImplementation::removeLightmap(int i)
{
	delete cGraphics::world->lightmaps[i];
	cGraphics::world->lightmaps.erase(cGraphics::world->lightmaps.begin()+i);
}

cBrowInterface::cPluginSound* cBrowInterfaceImplementation::getSound(int i)
{
	return &cGraphics::world->sounds[i];
}

cBrowInterfaceImplementation::cPluginEffect* cBrowInterfaceImplementation::getEffect(int i)
{
	return &cGraphics::world->effects[i];
}

void cBrowInterfaceImplementation::addLightmap(cPluginLightmap* p)
{
	cGraphics::world->lightmaps.push_back((cLightmap*)p);
}

cBrowInterface::cPluginLightmap* cBrowInterfaceImplementation::newLightmap()
{
	return new cLightmap();
}

void cBrowInterfaceImplementation::setWaterHeight(float f)
{
	cGraphics::world->water.height = f;
}

int cBrowInterfaceImplementation::getWorldHeight()
{
	return cGraphics::world->height;
}

int cBrowInterfaceImplementation::getWorldWidth()
{
	return cGraphics::world->width;
}

void cBrowInterfaceImplementation::makeTilesUnique()
{
	cGraphics::world->makeLightmapsUnique();
}

void cBrowInterfaceImplementation::fixGridding()
{
	cGraphics::world->fixGridding();
}

int cBrowInterfaceImplementation::spriteCount()
{
	return cGraphics::world->sprites.size();
}

int cBrowInterfaceImplementation::modelCount()
{
	return cGraphics::world->models.size();
}

int cBrowInterfaceImplementation::lightCount()
{
	return cGraphics::world->lights.size();
}

cBrowInterface::cPluginLight* cBrowInterfaceImplementation::getLight(int index)
{
	return &cGraphics::world->lights[index];
}

cBrowInterface::cPluginGatCube* cBrowInterfaceImplementation::getGatCube(int x, int y)
{
	return &cGraphics::world->gattiles[y][x];
}