#include "texture.h"
#ifdef _WIN32
#include <gd/gd.h>
#else
#include <gd.h>
#endif

extern cFileSystem fs;

int cTexture::Load(string Filename, bool freedat)
{
	loaded = false;
	freedata = freedat;
	filename = Filename;
	if (freedat == false)
		return texid();
	return 1;
}

GLuint cTexture::texid()
{
	if (!loaded && filename != "")
	{
		if (filename.find(".tga") != string::npos || filename.find(".TGA") != string::npos)
			LoadFromTGA(filename);
		else if (lcase(filename).find(".jpg") != string::npos)
			LoadFromJpeg(filename);
		else if (lcase(filename).find(".bmp") != string::npos)
		{
			Log(3,0,"Loading %s...", filename.c_str());
			BMPClass bmp;
			int ret = BMPLoad(filename,bmp);
			if (ret == 0)
			{
				glGenTextures(1, &tid);
				glBindTexture(GL_TEXTURE_2D, tid);
				glTexImage2D(GL_TEXTURE_2D,0,3,bmp.width,bmp.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp.bytes);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				Log(3,0,"Loaded %s successfully", filename.c_str());
			}
			else
				Log(3,0,"Couldn't load %s", filename.c_str());
			
		}
		else
			Log(2,0,"Error loading texture: %s", filename.c_str());
		loaded = true;
	}
	return tid;
}

int cTexture::unLoad()
{
	if (!freedata)
		free(imageData);
	glDeleteTextures(1, &tid);
	loaded = false;
	return 1;
}




typedef struct
{
	GLubyte Header[12];
} TGAHeader;


typedef struct
{
	GLubyte		header[6];
	GLuint		bytesPerPixel;
	GLuint		imageSize;
	GLuint		temp;
	GLuint		type;
	GLuint		Height;
	GLuint		Width;
	GLuint		Bpp;
} TGA;


TGAHeader tgaheader;
TGA tga;

GLubyte uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};
GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};


int cTexture::LoadFromTGA(string Filename)
{
	//Log(3,0,"Texture: Loading texture %s", Filename.c_str());
	cFile* fTGA;
	fTGA = fs.open(Filename.c_str());
	if(fTGA == NULL)
	{
		//MessageBox(NULL, ("Could not open texture file " + Filename).c_str(), "ERROR", MB_OK);
		Log(3,0,"Could not open texture file %s", Filename.c_str());
		return 0;
	}

	
	if(fTGA->read((char*)&tgaheader, sizeof(tgaheader)) == 0)
	{
		Log(1,0, "Could not read file header");
		if(fTGA != NULL)
			fTGA->close();
		return 0;
	}

//	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	if (tgaheader.Header[2] == 2)
		return LoadUncompressedTGA(Filename, fTGA);
//	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	if (tgaheader.Header[2] == 10)
		return LoadCompressedTGA(Filename, fTGA);
	else
	{
		Log(1,0,"TGA file be type 2 or type 10, this one is %i ", tgaheader.Header[2]);
		char* buf = new char(sizeof(tgaheader));
		memcpy(buf, &tgaheader, sizeof(tgaheader));

		for(int i = 0; i < (int)sizeof(tgaheader); i++)
		{
			Log(3,0,"%i: %i", i, buf[i]);
		}
		fTGA->close();
		return 0;
	}
}

int cTexture::LoadUncompressedTGA(string Filename, cFile* fTGA)
{
	if(fTGA->read((char*)&tga.header, sizeof(tga.header)) == 0)
	{										
		Log(1,0,"Could not read info header");
		if(fTGA != NULL)
			fTGA->close();
		return 0;
	}

	width  = tga.header[1] * 256 + tga.header[0];
	height = tga.header[3] * 256 + tga.header[2];
	bpp	= tga.header[4];
	tga.Width		= width;
	tga.Height		= height;
	tga.Bpp			= bpp;

	if((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp !=32)))
	{
		Log(1,0, "Invalid texture information");
		if(fTGA != NULL)
			fTGA->close();
		return 0;
	}

	if(bpp == 24)
		type	= GL_BGR_EXT;
	else
		type	= GL_BGRA_EXT;

	tga.bytesPerPixel	= (tga.Bpp / 8);
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);
	imageData	= (GLubyte *)malloc(tga.imageSize);
	
	if(imageData == NULL)
	{
		Log(1,0, "Could not allocate memory for image");
		fTGA->close();
		return 0;
	}

	if (fTGA->read((char*)imageData, tga.imageSize) != (int)tga.imageSize)
	{
		Log(1,0, "Could not read image data");
		if(imageData != NULL)
			free(imageData);
		fTGA->close();
		return 0;
	}

	//for(GLuint cswap = 0; (int)cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	//	imageData[cswap] ^= imageData[cswap+2] ^= imageData[cswap] ^= imageData[cswap+2];

	fTGA->close();
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D, 0, bpp / 8, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	if (freedata)
		free(imageData);
	Log(3,0,"Texture: Loaded Uncompressed TGA '%s'", filename.c_str());
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
int cTexture::LoadCompressedTGA(string Filename, cFile* fTGA)
{ 
	fTGA->read((char*)&tga.header, sizeof(tga.header));
	if(fTGA->eof())
	{
		Log(1,0, "Could not read info header");
		if(fTGA != NULL)
			fTGA->close();
		return 0;
	}

	width  = tga.header[1] * 256 + tga.header[0];
	height = tga.header[3] * 256 + tga.header[2];
	bpp	= tga.header[4];
	tga.Width		= width;
	tga.Height		= height;
	tga.Bpp			= bpp;

	if((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp !=32)))
	{
		Log(1,0, "Invalid texture information");
		if(fTGA != NULL)
			fTGA->close();
		return 0;
	}

	if(bpp == 24)
		type	= GL_RGB;
	else
		type	= GL_RGBA;

	tga.bytesPerPixel	= (tga.Bpp / 8);
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);
	imageData	= (GLubyte *)malloc(tga.imageSize);

	if(imageData == NULL)
	{
		Log(1,0,"Could not allocate memory for image");
		fTGA->close();
		return 0;
	}

	GLuint pixelcount	= tga.Height * tga.Width;
	GLuint currentpixel	= 0;
	GLuint currentbyte	= 0;
	GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);

	do
	{
		GLubyte chunkheader = 0;

		if(fTGA->read((char*)&chunkheader, sizeof(GLubyte)) == 0)
		{
			Log(1,0, "Could not read RLE header");
			if(fTGA != NULL)
				fTGA->close();
			if(imageData != NULL)
				free(imageData);
			return 0;
		}

		if(chunkheader < 128)
		{
			chunkheader++;
			for(short counter = 0; counter < chunkheader; counter++)
			{
				if(fTGA->read((char*)colorbuffer, tga.bytesPerPixel) != (int)tga.bytesPerPixel)
				{
					Log(1,0, "Could not read image data", "ERROR");

					if(fTGA != NULL)
						fTGA->close();
					if(colorbuffer != NULL)
						free(colorbuffer);
					if(imageData != NULL)
						free(imageData);
					return 0;
				}

				imageData[currentbyte		] = colorbuffer[2];
				imageData[currentbyte + 1	] = colorbuffer[1];
				imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)
					imageData[currentbyte + 3] = colorbuffer[3];

				currentbyte += tga.bytesPerPixel;
				currentpixel++;

				if(currentpixel > pixelcount)
				{
					Log(1,0,"Too many pixels read");
					if(fTGA != NULL)
						fTGA->close();
					if(colorbuffer != NULL)
						free(colorbuffer);
					if(imageData != NULL)
						free(imageData);
					return 0;
				}
			}
		}
		else
		{
			chunkheader -= 127;
			if(fTGA->read((char*)colorbuffer, tga.bytesPerPixel) != (int)tga.bytesPerPixel)
			{	
				Log(1,0,"Could not read from file");
				if(fTGA != NULL)
					fTGA->close();
				if(colorbuffer != NULL)
					free(colorbuffer);
				if(imageData != NULL)
					free(imageData);
				return 0;
			}

			for(short counter = 0; counter < chunkheader; counter++)
			{
				imageData[currentbyte		] = colorbuffer[2];
				imageData[currentbyte + 1	] = colorbuffer[1];
				imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)
					imageData[currentbyte + 3] = colorbuffer[3];

				currentbyte += tga.bytesPerPixel;
				currentpixel++;

				if(currentpixel > pixelcount)
				{
					Log(1,0,"Too many pixels read");
					if(fTGA != NULL)
						fTGA->close();
					if(colorbuffer != NULL)
						free(colorbuffer);
					if(imageData != NULL)
						free(imageData);
					return 0;
				}
			}
		}
	}
	while(currentpixel < pixelcount);
	fTGA->close();
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D, 0, bpp / 8, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	if (freedata)
		free(imageData);
	Log(3,0,"Texture: Loaded Compressed TGA %s", filename.c_str());
	return 1;
}



int cTexture::LoadFromJpeg(string filename)
{
	cFile* pFile;
	pFile = fs.open(filename);
	if(pFile == NULL)
	{
		Log(1,0,"Error opening %s", filename.c_str());
		return 0;
	}
	gdImage* pImageData = gdImageCreateFromJpegPtr(pFile->size, pFile->data);
	if(!pImageData)
		return 0;
	char* data = new char[pImageData->sx*pImageData->sy*3];

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
			memcpy(data+xx+width*y, (void*)((&color)), 3);
		}
	}

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &tid);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, tid);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pImageData->sx, pImageData->sy, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though. 
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	gdImageDestroy(pImageData);
	pFile->close();
	delete[] data;
	Log(3,0,"Loaded %s successfully", filename.c_str());
	return 0;
}



cTexture::BMPClass::BMPClass(){bytes=0;}
cTexture::BMPClass::~BMPClass(){delete[] bytes;}
BYTE& cTexture::BMPClass::pixel(int x,int y,int c){return bytes[(y*width+x)*3+c];}
void cTexture::BMPClass::allocateMem(){delete[] bytes;bytes=new BYTE[width*height*3];}


BMPError cTexture::BMPLoad(std::string fname,BMPClass& bmp)
{
	if(sizeof(int)!=4) return BMPBADINT;
		
	FILE* f=fopen(fname.c_str(),"rb");		//open for reading in binary mode
	if(!f) return BMPNOOPEN;
	char header[54];
	fread(header,54,1,f);			//read the 54bit main header

	if(header[0]!='B' || header[1]!='M') 
	{
		fclose(f);
		return BMPNOTABITMAP;		//all bitmaps should start "BM"
	}

	//it seems gimp sometimes makes its headers small, so we have to do this. hence all the fseeks
	int offset=*(unsigned int*)(header+10);
	
	bmp.width=*(int*)(header+18);
	bmp.height=*(int*)(header+22);
	//now the bitmap knows how big it is it can allocate its memory
	bmp.allocateMem();

	int bits=int(header[28]);		//colourdepth

	int x,y,c;
	BYTE cols[256*4];				//colourtable
	switch(bits)
	{
	case(24):
		fseek(f,offset,SEEK_SET);
		fread(bmp.bytes,bmp.width*bmp.height*3,1,f);	//24bit is easy
		for(x=0;x<bmp.width*bmp.height*3;x+=3)			//except the format is BGR, grr
		{
			BYTE temp=bmp.bytes[x];
			bmp.bytes[x]=bmp.bytes[x+2];
			bmp.bytes[x+2]=temp;
		}
		break;

	case(8):
		fread(cols,256*4,1,f);							//read colortable
		fseek(f,offset,SEEK_SET);
		for(y=0;y<bmp.height;++y)						//(Notice 4bytes/col for some reason)
			for(x=0;x<bmp.width;++x)
			{
				BYTE byte;			
				fread(&byte,1,1,f);						//just read byte					
				for(int c=0;c<3;++c)
					bmp.pixel(x,y,c)=cols[byte*4+2-c];	//and look up in the table
			}
		break;

	case(4):
		fread(cols,16*4,1,f);
		fseek(f,offset,SEEK_SET);
		for(y=0;y<256;++y)
			for(x=0;x<256;x+=2)
			{
				BYTE byte;
				fread(&byte,1,1,f);						//as above, but need to exract two
				for(c=0;c<3;++c)						//pixels from each byte
					bmp.pixel(x,y,c)=cols[byte/16*4+2-c];
				for(c=0;c<3;++c)
					bmp.pixel(x+1,y,c)=cols[byte%16*4+2-c];
			}
		break;

	case(1):
		fread(cols,8,1,f);
		fseek(f,offset,SEEK_SET);
		for(y=0;y<bmp.height;++y)
			for(x=0;x<bmp.width;x+=8)
			{
				BYTE byte;
				fread(&byte,1,1,f);
				//Every byte is eight pixels
				//so I'm shifting the byte to the relevant position, then masking out
				//all but the lowest bit in order to get the index into the colourtable.
				for(int x2=0;x2<8;++x2)
					for(int c=0;c<3;++c)
						bmp.pixel(x+x2,y,c)=cols[((byte>>(7-x2))&1)*4+2-c];
			}
		break;

	default:
		fclose(f);
		return BMPUNKNOWNFORMAT;
	}

	if(ferror(f))
	{
		fclose(f);
		return BMPFILEERROR;
	}
	
	fclose(f);

	return BMPNOERROR;
}
