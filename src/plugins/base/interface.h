#ifndef __INTERFACE_H__
#define __INTERFACE_H__

class cWindow;
#include <mymath.h>
#include <string>//TODO

class cBrowInterface
{
public:
	class cPluginCube
	{

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

	
	
	
	virtual cPluginCube*			getCube(int, int) = 0;

	virtual cPluginTile*			getTile(int) = 0;
	virtual int						tileCount() = 0;
	virtual void					removeTile(int) = 0;
	virtual void					addTile(cPluginTile) = 0;

	virtual cPluginTexture*			getTexture(int) = 0;
	virtual int						textureCount() = 0;
	virtual void					removeTexture(int) = 0;
	virtual void					addTexture(cPluginTexture*) = 0;

	virtual cPluginLightmap*		getLightmap(int) = 0;
	virtual int						lightmapCount() = 0;
	virtual void					removeLightmap(int) = 0;
	virtual void					addLightmap(cPluginLightmap*) = 0;

	virtual cPluginEffect*			getEffect(int) = 0;
	virtual int						effectCount() = 0;
	virtual void					removeEffect(int) = 0;
	virtual void					addEffect(cPluginEffect) = 0;

	virtual cPluginSound*			getSound(int) = 0;
	virtual int						soundCount() = 0;
	virtual void					removeSound(int) = 0;
	virtual void					addSound(cPluginSound) = 0;


	virtual cPluginRSMModel* loadModel(const char*) = 0;
	virtual void removeModel(int) = 0;
	virtual void addModel(cPluginRSMModel*) = 0;
	virtual void removeSprite(int) = 0;


	virtual void fixNormals() = 0;
	virtual int  getSelectedTextureIndex() = 0;
	virtual void render() = 0;

	virtual cWindow* addXmlWindow(const char*) = 0;
	virtual const char* inputWindow(const char*, const char*) = 0;
	virtual void messageWindow(const char*) = 0;

	
};


#endif
