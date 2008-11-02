#ifndef __TEXTURECACHE_H__
#define __TEXTURECACHE_H__

#include "common.h"
#include <map>
#include <string>
#include "texture.h"

class cTextureCache
{
public:
	void status();
	std::map<std::string, cTexture*, std::less<std::string> >	textures;
	std::map<cTexture*, int, std::less<cTexture*> >	used;

	cTexture*	load(std::string, eTextureOptions = TEX_NORMAL);
	void		unload(cTexture*);

};

extern cTextureCache TextureCache;

#endif
