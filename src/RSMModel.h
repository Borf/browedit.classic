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
	vector<int> textures;
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
	cBoundingbox realbb;

	void boundingbox(float* = NULL, bool = true);


	void draw(cBoundingbox*, float*, bool, cRSMModel*, bool,bool,bool);
	bool collides(cBoundingbox*, float*, bool, cRSMModel*, cVector3, cVector3);
	cRSMModelMesh()
	{
		nstep = 0;
	}

};

class cRSMModel
{
public:
	~cRSMModel();
	cRSMModel()
	{
		selected = false;
	}
	string filename;
	string rofilename;

	cVector3 pos;
	cVector3 scale;
	cVector3 rot;

	void load(string filename);
	vector<cRSMModelMesh*>	meshes;
	vector<int>				fathers;
	vector<cTexture*> textures;
	bool collides(cVector3, cVector3);
	bool collides2(cBoundingbox*, int, float*, bool, cVector3, cVector3);
	void precollides();

	void draw(bool = true, bool = true,bool=false,bool=false);
	void draw2(cBoundingbox*, int, float*, bool,bool,bool,bool);

	void boundingbox();
	bool recalcbb;
	bool selected;



	cBoundingbox bb;
	cBoundingbox bb2;


	float absolutebbv1_[3];
	float absolutebbv2_[3];
	float absolutebbv3_[3];
	float absolutebbv4_[3];
	float absolutebbv5_[3];
	float absolutebbv6_[3];
	float absolutebbv7_[3];
	float absolutebbv8_[3];
};

#endif
