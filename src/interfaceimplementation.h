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
	
	cPluginTexture*			getTexture(int);
	int						textureCount();
	void					removeTexture(int);
	void					addTexture(cPluginTexture*);
	
	cPluginLightmap*		newLightmap();
	cPluginLightmap*		getLightmap(int);
	int						lightmapCount();
	void					removeLightmap(int);
	void					addLightmap(cPluginLightmap*);
	
	cPluginEffect*			getEffect(int);
	int						effectCount();
	void					removeEffect(int);
	void					addEffect(cPluginEffect);
	
	cPluginSound*			getSound(int);
	int						soundCount();
	void					removeSound(int);
	void					addSound(cPluginSound);
	
	cPluginLight*			getLight(int);
	int						lightCount();
	void					removeLight(int);
	void					addLight(cPluginLight);
	
	
	cPluginRSMModel*		loadModel(const char*);
	void					removeModel(int);
	void					addModel(cPluginRSMModel*);
	int						modelCount();



	void					removeSprite(int);
	int						spriteCount();
	
	
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
	
	cWindow*				addXmlWindow(const char*);
	const char*				inputWindow(const char*, const char*);
	void					messageWindow(const char*);
	bool					confirmWindow(const char*);
	cPluginBase*			getPlugin(const char*);
	const char*				getMapFile();
};


#endif
