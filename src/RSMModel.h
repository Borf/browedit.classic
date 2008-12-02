#ifndef __RSMMODEL_H__
#define __RSMMODEL_H__

#include "texture.h"
#include "filesystem.h"
#include <mymath.h>

class cRSMModel;

class cRSMModelFace
{
public:
	unsigned short v[3];
	unsigned short t[3];
	unsigned short texid;
	unsigned short todo;
	unsigned int todo2;
	unsigned int nsurf;
	cVector3	 normal;
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
	
	cBoundingbox()
	{
		for(int i = 0; i < 3; i++)
		{
			bbmin[i] = 0;
			bbmax[i] = 0;
			bbrange[i] = 0;
		}
	}
	cBoundingbox(cBoundingbox &other)
	{
		for(int i = 0; i < 3; i++)
		{
			bbmin[i] = other.bbmin[i];
			bbmax[i] = other.bbmax[i];
			bbrange[i] = other.bbrange[i];
		}
	}
};

class cRSMModelMesh
{
public:
	int nstep;
	void load(cFile*, cRSMModel*, bool);
	int todo;
	float ftodo[10];
	std::string name;
	std::string parent;
	std::vector<int> textures;
	float trans[22];

	unsigned int nVertices;
	std::vector<cVector3> vertices;
	unsigned int nTexCoord;
	std::vector<cVector3> texcoords;
	unsigned int nFaces;
	std::vector<cRSMModelFace> faces;
	unsigned int nFrameAnimations;
	std::vector<cRSMModelFrame> frames;
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
#ifdef DISPLAYLIST
		displaylisted = false;
		displaylist = 0;
#endif
	}
	std::string name;
	std::string filename;
	std::string rofilename;

	cVector3 pos;
	cVector3 scale;
	cVector3 rot;

	void load(std::string filename);
	std::vector<cRSMModelMesh*>	meshes;
	std::vector<int>				fathers;
	std::vector<cTexture*> textures;
	bool collides(cVector3, cVector3);
	bool collides2(cBoundingbox*, int, float*, bool, cVector3, cVector3);
	void precollides();

	void draw(bool = true, bool = true,bool=false,bool=false);
	void draw2(cBoundingbox*, int, float*, bool,bool,bool,bool);

	void boundingbox();
	bool recalcbb;
	bool selected;


	float lightopacity;

	bool animated;
#ifdef DISPLAYLIST
	bool displaylisted;
	GLuint displaylist;
#endif


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
