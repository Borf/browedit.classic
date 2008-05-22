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
	map<string, cTexture*, less<string> >	textures;
	map<cTexture*, int, less<cTexture*> >	used;

	cTexture*	load(string, bool = false);
	void		unload(cTexture*);

};

extern cTextureCache TextureCache;

#endif
