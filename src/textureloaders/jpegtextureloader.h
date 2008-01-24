#ifndef __JPEGTEXTURELOADER_H__
#define __JPEGTEXTURELOADER_H__

#include "textureloader.h"

class cJpegTextureLoader : public cTextureLoader
{
public:
	cJpegTextureLoader();
	void load(char*, int,cTexture*);
};


#endif