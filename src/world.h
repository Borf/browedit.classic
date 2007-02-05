#ifndef __WORLD_H__
#define __WORLD_H__

#include "texture.h"
#ifdef WIN32
#include <gd/gd.h>
#else
#include <gd.h>
#endif
#include "RSMModel.h"
#include "texturecache.h"
#include "texturemodel.h"

class cLightmap
{
public:
	cLightmap()
	{
		generated = false;
		generated2 = false;
	}
	~cLightmap();
	char buf[256];
	int texid();
	int texid2();
	void del();
	void del2();

	bool generated;
	bool generated2;
	GLuint tid;
	GLuint tid2;
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

	float maxh;
	float minh;

	void calcnormal();
	cVector3 normal;
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
	string readablename;


	string name;
	float todo1;
	float todo2;
	float todo3;
	cVector3 rotation;
	cVector3 scale;
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
	void save(vector<cVector3>&);
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
	float	surfacecurve;
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
	cAmbientLight ambientlight;
	cWater water;
	bool loaded;

	char filename[128];
	bool showtextures;

	int height;
	int width;
	int texsize;
	vector<cTextureContainer*> textures;
	vector<cLightmap*> lightmaps;
	vector<cTile> tiles;
	vector<vector<cCube> > cubes;
	vector<vector<cGatTile> > gattiles;

	vector<cRSMModel*>	models;

	vector<cLight>		lights;
	vector<cSound>		sounds;
	vector<cEffect>		effects;

	cTexture*			notile;

	cWorld()
	{
		showtextures = false;
		loaded = false;
		light = NULL;
		sound = NULL;
		effect = NULL;
		notile = NULL;
	}
	~cWorld()
	{
		unload();
	}
	void draw();
	void load();
	void save();
	void importalpha();
	void exportheight();

	void unload();

	void newworld();


	void clean();

	void savelightmap();
	void loadlightmap();


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
