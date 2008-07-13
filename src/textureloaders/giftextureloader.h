#ifndef __GIFTEXTURELOADER_H__
#define __GIFTEXTURELOADER_H__

#include "textureloader.h"

class cGifTextureLoader : public cTextureLoader
{
public:
	cGifTextureLoader();
	void load(char*, int,cTexture*);
};


#endif
