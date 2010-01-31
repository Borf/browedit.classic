#ifndef __TEXTUREMODEL_H__
#define __TEXTUREMODEL_H__

#include <bengine/forwards.h>
#include <bengine/math/vector3.h>

class cTextureModel
{
private:
	bEngine::cTexture* tex;
public:
	bEngine::math::cVector3 boundingbox1;
	bEngine::math::cVector3 boundingbox2;

	cTextureModel();
	virtual ~cTextureModel();

	int open(std::string);
	int close();
	int draw(float x = 0.0f, float y = 0.0f, float z = 0.0f, float scale = 1.0f);
	int setstate(int);
};


#endif
