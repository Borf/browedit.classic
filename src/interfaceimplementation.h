#ifndef __INTERFACEIMPLEMENTATION_H__
#define __INTERFACEIMPLEMENTATION_H__

#include "plugins/base/interface.h"
class cPluginBase;

class cBrowInterfaceImplementation : public cBrowInterface
{
public:
	cPluginCube*			getCube(int, int);
	cPluginGatCube*			getGatCube(int, int);
	
	cPluginTile*			getTile(int);
	int						tileCount();
	void					removeTile(int);
	void					addTile(cPluginTile);
	void					clearTiles();
	
	cPluginTexture*			getTexture(int);
	int						textureCount();
	void					removeTexture(int);
	void					addTexture(cPluginTexture*);
	void					clearTextures();
	
	cPluginLightmap*		newLightmap();
	cPluginLightmap*		getLightmap(int);
	int						lightmapCount();
	void					removeLightmap(int);
	void					addLightmap(cPluginLightmap*);
	void					clearLightmaps();

	cPluginEffect*			getEffect(int);
	int						effectCount();
	void					removeEffect(int);
	void					addEffect(cPluginEffect);
	void					clearEffects();
	
	cPluginSound*			getSound(int);
	int						soundCount();
	void					removeSound(int);
	void					addSound(cPluginSound);
	void					clearSounds();
	
	cPluginLight*			getLight(int);
	int						lightCount();
	void					removeLight(int);
	void					addLight(cPluginLight);
	void					clearLights();
	
	
	cPluginRSMModel*		loadModel(const char*);
	void					removeModel(int);
	void					addModel(cPluginRSMModel*);
	int						modelCount();
	void					clearModels();



	void					removeSprite(int);
	int						spriteCount();
	void					clearSprites();
	
	
	int						getWorldHeight();
	int						getWorldWidth();
	void					setWaterHeight(float);
	int						getSelectedTextureIndex();
	
	void					makeTilesUnique();
	void					makeLightmapsUnique();
	void					makeLightmapsBlack();
	void					fixGridding();
	void					fixNormals();
	void					render();
	void					checkModelsOnCubes();
	void					resetLightmaps();
	
	cWindow*				addXmlWindow(const char*);
	const char*				inputWindow(const char*, const char*);
	void					messageWindow(const char*);
	bool					confirmWindow(const char*);
	cPluginBase*			getPlugin(const char*);
	const char*				getMapFile();

	std::string				getOpenFile(const char* defaultFilename, const char* filters);
	std::string				getSaveFile(const char* defaultFilename, const char* filters);
	void					Log(int lvl, const char*, ...);

};


#endif
