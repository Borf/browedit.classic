#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__


#include <common.h>
#include <texture.h>

class cTextureLoader
{
public:
	vector<string>					extensions;
	virtual	void					load(char*, int,cTexture*) = 0;
};




#endif
