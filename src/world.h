#ifndef __WORLD_H__
#define __WORLD_H__


#ifdef WIN32
#include <windows.h>
#include <gd/gd.h>
#else
#include <gd.h>
#endif
#include <GL/gl.h>
#include <list>
#include <vector>

#include <tinyxml/tinyxml.h>
#include "plugins/base/interface.h"

#include <bengine/forwards.h>
#include <bengine/util/serializabledata.h>

class cTexture;
class cTextureModel;
class cSprite;
class cRsmModel;


class cLightmap : public cBrowInterface::cPluginLightmap, public bEngine::util::cSerializable
{
public:
	cLightmap()
	{
		generated = false;
		generated2 = false;
	}
	~cLightmap()
	{
		if (generated)
			glDeleteTextures(1, &tid);
		generated = false;
		if (generated2)
			glDeleteTextures(1, &tid2);
		generated2 = false;
	}
	void regenerate()
	{
		del();
		del2();
	}
	int texId();
	int texId2();
	void del();
	void del2();

	bool generated;
	bool generated2;
	GLuint tid;
	GLuint tid2;

	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);
};

class cRealLightMap
{
public:
	cRealLightMap(int px, int py);
	int texId();
	int texId2();

	bool generated;
	bool generated2;
	GLuint tid;
	GLuint tid2;
	int x;
	int y;

	void reset();
};

class cTile : public cBrowInterface::cPluginTile, public bEngine::util::cSerializable
{
public:
	std::vector<std::vector<int> > lightsWithShadow;
	cTile() {};
	cTile(cBrowInterface::cPluginTile t);

	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);

};


class cCube : public cBrowInterface::cPluginCube, public bEngine::util::cSerializable
{
public:
	float maxHeight;
	float minHeight;
	bEngine::math::cVector3 normal;
	bEngine::math::cVector3 vNormal1;
	bEngine::math::cVector3 vNormal2;
	bEngine::math::cVector3 vNormal3;
	bEngine::math::cVector3 vNormal4;

	cCube();
	cCube(const cCube &other);
	void calcNormal();
	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);
};

class cTextureContainer : public cBrowInterface::cPluginTexture, public bEngine::util::cSerializable
{
public:
	cTextureContainer() {}
	cTextureContainer(const cBrowInterface::cPluginTexture& pTexture);


	bEngine::cTexture* texture;
	cBrowInterface::cRoString<40> RoFilename;
	cBrowInterface::cRoString<40> RoFilename2;
	GLuint texId();


	void generateTexture();
	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);
};

class cLight : public cBrowInterface::cPluginLight,  public bEngine::util::cSerializable
{
public:
	cLight() {};
	cLight(cBrowInterface::cPluginLight other);
	bool operator == (cLight other);

	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);
};


class cSound : public cBrowInterface::cPluginSound,  public bEngine::util::cSerializable
{
public:
	cSound(){};
	cSound(cBrowInterface::cPluginSound);
	bool operator == (cSound other);
	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);
};

class cEffect : public cBrowInterface::cPluginEffect, public bEngine::util::cSerializable
{
public:
	cEffect() {};
	cEffect(cBrowInterface::cPluginEffect e);
	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);
};


class cQuadTreeNode
{
public:
	int load(std::vector<bEngine::math::cVector3>&, int index, int level);
	void save(std::vector<bEngine::math::cVector3>&);
	void generate(float w, float h, float, float,int);
	void draw(int level);

	void recalculate();
	bEngine::math::cVector3 box1;
	bEngine::math::cVector3 box2;


	bEngine::math::cVector3 range1;
	bEngine::math::cVector3 range2;

	cQuadTreeNode* child1;
	cQuadTreeNode* child2;
	cQuadTreeNode* child3;
	cQuadTreeNode* child4;
	
	~cQuadTreeNode();
};


class cGatTile : public cBrowInterface::cPluginGatCube, public bEngine::util::cSerializable
{
public:
	cGatTile();
	cGatTile(const cGatTile&);
	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);
};

class cWater : public bEngine::util::cSerializable
{
public:
	float	height;
	int		type;
	float	amplitude;
	float	phase;
	float	surfaceCurve;
	int		animSpeed;

	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);
};

class cAmbientLight : public bEngine::util::cSerializable
{
public:
	//Correct light information. by Henko
	int lightLongitude;
	int lightLatitude;
	bEngine::math::cVector3	diffuse;
	bEngine::math::cVector3	ambient;
	float ambintensity;

	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);
};

class cWorld : public bEngine::util::cSerializable
{

public:
	cAmbientLight						ambientLight;
	cWater								water;
	bool								loaded;

	char								fileName[128];
	int									height;
	int									width;
	int									texSize;
	bool								quickSave;
	std::vector<cTextureContainer>		textures;
	std::vector<cLightmap*>				lightmaps;
	std::vector<cTile>					tiles;
	std::vector<std::vector<cCube> >	cubes;
	std::vector<std::vector<cGatTile> >		gattiles;
	std::vector<std::vector<cRealLightMap*> >		realLightmaps;

	std::vector<cRsmModel*>				models;
	std::vector<cLight>					lights;
	std::vector<cSound>					sounds;
	std::vector<cEffect>				effects;
	std::vector<cSprite*>				sprites;

	
	long								size;
	float								tileScale;
	
	int									lightmapWidth;
	int									lightmapHeight;
	int									gridSizeCell;
	
	cTextureModel*						light;
	cTextureModel*						light2;
	cTextureModel*						sound;
	cTextureModel*						effect;
	
	std::vector<bEngine::math::cVector3>				quadTreeFloats;
	void								setHeight();
	cQuadTreeNode* root;
	
	
	cWorld();
	~cWorld();
	void newEmpty(int,int);
	void								draw();
	void								load();
	void								save();
	void								unload();
	void								newWorld();
	void								clean();

	bool								checkSanity();

	void								fixGridding();
	bool								makeLightmapsUnique();
	bool								blackLightmaps();
	void								calcVertexNormals(int = 0, int = 0, int = -1, int=-1);
	std::vector<cCube*>					getWall(int,int,bool,bool);

	bEngine::util::cInStream &readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream &writeData(bEngine::util::cOutStream &outstream);
	

	cBrowInterface::cRoString<40>		iniFile;
	cBrowInterface::cRoString<40>		gndFile;
	cBrowInterface::cRoString<40>		gatFile;


	int									unknown1;
	int									unknown2;
	int									unknown3;
	int									unknown4;



};

#endif
