#include "common.h"
#include "filesystem.h"
#include "texture.h"

#ifdef __MINGW32__
#include <GL/glext.h>											// We use a define from this file: GL_BGRA_EXT
#endif

#define GL_CLAMP_TO_EDGE 0x812F

GLint glMaxTexDim;

cTexture::cTexture(std::string pFilename, eTextureOptions pOptions)
{
	options = pOptions;
	filename = pFilename;
	loaded = false;
	data = NULL;
	datatype = 0;
	tid = 0;
}

cTextureFromMemory::cTextureFromMemory(std::string ext, char* d, int l) : cTexture("",TEX_NOFREEDATA)
{
	mem_data = new char[l];
	memcpy(mem_data, d, l);
	extension = ext;
	mem_length = l;
}

GLuint cTextureFromMemory::texId()
{
	if(!loaded)
	{
		cTextureLoaders::loadfrommem(extension, mem_data, mem_length, this, options);
		delete[] mem_data;
		loaded = true;
	}
	return tid;
}

GLuint cTexture::texId()
{
	if(!loaded && filename != "")
	{
		cTextureLoaders::load(filename, this, options);
		loaded = true;
	}
	return tid;
}


void cTexture::unLoad()
{
	if (options & TEX_NOFREEDATA)
		delete[] data;
	if(tid != 0)
		glDeleteTextures(1, &tid);
	loaded = false;
}

cTextureLoaders& GetTextureLoaders()
{
	static cTextureLoaders* l = new cTextureLoaders();
	return *l;
}

//vector<cTextureLoader*> cTextureLoaders::loaders;
cTexture* cTextureLoaders::load(std::string filename, eTextureOptions options)
{
	cTexture* t = new cTexture(filename, options);
	return t;
}

void cTexture::generate()
{
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	if(datatype == 0)
		datatype = GL_RGB;


	glTexImage2D(GL_TEXTURE_2D,0,bpp/8,width,height,0,datatype,GL_UNSIGNED_BYTE,data);

	if(!(options & TEX_NEARESTFILTER))
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	}

	if(options & TEX_NOCLAMP)
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	}
	if(!(options & TEX_NOFREEDATA))
		delete[] data;

}


void cTextureLoaders::load(std::string filename, cTexture* tex, eTextureOptions options)
{
	if(filename.rfind(".") == std::string::npos)
	{
		Log(1,0,"Error loading texture %s, not able to determine filetype!", filename.c_str());
		return;
	}

	std::vector<cTextureLoader*> loaders = GetTextureLoaders().loaders;

	std::string ext = lcase(filename.substr(filename.rfind(".")));
	for(unsigned int i = 0; i < loaders.size(); i++)
	{
		for(unsigned int ii = 0; ii < loaders[i]->extensions.size(); ii++)
		{
			if(loaders[i]->extensions[ii] == ext)
			{
				cFile* pFile = cFileSystem::open(filename);
				if(!pFile)
				{
					Log(2,0,"Error loading texture %s!", filename.c_str());
					return;
				}
				loaders[i]->load(pFile->data, pFile->size, tex);
				if(tex->data == NULL)
					Log(2,0,"Error loading texture %s!", filename.c_str());
				else
				{
					pFile->close();
					tex->resizeToLog();
					tex->generate();
				}
				return;
			}
		}
	}
	Log(1,0,"Unknown texture type: %s at file %s", ext.c_str(), filename.c_str()); 
}

void cTextureLoaders::loadfrommem(std::string ext, char* data, int length, cTexture* tex, eTextureOptions options)
{
	std::vector<cTextureLoader*> loaders = GetTextureLoaders().loaders;

	ext = lcase(ext);
	for(unsigned int i = 0; i < loaders.size(); i++)
	{
		for(unsigned int ii = 0; ii < loaders[i]->extensions.size(); ii++)
		{
			if(loaders[i]->extensions[ii] == ext)
			{
				loaders[i]->load(data, length, tex);
				if(tex->data == NULL)
					Log(2,0,"Error loading texture from memory!");
				else
				{
					tex->resizeToLog();
					tex->generate();
				}
				return;
			}
		}
	}
}


void cTexture::resizeToLog()
{
	// Resize Image To Closest Power Of Two
	if(glMaxTexDim == 0)
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);
	if (width <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
		width = 1 << (int)floor((log((double)width)/log(2.0f)) + 0.5f); 
	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
		width = glMaxTexDim;
	
	if (height <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
		height = 1 << (int)floor((log((double)height)/log(2.0f)) + 0.5f);
	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
		height = glMaxTexDim;

	while(height < heightOriginal)
		height*=2;
	while(width< widthOriginal)
		width*=2;

	if(width == widthOriginal && height == heightOriginal)
		return;

	float factorx = (float)widthOriginal / (float)width;
	float factory = (float)heightOriginal / (float)height;

	char* data2 = new char[width* height*(bpp/8)];
	ZeroMemory(data2, width*height*(bpp/8));

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			int tx = (int)floor(x*factorx);
			int ty = (int)floor(y*factory);
			int target = (bpp/8)*(tx+(widthOriginal)*ty);

			for(int i = 0; i < (bpp/8); i++)
	  			data2[(bpp/8)*(x+width*y)+i] = data[target+i];
		}
	}
	delete[] data;

	data = (BYTE*)data2;

}
