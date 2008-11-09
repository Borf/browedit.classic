#include "giftextureloader.h"
#ifdef __MINGW32__
#include <GL/glext.h> // for GL_BGR_EXT
#endif

#ifdef _WIN32
#include <gd/gd.h>
#else
#include <gd.h>
#endif


cGifTextureLoader	GifTextureLoader;


cGifTextureLoader::cGifTextureLoader()
{
	extensions.push_back(".gif");
	GetTextureLoaders().loaders.push_back(this);
}

void cGifTextureLoader::load(char* filedata,int datlen, cTexture* tex)
{
	gdImage* pImageData = gdImageCreateFromGifPtr(datlen, filedata);
	if(!pImageData)
		return;
	tex->data = new BYTE[pImageData->sx*pImageData->sy*3];

	tex->widthOriginal = tex->width = pImageData->sx;
	tex->heightOriginal = tex->height = pImageData->sy;
	tex->bpp = 24;
	tex->datatype = GL_BGR_EXT;


	int color,x,y;

	for(x = 0; x < pImageData->sx; x++)
	{
		for(y = 0; y < pImageData->sy; y++)
		{
			color = gdImagePalettePixel(pImageData, x, pImageData->sy - y-1);
			tex->data[3*x+3*pImageData->sx*y+0] = gdImageRed(pImageData, color);
			tex->data[3*x+3*pImageData->sx*y+1] = gdImageGreen(pImageData, color);
			tex->data[3*x+3*pImageData->sx*y+2] = gdImageBlue(pImageData, color);
//			memcpy(tex->data+xx+width*y, (void*)((&color)), 3);
		}
	}
	gdImageDestroy(pImageData);
}
