#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__


#include <texture.h>

class cTextureLoader
{
public:
	virtual ~cTextureLoader() {};
	std::vector<std::string>		extensions;
	virtual	void					load(char*, int,cTexture*) = 0;
};




#endif
