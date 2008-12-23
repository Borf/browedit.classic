#include "common.h"
#include "texturecache.h"

std::map<std::string, cTexture*, std::less<std::string> >	cTextureCache::textures;
std::map<cTexture*, int, std::less<cTexture*> >				cTextureCache::used;


cTexture* cTextureCache::load(std::string filename, eTextureOptions options)
{
	if (textures.find(filename) == textures.end())
	{
		cTexture* tex = cTextureLoaders::load(filename,options);
		textures[filename] = tex;
		
		used[tex] = 1;
	}
	else
		used[textures[filename]]++;
	return textures[filename];

}

void cTextureCache::unload(cTexture* tex)
{
	if (used.find(tex) == used.end())
	{
		Log(3,0,GetMsg("graphics/texturecache/UNLOADFAIL"), tex->getfilename().c_str());
		return;
	}
	used[tex]--;
	if (used[tex] == 0)
	{
		//Log(3,0,"Texture %s seems to be unused", tex->getfilename().c_str());
		textures[tex->getfilename()]->unLoad();
		textures.erase(textures.find(tex->getfilename()));
		used.erase(used.find(tex));
		delete tex;
	}
	if (used[tex] < 0)
	{
		Log(3,0,GetMsg("graphics/texturecache/UNLOADALOT"), tex->getfilename().c_str());
	}
}


void cTextureCache::status()
{
	int count = 0;
	for(std::map<std::string, cTexture*, std::less<std::string> >::iterator i = textures.begin(); i != textures.end(); i++)
	{
		if (used[i->second] > 0)
		{
			Log(3,0,GetMsg("graphics/texturecache/STILLLOADED"), i->first.c_str(), used[i->second]);
			count++;
		}
	}
	if(count == 0)
		Log(3,0,"TextureCache: No textures left (that's a good thing when you're closing down)");

}

void cTextureCache::unloadall()
{
	for(std::map<std::string, cTexture*, std::less<std::string> >::iterator i = textures.begin(); i != textures.end(); i++)
	{
		i->second->unLoad();
	}
	textures.clear();
	used.clear();
}
