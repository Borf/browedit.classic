#ifndef __WORLD_H__
#define __WORLD_H__

#include "texture.h"
#include <gd/gd.h>
#include "rsmmodel.h"

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

class cTextureContainer : public cTexture
{
public:
	string RoFilename;
	string RoFilename2;
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


	cWorld()
	{
		showtextures = false;
		loaded = false;
	}
	~cWorld()
	{

	}
	void draw();
	void load();
	void save();
	void exportheight();

	void newworld();


	void clean();


	long size;
	float tilescale;

	int lightmapWidth;
	int lightmapHeight;
	int gridSizeCell;


};

#endif