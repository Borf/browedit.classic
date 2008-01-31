#include "jpegtextureloader.h"
#ifdef __MINGW32__
#include <GL/glext.h> // for GL_BGR_EXT
#endif

#ifdef _WIN32
#include <gd/gd.h>
#else
#include <gd.h>
#endif


cJpegTextureLoader	JpegTextureLoader;


cJpegTextureLoader::cJpegTextureLoader()
{
	extensions.push_back(".jpg");
	GetTextureLoaders().loaders.push_back(this);
}

void cJpegTextureLoader::load(char* filedata,int datlen, cTexture* tex)
{
	gdImage* pImageData = gdImageCreateFromJpegPtr(datlen, filedata);
	if(!pImageData)
		return;
	tex->data = new BYTE[pImageData->sx*pImageData->sy*3];

	tex->widthOriginal = tex->width = pImageData->sx;
	tex->heightOriginal = tex->height = pImageData->sy;
	tex->bpp = 24;
	tex->datatype = GL_BGR_EXT;


	int color,x,y;
	int width = pImageData->sx*3;

	for(x = 0; x < pImageData->sx; x++)
	{
		int xx = 3*x;
		for(y = 0; y < pImageData->sy; y++)
		{
			color = gdImageTrueColorPixel(pImageData, x, pImageData->sy - y-1);
	//		data[3*x+3*pImageData->sx*y+0] = gdTrueColorGetRed(color);
	//		data[3*x+3*pImageData->sx*y+1] = gdTrueColorGetGreen(color);
	//		data[3*x+3*pImageData->sx*y+2] = gdTrueColorGetBlue(color);
			memcpy(tex->data+xx+width*y, (void*)((&color)), 3);
		}
	}
	gdImageDestroy(pImageData);
}
