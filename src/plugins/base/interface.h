#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <mymath.h>
#include <string>//TODO: remove this :P

class cWindow;
class cPluginBase;


class cBrowInterface
{
public:
	class cPluginCube
	{
	public:
		float	cell1,
				cell2,
				cell3,
				cell4;
		int		tileUp,
				tileSide,
				tileOtherSide;
		bool	selected;

		bool	hasModelOnTop;//needs to be calculated

	};
	class cPluginTile
	{
	public:
		float	u1,
				u2,
				u3,
				u4,
				v1,
				v2,
				v3,
				v4;
		int texture;
		int lightmap;
		char color[4];
		bool used;
	};
	class cPluginTexture
	{

	};
	class cPluginLightmap
	{
	public:
		char buf[256];
		virtual void regenerate() = 0;
	};

	
	class cPluginObject
	{
	public:
		cVector3 pos;
		cVector3 scale;
		cVector3 rot;
	};

	class cPluginRSMModel : public cPluginObject
	{
	};

	class cPluginLight : public cPluginObject
	{
	public:
		std::string		name;
		std::string		todo;
		cVector3	color;
		float		todo2;
		// custom properties
		
		float		range;
		float		maxLightIncrement;
		bool		givesShadow;
		float		lightFalloff;
	};
	class cPluginEffect : public cPluginObject
	{
	public:
		std::string readablename;
		std::string name;
		float todo1;
		float todo2;
		float todo3;
		std::string category;
		int	type;
		float loop;
		float todo10;
		float todo11;
		int todo12;
		int todo13;
	};
	class cPluginSound : public cPluginObject
	{
	public:
		std::string name;
		std::string todo1;
		std::string fileName;
		
		float repeatDelay;
		float unknown2;
		long	unknown3;
		long	unknown4;
		float	unknown5;
		char	unknown6[6];
		float	unknown7;
		float	unknown8;
	};

	class cPluginGatCube
	{
	public:
		float	cell1,
				cell2,
				cell3,
				cell4;
		int type;
	};

	
	
	
	virtual cPluginCube*			getCube(int x, int y) = 0;
	virtual cPluginGatCube*			getGatCube(int x, int y) = 0;

	virtual cPluginTile*			getTile(int index) = 0;
	virtual int						tileCount() = 0;
	virtual void					removeTile(int) = 0;
	virtual void					addTile(cPluginTile) = 0;
	virtual void					clearTiles() = 0;

	virtual cPluginTexture*			getTexture(int) = 0;
	virtual int						textureCount() = 0;
	virtual void					removeTexture(int) = 0;
	virtual void					addTexture(cPluginTexture*) = 0;
	virtual void					clearTextures() = 0;

	virtual cPluginLightmap*		newLightmap() = 0;
	virtual cPluginLightmap*		getLightmap(int) = 0;
	virtual int						lightmapCount() = 0;
	virtual void					removeLightmap(int) = 0;
	virtual void					addLightmap(cPluginLightmap*) = 0;
	virtual void					clearLightmaps() = 0;

	virtual cPluginEffect*			getEffect(int) = 0;
	virtual int						effectCount() = 0;
	virtual void					removeEffect(int) = 0;
	virtual void					addEffect(cPluginEffect) = 0;
	virtual void					clearEffects() = 0;

	virtual cPluginSound*			getSound(int) = 0;
	virtual int						soundCount() = 0;
	virtual void					removeSound(int) = 0;
	virtual void					addSound(cPluginSound) = 0;
	virtual void					clearSounds() = 0;

	virtual cPluginLight*			getLight(int) = 0;
	virtual int						lightCount() = 0;
	virtual void					removeLight(int) = 0;
	virtual void					addLight(cPluginLight) = 0;
	virtual void					clearLights() = 0;



	virtual cPluginRSMModel*		loadModel(const char* fileName) = 0;
	virtual int						modelCount() = 0;
	virtual void					removeModel(int index) = 0;
	virtual void					addModel(cPluginRSMModel* model) = 0;
	virtual void					clearModels() = 0;

	
	virtual int						spriteCount() = 0;
	virtual void					removeSprite(int) = 0;
	virtual void					clearSprites() = 0;


	virtual void					fixNormals() = 0;
	virtual void					fixGridding() = 0;
	virtual void					checkModelsOnCubes() = 0;
	virtual void					render() = 0;
	virtual void					makeTilesUnique() = 0;
	virtual void					makeLightmapsUnique() = 0; // make tiles unique before running this method
	virtual void					makeLightmapsBlack() = 0;


	virtual cWindow*				addXmlWindow(const char* fileName) = 0;
	virtual const char*				inputWindow(const char* message, const char* defaultValue) = 0;
	virtual bool					confirmWindow(const char* message) = 0;
	virtual void					messageWindow(const char* message) = 0;

	virtual int						getSelectedTextureIndex() = 0;
	virtual int						getWorldHeight() = 0;
	virtual int						getWorldWidth() = 0;
	virtual void					setWaterHeight(float) = 0;

	virtual cPluginBase*			getPlugin(const char* name) = 0;
	virtual const char*				getMapFile() = 0;

	
};


#endif
