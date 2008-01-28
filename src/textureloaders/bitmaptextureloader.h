#ifndef __BITMAPTEXTURELOADER_H__
#define __BITMAPTEXTURELOADER_H__


#include "textureloader.h"



class cBitmapTextureLoader : public cTextureLoader
{
public:
	cBitmapTextureLoader();
	void load(char*, int,cTexture*);
};


#endif
