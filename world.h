#ifndef __WORLD_H__
#define __WORLD_H__

#include "texture.h"
#include <gd/gd.h>
#include "rsmmodel.h"
#include "texturecache.h"
#include "texturemodel.h"

class cLightmap
{
public:
	char buf[256];
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
};


class cCube
{
public:
	float	cell1,
			cell2,
			cell3,
			cell4;
	int tileup,
		tileside,
		tileaside;
};

class cTextureContainer
{
public:
	cTexture* texture;
	string RoFilename;
	string RoFilename2;
	GLuint texid() { return texture->texid(); }

};

class cLight
{
public:
	string name;
	cVector3 pos;
	string todo;
	cVector3 color;
	float todo2;
};


class cSound
{
public:
	string name;
	string todo1;
	string filename;
	string todo2;
	cVector3 pos;
	string id;
};

class cEffect
{
public:
	string name;
	float todo1;
	float todo2;
	float todo3;
	float todo4;
	float todo5;
	float todo6;
	float todo7;
	float todo8;
	float todo9;
	string category;
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
	int load(vector<cVector3>&, int index, int level);
	void draw(int level);

	void calculate();
	cVector3 box1;
	cVector3 box2;


	cVector3 range1;
	cVector3 range2;

	cQuadTreeNode* child1;
	cQuadTreeNode* child2;
	cQuadTreeNode* child3;
	cQuadTreeNode* child4;
};


class cWorld
{

public:
	bool loaded;

	char filename[128];
	bool showtextures;

	int height;
	int width;
	int texsize;
	vector<cTextureContainer*> textures;
	vector<cLightmap> lightmaps;
	vector<cTile> tiles;
	vector<vector<cCube> > cubes;

	vector<cRSMModel*>	models;

	vector<cLight>		lights;
	vector<cSound>		sounds;
	vector<cEffect>		effects;


	cWorld()
	{
		showtextures = false;
		loaded = false;
		light = NULL;
		sound = NULL;
		effect = NULL;
	}
	~cWorld()
	{
		unload();
	}
	void draw();
	void load();
	void save();
	void exportheight();

	void unload();

	void newworld();


	void clean();


	long size;
	float tilescale;

	int lightmapWidth;
	int lightmapHeight;
	int gridSizeCell;

	cTextureModel* light;
	cTextureModel* sound;
	cTextureModel* effect;
	
	vector<cVector3> quadtreefloats;
	cQuadTreeNode* root;


	string useless;

};

#endif