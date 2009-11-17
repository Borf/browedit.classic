#include <common.h>
#include "tgatextureloader.h"
#ifdef __MINGW32__
#include <GL/glext.h> // for GL_BGR_EXT & GL_BGRA_EXT
#endif

cTGATextureLoader	tgaTextureLoader;


cTGATextureLoader::cTGATextureLoader()
{
	extensions.push_back(".tga");
	GetTextureLoaders().loaders.push_back(this);
}


void cTGATextureLoader::load(char* filedata, int length, cTexture* tex)
{
	TGAHeader tgaheader;

	memcpy(&tgaheader, filedata, sizeof(tgaheader));

	
	if (tgaheader.Header[2] == 2)
		LoadUncompressedTGA(filedata+sizeof(tgaheader), tex);
	if (tgaheader.Header[2] == 10)
	{
		Log(1,0,"Unsupported TGA: %s", tex->getfilename().c_str());
//		LoadCompressedTGA(filedata+sizeof(tgaheader));
	}

}



void cTGATextureLoader::LoadUncompressedTGA(char* filedata, cTexture* tex)
{
	GLubyte header[6];
	int index = 0;
	memcpy(&header, filedata, sizeof(header));
	tex->widthOriginal = tex->width  = header[1] * 256 + header[0];
	tex->heightOriginal= tex->height = header[3] * 256 + header[2];
	tex->bpp = header[4];	
	index+=sizeof(header);

	tex->data = new BYTE[tex->width * tex->height * (tex->bpp/8)];

	memcpy(tex->data, filedata+index, tex->width * tex->height * (tex->bpp/8));

	if(tex->bpp == 32)
		tex->datatype = GL_BGRA_EXT;
	else if(tex->bpp == 24)
		tex->datatype = GL_BGR_EXT;
	else
		Log(3,0,"Unsupported bitdepth in tga file: %s:%i", tex->getfilename().c_str(), tex->bpp);
}
