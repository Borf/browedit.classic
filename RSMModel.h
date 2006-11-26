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

class cRSMModelMesh
{
public:
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


	float bbmin[3];
	float bbmax[3];
	float range[3];

	void boundingbox(float* = NULL);


	void draw();

};

class cRSMModel
{
public:
	cVector3 pos;
	cVector3 scale;
	cVector3 rot;

	void load(string filename);
	vector<cRSMModelMesh*> meshes;
	vector<cTexture*> textures;
	void draw();
};

#endif