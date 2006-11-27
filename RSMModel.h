#ifndef __RSMMODEL_H__
#define __RSMMODEL_H__

#include "common.h"
#include "texture.h"
#include "filesystem.h"

class cRSMModel;

class cRSMModelFace
{
public:
	short v[3];
	short t[3];
	unsigned short texid;
	unsigned short todo;
	unsigned int todo2;
	unsigned int nsurf;
};

class cRSMModelFrame
{
public:
	int time;
	float quat[4];
};

class cBoundingbox
{
public:
	float bbmin[3];
	float bbmax[3];
	float bbrange[3];
};

class cRSMModelMesh
{
public:
	int nstep;
	void load(cFile*, cRSMModel*, bool);
	int todo;
	float ftodo[10];
	string name;
	string parent;
	vector<cTexture*> textures;
	float trans[22];

	int nVertices;
	vector<cVector3> vertices;
	int nTexCoord;
	vector<cVector3> texcoords;
	int nFaces;
	vector<cRSMModelFace> faces;
	int nFrameAnimations;
	vector<cRSMModelFrame> frames;
	cBoundingbox bb;

	void boundingbox(float* = NULL, bool = true);


	void draw(cBoundingbox*, float*, bool);
	cRSMModelMesh()
	{
		nstep = 0;
	}

};

class cRSMModel
{
public:
	~cRSMModel();
	string filename;
	string rofilename;

	cVector3 pos;
	cVector3 scale;
	cVector3 rot;

	void load(string filename);
	vector<cRSMModelMesh*>	meshes;
	vector<int>				fathers;
	vector<cTexture*> textures;
	void draw(bool = true);
	void draw2(cBoundingbox*, int, float*, bool);

	void boundingbox();

	cBoundingbox bb;


};

#endif