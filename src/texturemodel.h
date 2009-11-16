#ifndef __TEXTUREMODEL_H__
#define __TEXTUREMODEL_H__

class cTexture;
#include <mymath.h>

class cTextureModel
{
private:
	cTexture* tex;
public:
	cVector3 boundingbox1;
	cVector3 boundingbox2;

	cTextureModel();
	virtual ~cTextureModel();

	int open(std::string);
	int close();
	int draw(float x = 0.0f, float y = 0.0f, float z = 0.0f, float scale = 1.0f);
	int setstate(int);
};


#endif
