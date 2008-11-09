#ifndef __TEXTURECACHE_H__
#define __TEXTURECACHE_H__

#include "common.h"
#include <map>
#include <string>
#include "texture.h"

class cTextureCache
{
public:
	static void status();
	static cTexture*													load(std::string, eTextureOptions = TEX_NORMAL);
	static void															unload(cTexture*);
private:
	static std::map<std::string, cTexture*, std::less<std::string> >	textures;
	static std::map<cTexture*, int, std::less<cTexture*> >				used;

};

#endif
