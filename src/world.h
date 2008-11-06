#ifndef __WORLD_H__
#define __WORLD_H__

#include <list>
#include "texture.h"
#ifdef WIN32
#include <gd/gd.h>
#else
#include <gd.h>
#endif
#include <tinyxml/tinyxml.h>
#include "RSMModel.h"
#include "texturecache.h"
#include "texturemodel.h"
#include "sprite.h"


class cLightmap
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
	char buf[256];
	int texId();
	int texId2();
	void del();
	void del2();

	bool generated;
	bool generated2;
	GLuint tid;
	GLuint tid2;
};

class cRealLightMap
{
public:
	cRealLightMap();
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

class cTile
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
	std::vector<std::vector<int> > lightsWithShadow;
};


class cCube
{
public:
	float	cell1,
			cell2,
			cell3,
			cell4;
	int tileUp,
		tileSide,
		tileOtherSide;

	float maxHeight;
	float minHeight;
	bool selected;

	void calcNormal()
	{
		cVector3 b1, b2;
		b1 = cVector3(10,-cell1,-10) - cVector3(0,-cell4,0);
		b2 = cVector3(0,-cell3,-10) - cVector3(0,-cell4,0);
		normal = b1.cross(b2).normalize();//cVector3(b1.y * b2.z - b1.z * b2.y, b1.z * b2.x - b1.x * b2.z, b1.x * b2.y - b1.y * b2.x);
	}
	cVector3 normal;
};

class cTextureContainer
{
public:
	cTexture* texture;
	std::string RoFilename;
	std::string RoFilename2;
	GLuint texId() { return texture->texId(); }

};

class cLight
{
public:
	std::string		name;
	cVector3	pos;
	std::string		todo;
	cVector3	color;
	float		todo2;
// custom properties

	float		range;
	float		maxLightIncrement;
	bool		givesShadow;
	float		lightFalloff;
	bool operator == (cLight other)
	{
		return	name == other.name &&
				pos == other.pos &&
				todo == other.todo &&
				color == other.color &&
				todo2 == other.todo2 &&
				range == other.range &&
				maxLightIncrement == other.maxLightIncrement &&
				givesShadow == other.givesShadow &&
				lightFalloff == other.lightFalloff;
	}
};


class cSound
{
public:
	std::string name;
	std::string todo1;
	std::string fileName;
	cVector3 pos;
	cVector3 rotation;
	cVector3 scale;

	float repeatDelay;
	float unknown2;
	long	unknown3;
	long	unknown4;
	float	unknown5;
	char	unknown6[6];
	float	unknown7;
	float	unknown8;

	bool operator == (cSound other)
	{
		return	name == other.name &&
				todo1 == other.todo1 &&
				fileName == other.fileName &&
				pos == other.pos &&
				rotation == other.rotation &&
				scale == other.scale &&
				repeatDelay == other.repeatDelay &&
				unknown2 == other.unknown2 &&
				unknown3 == other.unknown3 &&
				unknown4 == other.unknown4 &&
				unknown5 == other.unknown5 &&
				memcmp(unknown6,other.unknown6,6) == 0 &&
				unknown7 == other.unknown7 &&
				unknown8 == other.unknown8;
	}

};

class cEffect
{
public:
	std::string readablename;


	std::string name;
	float todo1;
	float todo2;
	float todo3;
	cVector3 rotation;
	cVector3 scale;
	std::string category;
	cVector3 pos;
	int	type;
	float loop;
	float todo10;
	float todo11;
	int todo12;
	int todo13;
};


class cQuadTreeNode
{
public:
	int load(std::vector<cVector3>&, int index, int level);
	void save(std::vector<cVector3>&);
	void generate(float w, float h, float, float,int);
	void draw(int level);

	void recalculate();
	cVector3 box1;
	cVector3 box2;


	cVector3 range1;
	cVector3 range2;

	cQuadTreeNode* child1;
	cQuadTreeNode* child2;
	cQuadTreeNode* child3;
	cQuadTreeNode* child4;
	
	~cQuadTreeNode();
};


class cGatTile
{
public:
	float	cell1,
			cell2,
			cell3,
			cell4;
	int type;
};

class cWater
{
public:
	float	height;
	int		type;
	float	amplitude;
	float	phase;
	float	surfaceCurve;
//	int		texcycle; // unknown
};

class cAmbientLight
{
public:
	int	ambientr;
	int	ambientg;
	int	ambientb;
	cVector3	diffuse;
	cVector3	shadow;
	float		alpha;
};

class cWorld
{

public:
	cAmbientLight						ambientLight;
	cWater								water;
	bool								loaded;

	char								fileName[128];
	bool								showTextures;

	int									height;
	int									width;
	int									texSize;
	bool								quickSave;
	std::vector<cTextureContainer*>		textures;
	std::vector<cLightmap*>				lightmaps;
	std::vector<cTile>					tiles;
	std::vector<std::vector<cCube> >	cubes;
	std::vector<std::vector<cGatTile> >		gattiles;
	std::vector<std::vector<cRealLightMap*> >		realLightmaps;

	std::vector<cRSMModel*>				models;
	std::vector<cLight>					lights;
	std::vector<cSound>					sounds;
	std::vector<cEffect>				effects;
	std::vector<cSprite*>				sprites;

	cWorld();
	~cWorld();
	void								draw();
	void								load();
	void								save();
	void								exportheight();
	void								unload();
	void								newWorld();


	void								clean();

	void								savelightmap();
	void								loadlightmap();


	long								size;
	float								tileScale;

	int									lightmapWidth;
	int									lightmapHeight;
	int									gridSizeCell;

	cTextureModel*						light;
	cTextureModel*						light2;
	cTextureModel*						sound;
	cTextureModel*						effect;
	
	std::vector<cVector3>				quadTreeFloats;
	cQuadTreeNode* root;


	void								fixGridding();
	void								makeLightmapsUnique();
	void								blackLightmaps();

	std::string							useless;

	std::list<int>						plugin_api_deleteobjects;
	std::list<int>						plugin_api_deletesprites;


};

#endif
