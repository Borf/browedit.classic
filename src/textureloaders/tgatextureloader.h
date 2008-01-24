#ifndef __TGATEXTURELOADER_H__
#define __TGATEXTURELOADER_H__


#include "textureloader.h"

class cTexture;

class cTGATextureLoader : public cTextureLoader
{
private:
	typedef struct
	{
		GLubyte Header[12];
	} TGAHeader;


	void LoadUncompressedTGA(char*, cTexture*);



public:
	cTGATextureLoader();
	void load(char*, int,cTexture*);
};




#endif
