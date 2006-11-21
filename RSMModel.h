#ifndef __RSMMODEL_H__
#define __RSMMODEL_H__

#include "common.h"

class cRSMModelMesh
{

};

class cRSMModel
{
public:
	cVector3 pos;
	cVector3 scale;
	cVector3 rot;

	void load(string filename);
	vector<cRSMModelMesh*> meshes;
	void draw();
};

#endif